#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#define ITERATIN_COUNT 30      // ��������
#define DAMPING_FACTOR 0.85    // ����ϵ��

int nodesCount = 0;            // �ܵĵ���
int edgesCount = 0;            // �ܵı���
int maxId = 0;                 // �����id����Ϊ���ݼ��е�id������
vector<vector<int> > graphFrom;// ���±�ڵ�Ϊ�յ����㼯��
vector<double> ranks;          // ÿ���ڵ��rank
vector<int> L;                 // ÿ���ڵ�ĳ���
vector<bool> existed;          // ���ڱ�Ǽ��ϣ���Ϊ���ݼ��е�id������

/* ���ļ��л�ȡͼ���� 
 * �ļ���һ��Ҫ�����ܽڵ����ͱ���
 * ������ÿ��2�����֣���ʾ�����յ㣬��
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
        printf("�ļ������ڣ�\n");
    }

    // �ļ���һ�б����ܵĽڵ����ͱ���
    fscanf(fp, "%d %d", &nodesCount, &edgesCount);
    // ��ȡ���idֵΪ�����������Ϊ���ݼ��е�id������
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
    // �ļ������б���ߵ������յ㣬�ڵ��±��0��ʼ
    while(fscanf(fp, "%d %d", &fromId, &toId) != EOF) {
        L[fromId]++; // ������+1
        graphFrom[toId].push_back(fromId); // �յ�ı�ָ�򼯺�����һ��Ԫ��
        existed[fromId] = true;
        existed[toId] = true;
    }
    fclose(fp);

    for (int i = 0; i < maxId; i++) {
        // �ڵ����
        if (existed[i] == true) {
            ranks[i] = 1.0 / nodesCount;     // ��ʼrankֵ
        }
        else {
            ranks[i] = 0;
        }
    }
}

/* ��ȡ�յ���index���������� PR/L */
double getSum(int index) {
    double sum = 0;
    for (int i = 0; i < graphFrom[index].size(); i++) {
        int fromNode = graphFrom[index][i];
        sum += (ranks[fromNode] / L[fromNode]);
    }
    return sum;
}

void pageRank() {
    // �������ϵ��
    double d = (1 - DAMPING_FACTOR) / nodesCount;
    // rank����
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

/* ������ */
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

