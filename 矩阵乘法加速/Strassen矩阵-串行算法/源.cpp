#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

// 传统的矩阵乘法
vector<vector<int>> matrix_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    int m = B[0].size();
    int p = A[0].size();

    vector<vector<int>> C(n, vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// 将两个矩阵相加
vector<vector<int>> add_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    int m = A[0].size();

    vector<vector<int>> C(n, vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    return C;
}

// 将矩阵B从矩阵A中减去
vector<vector<int>> subtract_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    int m = A[0].size();

    vector<vector<int>> C(n, vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] - B[i][j];
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

    vector<vector<int>> M1 = strassen_multiply(add_matrices(A11, A22), add_matrices(B11, B22), threshold);
    vector<vector<int>> M2 = strassen_multiply(add_matrices(A21, A22), B11, threshold);
    vector<vector<int>> M3 = strassen_multiply(A11, subtract_matrices(B12, B22), threshold);
    vector<vector<int>> M4 = strassen_multiply(A22, subtract_matrices(B21, B11), threshold);
    vector<vector<int>> M5 = strassen_multiply(add_matrices(A11, A12), B22, threshold);
    vector<vector<int>> M6 = strassen_multiply(subtract_matrices(A21, A11), add_matrices(B11, B12), threshold);
    vector<vector<int>> M7 = strassen_multiply(subtract_matrices(A12, A22), add_matrices(B21, B22), threshold);


    vector<vector<int>> C11 = subtract_matrices(add_matrices(add_matrices(M1, M4), M7), M5);
    vector<vector<int>> C12 = add_matrices(M3, M5);
    vector<vector<int>> C21 = add_matrices(M2, M4);
    vector<vector<int>> C22 = subtract_matrices(add_matrices(add_matrices(M1, M3), M6), M2);

    vector<vector<int>> C(n, vector<int>(n, 0));

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
    int n = 1024;  // 矩阵的维度
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
    ////实验不同的阈值
    //vector<int> thresholds = { 8,16, 32, 64, 128, 256, 512,1024,2048 };

    //for (int threshold : thresholds) {
    //    // 计算不同阈值的 Strassen 算法执行时间
    //    auto start_time = chrono::steady_clock::now();
    //    vector<vector<int>> C_strassen = strassen_multiply(A, B, threshold);
    //    auto end_time = chrono::steady_clock::now();
    //    double strassen_time = chrono::duration<double>(end_time - start_time).count();

    //    // 打印执行时间
    //    std::cout << "阈值: " << threshold << " \nStrassen 算法的执行时间：" << strassen_time << " 秒" << endl;
    //}
    //std::cout << "------------------------------------" << endl;

    int threshold = 64;
    // 计算阈值为 64 的Strassen算法执行时间
    auto start_time = chrono::steady_clock::now();
    vector<vector<int>> C_strassen = strassen_multiply(A, B, threshold);
    auto end_time = chrono::steady_clock::now();
    double strassen_time = chrono::duration<double>(end_time - start_time).count();

    // 打印执行时间
    std::cout << "阈值: " << threshold << "\nStrassen算法的执行时间：" << strassen_time << " 秒" << endl;
    std::cout << "------------------------------------" << endl;

    return 0;
}