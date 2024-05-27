#include <iostream>
#include <stdio.h>
#include <functional>
#include "MyIterator.h"
#include "Node.h"
#include "Node.cpp"
#include "List.h"
#include "List.cpp"
#include "Map.h"
#include "Map.cpp"

using namespace std;



int main()
{
    Map<int, const char*> m;
    m[10] = "C++";
    m[20] = "test";
    m[20] = "Previous one is deleted";
    m[30] = "Poo";
    for (auto [key, value, index] : m)
    {
        printf("Index:%d, Key=%d, Value=%s\n", index, key, value);
    }
    m[20] = "result";
    m.Delete(20);

    cout <<"\n"<< m.Get(10, "Java")<<"\n";
    for (auto [key, value, index] : m)
    {
        printf("Index:%d, Key=%d, Value=%s\n", index, key, value);
    }


    m.Clear();
    cout << "Map Cleared\n";
    for (auto [key, value, index] : m)
    {
        printf("Index:%d, Key=%d, Value=%s\n", index, key, value);
    }
    m[20] = "result";
    for (auto [key, value, index] : m)
    {
        printf("Index:%d, Key=%d, Value=%s\n", index, key, value);
    }

    m.Clear();
    return 0;
}

//int main() {
//
//    Map<int, const char*> map;
//    map[10] = "Ana";
//    map[20] = "are";
//    map[30] = "mere";
//    for (int i = 0; i < 11; i++) {
//        char* s = new char[2];
//        s[0] = 'a' + i;
//        s[1] = '\0';
//        map.Set(10 * i, s);
//    }
//
//
//    map.printAll();
//    cout << "\n\n" << map.Count();
//
//    return 0;
//}