#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <iostream>
#include <cassert>

// Template for Vector
template<size_t dimension, typename T> class vec {
private:    
    T element[dimension];
public:
    vec() { for (size_t i = 0; i < dimension; i++) {element[i] = T();}}
    T& operator[] (const size_t i) {
        assert(i < dimension && "index out of range");
        return element[i];
    }
    const T& operator[]  (const size_t i) const {
        assert(i < dimension && "index out of range");
        return element[i];
    }
    float norm() {
        float sum = 0;
        for (size_t i = 0; i < dimension; i++) {
            sum += element[i] * element[i];
        }
        return std::sqrt(sum);
    }
    vec<dimension, T>& normalize() {
        *this = *this / (norm());
        return *this;
    }
    vec<2, T> projectTo2 () {
        return vec<2, T>(element[0], element[1]);
    }
};

// Calculator for Vector
template<size_t dimension, typename T> vec<dimension, T> operator+(const vec<dimension, T>& vector1, const vec<dimension, T>& vector2) {
    vec<dimension, T> result;
    for (size_t i = 0; i < dimension; i++) result[i] = vector1[i] + vector2[i];
    return result;
}

template<size_t dimension, typename T> vec<dimension, T> operator-(const vec<dimension, T>& vector1, const vec<dimension, T>& vector2) {
    vec<dimension, T> result;
    for (size_t i = 0; i < dimension; i++) result[i] = vector1[i] - vector2[i];
    return result;
}

template<size_t dimension, typename T> T operator*(const vec<dimension, T>& vector1, const vec<dimension, T>& vector2) {
    T result = T();
    for (size_t i = 0; i < dimension; i++) result += vector1[i] * vector2[i];
    return result;
}

template<size_t dimension, typename T, typename U> vec<dimension, T> operator*(const vec<dimension, T>& vector, const U& num) {
    vec<dimension, T> result;
    for (size_t i = 0; i < dimension; i++) result[i] = vector[i] * num;
    return result;
}

template<size_t dimension, typename T, typename U> vec<dimension, T> operator/(const vec<dimension, T>& vector, const U& num) {
    vec<dimension, T> result;
    for (size_t i = 0; i < dimension; i++) result[i] = vector[i] / num;
    return result;
}

template<size_t dimension, typename T> std::ostream& operator<<(std::ostream& output, const vec<dimension, T>& vector) {
    output << "(" ;
    for (size_t i = 0; i < dimension - 1; i++) {
        output << vector[i] << ", ";
    }
    output << vector[dimension - 1] << ")";
    return output;
}

typedef vec<2, float> Vec2f;
typedef vec<2, int> Vec2i;
typedef vec<3, float> Vec3f;
typedef vec<3, int> Vec3i;
typedef vec<4, float> Vec4f;
typedef vec<4, int> Vec4i;

// Specification
template<typename T> class vec<2, T> {
public:
    T x, y;
    vec() : x(T()), y(T()) {}
    vec(T _x, T _y) : x(_x), y(_y) {}
    T& operator[] (const size_t i) {
        assert(i < 2 && "index out of range");
        return i == 0 ? x : y;
    }
    const T& operator[] (const size_t i) const {
        assert(i < 2 && "index out of range");
        return i == 0 ? x : y;
    }
};

template<typename T> class vec<3, T> {
public:
    T x, y, z;
    vec() : x(T()), y(T()), z(T()) {}
    vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    T& operator[] (const size_t i) {
        assert(i < 3 && "index out of range");
        return i == 0 ? x : (i == 1 ? y : z);
    }
    const T& operator[] (const size_t i) const {
        assert(i < 3 && "index out of range");
        return i == 0 ? x : (i == 1 ? y : z);
    }
    vec<2, T> projectTo2() {
        return vec<2, T>(x, y);
    }
    float norm() {
        return std::sqrt(x * x + y * y + z * z);
    }
    vec<3, T>& normalize() {
        *this = *this / (norm());
        return *this;
    }
    vec<4, T> projectTo4() {
        vec<4, T> result;
        result[0] = x;
        result[1] = y;
        result[2] = z;
        result[3] = T(1);
        return result;
    }
};

template<typename T> class vec<4, T> {
public:
    T x, y, z, t;
    vec() : x(T()), y(T()), z(T()), t(T()) {}
    vec(T _x, T _y, T _z, T _t) : x(_x), y(_y), z(_z), t(_t) {}
    T& operator[] (const size_t i) {
        assert(i < 4 && "index out of range");
        return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : t));
    }
    const T& operator[] (const size_t i) const {
        assert(i < 4 && "index out of range");
        return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : t));
    }
    vec<2, T> projectTo2() {
        return vec<2, T>(x, y);
    }
    vec<3, T> projectTo3() {
        return vec<3, T>(x / t, y / t, z / t);
    }
    vec<4, T> renormalize() {
        return vec<4, T>(x / t, y / t, z / t, T(1));
    }
};

template<typename T> vec<3, T> cross(const vec<3, T>& vector1, const vec<3, T>& vector2) {
    return vec<3, T>(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x);
}

// Template for Matrix
template<size_t Row, size_t Column, typename T> class mat {
private:
    vec<Column, T> rows[Row];
public:
    mat() {}
    mat(const T (&element)[Row][Column]) {
        for (size_t i = 0; i < Row; i++) {
            for (size_t j = 0; j < Column; j++) {
                rows[i][j] = element[i][j];
            }
        }
    }
    vec<Column, T>& operator[] (const size_t index) {
        assert(index < Row && "index out of range");
        return rows[index];
    }
    const vec<Column, T>& operator[] (const size_t index) const {
        assert(index < Row && "index out of range");
        return rows[index];
    }
    vec<Row, T> col(const size_t index) const {
        assert(index < Column && "index out of range");
        vec<Row, T> column;
        for (size_t i = 0; i < Row; i++) {column[i] = rows[i][index];}
        return column;
    }
    static mat<Row, Column, T> identity() {
        assert(Row == Column && "Identity Matrix must be square");
        mat<Row, Column, T> Identity;
        for (size_t i = 0; i < Row; i++) {
            Identity[i][i] = 1;
        }
        return Identity;
    }
};

// operator for matrix.
template<size_t Row1, size_t Column1, size_t Row2, size_t Column2, typename T> mat<Row1, Column2, T> operator*(const mat<Row1, Column1, T>& Matrix1, const mat<Row2, Column2, T>& Matrix2) {
    assert(Column1 == Row2 && "Column1 is not equal to Row2");
    mat<Row1, Column2, T> result;
    for (size_t i = 0; i < Row1; i++) {
        for (size_t j = 0; j < Column2; j++) {
            result[i][j] = Matrix1[i] * Matrix2.col(j);
        }
    }
    return result;
}

template<size_t Row, size_t Column, typename T> vec<Row, T> operator*(const mat<Row, Column, T>& Matrix, const vec<Column, T>& vector) {
    // assert(Column1 == Row2 && "Column1 is not equal to Row2");
    vec<Row, T> result;
    for (size_t i = 0; i < Row; i++) {
        result[i] = Matrix[i] * vector;
    }
    return result;
}



template<size_t Row, size_t Column, typename T> std::ostream& operator<<(std::ostream& output, const mat<Row, Column, T>& Matrix) {
    for (size_t i = 0; i < Row; i++) {
        output << Matrix[i] << '\n';
    }
    return output;
}
typedef mat<4, 4, float> Matrix;
float deter(const mat<3, 3, float> &a);
mat<3, 3, float> coj(Matrix &matrix, size_t row, size_t column);

Matrix inverse(Matrix &matrix);



#endif