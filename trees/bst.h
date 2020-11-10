//
//  bst.h
//  sqb
//
//  Created by William McCarthy on 03//11//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef bst_h
#define bst_h


#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>


template <typename Key, typename Value>
class bst {
private:
  struct node {
    Key key;             // sorted by key
    Value val;           // associated data
    node* left;
    node* right;         // left and right subtrees
    int size;            // number of nodes in subtree

    node(Key key_, Value value, int size_)
    : key(key_), val(value), size(size_), left(nullptr), right(nullptr) {
//      std::cout << "new node is: " << *this << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const node& no) {
      return os << no.left << " <-- "
                << "(" << no.key << "," << no.val << " (" << &no << ")) --> "
                << no.right << "\n";
    }
  };

  node* root;

public:
  bst() : root(nullptr) { }

  bool empty() { return size() == 0; }

public:
  int size()   { return size(root); }
private:
  int size(node* x) { return x == nullptr ? 0 : x->size; }

public:
  bool contains(Key& key) {
    if (key == Key()) { throw new std::invalid_argument("argument to contains() is null"); }
    return get(key) != nullptr;
  }

public:
  Value get(Key& key) { return get(root, key); }
private:
  Value get(node* x, Key& key) {
    if (key == Key()) { throw new std::invalid_argument("calls get() with a null key"); }
    if (x->key == Key()) { return Value(); }
    if      (less(key,    x->key)) { return get(x->left,  key); }
    else if (less(x->key, key))    { return get(x->right, key); }
    else                           { return x->val; }
  }

public:
  void put(Key& key, Value val) {
    if (key == Key()) { std::cerr << "..... warning: calling put() with a null key" << "\n";  return; }
//    if (val == Value()) {
//      delete_key(key);
//      return;
//    }
    root = put(root, key, val);
    assert(check());
  }
private:
  node* put(node* x, Key key, Value val) {
    if (x == nullptr) { return new node(key, val, 1); }

    if      (less(key,    x->key)) { x->left  = put(x->left,  key, val); }
    else if (less(x->key, key))    { x->right = put(x->right, key, val); }
    else                           { x->val   = val; }
    x->size = 1 + size(x->left) + size(x->right);
    return x;
  }

public:
  void delete_min() {
    if (empty()) { throw new std::logic_error("Symbol table underflow"); }
    root = deleteMin(root);
    assert(check());
  }
private:
  node* delete_min(node* x) {
    if (x->left == nullptr) { return x->right; }
    x->left = delete_min(x->left);
    x->size = size(x->left) + size(x->right) + 1;
    return x;
  }

public:
  void delete_max() {
    if (empty()) { throw new std::logic_error("Symbol table underflow"); }
    root = deleteMax(root);
    assert(check());
  }
private:
  node* delete_max(node* x) {
    if (x->right == nullptr) { return x->left; }
    x->right = delete_max(x->right);
    x->size = size(x->left) + size(x->right) + 1;
    return x;
  }

public:
  void delete_key(Key& key) {
    if (key == Key()) { throw new std::invalid_argument("calls delete() with a null key"); }
    root = delete_key(root, key);
    assert(check());
  }

private:
  node* delete_key(node* x, Key& key) {
    if (x == nullptr) { return nullptr; }

    if      (less(key,    x->key)) { x->left  = delete_key(x->left,  key); }
    else if (less(x->key, key))    { x->right = delete_key(x->right, key); }
    else {
      if (x->right == nullptr) { return x->left; }
      if (x->left  == nullptr) { return x->right; }
      node* t = x;
      x = min(t->right);
      x->right = delete_min(t->right);
      x->left = t->left;
    }
    x->size = size(x->left) + size(x->right) + 1;
    return x;
  }

public:
  Key min() {
    if (empty()) { throw new std::logic_error("calls min() with empty symbol table"); }
    return min(root)->key;
  }
private:
  node* min(node* x) { return x->left  == nullptr ? x : min(x->left);  }

public:
  Key max() {
    if (empty()) { throw new std::logic_error("calls max() with empty symbol table"); }
    return max(root)->key;
  }
private:
  node* max(node* x) { return x->right == nullptr ? x : max(x->right); }

public:
  Key floor(Key& key) {
    if (key == Key())   { throw new std::invalid_argument("argument to floor() is null"); }
    if (empty())        { throw new std::logic_error("calls floor() with empty symbol table"); }
    node x = floor(root, key);
    if (x == nullptr)   { throw new std::logic_error("argument to floor() is too small"); }
    else { return x->key; }
  }
private:
  node floor(node* x, Key key) {
    if (x == nullptr) { return nullptr; }

    bool key_less = less(key,    x->key);
    bool key_more = less(x->key, key);

    if (key_less && key_more) { return x; }  // equal
    if (key_less)             { return floor(x->left, key); }

    node* t = floor(x->right, key);
    if (t != nullptr) { return t; }
  }

public:
  Key floor2(Key& key) {
    Key x = floor2(root, key, nullptr);
    if (x == nullptr) { throw new std::logic_error("argument to floor() is too small"); }
    else return x;
  }
private:
  Key floor2(node* x, Key key, Key best) {
    if (x == nullptr) { return best; }

    int cmp = key.compareTo(x->key);
    if      (cmp  < 0) { return floor2(x->left, key, best); }
    else if (cmp  > 0) { return floor2(x->right, key, x->key); }
    else               { return x->key; }
  }

public:
  Key ceiling(Key& key) {
    if (key == Key())   { throw new std::invalid_argument("argument to ceiling() is null"); }
    if (empty())        { throw new std::logic_error("calls ceiling() with empty symbol table"); }
    node x = ceiling(root, key);
    if (x == Key())     { throw new std::logic_error("argument to floor() is too large"); }
    else { return x->key; }
  }
private:
  node* ceiling(node* x, Key key) {
    if (x == Key()) { return nullptr; }

    bool key_less = less(key,    x->key);
    bool key_more = less(x->key, key);

    if (key_less && key_more) { return x; }  // equal
    if (key_less) {
      node* t = ceiling(x->left, key);
      return t != nullptr ? t : x;
    }
    return ceiling(x->right, key);
  }

public:
  Key select(int rank) {
    if (rank < 0 || rank >= size()) {
      std::cerr << "argument to select() is invalid: " << rank << "\n";
      throw new std::invalid_argument("invalid select");
    }
    return select(root, rank);
  }
private:
  Key select(node* x, int rank) {
    if (x == nullptr) { return nullptr; }

    int leftSize = size(x->left);
    if      (leftSize > rank) { return select(x->left,  rank); }
    else if (leftSize < rank) { return select(x->right, rank - leftSize - 1); }
    else                      { return x->key; }
  }

public:
  int rank(Key& key) {
    if (key == Key()) { throw new std::invalid_argument("argument to rank() is null"); }
    return rank(key, root);
  }
private:
  int rank(Key key, node* x) {
    if (x == nullptr) { return 0; }

    if      (less(key,    x->key)) { return rank(key, x->left); }
    else if (less(x->key, key))    { return 1 + size(x->left) + rank(key, x->right); }
    else                           { return size(x->left); }
  }

public:
  void keys(node* x, queue_<Key>& q, Key low, Key high) {
    if (x == nullptr) { return; }

    bool low_le  = less(low, x->key)  || low  == x->key;
    bool high_ge = less(x->key, high) || high == x->key;

    if (low_le)             { keys(x->left, q, low, high); }   // le == less than or equal to (a la Python)
    if (low_le && high_ge)  { q.enqueue(x->key); }             // ge == greater than or equal to
    if (high_ge)            { keys(x->right, q, low, high); }
  }

