#include <iostream>
#include <cmath>
using namespace std;

union FloatUnion {
    float value;
    struct {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
    unsigned int bits;
};

void floatBits(float val) {
    cout << "1-------------------------------------------------------------" << endl;

    FloatUnion u;
    u.value = val;
    cout << "Двоичный вид: " << endl;
    
    for (int i = 31; i >= 0; i--) {
        cout << ((u.bits >> i) & 1);
        if (i == 31 || i == 23) {
            cout << " ";
        }
    }
    cout << endl;
    
    cout << "Знак (1 бит): " << u.parts.sign << endl;
    
    cout << "Экспонента (8 бит): ";
    for (int i = 7; i >= 0; i--) {
        cout << ((u.parts.exponent >> i) & 1);
    }
    cout << " (в десятичном виде: " << u.parts.exponent << ")" << endl;
    
    cout << "Мантисса (23 бита): ";
    for (int i = 22; i >= 0; i--) {
        cout << ((u.parts.mantissa >> i) & 1);
    }
    cout << endl;

    if (u.parts.exponent == 0 && u.parts.mantissa == 0) {
        cout << "Значение: ";
        if (u.parts.sign == 1) {
            cout << "-0";
        } else {
            cout << "+0";
        }
        cout << endl;
    } else {
        int realExp = u.parts.exponent - 127;
        
        double mantissaValue = 1.0;
        for (int i = 0; i < 23; i++) {
            if (u.parts.mantissa & (1 << (22 - i))) {
                mantissaValue += pow(2.0, -(i + 1));
            }
        }
        
        float result;
        if (u.parts.sign == 1) {
            result = -mantissaValue * pow(2.0, realExp);
        } else {
            result = mantissaValue * pow(2.0, realExp);
        }
        
        cout << "Расчет: ";
        if (u.parts.sign == 1) {
            cout << "-";
        }
        cout << mantissaValue;
        cout << " * 2^" << realExp;
        cout << " = " << result;
        cout << endl;
    }
    cout << endl;
}

void distance() {
    cout << "2-------------------------------------------------------------" << endl;

    float a = pow(2.0f, 23);
    float b = a + 1.0f;
    
    cout << "a = 2^23 = " << a << endl;
    cout << "b = a + 1 = " << b << endl;
    cout << "Разность: " << b - a << endl;
    
    float x0 = pow(2.0f, 24);
    
    cout << "x0 = 2^24 = " << x0 << endl;
    cout << "Проверяем: " << x0 << " == " << x0 << " + 1 ? " << (x0 == x0 + 1.0f) << endl;
    cout << endl;
}

void associativity() {
    cout << "3-------------------------------------------------------------" << endl;
    
    float a = 1000000000.0f;
    float b = -1000000000.0f;
    float c = 1.0f;
    
    float result1 = (a + b) + c;
    float result2 = a + (b + c);

    cout << "При a = 1000000000, b = -1000000000, c = 1" << endl;
    cout << "(a + b) + c = " << result1 << endl;
    cout << "a + (b + c) = " << result2 << endl;
    cout << "Ассоциативность нарушена: " << (result1 != result2) << endl;
    cout << endl;
}

void harmonicLimit() {
    cout << "4-------------------------------------------------------------" << endl;
    
    float sum = 0.0f;
    int k = 1;
    FloatUnion current;
    FloatUnion previous;
    
    while (true) {
        float term = 1.0f / k;
        
        previous.value = sum;
        sum += term;
        current.value = sum;
        
        if (current.bits == previous.bits) {
            cout << "При k = " << k << " сумма не увеличивается" << endl;
            cout << "Остановился на значении 1/" << k << " = " << term << endl;
            cout << "Текущая сумма: " << sum << endl;
            cout << "Следующая сумма: " << sum + term << endl;
            break;
        }
        
        k++;
        
        if (k > 10000000) { 
            cout << "Прервано на k = " << k << endl;
            break;
        }
    }
    cout << endl;
}

void harmonicSum() {
    cout << "5-------------------------------------------------------------" << endl;
    
    int limit_k = 2097152;
    int start = 1;
    int end = limit_k + 100;

    float sumForward = 0.0f;
    for (int i = start; i <= end; ++i) {
        sumForward += 1.0f / i;
    }

    float sumBackward = 0.0f;
    for (int i = end; i >= start; --i) {
        sumBackward += 1.0f / i;
    }

    cout << "Прямое суммирование: " << sumForward << endl;
    cout << "Обратное суммирование: " << sumBackward << endl;
    cout << "Разница: " << sumBackward - sumForward << endl;
    
    FloatUnion forwardUnion;
    FloatUnion backwardUnion;
    forwardUnion.value = sumForward;
    backwardUnion.value = sumBackward;
    
    cout << "Битовое представление прямой суммы: ";
    for (int i = 31; i >= 0; i--) {
        cout << ((forwardUnion.bits >> i) & 1);
    }
    cout << endl;
    
    cout << "Битовое представление обратной суммы: ";
    for (int i = 31; i >= 0; i--) {
        cout << ((backwardUnion.bits >> i) & 1);
    }
    cout << endl;
    cout << endl;
}

void safeComparison() {
    cout << "6-------------------------------------------------------------" << endl;
    
    float a = 0.1f;
    float b = 0.2f;
    float c = 0.3f;
    
    cout << "a = 0.1, b = 0.2, c = 0.3" << endl;
    cout << "a + b = " << a + b << endl;
    cout << "c = " << c << endl;
    cout << "a + b == c: " << ((a + b) == c) << endl;
    cout << "Опасно сравнивать после арифметических операций!" << endl;
    
    cout << "\nБезопасное сравнение с допуском:" << endl;
    float epsilon = 1e-6f;
    bool safeEqual = fabs((a + b) - c) < epsilon;
    cout << "|(a + b) - c| < " << epsilon << ": " << safeEqual << endl;
    
    cout << "\nСравнение без арифметических операций:" << endl;
    float x = 1.5f, y = 1.5f;
    cout << "x = " << x << ", y = " << y << endl;
    cout << "x == y: " << (x == y) << endl;
    cout << "Безопасно без операций" << endl;
}

int main() {
    // 1. Двоичное представление
    floatBits(6.5f);
    
    // 2. Числа с расстоянием 1
    distance();
    
    // 3. Отсутствие ассоциативности
    associativity();
    
    // 4. Сходимость гармонического ряда
    harmonicLimit();
    
    // 5. Суммирование гармонического ряда
    harmonicSum();
    
    // 6. Сравнение float
    safeComparison();
    
    return 0;
}