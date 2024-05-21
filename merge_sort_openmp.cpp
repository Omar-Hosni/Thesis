#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include <random>
#include <iomanip>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSortParallel(vector<int>& arr, int left, int right, int depth) {
    if (depth <= 0 || left >= right) {
        mergeSortSequential(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;

#pragma omp parallel sections
    {
#pragma omp section
        {
            mergeSortParallel(arr, left, mid, depth - 1);
        }
#pragma omp section
        {
            mergeSortParallel(arr, mid + 1, right, depth - 1);
        }
    }

    merge(arr, left, mid, right);
}

int main() {
    const size_t N = 1e8;
    std::vector<int> arr(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    // Fill the vector with random numbers
    for (size_t i = 0; i < N; ++i) {
        arr[i] = dis(gen);
    }

    vector<int> arrCopy = arr;

    // Sequential sort
    auto startSequential = chrono::high_resolution_clock::now();
    mergeSortSequential(arr, 0, arr.size() - 1);
    auto endSequential = chrono::high_resolution_clock::now();
    auto durationSequential = chrono::duration_cast<chrono::milliseconds>(endSequential - startSequential);
    cout << "Time taken for sequential merge sort: " << durationSequential.count() << " milliseconds" << endl;

    // Parallel sort
    int maxDepth = 4;  // You can adjust this depth
    auto startParallel = chrono::high_resolution_clock::now();
    mergeSortParallel(arrCopy, 0, arrCopy.size() - 1, maxDepth);
    auto endParallel = chrono::high_resolution_clock::now();
    auto durationParallel = chrono::duration_cast<chrono::milliseconds>(endParallel - startParallel);
    cout << "Time taken for parallel merge sort: " << durationParallel.count() << " milliseconds" << endl;

    // Verify the result
    vector<int> sortedArr = arr;
    sort(sortedArr.begin(), sortedArr.end());
    if (arrCopy == sortedArr) {
        cout << "Sorting successful." << endl;
    }
    else {
        cout << "Sorting failed." << endl;
    }

    return 0;
}
