#include <iostream>
#include "Sort.h"
using namespace std;

int cmp(int, int);

int main() {
    
    int v[10] = {9, 7, 6, 5, 10, 3, 2, 8, 1, 11};

    Sort a(10, 30, 30),
        b{ new int[8]{5, 2, 7, 6, 4, 8, 9 ,12}, 8 },
        c(v, 10),
        d(8, 10, 4, 3, 12, 1, 6, 4, 8), 
        e("8,5,3,2,5,12,14,19,3,34,54,65,3,4,6,8");

    cout << "Array Length : " << a.GetElementsCount() << "\n";
    a.InsertSort();
    a.Print();
    cout << "\n";

    cout << "Array Length : " << b.GetElementsCount() << "\n";
    b.QuickSort();
    b.Print();
    cout << "\n";

    cout << "Array Length : " << c.GetElementsCount() << "\n";
    c.BubbleSort();
    c.Print();
    cout << "\n";

    cout << "Array Length : " << d.GetElementsCount() << "\n";
    d.BubbleSort(true);
    d.Print();
    cout << "\n";

    cout << "Array Length : " << e.GetElementsCount() << "\n";
    e.QuickSort(true);
    e.Print();
    cout << "\n";

    return 0;
}

