#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
class Graph
{
private:
    int** matrix;
public:
    int N;
    int edges;

    ~Graph() {
        for (int i = 0; i < N; ++i)
            delete[] matrix[i];
        delete[] matrix;
    }

    void FloydWarshall() {
        int i, j, k;
        for (i = 0; i < N; i++)
            if (matrix[i][i] != -1)
                matrix[i][i] = 0;

        for (k = 0; k < N; k++) {
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++) {
                    if (matrix[i][k] <= 0 || matrix[k][j] <= 0 || matrix[i][j] == -1) continue;
                    if ((matrix[i][k] + matrix[k][j] < matrix[i][j] || matrix[i][j] == 0) && (i != j)) {
                        matrix[i][j] = matrix[i][k] + matrix[k][j];
                    }
                }
            }
        }
    }

    Graph() {
        cout << "Enter the number of vertex in the range [1;10000]:" << endl;
        cin >> N;
        cout << "Enter the number of edges :" << endl;
        cin >> edges;
        while (N > 10000 || N < 0) {
            cout << "Wrong value for vertexes, enter number again:" << endl;
            cin >> N;
        }
        while (edges <= 0 || edges > N * (N - 1)) {
            cout << "Wrong value for edges, enter number again:" << endl;
            cin >> edges;
        }
        int i, j;
        int tmp = 0;
        cout << "If you want to enter the graph manually - press '1', if you want to generate a graph - press '2'. " << endl;
        cin >> tmp;
        matrix = new int* [N];

        for (int i = 0; i < N; i++) // создание каждого одномерного массива в динамическом двумерном массиве, или иначе - создание столбцов размерность n
            matrix[i] = new int[N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = -1;
                if (i == j)
                    matrix[i][j] = 0;
            }
        }

        if (tmp == 2) {
            const int R_MIN = -1;
            const int R_MAX = 50;
            int i, j;
            int* randomArray = new int[edges];
            srand(time(NULL));

            for (int i = 0; i < edges; i++) {
                randomArray[i] = rand() % (R_MAX - R_MIN + 1) + R_MIN;
                if (randomArray[i] == 0) {
                    i--;
                }
            }


            for (int i = 0; i < edges; i++) {
                int k = rand() % (N);
                int j = rand() % (N);
                if (matrix[k][j] > 0 || k == j) {
                    i--;
                }
                else {
                    matrix[k][j] = randomArray[i];
                }

            }
        }
        else if (tmp == 1) {
            cout << "Fill in the adjacency matrix:" << endl;
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++)
                {
                    cout << "matrix[" << i << "][" << j << "] = ";
                    cin >> matrix[i][j];
                }
            }
        }
    }

    void Print(bool flag) {
        FILE* f;
        if (flag)
            f = fopen("before.txt", "w");
        else
            f = fopen("after.txt", "w");
        fprintf(f, "[\\]");
        for (int i = 0; i < N; ++i)
            fprintf(f, "[%d] ", i);
        fprintf(f, "\n");
        for (int i = 0; i < N; i++) {
            fprintf(f, "[%d]", i);
            for (int j = 0; j < N; j++) {
                fprintf(f, "[%d]", matrix[i][j]);
            }
            putc('\n', f);
        }
        fclose(f);
    }

};

int main()
{

    Graph graph;
    unsigned int tmp1 = clock();
    graph.Print(true);
    graph.FloydWarshall();
    graph.Print(false);
    unsigned int tmp2 = clock();
    cout << "Runtime of program: " << (tmp2 - tmp1) / 1000.0 << endl;
    cout << "The result of the program is in the file after.txt" << endl;
    system("PAUSE");
    return 0;
}