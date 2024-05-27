#include "Person.h"
#include <iostream>
Person::Person(char* s, int age, int i) {
    this->age = age;
    name = s;
    index = i;
}
void Person::showPerson() {
    std::cout << "Person index: " << this->index << ".    Name: " << this->name << "    Age: " << this->age << "\n";
}
bool Person::operator< (const Person& p) {
    return this->name[0] < p.name[0];
}

bool Person::operator== (const Person& p) {
    return this->age == p.age;
}