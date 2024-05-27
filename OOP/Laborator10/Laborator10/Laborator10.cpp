#include <iostream>
#include <vector>
#include <string>
#include "RawVector.h"
using namespace std;


void Sort(vector<string>& v, bool(*cmp)(string a, string b));

int main() {
    vector<string> v;
    v.push_back("hkdsag");
    v.push_back("dagkhk");
    v.push_back("ljdasj");
    v.push_back("agbfhk");

    Sort(v, [](string a, string b) {
        if (a.length() != b.length())
            return a.length() > b.length();
        return a > b;
        }
    );

    for (int i = 0; i < v.size(); i++) {
        cout << v.at(i) << "\n";
    }

    //

    int n = 20;
    int *a = new int[20];
    for (int i = 0; i < n; i++) {
        a[i] = rand() % n;
    }

    auto getMax = [](int *v, int n) -> int {
        int max = v[0];
        for (int i = 0; i < n; i++) {
            if (max < v[i]) {
                max = v[i];
            }
        }
        return max;
    };

    int max = getMax(a, n);
    cout << max<<"\n";

    //
    RawVector rv(20);
    for (int i = 0; i < 15; i++) {
        rv.Add(i);
    }
    rv.Delete(3);
    rv.Iterate([](int& x) -> void { x *= 2; });
    rv.PrintAll();
    rv.Filter([](int x) -> bool {return x % 4 == 0; });
    rv.PrintAll();
    cout << "\nDone";
    return 0;
}

void Sort(vector<string>& v, bool(*cmp)(string a, string b)) {
    for (int i = 0; i < v.size() - 1; i++) {
        for (int j = 0; j < v.size() - i - 1; j++) {
            if (cmp(v.at(j), v.at(j + 1))) {
                string aux = v.at(j);
                v.at(j) = v.at(j + 1);
                v.at(j + 1) = aux;
            }
        }
    }
}
