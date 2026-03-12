/*
 *Noah Wiley
 *CSCI 114
 *Programming Assignment 3
 *
 *The Program Assignment 3 was written, tested  & complied using Linux on macOS
 *
 *To execute the program:
 *By opening the Terminal using in the same directory, I use cd ~/CLionProjects/CSci114_P3
 *
 *Compile the file by executing the file:
 *g++ CSci114_P3.cpp
 *
 *To run the program we use:
 *./a.out 4 5 6
 *
 *The Output of the program should be:
 * 72 69 66 63 60 57
 * 91 89 87 85 83 81
 * 61 71 81 91 101 111
 * 42 51 60 69 78 87
 *
 * If you are using Windows platform using Cygwin execute with ./a.exe instead
 *Ensure A.txt & B.txt contain the input matrices in proper format
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

using namespace std;

vector<vector<int>> A, B, C;
int M, N, K;

void multiplyRow(int row) {
    for (int col = 0; col < K; ++col) {
        C[row][col] = 0;
        for (int i = 0; i < N; ++i) {
            C[row][col] += A[row][i] * B[i][col];
        }
    }
}

void readMatrix(const string &filename, vector<vector<int>> &matrix, int rows, int cols) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    matrix.resize(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> matrix[i][j];
        }
    }
    file.close();
}

void printMatrix(const vector<vector<int>> &matrix) {
    for (const auto &row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " M N K" << endl;
        return 1;
    }

    M = stoi(argv[1]);
    N = stoi(argv[2]);
    K = stoi(argv[3]);

    readMatrix("A.txt", A, M, N);
    readMatrix("B.txt", B, N, K);
    C.resize(M, vector<int>(K, 0));

    vector<thread> threads;
    for (int i = 0; i < M; ++i) {
        threads.emplace_back(multiplyRow, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    printMatrix(C);
    return 0;
}