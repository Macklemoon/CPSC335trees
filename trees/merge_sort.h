#include "utils.h"
template <typename T>
class merge_sort {
public:
    // Overload for public use. Recursion procs in private helper.
    static void sort(T* arr, size_t n, const comparator<T>& comp=fwd_comparator<T>()) {
      T* aux = new T[n];
      assert(aux != nullptr);
      sort(arr, aux, 0, n - 1, comp);
    }

private:
    // Var indicates how large arrays will be at the base case.
    static const int CUTOFF = 7;

    // Recursive method that sorts both sides separately, and merges the two mostly sorted arrays.
    static void sort(T* arr, T* aux, size_t low, size_t high, const comparator<T>& comp) {
      // if the size of the array is at a certain point,
      // sort before more cracking, then return.
      insertion_sort<T> insert;
      if(high <= (low + CUTOFF - 1)) {
          insert.sort(arr, low, high, comp);
          return;
      }

      // gets middle of current array's size.
      size_t mid = low + ((high - low) / 2);

      // Sort from lower end of currently spectating chunk.
      sort(arr, aux, low, mid, comp);
      // Sort the higher portion of the array.
      sort(arr, aux, mid + 1, high, comp);
      merge(arr, aux, low, mid, high, comp);
    }

    // Nitty gritty pretty details. Iterates through the two, equal sized arrays,
    // And merges them into arr[]
    static void merge(T* arr, T* aux, size_t low, size_t mid, size_t high, const comparator<T>& comp) {
        // Check if sort() does its job.
        assert(is_sorted(arr, low, mid, comp));
        assert(is_sorted(arr, mid+1, high, comp));
        // Local iterator vars.
        size_t i = low;
        size_t j = mid + 1;

        // Populate aux[]
        for(size_t k = low; k <= high; ++k) {
          aux[k] = arr[k];
        }

        // Iterates through the arrays.
        for(size_t k = low; k <= high; ++k) {
          if(i > mid) {
              // If lowest index > mid, move the val right of current "middle"
              // in aux[] to the current value.
              arr[k] = aux[j++];
          } else if(j > high) {
              // if the middle index > highest index,
              // put aux[lowest current] in arr[current]
              arr[k] = aux[i++];
          } else if(less(aux[j], aux[i], comp)) {
              // if the value in aux[current middle + 1]
              // is less than the aux[lowest index value]
              // insert arr[current] aux[current middle + 1]
              arr[k] = aux[j++];
          } else {
              // If no case fits, just put lowest index and continue.
              arr[k] = aux[i++];
          }
        }
    }
};

template <typename T>
class merge_bu_sort {
public:
// merge_bu_sort() = delete;
    static void sort(T* arr, size_t n, const comparator<T>& comp=fwd_comparator<T>()) {
      T* aux = new T[n];
      assert(aux != nullptr);

      // Note, no sorting is done externally; all merges are the sorts.
      // Each iteration dictates a larger amount of the array that is sorted.
      for(size_t subsize = 1; subsize < n; subsize = subsize + subsize) {
        for(size_t low = 0; low < n - subsize; low += subsize + subsize) {
            merge(arr, aux, low, low + subsize - 1, std::min(low + subsize + subsize - 1, n - 1), comp);
        }
      }
    }
private:
    // Same in-place merge function as in class merge_sort above.
    static void merge(T* arr, T* aux, size_t low, size_t mid, size_t high, const comparator<T>& comp) {
	    assert(is_sorted(arr, high, low, comp));
      // Local iterator vars.
      size_t i = low;
      size_t j = mid + 1;

      // Populates aux[]
      for(size_t k = low; k <= high; ++k) {
        aux[k] = arr[k];
      }

      // Iterates through the arrays.
      for(size_t k = low; k <= high; ++k) {
        if(i > mid) {
            // If lowest index > mid, move the val right of current "middle"
            // in aux[] to the current value.
            arr[k] = aux[j++];
        } else if(j > high) {
            // if the middle index > highest index,
            // put aux[lowest current] in arr[current]
            arr[k] = aux[i++];
        } else if(less(aux[j], aux[i], comp)) {
            // if the value in aux[current middle + 1]
            // is greater than the aux[lowest index value]
            // insert arr[current] aux[current middle + 1]
            arr[k] = aux[j++];
        } else {
            // If no case fits, just put lowest index and continue.
            arr[k] = aux[i++];
        }
      }
    }
};
