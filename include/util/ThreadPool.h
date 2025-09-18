//
// Created by Alienson on 2.3.2024.
//

#ifndef WINTER_THREADPOOL_H
#define WINTER_THREADPOOL_H

#endif //WINTER_THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

class ThreadPool{
public:
    //TODO use tsqueue for this?
    explicit ThreadPool(int size) : stop(false){
        for (int i=0;i<size;i++){
            threads.emplace_back([this]{
                std::function<void()> task;
                while(true){
                    std::unique_lock<std::mutex> ul(mutexBlocking);
                    cvBlocking.wait(ul, [this] {
                        return this->stop || !this->tasks.empty();
                    });
                    if(this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    task();
                }
            });
        }
    }

    ~ThreadPool(){
        std::unique_lock<std::mutex> lock(mutexBlocking);
        stop = true;
        cvBlocking.notify_all();
        for(std::thread &thread: threads)
            thread.join();
    }

    template<typename Callable, typename... Args>
    void queueTask(Callable&& f, Args&&... args){
        std::unique_lock<std::mutex> lock(queueMutex);
        auto task = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
        tasks.emplace(task);
        cvBlocking.notify_one();
   }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()> > tasks;
    std::condition_variable cvBlocking;
    std::mutex mutexBlocking, queueMutex;
    bool stop;
};