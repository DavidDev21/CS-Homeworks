/*
  Vector Class
  Demonstrates:
     Good example of copy control
     Dynamic arrays and pointer arithmetic
     Square bracket operator
     Implementation of an important data structure
 */
#include <iostream>
#include <vector>
using namespace std;

class Vector {
public:
    typedef int* iterator;

    Vector() : theSize(0), theCapacity(0), data(nullptr) {
        cerr << "In Vector()\n";
    }

    explicit Vector(size_t n, int val = 0)
        : theSize(n), theCapacity(n), data(new int[n])
    {
        for (size_t i = 0; i < theSize; ++i) data[i] = val;
    }

    // Copy control
    ~Vector() { delete [] data; }
    Vector(const Vector& rhs)
        : theSize(rhs.theSize), theCapacity(rhs.theCapacity),
          data(new int[rhs.theCapacity]) {
        for (size_t i = 0; i < theCapacity; ++i) {
            data[i] = rhs.data[i];
        }
    }
    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            delete [] data;

            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;
            data = new int[rhs.theCapacity];
            for (size_t i = 0; i < theCapacity; ++i) {
                data[i] = rhs.data[i];
            }
        }
        return *this;
    }

    void push_back(int val) {
        if (theCapacity == 0) {
            data = new int[1];
            ++theCapacity;
        }
        if (theSize == theCapacity) {
            int* oldData = data;
            data = new int[2*theCapacity];
            theCapacity *= 2;
            for (size_t i = 0; i < theSize; ++i) {
                data[i] = oldData[i];
            }
            delete [] oldData;
        }

        // data[theSize] = val;
        // ++theSize;
        data[theSize++] = val;
    }

    size_t size() const { return theSize; }
    size_t capacity() const { return theCapacity; }

    void clear() { theSize = 0; }
    void pop_back() {
        if (theSize > 0) {
            --theSize;
        }
    }
    

    // Square brackets?
    int operator[](size_t index) const { return data[index]; }
    int& operator[](size_t index) { return data[index]; }
    
    int* begin() { return data; }
    int* end() { return data + theSize; }

private:
    size_t theSize;
    size_t theCapacity;
    int* data;
};

void testOurVector();

int main() {
    testOurVector();

    Vector v;  // Not templated.  Our Vector class can only hold ints.
    v.push_back(17);
    v.push_back(42);
    v.push_back(6);
    v.push_back(28);

    cout << "Ranged for with Vector\n";
    for (int x : v) {
        cout << x << ' ';
    }
    cout << endl;
    for (int* iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << ' ';
    }
    cout << endl;
    for (Vector::iterator iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << ' ';
    }
    cout << endl;

    Vector v2(v);
    Vector v3;
    v3 = v;

    for (size_t i = 0; i < v.size(); ++i)
        cout << v[i] << endl;
    cout << "================\n";
    v[0] = 100;
    for (size_t i = 0; i < v.size(); ++i)
        cout << "v[i]: " << v[i] << endl;
    cout << "================\n";

    //    v = 3;
    v = Vector(3);
    // for (size_t i = 0; i < v.size(); ++i)
    //     cout << "v[i]: " << v[i] << endl;

    vector<int> vi {2, 3, 5, 8, 13, 21};
    for (vector<int>::iterator iter = vi.begin(); iter != vi.end(); ++iter) {
        cout << *iter << ' ';
    }
    cout << endl;
}


void testOurVector() {
    cout << "Starting testOurVector()\n";
    Vector v;
    cout << "Empty vector: sizeof(v) = " << sizeof(v)
         << ", v.capacity() = " << v.capacity()
         << ", v.size() = " << v.size() << endl;
    for (int i = 1; i < 20; ++i) {
        v.push_back(i);
        cout << "v.push_back(" << i << "): "
             << "sizeof(v) = " << sizeof(v)
         << ", v.capacity() = " << v.capacity()
         << ", v.size() = " << v.size() << endl;
    }
    cout << "Ending testOurVector()\n";
}

