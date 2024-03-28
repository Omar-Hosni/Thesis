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
using namespace chrono;


long long sumSequential(const std::vector<int>& arr) {
    long long total = 0;
    for (int num : arr) {
        total += num;
    }
    return total;
}

long long sumParallel(const std::vector<int>& arr) {
    long long total = 0;
    #pragma omp parallel for reduction(+:total)
    for (size_t i = 0; i < arr.size(); ++i) {
        total += arr[i];
    }
    return total;
}



int main() {
    const size_t N = 1e9; 
    std::vector<int> arr(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    // Fill the vector with random numbers
    for (size_t i = 0; i < N; ++i) {
        arr[i] = dis(gen);
    }

    auto start = high_resolution_clock::now();
    long long total = sumSequential(arr);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Time taken by Sum Sequential: "
        << fixed << std::setprecision(3)
        << duration.count()
        << " milliseconds" << endl;

    auto start = high_resolution_clock::now();
    long long total = sumParallel(arr);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Time taken by Sum Parallel: "
        << fixed << setprecision(3)
        << duration.count()
        << " milliseconds" << endl;
    return 0;
}

