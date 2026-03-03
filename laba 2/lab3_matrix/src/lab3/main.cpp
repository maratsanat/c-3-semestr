#include "matrix.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

int main() {
    std::cout << std::fixed;
    std::cout.precision(6);
    
    Matrix<double> m1(10, 10);
    
    std::cout << "=== Test 1: 5x5 matrix ===" << std::endl;
    Matrix<double> m5 = Matrix<double>::getSpecificDeterminant(5, 100.0);
    double det5 = determinant(m5);
    std::cout << "Determinant: " << det5 << std::endl;
    Matrix<double> m5t = m5.transpose();
    double det5t = determinant(m5t);
    std::cout << "Determinant of transposed: " << det5t << std::endl;
    std::cout << "Difference: " << std::abs(det5 - det5t) << std::endl << std::endl;
    
    std::cout << "=== Test 2: 50x50 matrix ===" << std::endl;
    Matrix<double> m50 = Matrix<double>::getSpecificDeterminant(50, 1000.0);
    double det50 = determinant(m50);
    std::cout << "Determinant: " << det50 << std::endl;
    Matrix<double> m50t = m50.transpose();
    double det50t = determinant(m50t);
    std::cout << "Determinant of transposed: " << det50t << std::endl;
    std::cout << "Difference: " << std::abs(det50 - det50t) << std::endl << std::endl;
    
    std::cout << "=== Test 3: 200x200 matrix ===" << std::endl;
    Matrix<double> m200 = Matrix<double>::getSpecificDeterminant(200, 10000.0);
    double det200 = determinant(m200);
    std::cout << "Determinant: " << det200 << std::endl;
    Matrix<double> m200t = m200.transpose();
    double det200t = determinant(m200t);
    std::cout << "Determinant of transposed: " << det200t << std::endl;
    std::cout << "Difference: " << std::abs(det200 - det200t) << std::endl << std::endl;
    
    std::cout << "=== Gaussian elimination test ===" << std::endl;
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis(-10.0, 10.0);
    
    unsigned n = 100;
    Matrix<double> A(n, n);
    for(unsigned i = 0; i < n; i++) {
        for(unsigned j = 0; j < n; j++) {
            A(i, j) = dis(gen);
        }
        A(i, i) += 20.0;
    }
    
    std::vector<double> b(n);
    for(unsigned i = 0; i < n; i++) {
        b[i] = dis(gen);
    }
    
    std::vector<double> x = gaussianElimination(A, b);
    std::vector<double> Ax = A * x;
    std::vector<double> residual = vectorSubtract(Ax, b);
    double norm = vectorNorm(residual);
    
    std::cout << "System size: " << n << "x" << n << std::endl;
    std::cout << "Residual norm ||Ax - b||: " << norm << std::endl;
    
    return 0;
}