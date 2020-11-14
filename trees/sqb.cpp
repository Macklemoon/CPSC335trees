//
//  sqb.cpp
//  stacks_n_queues
//
//  Created by William McCarthy on 267//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include "slist.h"
#include "stack.h"
#include "queue.h"
//#include "bag.h"
//#include "linked_stackofstuff.h"
//#include "array_stackofstuff.h"
//#include "stack_arrayexpand.h"

#include "utils.h"

#include "selection_sort.h"
#include "insertion_sort.h"
#include "shell_sort.h"

#include "merge_sort.h"

#include "random.h"
//#include "students.h"

#include "array.h"

//#include "st.h"
#include "bst.h"
#include "bst_redblack.h"

//#include "graph.h"
//#include "digraph.h"

//#include "btree.h"


template <typename T>
void run_node_list_stack_queue_tests() {
//  node<T>::run_tests();
//  slist<T>::run_tests();
//  stack_<T>::run_tests();
//  list_queue<T>::run_tests();
//  array_queue<T>::run_tests();
//
//  linked_stackofstuff<std::string>::run_tests();
//  array_stackofstuff<std::string>::run_tests();
//  bag_<T>::run_tests();
}

#define ARGC_ERROR  1
#define FILE_ERROR 2
#define STRING_NUMBER_ERROR 3

void kth_from_last(int argc, const char* argv[]) {
  std::cout << "argc is: " << argc << "\n\n";

  if (argc != 2) { std::cerr << "Usage: ./stacks_queues_bags k\n";  exit(ARGC_ERROR); }
  int k = atoi(argv[1]);
  if (k < 0) { std::cerr << "Error: k < 0\n";  exit(STRING_NUMBER_ERROR); }

  stack_<std::string> st;
  std::string s;
  while (std::cin >> s) {
    st.push(s);
  }
  int countdown = k;
  while (countdown-- > 0) {
    s = st.pop();
  }
  std::cout << k << "th string from end is: " << s << "\n";
}

bool missing_parenthesis(const std::string& s) {   // (abc) or (ab(cd)ef)   )))
  stack_<char> st;
  bool extra_close_parenthesis = false;

  const char* p = s.c_str();
  while (*p != '\0' and not extra_close_parenthesis) {
    char c = *p++;
    switch(c) {
    case '(': case '[': case '{':  st.push(c);  break;
    case ')': case ']': case '}':
      if (st.empty()) { extra_close_parenthesis = true; }
      if ((c == ')' && st.top() == '(') ||
          (c == ']' && st.top() == '[') ||
          (c == '}' && st.top() == '{'))     { st.pop(); }
      break;
    default: { }
    }
  }
  std::cout << "st is: " << st << "\n";
  return !st.empty() || extra_close_parenthesis;
}

void get_input(const std::string& msg, std::string& input) {
  char c;

  std::cout << msg;
  while (std::cin >> c) {
    if (isprint(c)) {
      input += c;
    }
  }
}

void missing_parenthesis_test() {
  std::string s;
  get_input("Missing (): enter an expression: ", s);
//  std::cout << "string is: " << s << "\n";

  bool missing = missing_parenthesis(s);
  std::cout << "are parentheses missing? " << yes_or_no(missing) << "\n\n";

  if (missing) {
    std::cout << "simple correction... add leading parenthesis...\n";
    std::cout << "(" << s << "\n\n";
  }
}

void dijkstra_algorithm() {
  std::string s;
  stack_<int> st_int;
  stack_<char> st_ops;

  get_input("Dijkstra algorithm: enter an expression: ", s);

  int i = 0;
  while (i < s.size()) {
    char c = s[i];

    if (isdigit(c)) {
      std::string str_operand = "";
      while (i < s.size() && isdigit(c)) {
        str_operand += c;
        c = s[++i];
      }
      st_int.push(std::stoi(str_operand));
      continue;
    }
    else if (c == '+' || c == '*') { st_ops.push(c); }
    else if (c == ')') {
      if (st_int.size() < 2) { throw new std::logic_error("Not enough parameters\n"); }
      else {
        int op1 = st_int.pop();
        int op2 = st_int.pop();
        char op = st_ops.pop();
        switch(op) {
          case '+': st_int.push(op1 + op2);  break;
          case '*': st_int.push(op1 * op2);  break;
          default: { }
        }
      }
    }
    c = s[++i];
  }
  std::cout << "\nresult is: " << st_int << "\n\n";
}


