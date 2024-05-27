#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

ifstream in("input.in");

struct cmp {
	bool operator()(pair<string, int> a, pair<string, int> b) {
		if (a.second == b.second) {
			return a.first > b.first;
		}
		return a.second < b.second;
	}
};
bool delim(char, string);


int main(){
	
	string s;
	string exp = ".,?! ";
	vector<string> v;

	getline(in, s);

	transform(s.begin(), s.end(), s.begin(), ::tolower);

	int l = 0;
	for (int i = 0; i < s.length(); i++) {
		char c = s.at(i);
		
		if (delim(c, exp)) {
			string sub = s.substr(l, i - l);
			l = i + 1;
			if (sub.length() > 0) {
				v.push_back(sub);
			}
		}
	}

	vector<string> uniq;
	map<string, int> mp;


	for (auto str : v) {
		if (mp[str] == 0) {
			uniq.push_back(str);
		}
		mp[str]++;
	}

	vector<pair<string, int>> fr;
	for (auto str : uniq) {
		fr.push_back(make_pair(str, mp[str]));
	}
	
	priority_queue<pair<string, int>, vector<pair<string, int>>, cmp> pq;
	for (auto pr : fr) {
		pq.push(pr);
	}

	while (!pq.empty()) {
		cout << pq.top().first << "\n";
		pq.pop();
	}

	return 0;
}

bool delim(char c, string exp) {
	for (auto i : exp) {
		if (c == i) {
			return true;
		}
	}
	return false;
}