#include <iostream>
#include "../include/geometry.h"

float deter(const mat<3, 3, float> &a) {
    float a1 = a[0][0] * a[1][1] * a[2][2];
    float a2 = a[0][1] * a[1][2] * a[2][0];
    float a3 = a[1][0] * a[2][1] * a[0][2];
    float a4 = a[0][2] * a[1][1] * a[2][0];
    float a5 = a[1][0] * a[0][1] * a[2][2];
    float a6 = a[0][0] * a[2][1] * a[1][2];
    return a1 + a2 + a3 - a4 - a5- a6;
}

mat<3, 3, float> coj(Matrix &matrix, size_t row, size_t column) {
    mat<3, 3, float> result;
    size_t r = 0;
    for (size_t i = 0; i < 4; i++) {
        if (i == row) continue;
        int c = 0;
        for (size_t j = 0; j < 4; j++) {
            if (j == column) continue;
            result[r][c] = matrix[i][j];
            c++;
        }
        r++;
    }
    return result;
}


Matrix inverse(Matrix &matrix) {
    Matrix result;
    float de = matrix[0][0] * deter(coj(matrix, 0, 0)) - matrix[0][1] * deter(coj(matrix, 0, 1)) + matrix[0][2] * deter(coj(matrix, 0, 2)) - matrix[0][3] * deter(coj(matrix, 0, 3));
    // std::cout << de <<std::endl;
    // std::cout << coj(matrix, 0, 0) << coj(matrix, 0, 1) <<coj(matrix, 0, 2) << std::endl;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            result[i][j] = deter(coj(matrix, j, i)) / de * ((i + j) % 2 == 0 ? 1 : -1);
        }
    }
    return result;
}