template <typename T>
void single_issorted_test(T* arr, size_t low, size_t high) {
  print("arr is: ", arr, low, high);
  std::cout << " -- is sorted: " << yes_or_no(is_sorted(arr, low, high)) << "\n";
}

template <typename T>
void single_issorted_test(T* arr, size_t n) { single_issorted_test(arr, 0, n); }

template <typename T>
void test_selection_sort(const std::string& msg, T* arr, size_t n) {
  selection_sort<T> select_sort;
  select_sort.sort(arr, n);
  print(msg, arr, n);
  std::cout << "\n";
}

template <typename T, typename S>
void test_sort(const std::string& msg, T* arr, size_t n,
               const comparator<T>& comp, const S& sort) {
  sort.sort(arr, n, comp);
  print(msg, arr, n);
  std::cout << "\n";
}


//void test_sorting_utilities() {
//  std::cout << "beginning test_sorted()..........................................\n";
//  int test1[] = { 72, 3, 25, 4, 26, 47 };
//  int test2[] = { 18, 37, 44, 5, 12, 99 };
//  int test3[] = { 86, 85, 74, 53, 36, 27 };
//
//  std::string test_str1[] = { "three", "rings", "for", "the", "elven", "kings" };
//  std::string test_str2[] = { "seven", "for", "the", "dwarf", "lords", "in", "their", "halls", "of", "stone" };
//  std::string test_str3[] = { "nine", "for", "mortal", "men", "doomed", "to", "die" };
//
//
//  single_issorted_test(test1, std::size(test1));                // NB: C++17 +
//  single_issorted_test(test2, std::size(test2));
//  single_issorted_test(test3, std::size(test3));
//  std::cout << "\n";
//  single_issorted_test(test_str1, std::size(test_str1));
//  single_issorted_test(test_str2, std::size(test_str2));
//  single_issorted_test(test_str3, std::size(test_str3));
//
//  std::cout << "\n";
//  single_issorted_test(test2, 2, std::size(test2));
//  single_issorted_test(test_str2, 2, std::size(test_str2));
//
//  std::cout << "\nsorting unsorted arrays now with selection_sort.........\n";
//  comparator<int> comp_int;
//  selection_sort<int> sel_sort;
//  test_sort("test1 is now: ", test1, std::size(test1), comp_int, sel_sort);
//  test_sort("test2 is now: ", test2, std::size(test2), comp_int, sel_sort);
//  test_sort("test3 is now: ", test3, std::size(test3), comp_int, sel_sort);
//
//  std::cout << "\nsorting unsorted arrays now with insertion_sort.........\n";
//  rev_comparator<std::string> rev_str;
//  insertion_sort<std::string> ins_sort;
//  test_sort("\ntest_str1 is now: ", test_str1, std::size(test_str1), rev_str, ins_sort);
//  test_sort("test_str2 is now: ", test_str2, std::size(test_str2), rev_str, ins_sort);
//  test_sort("test_str3 is now: ", test_str3, std::size(test_str3), rev_str, ins_sort);
//
//  std::cout << "..........................................ending test_sorted()\n\n";
//}

template <typename T, typename S>
void test_sort_from_file(const std::string& msg, const comparator<T>& comp, const S& sort) {
  const size_t BUFFER_SIZE = 1000;
  std::string words[BUFFER_SIZE];
  std::fill(words, words + BUFFER_SIZE, "");
  size_t n = 0;

  std::string s = "";
  while (std::cin >> s) {
    if (s != " ") { words[n++] = s; }
  }
  for (const std::string& word : words) {
    if (word != "") {
      std::cout << word << " ";
    }
  }
//  insertion_sort<std::string> ins_sort;
//  rev_comparator<std::string> rev_str;
  test_sort("\n\nAfter " + msg + " sorting, words is now: \n\n", words, n, comp, sort);

  std::cout << "\n\n";
}

void test_elementary_sorts() {
  //  test_sorting_utilities();

//  test_sort_from_file("selection", comparator<std::string>(), selection_sort<std::string>());
//  test_sort_from_file("insertion", comparator<std::string>(), insertion_sort<std::string>());
//  test_sort_from_file("shell_sort", comparator<std::string>(), shell_sort<std::string>());

//  freopen("shellsort.txt", "r", stdin);
}

