#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#define ITERATIN_COUNT 30      // 迭代次数
#define DAMPING_FACTOR 0.85    // 阻尼系数

int nodesCount = 0;            // 总的点数
int edgesCount = 0;            // 总的边数
int maxId = 0;                 // 点最大id，因为数据集中点id不连续
vector<vector<int> > graphFrom;// 以下标节点为终点的起点集合
vector<double> ranks;          // 每个节点的rank
vector<int> L;                 // 每个节点的出度
vector<bool> existed;          // 存在标记集合，因为数据集中点id不连续

/* 从文件中获取图数据 
 * 文件第一行要保存总节点数和边数
 * 其他行每行2个数字，表示起点和终点，如
 * 5 8
 * 0 1
 * 0 2
 * 0 3
 * 1 3
 * 2 4
 * 3 4
 * 1 4
 * 4 0
 */
void readFileDate(char* fileName) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("文件不存在！\n");
    }

    // 文件第一行保存总的节点数和边数
    fscanf(fp, "%d %d", &nodesCount, &edgesCount);
    // 获取最大id值为点的数量，因为数据集中点id不连续
    int fromId = 0, toId = 0;
    for (int i = 0; fscanf(fp, "%d %d", &fromId, &toId) != EOF; i++) {
        maxId = maxId < fromId ? fromId : maxId;
        maxId = maxId < toId ? toId : maxId;
    }
    maxId = maxId + 1;
    fclose(fp);

    graphFrom.resize(maxId);
    ranks.resize(maxId);
    L.resize(maxId);
    existed.resize(maxId);

    fp = fopen(fileName, "r");
    fscanf(fp, "%d %d", &nodesCount, &edgesCount);
    // 文件其他行保存边的起点和终点，节点下标从0开始
    while(fscanf(fp, "%d %d", &fromId, &toId) != EOF) {
        L[fromId]++; // 起点出度+1
        graphFrom[toId].push_back(fromId); // 终点的被指向集合增加一个元素
        existed[fromId] = true;
        existed[toId] = true;
    }
    fclose(fp);

    for (int i = 0; i < maxId; i++) {
        // 节点存在
        if (existed[i] == true) {
            ranks[i] = 1.0 / nodesCount;     // 初始rank值
        }
        else {
            ranks[i] = 0;
        }
    }
}

/* 获取终点是index的所有起点的 PR/L */
double getSum(int index) {
    double sum = 0;
    for (int i = 0; i < graphFrom[index].size(); i++) {
        int fromNode = graphFrom[index][i];
        sum += (ranks[fromNode] / L[fromNode]);
    }
    return sum;
}

void pageRank() {
    // 随机概率系数
    double d = (1 - DAMPING_FACTOR) / nodesCount;
    // rank迭代
    for (int k = 0; k < ITERATIN_COUNT; k++) {
        for (int i = 0; i < maxId; i++) {
            if (existed[i] == true) {
                ranks[i] = getSum(i) * DAMPING_FACTOR + d;
            }
            else {
                ranks[i] = 0;
            }
        }
        printf("%dth iteration completes\n", k + 1);
    }
}

/* 保存结果 */
void writeResult(char *fileName) {
    FILE * fp = fopen(fileName, "w");

    for (int i = 0; i < maxId; i++) {
        if (existed[i] == true) {
            fprintf(fp, "%d    %lf\n", i, ranks[i]);
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        readFileDate(argv[1]);
        pageRank();
        writeResult(argv[2]);
    }
    else {
        printf("pageRank [inFileName] [outFileName]\n");
    }

    return 0;
}

