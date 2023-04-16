#include <vector>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <iostream>
#include <random>
#include <functional>
#include <chrono>
using namespace std;

/* Returns the amount of milliseconds the closure took to execute
 */
double timeit(std::function<void()> totime) {
    using std::chrono::steady_clock,
    std::chrono::duration,
    std::chrono::time_point;

    time_point start = steady_clock::now();
    totime();
    time_point end = steady_clock::now();
    duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

/* Checks if vector is sorted
 */
bool sorted(const std::vector<int> &vec) {
    for (unsigned int i = 1; i < vec.size(); i++) {
        if (vec[i - 1] > vec[i])
            return false;
    }
    return true;
}

void swap(vector<int> &v, int x, int y) {
    int temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

void quicksort(vector<int> &vec, int Left_Index, int Right_Index) {
    int i, j, mid, piv;
    i = Left_Index;
    j = Right_Index;
    mid = Left_Index + (Right_Index - Left_Index) / 2;
    piv = vec[mid];

    while (i<Right_Index || j>Left_Index) {
        while (vec[i] < piv)
            i++;
        while (vec[j] > piv)
            j--;

        if (i <= j) {
            swap(vec, i, j);
            i++;
            j--;
        }
        else {
            if (i < Right_Index)
                quicksort(vec, i,Right_Index);
            if (j > Left_Index)
                quicksort(vec, Left_Index, j);
            return;
        }
    }
}


void MergeSortedIntervals(vector<int>& v, int Left, int m, int Right) {
    vector<int> temp;

    int i, j;
    i = Left;
    j = m + 1;

    while (i <= m && j <= Right) {

        if (v[i] <= v[j]) {
            temp.push_back(v[i]);
            ++i;
        }
        else {
            temp.push_back(v[j]);
            ++j;
        }

    }

    while (i <= m) {
        temp.push_back(v[i]);
        ++i;
    }

    while (j <= Right) {
        temp.push_back(v[j]);
        ++j;
    }

    for ( i = Left; i <= Right; ++i)
        v[i] = temp[i - Left];

}
void MergeSort(vector<int>& v, int Left, int Right) {
    if (Left < Right) {
        int m = (Left + Right) / 2;
        MergeSort(v, Left, m);
        MergeSort(v, m + 1, Right);
        MergeSortedIntervals(v, Left, m, Right);
    }
}


void heapify(std::vector<int> &v, int size, int i) {
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;


    if (left < size && v[left] > v[max])
        max = left;


    if (right < size && v[right] > v[max])
        max = right;


    if (max != i) {
        // swap
        int temp = v[i];
        v[i] = v[max];
        v[max] = temp;

        heapify(v, size, max);
    }
}

void heapSort(std::vector<int> &v) {
    int size = v.size();


    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(v, size, i);
    }

    for (int i = size - 1; i >= 0; i--) {

        int temp = v[0];
        v[0] = v[i];
        v[i] = temp;


        heapify(v, i, 0);
    }
}
void p(std::vector<int> &v, int len) {
    for (int i = 0 ; i < len ; i ++) {
        std::cout << v[i] << ", ";
    }
}

int main() {


    constexpr int step = 100;
    constexpr int maxlen = 10000;
    constexpr int times = 100;
    srand(time(nullptr));

    if (!sorted(std::vector { -5, -3, -1, -1, 0, 1, 1, 2, 5, 7, 9 })) {
        cout << "sorted() is broken" << endl;
        exit(1);
    }


    cout << "step: " << step << "\nmaxlen: " << maxlen << "\ntimes: " << times << "\n";
    cout << "size heap merge quick heap_sorted merge_sorted quick_sorted (in milliseconds)" << endl;

    for (int i = 1;; i++) {
        int count = i * step;

        if (count > maxlen)
            break;

        vector<int> tosort(count);
        generate(tosort.begin(), tosort.end(), std::rand);

        double heap_time = 0, heap_time_sorted = 0;
        for (int j = 0; j < times; j++) {
            auto copy = tosort;
            heap_time += timeit( [&copy]{ heapSort(copy); } );
            if (!sorted(copy)) {
                cout << "Heap sort is broken" << endl;
                exit(1);
            }
            heap_time_sorted += timeit( [&]{ heapSort(copy); } );
        }
        heap_time /= times;
        heap_time_sorted /= times;

        double merge_time = 0, merge_time_sorted = 0;
        for (int j = 0; j < times; j++) {
            auto copy = tosort;
            merge_time += timeit( [&copy, count]{ MergeSort(copy, 0, count - 1); } );
            if (!sorted(copy)) {
                cout << "Merge sort is broken" << endl;
                exit(1);
            }
            merge_time_sorted += timeit( [&copy, count]{ MergeSort(copy, 0, count - 1); } );
        }
        merge_time /= times;
        merge_time_sorted /= times;

        double quick_time = 0, quick_time_sorted = 0;
        for (int j = 0; j < times; j++) {
            auto copy = tosort;
            quick_time += timeit( [&copy, count]{ quicksort(copy, 0, count - 1); } );
            if (!sorted(copy)) {
                cout << "Quick sort is broken" << endl;
                exit(1);
            }
            quick_time_sorted += timeit( [&copy, count]{ quicksort(copy, 0, count - 1); } );
        }
        quick_time /= times;
        quick_time_sorted /= times;

        cout << count << " " <<
             heap_time << " " <<
             merge_time << " " <<
             quick_time << " " <<
             heap_time_sorted << " " <<
             merge_time_sorted << " " <<
             quick_time_sorted << endl;
    }
}
