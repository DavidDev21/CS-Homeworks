/*
   Arithmetic Operator Overloading
   Sec 
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Complex {
    friend ostream& operator<<(ostream& os, const Complex& rhs) {
        os << rhs.real;
        if (rhs.imaginary >= 0) os << '+';
        os << rhs.imaginary << 'i';
        return os;
    }
    friend bool operator==(const Complex& lhs, const Complex& rhs) {
        return lhs.real == rhs.real && lhs.imaginary == rhs.imaginary;
    }
public:
    // Complex() : real(0), imaginary(0) {}
    // Complex(double real) : real(real), imaginary(0) {}
    // Complex(double real, double imag) : real(real), imaginary(imag) {}
    Complex(double real = 0, double imag = 0) : real(real), imaginary(imag) {}

    void foo(const Complex& lhs, const Complex& rhs) const {
        if (lhs == rhs) {}
        if (*this == rhs) {}
    }

    Complex& operator+=(const Complex& rhs) {
        real += rhs.real;
        imaginary += rhs.imaginary;
        return *this;
    }

    // ++c
    Complex& operator++() {
        ++real;
        return *this;
    }

    // c++
    Complex operator++(int) {
        Complex old = *this;
        ++real;
        return old;
    }

    // if (c1) ...
    explicit operator bool() const {
        return real != 0 || imaginary != 0;
    }

private:
    double real, imaginary;
};

bool operator!=(const Complex& lhs, const Complex& rhs) {
    return !(lhs == rhs);
}

Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex result = lhs;
    result += rhs;
    return result;
}

int main() {
    Complex c1;         // 0+0i
    Complex c2(17);     // 17+0i
    Complex c3(3, -5);  // 3-5i
    cout << "c1: " << c1 << endl
         << "c2: " << c2 << endl
         << "c3: " << c3 << endl;
    // c1 = c2 + c3;
    // c1 += c2;

    ++c1;
    c1.operator++();

    c1++;
    c1.operator++(0);

    if (c1 == 5) {}
    if (5 == c1) {}

    if (c1) {

    }

    cout << c1 << endl;
    c1++ ++;
    cout << c1 << endl;
}
