#ifndef bst_red_black_h
#define bst_red_black_h


#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>


template <typename Key, typename Value>
class bst_red_black {
private:
  static const bool RED = true;
  static const bool BLACK = false;

  struct node {
    Key key;
    Value val;
    node* left;
    node* right;
    bool color;
    size_t size;

    node(Key key_, Value val_, bool color_, size_t size_)
    : key(key_), val(val_), color(color_), size(size_), left(nullptr), right(nullptr) {

    }
    friend std::ostream& operator<<(std::ostream& os, const node& no) {
      return os << no.left << " <-- "
                << "(" << no.key << "," << no.val << " (" << &no << ")) --> "
                << no.right << "\n";
    }

  public:
    bool col() { return color; };
  };

  node* root;

  bool isRed(node* x) {
      if(x == nullptr) { return false; }
      return (x->color == RED);
  }

public:
  bst_red_black() : root(nullptr) { }
  bool empty() { return size() == 0; }

public:
  size_t size()   { return size(root); }
private:
  size_t size(node* x) { return x == nullptr ? 0 : x->size; }

public:
  Value get(Key& key) { return get(root, key); }
private:
  Value get(node* x, Key& key) {
    if (key == Key()) { throw new std::invalid_argument("calls get() with a null key"); }
    node* temp = x->left;
    while(x != nullptr) {
      if(less(key, x->key)) {
        temp = x->left;
        x = temp;
      }
      else if(less(x->key, key)) {
        temp = x->right;
        x = temp;
      }
      else {
        return x->val;
      }
    }

    return Value();
  }

public:
  void put(Key& key, Value val) {
    if (key == Key()) { std::cerr << "..... warning: calling put() with a null key" << "\n";  return; }
//    if (val == Value()) {
//      delete_key(key);
//      return;
//    }

    root = put(root, key, val);
    root->color = BLACK;
    assert(check());
  }
private:
  node* put(node* x, Key key, Value val) {
    if (x == nullptr) { return new node(key, val, RED, 1); }

    if      (less(key,    x->key)) { x->left  = put(x->left,  key, val); }
    else if (less(x->key, key))    { x->right = put(x->right, key, val); }
    else                           { x->val   = val; }

    if(isRed(x->right) && !isRed(x->left))      { x = rotateLeft(x); }
    if(isRed(x->left) && isRed(x->left->left))  { x = rotateRight(x); }
    if(isRed(x->left) && isRed(x->right))       { flipColors(x); }

    x->size = 1 + size(x->left) + size(x->right);
    return x;
  }

public:
  void delete_min() {
    if (empty()) { throw new std::logic_error("Symbol table underflow"); }

    if(!isRed(root->left) && !isRed(root->right)) {
      root->color = RED;
    }

    root = delete_min(root);
    if(!empty()) { root->color = BLACK; }
    assert(check());
  }

private:
  node* delete_min(node* x) {
    if (x->left == nullptr) { return nullptr; }

    if(!isRed(x->left) && !isRed(x->left->left)) {
      x = moveRedLeft(x);
    }

    x->left = delete_min(x->left);
    x->size = size(x->left) + size(x->right) + 1;
    return x;
  }

public:
  void delete_max() {
    if (empty()) { throw new std::logic_error("Symbol table underflow"); }

    if(!isRed(root->left) && !isRed(root->right)) {
      root->color = RED;
    }

    root = deleteMax(root);
    if(!empty()) { root->color = BLACK; }
    assert(check());
  }
private:
  node* delete_max(node* x) {
    if (x->right == nullptr) { return nullptr; }

    if(isRed(x->left)) {
      x = rotateRight(x);
    }

    if(!isRed(x->right) && !isRed(x->right->left)) {
      x = moveRedRight(x);
    }

    x->right = delete_max(x->right);
    x->size = size(x->left) + size(x->right) + 1;
    return balance(x);
  }

public:
  void delete_key(Key& key) {
    if (key == Key()) { throw new std::invalid_argument("calls delete() with a null key"); }

    if(!isRed(root->left) && !isRed(root->right)) {
      root->color = RED;
    }

    root = delete_key(root, key);
    if(empty()) { root->color = BLACK; }
    assert(check());
  }

private:
  node* delete_key(node* x, Key& key) {
    if (x == nullptr) { return nullptr; }

    if      (less(key,    x->key)) {
      if(!isRed(x->left) && !isRed(x->left->left)) {
        x = moveRedLeft(x);
      }
      x->left  = delete_key(x->left,  key);
    }
    else {
      if(isRed(x->left)) {
        x = rotateRight(x);
      }
      if(!left(x->key, key) && x->right == nullptr) {
        return nullptr;
      }
      if(!isRed(x->right) && !isRed(x->right->left)) {
        x = moveRedRight(x);
      }
      if(!left(x->key, key)) {
        // if (x->right == nullptr) { return x->left; }
        // if (x->left  == nullptr) { return x->right; }
        node* t = min(x->right);
        x->key = t->key;
        x->val = t->val;
        x = min(t->right);
        x->right = delete_min(t->right);
      }

      else {
        x->right = delete_key(x->right, key);
      }
    }
    x->size = size(x->left) + size(x->right) + 1;
    return x;
  }

/***************************************************************************
 *  Utility Red Black Functions.
 ***************************************************************************/
// left leaning link lean to right
node* rotateRight(node* h) {
  // assert(h != nullptr) && isRed(h->left)
  node* x = h->left;
  h->left = x->right;
  x->right = h;
  x->color = x->right->color;
  x->right->color = RED;
  x->size = h->size;
  h->size = size(h->left) + size(h->right) + 1;

  return x;
}

node* rotateLeft(node* h) {
    // assert (h != null) && isRed(h.right);
    node* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    x->size = h->size;
    h->size = size(h->left) + size(h->right) + 1;

    return x;
}

// flip the colors of a node and its two children
void flipColors(node* h) {
    // h must have opposite color of its two children
    // assert (h != null) && (h.left != null) && (h.right != null);
    // assert (!isRed(h) &&  isRed(h.left) &&  isRed(h.right))
    //    || (isRed(h)  && !isRed(h.left) && !isRed(h.right));
    h->color = !h->color;
    h->left->color = !h->left->color;
    h->right->color = !h->right->color;
}

// Assuming that h is red and both h.left and h.left.left
// are black, make h.left or one of its children red.
node* moveRedLeft(node* h) {
    // assert (h != null);
    // assert isRed(h) && !isRed(h.left) && !isRed(h.left.left);

    flipColors(h);
    if (isRed(h->right->left)) {
        h->right = rotateRight(h->right);
        h = rotateLeft(h);
        flipColors(h);
    }
    return h;
}

// Assuming that h is red and both h.right and h.right.left
// are black, make h.right or one of its children red.
node* moveRedRight(node* h) {
    // assert (h != null);
    // assert isRed(h) && !isRed(h.right) && !isRed(h.right.left);
    flipColors(h);
    if (isRed(h->left->left)) {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

// restore red-black tree invariant
node* balance(node* h) {
    // assert (h != null);

    if (isRed(h->right))                        { h = rotateLeft(h); }
    if (isRed(h->left) && isRed(h->left->left)) { h = rotateRight(h); }
    if (isRed(h->left) && isRed(h->right))      { flipColors(h); }

    h->size = size(h->left) + size(h->right) + 1;
    return h;
}


 /***************************************************************************
  *  Ordered symbol table methods.
  ***************************************************************************/
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

    size_t cmp = key.compareTo(x->key);
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
  Key select(size_t rank) {
    if (rank < 0 || rank >= size()) {
      std::cerr << "argument to select() is invalid: " << rank << "\n";
      throw new std::invalid_argument("invalid select");
    }
    return select(root, rank);
  }
private:
  Key select(node* x, size_t rank) {
    if (x == nullptr) { return nullptr; }

    size_t leftSize = size(x->left);
    if      (leftSize > rank) { return select(x->left,  rank); }
    else if (leftSize < rank) { return select(x->right, rank - leftSize - 1); }
    else                      { return x->key; }
  }

public:
  size_t rank(Key& key) {
    if (key == Key()) { throw new std::invalid_argument("argument to rank() is null"); }
    return rank(key, root);
  }
private:
  size_t rank(Key key, node* x) {
    if (x == nullptr) { return 0; }

    if      (less(key,    x->key)) { return rank(key, x->left); }
    else if (less(x->key, key))    { return 1 + size(x->left) + rank(key, x->right); }
    else                           { return size(x->left); }
  }


// All the fun bits...
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

  size_t size(Key& low, Key& high) {
    if (low == Key())  { throw new std::invalid_argument("first argument to size() is null"); }
    if (high == Key()) { throw new std::invalid_argument("second argument to size() is null"); }

    if (low.compareTo(high) > 0) { return 0; }
    if (contains(high)) { return rank(high) - rank(low) + 1; }
    else                { return rank(high) - rank(low); }
  }

public:
  size_t height() { return height(root); }
private:
  size_t height(node* x) {
    if (x == nullptr) { return -1; }
    return 1 + std::max(height(x->left), height(x->right));
  }

public:
  void prsize_t_inorder() {
    std::cout << "========================================================================= prsize_ting inorder...\n";
    prsize_t_inorder(root);
    std::cout << "========================================================================= end prsize_ting inorder...\n\n";
  }
private:
  void prsize_t_inorder(node* x) {
    if (x == nullptr) { return; }
    prsize_t_inorder(x->left);
    std::cout << *x;
    prsize_t_inorder(x->right);
  }

private:
  //-------- bst validity checks --------------------------------------------------
  bool check() {
    if (!is_bst())             { std::cerr << "Not in symmetric order\n";         return false;  }
    if (!is_size_consistent()) { std::cerr << "Subtree counts not consistent\n";  return false;  }
    if (!is_rank_consistent()) { std::cerr << "Ranks not consistent\n";           return false;  }
    //if (!is23())               { std::cerr << "Not a 2-3 Tree\n";                return false;  }
    if (!is_balanced())        { std::cerr << "Not balanced\n";                return false;  }
    return true;
  }
  bool is_bst() {
    static Key default_key = Key();
//    prsize_t_inorder();

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
    for (size_t i = 0; i < size(); i++) {
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

  bool is23() { return is23(root); }
  bool is23(node* x) {
    if(x == nullptr) { return true; }
    if (isRed(x->right)) { return false; }
    if(x != root && isRed(x) && isRed(x->left)) { return false; }

    return is23(x->left) && is23(x->right);
  }

  bool is_balanced() {
    size_t black = 0;
    node* x = root;
    while(x != nullptr) {
      if(!isRed(x)) { ++black; }
      x= x->left;
    }

    return is_balanced(root, black);
  }

  bool is_balanced(node* x, size_t black) {
    if(x == nullptr) { return black == 0; }

    if(!isRed(x)) { --black; }

    return is_balanced(x->left, black) && is_balanced(x->right, black);
  }

public:
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

  static void run_tests() {
    char buf[BUFSIZ];
    bst_red_black<std::string, size_t> st;
    // tests tinyST.txt
    std::string filename = "tinyST.txt";
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
      std::cerr << "Could not open file: '" << filename << "'\n";  exit(2);
    }

    std::cout << "Building symbol table for file: '" << filename << "'. \n";
    std::cout << "(punctuation automatically stripped)... \n";
    size_t i = 0;
    std::string s;
    while (ifs >> s) {
      strcpy(buf, s.c_str());
      strconvert(buf, tolower);
      strstrip(buf);
      std::string key = std::string(buf);
      if (key != "") { st.put(key, i++); }
//      st.prsize_t_inorder();
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



#endif /* bst_red_black_h */
