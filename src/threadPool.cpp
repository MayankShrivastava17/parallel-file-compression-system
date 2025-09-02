#include "threadPool.hpp"

ThreadPool::ThreadPool(size_t n_threads) {
    workers_.reserve(n_threads);
    for (size_t i = 0; i < n_threads; ++i) {
        workers_.emplace_back([this]{
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lk(m_);
                    cv_.wait(lk, [this]{ return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lk(m_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto& w : workers_) w.join();
}
