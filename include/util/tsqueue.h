//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_TSQUEUE_H
#define WINTER_TSQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

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

    template<typename... Args>
    void emplace_front(Args&&... args);
    template<typename... Args>
    void emplace_back(Args&&... args);
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
    std::deque<T> queue;
    std::mutex mutex_;

    std::mutex mutexEmpty;
    std::condition_variable cvEmpty;
    std::mutex mutexFull;
    std::condition_variable cvFull;
};

template<typename T>
void tsqueue<T>::push_front(const T& val) {
    std::scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.push_front(val);
}

template<typename T>
void tsqueue<T>::push_back(const T& val) {
    std::scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.push_back(val);
}

template<typename T>
template<typename... Args>
void tsqueue<T>::emplace_back(Args&&... args){
    std::scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.emplace_back(args...);
}

template<typename T>
template<typename... Args>
void tsqueue<T>::emplace_front(Args&&... args){
    std::scoped_lock lock(mutex_);
    cvEmpty.notify_one();
    queue.emplace_front(args...);
}


template<typename T>
T tsqueue<T>::pop_front() {
    std::scoped_lock lock(mutex_);
    T val = std::move(queue.front());
    queue.pop_front();
    cvFull.notify_one();
    return val;
}

template<typename T>
T tsqueue<T>::pop_back() {
    std::scoped_lock lock(mutex_);
    T val = std::move(queue.back());
    queue.pop_back();
    cvFull.notify_one();
    return val;
}

template<typename T>
const T& tsqueue<T>::front() {
    std::scoped_lock lock(mutex_);
    queue.front();
}

template<typename T>
const T& tsqueue<T>::back() {
    std::scoped_lock lock(mutex_);
    queue.back();
}

template<typename T>
size_t tsqueue<T>::size() {
    std::scoped_lock lock(mutex_);
    return queue.size();
}

template<typename T>
bool tsqueue<T>::empty() {
    std::scoped_lock lock(mutex_);
    return queue.empty();
}

template<typename T>
void tsqueue<T>::clear() {
    std::scoped_lock lock(mutex_);
    queue.clear();
}

template<typename T>
void tsqueue<T>::waitForEvent() {
    while(empty()){
        std::unique_lock<std::mutex> ul(mutexEmpty);
        cvEmpty.wait(ul);
    }
}

template<typename T>
void tsqueue<T>::waitForEvent(size_t size) {
    while(this->size() > size){
        std::unique_lock<std::mutex> ul(mutexFull);
        cvFull.wait(ul);
    }
}

#endif //WINTER_TSQUEUE_H
