#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int nodesCount = 0;            // �ܵĵ���
int edgesCount = 0;            // �ܵı���
int maxId = 0;                 // �����id����Ϊ���ݼ��е�id������
vector<vector<int> > graphTo;  // ���±�ڵ�Ϊ�����յ㼯��
vector<bool> visited;          // ������Ǽ���
vector<bool> existed;          // ���ڱ�Ǽ��ϣ���Ϊ���ݼ��е�id������

void readFileDate(char* fileName) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("�ļ������ڣ�\n");
        return;
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

    graphTo.resize(maxId);
    visited.resize(maxId);
    existed.resize(maxId);

    fp = fopen(fileName, "r");
    fscanf(fp, "%d %d", &nodesCount, &edgesCount);
    // �ļ������б���ߵ������յ㣬�ڵ��±��0��ʼ
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
        // �ڵ������δ����
        if (existed[i] == true && visited[i] == false) {
            visit(i, fp);
            q.push(i);
            while (!q.empty()) {
                int temp = q.front();
                q.pop();
                // �˽ڵ�ָ���δ�����ڵ�����
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

