/*
 * Noah Wiley
 * CSCI 114
 * Programming 4... This Program was written, tested & complied using CLion on Linux (macOS) using C++ 11
 *
 * To Execute The Program:
 * To compile:  g++ -std=c++11 -pthread CSci114_P4.cpp -o Csci114_P4
 * Run ./CSci114_P4 5 6
 *      Where:
 *          N = Number of threads (between 5 and 15)
 *          M = Total resource units available (between 5 and 10)
 *
 * The file "requests-3.txt" must be in the same directory
 * Each line in requests-3.txt should contain: <thread_id> <request_units>
 *      where thread_id ranges from 1 to N
 *
 * Output:
 * The program prints the allocation vector after each allocation
 * Each number represents how many resource units are allocated to that thread
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;
vector<int> allocation;
int available;
mutex mtx;
condition_variable cv;

// Each thread's queue of requests
vector<queue<int>> thread_requests;
vector<int> prev_allocation;



void print_allocation() {
    int changed_thread = -1;
    if (!prev_allocation.empty()) {
        for (size_t i = 0; i < allocation.size(); i++) {
            if (allocation[i] > prev_allocation[i]) {
                changed_thread = i;
                break;
            }
        }
    }

    cout << "[";
    for (int i = 0; i < allocation.size(); i++) {
        cout << allocation[i];
        if (i != allocation.size() - 1)
            cout << " ";
    }
    cout << "]\n";
}

void worker(int tid) {
    while (!thread_requests[tid].empty()) {
        int request = thread_requests[tid].front(); // How many units to request
        int granted = 0;
        while (granted < request) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&] {return available > 0; });
            --available;
            ++allocation[tid];
            ++granted;
            print_allocation();
            lock.unlock();
            this_thread::yield();
        }

        thread_requests[tid].pop();
    }
}

void release_resources(int N) {
    for (int i = 0; i < N; i++) {
        available += allocation[i];
        allocation[i] = 0;
    }
    print_allocation();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " N M\n";
        return 1;
    }

    int N = stoi(argv[1]); // Number of Threads
    int M = atoi (argv[2]); // Total Units

    if (N < 5 || N > 15 || M < 5 || M > 10) {
        cerr << "Error: N must be [5,15], M must be [5,10].\n";
        return 1;
    }

    available = M;
    allocation.resize(N, 0);
    thread_requests.resize(N);

    // Read from requests-3.txt
    ifstream infile("requests-3.txt");
    if (!infile) {
        cerr << "Error opening file " << argv[1] << "\n";
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        int tid, units;
        if (!(iss >> tid >> units)) {
            continue;
        }
        if (tid >= 1 && tid <= N) {
            thread_requests[tid - 1].push(units);
        }
    }

    infile.close();

    vector<thread> threads;
    for (int i = 0; i < N; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    {
        unique_lock<mutex> lock(mtx);
        release_resources(N);
    }

    return 0;
}