void test_shuffle() {
  //  const int TINY = 10;
  //  int low = 30, high = 50;
  //  int buf[TINY];
  //  std::fill(buf, buf + TINY, 0);
  //  std_random<int>::generate_uniform_int(buf, TINY, low, high);
  //  test_sort("random buffer: ", buf, TINY, comparator<int>(), shell_sort<int>());
  //
  //  for (int i = 0; i < TINY; ++i) {
  //    buf[i] = i;
  //  }
  //  print("\nunshuffled: ", buf, TINY);
  ////  std_random<int>::shuffle(buf, TINY);
  //  std_random<int>::shuffle_alternate(buf, TINY);
  //
  //  print("\n  shuffled: ", buf, TINY);
  //  std::cout << "\n\n";

  freopen("words3.txt", "r", stdin);
  std::string s;
  int len = 0;
  std::string cards[100];

  while (std::cin >> s) {
    cards[len++] = s;
  }
  print("\nunshuffled: ", cards, len);
  std_random<std::string>::shuffle(cards, len);
  print("\n\n  shuffled: ", cards, len);
  std::cout << "\n\n";
}

#include <cstdio>

template <typename Key, typename Value, typename SymbolTable>
void test_bst(const std::string& msg, const std::string& filename, SymbolTable& st) {
  char buf[BUFSIZ];

  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Could not open file: '" << filename << "'\n";  exit(2);
  }

  std::cout << "\n\n===========================\n >>> " << msg << " <<< \n\n";
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
    st.print_inorder();
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


//------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
//run_node_list_stack_queue_tests<std::string>();
//  kth_from_last(argc, argv);
//  missing_parenthesis_test();
//  dijkstra_algorithm();
//  test_elementary_sorts();
//  test_shuffle();

//  freopen("words3.txt", "r", stdin);
//  test_sort_from_file("merge", rev_comparator<std::string>(), merge_sort<std::string>());
//  test_sort_from_file("merge_bottom_up", rev_comparator<std::string>(), merge_bu_sort<std::string>());

//  student::run_tests();

//  array_<std::string>::run_tests();


//  binary_search_st<std::string, std::shared_ptr<size_t>> stable;
//  binary_search_st<std::string, std::shared_ptr<size_t>> stable;
//  std::string key;
//  for (size_t i = 0; std::cin >> key; ++i) {
//    stable.put(key, std::shared_ptr<size_t>(new size_t(i)));
////    stable.put(key, wrapper<std::shared_ptr<size_t>, size_t>(i));
//  }
//  std::cout << "stable built...\n";
//  for (const std::string& key : stable.keys()) {
//    std::cout << key << " " << stable.get(key) << "\n";
//  }
//  for (const std::shared_ptr<size_t>& value : stable.values()) {
//    std::cout << *value << "\n";
//  }


//  bst<std::string, std::string>::run_tests();

//  interval_st<int>::run_tests(argc, argv);

//  graph::run_tests(argc, argv);
//  depth_first_search::run_tests(argc, argv);
//  breadth_first_search::run_tests(argc, argv);

//  conn_comp::run_tests(argc, argv);

//  digraph::run_tests(argc, argv);
//  directed_dfs::run_tests(argc, argv);
//  depth_first_order::run_tests(argc, argv);

//  directed_cycle::run_tests(argc, argv);

//  array_<std::string>::run_tests();

//  symbol_digraph::run_tests(argc, argv);
//

//  array_expand<int>::run_tests();

//  stack_arrayexpand<int>::run_tests();


//  char buf[BUFSIZ];
//  if (argc != 2) { std::cerr << "Usage ./sqb filename\n";  exit(1); }

//  bst<std::string, int>::test_bst("tinyST.txt");
//  bst<std::string, int>::test_bst("gettysburgST.txt");
//  bst<std::string, int>::test_bst(argv[1]);

  bst<std::string, int>  bst;
  test_bst<std::string, int>("bst", "tinyST.txt", bst);

  bst_red_black<std::string, int> red_black;
  test_bst<std::string, int>("red_black_bst", "tinyST.txt", red_black);

//  bst<std::string, int>::test_bst("tinyST.txt");



//  btree<std::string, std::string>::run_tests();

  std::cout << "\t\t...done.\n";

  return 0;
}
