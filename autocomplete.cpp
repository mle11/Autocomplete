#include "autocomplete.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void testBSTAll();

// helper function for the complete function, will sort the results of the
// vector in descending order
void Insertion(vector<BSTMap::value_type> &list, int a, int b) {
  for (int i = a; i <= b; i++) {
    BSTMap::value_type b = list[i];
    int c = i - 1;
    while (c >= a && list[c].second < b.second) {
      list[c + 1] = list[c];
      c--;
    }
    list[c + 1] = b;
  }
}

// takes in a file, and will parse the file by weight and city
void Autocomplete::readFile(const string &fileName) {
  ifstream ifs;
  ifs.open(fileName);
  if (!ifs.is_open()) {
    cout << "Cannot open!" << endl;
    exit(1);
  }
  string temp;
  getline(ifs, temp);
  while (!ifs.eof()) {
    string text;
    getline(ifs, text);
    istringstream ss(text);
    vector<string> list;
    string temp2;
    while (ss >> temp2) {
      list.push_back(temp2);
    }
    if (!list.empty()) {
      string city;
      for (int i = 1; i < list.size(); i++) {
        city += list[i];
        city += ((i == list.size() - 1) ? "" : " ");
      }
      int num = stoi(list[0]);
      phrases[city] = num;
    }
  }
  ifs.close();
  phrases.rebalance();
  // cout << phrases << endl;
}

// performs the autocomplete using insertion and getAll functions
vector<BSTMap::value_type>
Autocomplete::complete(const BSTMap::key_type &prefix) const {
  vector<BSTMap::value_type> list1;
  vector<BSTMap::value_type> list2 = phrases.getAll(prefix);
  for (int i = 0; i < list2.size(); i++) {
    string a = list2[i].first.substr(0, prefix.size());
    if (a == prefix) {
      list1.push_back(list2[i]);
    }
  }
  Insertion(list1, 0, list1.size() - 1);
  return list1;
}
