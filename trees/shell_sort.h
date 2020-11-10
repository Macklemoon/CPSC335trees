//
//  shell_sort.h
//  stacks_queues_bags
//
//  Created by William McCarthy on 108//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef shell_sort_h
#define shell_sort_h

#include <cassert>
#include "utils.h"


template <typename T>
class shell_sort {
public:
  static void sort(T* arr, size_t n, const comparator<T>& comp=fwd_comparator<T>()) {
    int h = 1;
    while (h < n / 3) { h = 3 * h + 1; }

    while (h >= 1) {
      for (size_t i = h; i < n; ++i) {
        for (size_t j = i; j >= h && less(arr[j], arr[j - h], comp); j -= h) {
          exchange(arr, j, j - h);
        }
      }
      assert(is_h_sorted(arr, n, h, comp));
      h = h / 3;
    }
    assert(is_sorted(arr, n, comp));
  }

//  static void sort(T* arr, size_t n) {
//    int h = 1;
//    while (h < n / 3) { h = 3 * h + 1; }
//    
//    while (h >= 1) {
//      for (size_t i = h; i < n; ++i) {
//        for (size_t j = i; j >= h && less(arr[j], arr[j - h]); j -= h) {
//          exchange(arr, j, j - h);
//        }
//      }
//      assert(is_h_sorted(arr, n, h));
//      h = h / 3;
//    }
//    assert(is_sorted(arr, n));
//  }
  
private:
  static bool is_h_sorted(T* arr, size_t n, size_t h) {
    for (size_t i = h; i < n; ++i) {
      if (less(arr[i], arr[i - h])) { return false; }
    }
    return true;
  }
  
  static bool is_h_sorted(T* arr, size_t n, size_t h, const comparator<T>& comp) {
    for (size_t i = h; i < n; ++i) {
      if (less(arr[i], arr[i - h], comp)) { return false; }
    }
    return true;
  }
};


#endif /* shell_sort_h */
