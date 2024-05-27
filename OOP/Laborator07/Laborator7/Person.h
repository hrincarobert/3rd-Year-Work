#pragma once
class Person {
public:
    char* name;
    int age;
    int index;
    Person(char* s, int age, int i);
    void showPerson();
    bool operator< (const Person& p);
    bool operator== (const Person& p);
};