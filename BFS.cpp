#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int nodesCount = 0;            // 总的点数
int edgesCount = 0;            // 总的边数
int maxId = 0;                 // 点最大id，因为数据集中点id不连续
vector<vector<int> > graphTo;  // 以下标节点为起点的终点集合
vector<bool> visited;          // 遍历标记集合
vector<bool> existed;          // 存在标记集合，因为数据集中点id不连续

void readFileDate(char* fileName) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("文件不存在！\n");
        return;
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

    graphTo.resize(maxId);
    visited.resize(maxId);
    existed.resize(maxId);

    fp = fopen(fileName, "r");
    fscanf(fp, "%d %d", &nodesCount, &edgesCount);
    // 文件其他行保存边的起点和终点，节点下标从0开始
    while (fscanf(fp, "%d %d", &fromId, &toId) != EOF) {
        graphTo[fromId].push_back(toId);
        existed[fromId] = true;
        existed[toId] = true;
    }
    fclose(fp);
}

FILE*  beginWriteResult(char *fileName) {
    FILE *fp = fopen(fileName, "w");
    return fp;
}

void visit(int node, FILE * fp) {
    fprintf(fp, "%d\n", node);
    visited[node] = true;
}

void beginWriteResult(FILE* fp) {
    fclose(fp);
}

void BFS(FILE * fp) {
    queue<int> q;
    
    for (int i = 0; i < maxId; i++) {
        // 节点存在且未遍历
        if (existed[i] == true && visited[i] == false) {
            visit(i, fp);
            q.push(i);
            while (!q.empty()) {
                int temp = q.front();
                q.pop();
                // 此节点指向的未遍历节点均入队
                for (int j = 0; j < graphTo[temp].size(); j++) {
                    if (visited[graphTo[temp][j]] == false) {
                        visit(graphTo[temp][j], fp);
                        q.push(graphTo[temp][j]);
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        readFileDate(argv[1]);
        FILE * fp = beginWriteResult(argv[2]);
        BFS(fp);
        beginWriteResult(fp);
    }
    else {
        printf("BFS [inFileName] [outFileName]\n");
    }

    return 0;
}

