#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;

// 两个矩阵相加
vector<vector<int>> matrix_add(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// 两个矩阵相减
vector<vector<int>> matrix_subtract(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// 普通矩阵乘法
vector<vector<int>> matrix_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Strassen算法进行矩阵乘法
vector<vector<int>> strassen_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int threshold) {
    int n = A.size();
    if (n <= threshold) {
        return matrix_multiply(A, B);
    }

    int mid = n / 2;

    vector<vector<int>> A11(mid, vector<int>(mid, 0));
    vector<vector<int>> A12(mid, vector<int>(mid, 0));
    vector<vector<int>> A21(mid, vector<int>(mid, 0));
    vector<vector<int>> A22(mid, vector<int>(mid, 0));
    vector<vector<int>> B11(mid, vector<int>(mid, 0));
    vector<vector<int>> B12(mid, vector<int>(mid, 0));
    vector<vector<int>> B21(mid, vector<int>(mid, 0));
    vector<vector<int>> B22(mid, vector<int>(mid, 0));

    // 使用OpenMP并行化循环，初始化A11、A12、A21、A22、B11、B12、B21、B22
#pragma omp parallel for collapse(2)
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }
    }
    vector<vector<int>> M1(mid, vector<int>(mid, 0));
    vector<vector<int>> M2(mid, vector<int>(mid, 0));
    vector<vector<int>> M3(mid, vector<int>(mid, 0));
    vector<vector<int>> M4(mid, vector<int>(mid, 0));
    vector<vector<int>> M5(mid, vector<int>(mid, 0));
    vector<vector<int>> M6(mid, vector<int>(mid, 0));
    vector<vector<int>> M7(mid, vector<int>(mid, 0));

#pragma omp parallel sections
    {
#pragma omp section
        {
            M1 = strassen_multiply(matrix_add(A11, A22), matrix_add(B11, B22), threshold);
        }

#pragma omp section
        {
            M2 = strassen_multiply(matrix_add(A21, A22), B11, threshold);
        }

#pragma omp section
        {
            M3 = strassen_multiply(A11, matrix_subtract(B12, B22), threshold);
        }

#pragma omp section
        {
            M4 = strassen_multiply(A22, matrix_subtract(B21, B11), threshold);
        }

#pragma omp section
        {
            M5 = strassen_multiply(matrix_add(A11, A12), B22, threshold);
        }

#pragma omp section
        {
            M6 = strassen_multiply(matrix_subtract(A21, A11), matrix_add(B11, B12), threshold);
        }

#pragma omp section
        {
            M7 = strassen_multiply(matrix_subtract(A12, A22), matrix_add(B21, B22), threshold);
        }
    }

    vector<vector<int>> C11 = matrix_subtract(matrix_add(matrix_add(M1, M4), M7), M5);
    vector<vector<int>> C12 = matrix_add(M3, M5);
    vector<vector<int>> C21 = matrix_add(M2, M4);
    vector<vector<int>> C22 = matrix_subtract(matrix_add(matrix_add(M1, M3), M6), M2);

    vector<vector<int>> C(n, vector<int>(n, 0));

    // 使用OpenMP并行化循环，初始化结果矩阵C
#pragma omp parallel for collapse(2)
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    return C;
}
int main() {
    int n = 2048; // 矩阵的维度 
    vector<vector<int>> A(n, vector<int>(n, 0));
    vector<vector<int>> B(n, vector<int>(n, 0));

    // 生成随机矩阵
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 99);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
        }
    }

    int threshold = 64;

    {
        // 测量带有阈值的Strassen算法的执行时间
        auto start_time = chrono::steady_clock::now();
        vector<vector<int>> C_strassen = strassen_multiply(A, B, threshold);
        auto end_time = chrono::steady_clock::now();
        double strassen_time = chrono::duration<double>(end_time - start_time).count();

        // 打印执行时间
        {
            cout << "Strassen算法的执行时间：" << strassen_time << " 秒" << endl;
            cout << "------------------------------------------" << endl;
        }
    }

    return 0;
}