  array_queue<Key> keys() {
    if (empty()) { return array_queue<Key>(); }
    Key min_key = min(), max_key = max();
    return keys(min_key, max_key);
  }

  array_queue<Key> keys(Key& low, Key& high) {
    if (low == Key())  { throw new std::invalid_argument("first argument to keys() is null"); }
    if (high == Key()) { throw new std::invalid_argument("second argument to keys() is null"); }

    array_queue<Key> q;
    keys(root, q, low, high);
    return q;
  }

  int size(Key& low, Key& high) {
    if (low == Key())  { throw new std::invalid_argument("first argument to size() is null"); }
    if (high == Key()) { throw new std::invalid_argument("second argument to size() is null"); }

    if (low.compareTo(high) > 0) { return 0; }
    if (contains(high)) { return rank(high) - rank(low) + 1; }
    else                { return rank(high) - rank(low); }
  }

public:
  int height() { return height(root); }
private:
  int height(node* x) {
    if (x == nullptr) { return -1; }
    return 1 + std::max(height(x->left), height(x->right));
  }

public:
  void print_inorder() {
    std::cout << "========================================================================= printing inorder...\n";
    print_inorder(root);
    std::cout << "========================================================================= end printing inorder...\n\n";
  }
private:
  void print_inorder(node* x) {
    if (x == nullptr) { return; }
    print_inorder(x->left);
    std::cout << *x;
    print_inorder(x->right);
  }

private:
  //-------- bst validity checks --------------------------------------------------
  bool check() {
    if (!is_bst())             { std::cerr << "Not in symmetric order\n";         return false;  }
    if (!is_size_consistent()) { std::cerr << "Subtree counts not consistent\n";  return false;  }
    if (!is_rank_consistent()) { std::cerr << "Ranks not consistent\n";           return false;  }
    return true;
  }
  bool is_bst() {
    static Key default_key = Key();
//    print_inorder();

    return is_bst(root, default_key, default_key);
  }
  bool is_bst(node* x, Key& min, Key& max) {
    if (x == nullptr) { return true; }
    Key default_key = Key();

    if (min != default_key && less(x->key, min) <= 0) { return false; }
    if (max != default_key && less(max,    x->key))   { return false; }
    bool left_bst = is_bst(x->left, min, x->key);
    bool right_bst = is_bst(x->left, min, x->key);
    return left_bst && right_bst;
  }

