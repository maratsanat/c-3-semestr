#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>

template<typename T>
class Matrix {
    std::vector<T> data;
    unsigned _rows, _cols;

public:
    Matrix(unsigned rows, unsigned cols, T value = 0) : _rows(rows), _cols(cols), data(rows * cols, value) {}

    static Matrix Identity(unsigned n) {
        Matrix m(n, n, 0);
        for(unsigned i = 0; i < n; i++) {
            m(i, i) = 1;
        }
        return m;
    }

    static Matrix getSpecificDeterminant(unsigned n, T target_det) {
        std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<T> dist(0.5, 1.5);
        
        Matrix result = Identity(n);
        
        T det_per_element = std::pow(target_det, 1.0 / n);
        for(unsigned i = 0; i < n; i++) {
            result(i, i) = det_per_element;
        }
        
        for(unsigned i = 0; i < n; i++) {
            for(unsigned j = i + 1; j < n; j++) {
                T val = dist(gen) * 0.01;
                result(i, j) = val;
                result(j, i) = val;
            }
        }
        
        return result;
    }

    unsigned rows() const { return _rows; }
    unsigned cols() const { return _cols; }

    T& operator()(unsigned row, unsigned col) {
        return data[row * _cols + col];
    }

    T operator()(unsigned row, unsigned col) const {
        return data[row * _cols + col];
    }

    Matrix& transpose() {
        Matrix temp(_cols, _rows);
        for(unsigned i = 0; i < _rows; i++) {
            for(unsigned j = 0; j < _cols; j++) {
                temp(j, i) = (*this)(i, j);
            }
        }
        *this = temp;
        return *this;
    }

    Matrix transpose() const {
        Matrix temp(_cols, _rows);
        for(unsigned i = 0; i < _rows; i++) {
            for(unsigned j = 0; j < _cols; j++) {
                temp(j, i) = (*this)(i, j);
            }
        }
        return temp;
    }

    void swapRows(unsigned i, unsigned j) {
        for(unsigned k = 0; k < _cols; k++) {
            std::swap((*this)(i, k), (*this)(j, k));
        }
    }

    void multiplyRow(unsigned i, T factor) {
        for(unsigned k = 0; k < _cols; k++) {
            (*this)(i, k) *= factor;
        }
    }

    void addRow(unsigned i, unsigned j, T factor) {
        for(unsigned k = 0; k < _cols; k++) {
            (*this)(i, k) += (*this)(j, k) * factor;
        }
    }
};

template<typename T>
T determinant(const Matrix<T>& mat) {
    unsigned n = mat.rows();
    Matrix<T> m = mat;
    T det = 1;
    int sign = 1;
    
    for(unsigned i = 0; i < n; i++) {
        unsigned max_row = i;
        for(unsigned j = i + 1; j < n; j++) {
            if(std::abs(m(j, i)) > std::abs(m(max_row, i))) {
                max_row = j;
            }
        }
        
        if(std::abs(m(max_row, i)) < 1e-12) {
            return 0;
        }
        
        if(max_row != i) {
            m.swapRows(i, max_row);
            sign = -sign;
        }
        
        det *= m(i, i);
        
        for(unsigned j = i + 1; j < n; j++) {
            T factor = m(j, i) / m(i, i);
            for(unsigned k = i + 1; k < n; k++) {
                m(j, k) -= factor * m(i, k);
            }
        }
    }
    
    return det * sign;
}

template<typename T>
std::vector<T> gaussianElimination(Matrix<T> A, std::vector<T> b) {
    unsigned n = A.rows();
    
    for(unsigned i = 0; i < n; i++) {
        unsigned pivot = i;
        for(unsigned j = i + 1; j < n; j++) {
            if(std::abs(A(j, i)) > std::abs(A(pivot, i))) {
                pivot = j;
            }
        }
        
        if(pivot != i) {
            A.swapRows(i, pivot);
            std::swap(b[i], b[pivot]);
        }
        
        for(unsigned j = i + 1; j < n; j++) {
            T factor = A(j, i) / A(i, i);
            for(unsigned k = i; k < n; k++) {
                A(j, k) -= factor * A(i, k);
            }
            b[j] -= factor * b[i];
        }
    }
    
    std::vector<T> x(n, 0);
    for(int i = n - 1; i >= 0; i--) {
        T sum = b[i];
        for(unsigned j = i + 1; j < n; j++) {
            sum -= A(i, j) * x[j];
        }
        x[i] = sum / A(i, i);
    }
    
    return x;
}

template<typename T>
T vectorNorm(const std::vector<T>& vec) {
    T sum = 0;
    for(const auto& val : vec) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

template<typename T>
std::vector<T> vectorSubtract(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<T> result(a.size());
    for(unsigned i = 0; i < a.size(); i++) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template<typename T>
std::vector<T> operator*(const Matrix<T>& mat, const std::vector<T>& vec) {
    std::vector<T> result(mat.rows(), 0);
    for(unsigned i = 0; i < mat.rows(); i++) {
        for(unsigned j = 0; j < mat.cols(); j++) {
            result[i] += mat(i, j) * vec[j];
        }
    }
    return result;
}

#endif