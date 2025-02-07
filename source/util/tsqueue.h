//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_TSQUEUE_H
#define WINTER_TSQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class tsqueue {
public:
    tsqueue() = default;
    tsqueue(const tsqueue&) = delete;
    ~tsqueue(){
        clear();
    }

    void push_front(const T& val);
    void push_back(const T& val);
    T pop_front();
    T pop_back();
    const T& front();
    const T& back();

    size_t size();
    bool empty();
    void clear();

    void waitForEvent();
    void waitForEvent(size_t maxSize);


private:
    deque<T> queue;
    mutex mutex_;

    mutex mutexEmpty;
    condition_variable cvEmpty;
    mutex mutexFull;
    condition_variable cvFull;
};

template<typename T>
void tsqueue<T>::push_front(const T& val) {
    scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.push_front(val);
}

template<typename T>
void tsqueue<T>::push_back(const T& val) {
    scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.push_back(val);
}

template<typename T>
T tsqueue<T>::pop_front() {
    scoped_lock lock(mutex_);
    T val = std::move(queue.front());
    queue.pop_front();
    cvFull.notify_one();
    return val;
}

template<typename T>
T tsqueue<T>::pop_back() {
    scoped_lock lock(mutex_);
    T val = std::move(queue.back());
    queue.pop_back();
    cvFull.notify_one();
    return val;
}

template<typename T>
const T& tsqueue<T>::front() {
    scoped_lock lock(mutex_);
    queue.front();
}

template<typename T>
const T& tsqueue<T>::back() {
    scoped_lock lock(mutex_);
    queue.back();
}

template<typename T>
size_t tsqueue<T>::size() {
    scoped_lock lock(mutex_);
    return queue.size();
}

template<typename T>
bool tsqueue<T>::empty() {
    scoped_lock lock(mutex_);
    return queue.empty();
}

template<typename T>
void tsqueue<T>::clear() {
    scoped_lock lock(mutex_);
    queue.clear();
}

template<typename T>
void tsqueue<T>::waitForEvent() {
    while(empty()){
        unique_lock<mutex> ul(mutexEmpty);
        cvEmpty.wait(ul);
    }
}

template<typename T>
void tsqueue<T>::waitForEvent(size_t size) {
    while(this->size() > size){
        unique_lock<mutex> ul(mutexFull);
        cvFull.wait(ul);
    }
}

#endif //WINTER_TSQUEUE_H
