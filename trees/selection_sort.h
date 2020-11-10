//
//  selection_sort.h
//  stacks_queues_bags
//
//  Created by William McCarthy on 108//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef selection_sort_h
#define selection_sort_h

#include <cassert>
#include "utils.h"


template <typename T>
class selection_sort {          
public:
  static void sort(T* arr, size_t n, const comparator<T>& comp=fwd_comparator<T>()) {
    for (size_t i = 0; i < n; ++i) {
      size_t min = i;
      for (size_t j = i + 1; j < n; ++j) {
        if (less(arr[j], arr[min], comp)) { min = j; }      // return (arr[j] < arr[min]);
      }
      exchange(arr, i, min);
    }
    assert(is_sorted(arr, n, comp));
  }

//  static void sort(T* arr, size_t n) {
//    for (size_t i = 0; i < n; ++i) {
//      size_t min = i;
//      for (size_t j = i + 1; j < n; ++j) {
//        if (less(arr[j], arr[min])) { min = j; }
//      }
//      exchange(arr, i, min);
//    }
//    assert(is_sorted(arr, n));
//  }
};


#endif /* selection_sort_h */
