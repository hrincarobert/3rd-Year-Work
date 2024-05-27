#include <iostream>
#include "ArrayIterator.h"
#include "OutOfRangeException.h"
#include "Array.h"
#include "Array.cpp"
#include "Compare.h"
using namespace std;

class Person {
public:
    int age;
    const char* name;
    Person(int age, const char* name) : age(age), name(name) {}
    bool operator==(const Person& p) {
        return age == p.age;
    }
    bool operator<(const Person& p) {
        return age > p.age;
    }
    bool operator>(const Person& p) {
        return age > p.age;
    }
};

class Comp : public Compare {
public:
    virtual int CompareElements(void* e1, void* e2) override {
        return *(Person* )e1 < *(Person* )e2;
    }
};

int main() {
    Array<Person> arr(60);
    Array<Person> brr(30);

    for (int i = 0; i < 31; i++) {
        Person* p = new Person(i % 7, "dssjssadi");

        try {
            brr += *p;
        }
        catch (const char* mess) {
            cout << mess << "\n";
        }
 
    }
    arr = brr;
    Person *p = new Person(3, "dasj");
    try {
        arr.Insert(7, *p);
    }
    catch (exception& e) {
        cout << e.what()<<"\n";
    }
    
    try {
        arr.Delete(90);
    }
    catch(const char *mess) {
        cout << mess << "\n";
    }
    try {
        arr[60];
    }
    catch (exception& e) {
        cout << e.what()<<"\n";
    }

   cout << arr.BinarySearch(*p)<<"\n\n";
    
   Compare* cmm = new Comp();
   arr.Sort(cmm);

    for (auto x : arr) {
        cout << (*x).age<<"\n";
    }

    return 0;
}