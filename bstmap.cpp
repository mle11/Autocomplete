// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <cassert>

using namespace std;

// copy constructor
BSTMap::BSTMap(const BSTMap &bst) {
  Node *temp = new Node(nullptr, nullptr, bst.root->data);
  this->root = temp;
  copyHelper(this->root, bst.root);
}

// helper function for the copy constructor
void BSTMap::copyHelper(Node *a, Node *b) {
  if (b->left == nullptr && b->right == nullptr) {
    return;
  }
  if (b->left != nullptr) {
    Node *temp = new Node(nullptr, nullptr, b->left->data);
    a->left = temp;
    copyHelper(a->left, b->left);
  }
  if (b->right != nullptr) {
    Node *temp = new Node(nullptr, nullptr, b->right->data);
    a->right = temp;
    copyHelper(a->right, b->right);
  }
}

// given an array of length n
// create a tree to have all items in that array
// with the minimum height (uses same helper as rebalance)
BSTMap::BSTMap(const vector<value_type> &v) {
  vector<value_type> nodes = v;
  int num = nodes.size() - 1;
  buildTree(root, nodes, 0, num);
}

BSTMap::Node::Node(Node *left, Node *right, value_type &pair) {
  this->data = pair;
  this->left = left;
  this->right = right;
}

// destructor
BSTMap::~BSTMap() { clear(); }

// delete all nodes in tree
void BSTMap::clear() {
  clearNodes(root);
  this->root = nullptr;
  totalNodes = 0;
}

void BSTMap::clearNodes(Node *n) {
  if (n == nullptr) {
    return;
  }
  clearNodes(n->left);
  clearNodes(n->right);
  delete n;
  n = nullptr;
}

// true if no nodes in BST
bool BSTMap::empty() const { return (root == nullptr); }

// Number of nodes in BST
int BSTMap::size() const { return totalNodes; }

// true if item is in BST
bool BSTMap::contains(const key_type &key) const {
  mapped_type map;
  return containsHelper(this->root, key, map);
}

// helper function for the contains function
bool BSTMap::containsHelper(Node *n, const key_type &key,
                            mapped_type &map) const {
  if (n == nullptr) {
    return false;
  }
  if (n->data.first == key) {
    map = n->data.second;
    return true;
  }
  return containsHelper(n->left, key, map) ||
         containsHelper(n->right, key, map);
}

// If k matches the key returns a reference to its value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const key_type &k) {
  if (this->root == nullptr) {
    value_type temp;
    temp.first = k;
    Node *temp2 = new Node(nullptr, nullptr, temp);
    this->root = temp2;
    return this->root->data.second;
  }
  Node *curr = this->root;
  while (curr != nullptr) {
    if (curr->data.first == k) {
      return curr->data.second;
    }
    if (k < curr->data.first) {
      if (curr->left == nullptr) {
        value_type temp;
        temp.first = k;
        Node *temp2 = new Node(nullptr, nullptr, temp);
        curr->left = temp2;
        return curr->left->data.second;
      }
      curr = curr->left;
    } else {
      if (curr->right == nullptr) {
        value_type temp;
        temp.first = k;
        Node *temp2 = new Node(nullptr, nullptr, temp);
        curr->right = temp2;
        return curr->right->data.second;
      }
      curr = curr->right;
    }
  }
  exit(1);
}

// retrieves all of the necessary values into the vector using a key_type
vector<BSTMap::value_type> BSTMap::getAll(const key_type &k) const {
  vector<value_type> v;
  this->getAllHelper(this->root, k, v);
  return v;
}

// helper function for the getAll function
void BSTMap::getAllHelper(Node *n, const key_type &k,
                          vector<value_type> &v) const {
  if (n == nullptr) {
    return;
  }
  this->getAllHelper(n->left, k, v);
  this->getAllHelper(n->right, k, v);
  if (n->data.first.find(k) != string::npos) {
    v.push_back(n->data);
  }
}

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const { return getHeight(this->root); }

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const Node *n) {
  if (n == nullptr) {
    return 0;
  }
  int l = getHeight(n->left);
  int r = getHeight(n->right);
  if (l > r) {
    return l + 1;
  }
  return r + 1;
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const string &k) const {
  if (this->contains(k)) {
    return 1;
  }
  return 0;
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const value_type &item)) const {
  this->inOrderHelper(this->root, visit);
}

// helper function for the inorder function
void BSTMap::inOrderHelper(Node *n, void visit(const value_type &item)) const {
  if (n == nullptr) {
    return;
  }
  inOrderHelper(n->left, visit);
  visit(n->data);
  inOrderHelper(n->right, visit);
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const value_type &item)) const {
  preOrderHelper(root, visit);
}

// helper function for the preOrder function
void BSTMap::preOrderHelper(Node *n, void visit(const value_type &item)) const {
  if (n == nullptr) {
    return;
  }
  visit(n->data);
  preOrderHelper(n->left, visit);
  preOrderHelper(n->right, visit);
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const value_type &item)) const {
  postOrderHelper(root, visit);
}

// helper function for the postOrder function
void BSTMap::postOrderHelper(Node *n,
                             void visit(const value_type &item)) const {
  if (n == nullptr) {
    return;
  }
  postOrderHelper(n->left, visit);
  postOrderHelper(n->right, visit);
  visit(n->data);
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  if (root == nullptr) {
    return;
  }
  vector<value_type> nodes;
  nodesToVector(nodes, root);
  this->clear();
  buildTree(this->root, nodes, 0, nodes.size() - 1);
}

// helper function for the rebalance function, puts the node in
// inorder traversal
void BSTMap::nodesToVector(vector<value_type> &nodes, Node *n) {
  if (n == nullptr) {
    return;
  }
  nodesToVector(nodes, n->left);
  nodes.push_back(n->data);
  nodesToVector(nodes, n->right);
}

// helper function for the rebalance function, creates a tree from the vector
// nodesToVector created
void BSTMap::buildTree(Node *temp, vector<value_type> &n, int begin, int end) {
  if (begin == end || end < 0) {
    (*this)[n[begin].first] = n[begin].second;
    return;
  }
  int mid = (begin + end) / 2;
  if (end - begin != 1) {
    (*this)[n[mid].first] = n[mid].second;
    buildTree(temp, n, begin, (mid - 1));
    buildTree(temp, n, (mid + 1), end);
  } else {
    (*this)[n[end].first] = n[end].second;
    (*this)[n[begin].first] = n[begin].second;
  }
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  if (this->root == nullptr && other.root == nullptr) {
    return true;
  }
  if (this->root == nullptr || other.root == nullptr) {
    return false;
  }
  return identicalHelper(root, other.root);
}

// helper function for the operator== function
bool BSTMap::identicalHelper(Node *a, Node *b) const {
  if (a == nullptr && b == nullptr) {
    return true;
  }
  if (a == nullptr || b == nullptr) {
    return false;
  }
  return (a->data == b->data) && (identicalHelper(a->left, b->left)) &&
         (identicalHelper(a->right, b->right));
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const {
  return (!(*this == other));
}