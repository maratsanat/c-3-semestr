#include <iostream>
#include <climits>
using namespace std;

void bits(int val) {
    union {
        int value;
        unsigned char bytes[sizeof(int)];
    } u;
    
    u.value = val;

    for (int i = sizeof(int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            cout << ((u.bytes[i] >> j) & 1);
        }
        cout << ' ';
    }
}

void unbits(unsigned int val) {
    union {
        unsigned int value;
        unsigned char bytes[sizeof(unsigned int)];
    } u;
    
    u.value = val;

    for (int i = sizeof(unsigned int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            cout << ((u.bytes[i] >> j) & 1);
        }
        cout << ' ';
    }
}

int main() {
    cout << "1. Побитовое представление int (положительные и отрицательные):" << endl;
    
    cout << "66:  ";
    bits(96);
    cout << endl;
    
    cout << "-66: ";
    bits(-96);
    cout << endl;
    
    cout << "1488:  ";
    bits(42);
    cout << endl;
    
    cout << "-1488: ";
    bits(-42);
    cout << endl;
    
    cout << "1:   ";
    bits(1);
    cout << endl;
    
    cout << "-1:  ";
    bits(-1);
    cout << endl;
    
    cout << "0:   ";
    bits(0);
    cout << endl;
    
    cout << "INT_MAX: ";
    bits(INT_MAX);
    cout << endl;
    
    cout << "INT_MIN: ";
    bits(INT_MIN);
    cout << endl;
    
    cout << "\n2. Побитовое представление unsigned int:" << endl;
    
    cout << "66:  ";
    unbits(66u);
    cout << endl;
    
    cout << "UINT_MAX/2: ";
    unbits(UINT_MAX / 2);
    cout << endl;
    
    cout << "UINT_MAX/2 + 1: ";
    unbits(UINT_MAX / 2 + 1);
    cout << endl;
    
    cout << "UINT_MAX: ";
    unbits(UINT_MAX);
    cout << endl;
    
    cout << "\nСравнение int и unsigned int для одного значения:" << endl;
    int signed_val = -1;
    unsigned int unsigned_val = static_cast<unsigned int>(signed_val);
    
    cout << "int -1:        ";
    bits(signed_val);
    cout << endl;
    
    cout << "unsigned int:  ";
    unbits(unsigned_val);
    cout << " (значение: " << unsigned_val << ")" << endl;
    
    cout << "\n3. Эксперименты с переполнением:" << endl;
    
    cout << "Прибавление 1 к INT_MAX:" << endl;
    int max_int = INT_MAX;
    cout << "INT_MAX:     ";
    bits(max_int);
    cout << " = " << max_int << endl;
    
    max_int += 1;
    cout << "INT_MAX + 1: ";
    bits(max_int);
    cout << " = " << max_int << " (INT_MIN!)" << endl;
    
    cout << "\nВычитание 1 из 0 (unsigned int):" << endl;
    unsigned int zero = 0;
    cout << "0:           ";
    unbits(zero);
    cout << " = " << zero << endl;
    
    zero -= 1;
    cout << "0 - 1:       ";
    unbits(zero);
    cout << " = " << zero << " (UINT_MAX!)" << endl;
    

    
    return 0;
}