  bool is_size_consistent() { return is_size_consistent(root); }
  bool is_size_consistent(node* x) {
    if (x == nullptr) { return true; }
    if (x->size != size(x->left) + size(x->right) + 1) { return false; }
    return is_size_consistent(x->left) && is_size_consistent(x->right);
  }

  bool is_rank_consistent() {
    for (int i = 0; i < size(); i++) {
      Key key_selected = select(i);
      if (i != rank(key_selected)) { return false; }
    }
    for (Key& key : keys()) {
      Key key_at_rank = select(rank(key));
      if (less(key, key_at_rank) || less(key_at_rank, key)) {  // if !equal
        return false;
      }
    }
    return true;
  }

  array_queue<Key> level_order() {
    array_queue<Key> keys;
    array_queue<node*> q;

    q.enqueue(root);
    while (!q.empty()) {
      node* x = q.dequeue();
      if (x == nullptr) { continue; }

      keys.enqueue(x->key);
      q.enqueue(x->left);
      q.enqueue(x->right);
    }
    return keys;
  }

public:
  static void test_bst(const std::string& filename) {
    char buf[BUFSIZ];
    bst<std::string, int> st;

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
      std::cerr << "Could not open file: '" << filename << "'\n";  exit(2);
    }

    std::cout << "Building symbol table for file: '" << filename << "'. \n";
    std::cout << "(punctuation automatically stripped)... \n";
    int i = 0;
    std::string s;
    while (ifs >> s) {
      strcpy(buf, s.c_str());
      strconvert(buf, tolower);
      strstrip(buf);
      std::string key = std::string(buf);
      if (key != "") { st.put(key, i++); }
//      st.print_inorder();
    }
    std::cout << "\n";

    std::cout << "\nin level order (root to leaves)...\n";
    array_queue<Key> keys = st.level_order();
    for (std::string& key : keys) {
      std::cout << std::setw(12) << key << "  " << std::setw(2) << st.get(key) << "\n";
    }

    std::cout << "\nin alphabetical order...\n";
    keys = st.keys();
    for (std::string& key : keys) {
      std::cout << std::setw(14) << key << "  " << std::setw(2) << st.get(key) << "\n";
    }
  }
};



#endif /* bst_h */
