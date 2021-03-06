#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include "bstmap.h"

using namespace std;

class Autocomplete {
public:
  Autocomplete() = default;
  void readFile(const string &fileName);
  vector<BSTMap::value_type> complete(const string &prefix) const;

private:
  // BST of phrases and their frequency
  BSTMap phrases;
};

#endif // AUTOCOMPLETE_H
