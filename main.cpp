#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;
using std::thread;
using std::vector;
using std::mutex;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::string;
using std::srand;
using std::time;

// Definition of all methods
void create();

void take(string name);

void watch();

// the vector where the numbers are saved
vector<int> numbers;
// a mutex for accessing the vector
mutex m;
// these variable is needed to stop the threads
int running;

int main() {
    // setting running to true
    running = 1;

    // creating the needed threads (watcher, creator, 2 takers)
    thread watcher(watch);
    thread creator(create);
    thread taker1(take, "Taker 1");
    thread taker2(take, "Taker 2");

    // waiting for the threads to finish
    creator.join();
    taker1.join();
    taker2.join();
    watcher.join();

    return 0;
}

// this method creates new numbers and puts them in the vector numbers
void create() {
    // new random seed
    srand(time(NULL));
    while (running) {
        // creating a random number between 1 and 1000
        int random = rand() % 1000 + 1;
        // locking the mutex
        m.lock();
        cout << "Creator: Random number " << random << endl;
        // putting the number
        numbers.push_back(random);
        m.unlock();
        // sleeps for 300 milliseconds before creating a new number
        sleep_for(milliseconds(300));
    }
}

// this method takes numbers from the vector numbers
void take(string name) {
    while (running) {
        // an initial sleep time if there are no number in the vector.
        int timeSleep = 50;
        m.lock();
        // checking if vector is empty
        if (numbers.size() != 0) {
            // if not, setting the sleep time to the new number
            timeSleep = numbers.back();
            // removing the number
            numbers.pop_back();
            cout << name << ": Taking number " << timeSleep << endl;
        }
        m.unlock();
        // sleeping
        sleep_for(milliseconds(timeSleep));
    }
}

// this method checks if the user wants to close the program
void watch() {
    // a while loop in case that the user writes something wrong
    while (running) {
        m.lock();
        cout << "Enter \"Exit\" to close the program." << endl;
        m.unlock();
        // clearing the cin buffer
        std::cin.clear();
        string input;
        // getting the input from the user
        getline(std::cin, input);
        if (input == "Exit") {
            // if the input is "Exit", running is set to false to stop all threads
            running = 0;
        }
    }
}