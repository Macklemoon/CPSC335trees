//
//  random.h
//  stacks_queues_bags
//
//  Created by William McCarthy on 118//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef random_h
#define random_h

#include <random>
#include "utils.h"


template <typename T>
class std_random {
  typedef std::default_random_engine engine;
  typedef std::uniform_int_distribution<int> uniform_int;
public:
  static void shuffle(T* arr, size_t n) {
    engine gen = get_gen();
    for (size_t i = 0; i < n; ++i) {
      uniform_int distro(0, i);
      size_t r = distro(gen);
      exchange(arr, i, r);
    }
  }

  static void shuffle_alternate(T* arr, size_t n) {
    engine gen = get_gen();
    for (size_t i = 0; i < n; ++i) {
      uniform_int distro(i, (int)n - 1);
      size_t r = distro(gen);
      T swap = arr[r];
      arr[r] = arr[i];
      arr[i] = swap;
    }
  }

  static void generate_uniform_int(int* arr, size_t n, int low, int high) {
    engine gen = get_gen();
    uniform_int distro(low, high);
    for (size_t i = 0; i < n; ++i) {
      arr[i] = distro(gen);
    }
  }
private:
  static engine get_gen() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::default_random_engine gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    return gen;
  }
};

#endif /* random_h */
