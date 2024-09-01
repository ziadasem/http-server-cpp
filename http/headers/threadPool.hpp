#ifndef _THREADPOOL_
#define _THREADPOOL_

#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional> 

#include <iostream>

class ThreadPool {
private:
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::queue<std::function<void()>> m_tasks;
    std::vector<std::thread> m_threads;
    int m_threadsNumber;
    bool m_stop{ false };
    void _createPool(int numOfThreads);
    void _disposePools(int numOfDisposedPools = -1);
    void _stopThreads(int count);
    void _freeMemory(int numOfRemovedThreads);

public:
    ThreadPool(int threadsNumber = std::thread::hardware_concurrency());
    virtual ~ThreadPool();
    void addTask(std::function<void()> task);
    void expandPool(int newNumber);

    int getThreadsNumber() const;
};

#endif