#ifndef MOSS_THREAD_POOL_SYNC_QUEUE_HPP_
#define MOSS_THREAD_POOL_SYNC_QUEUE_HPP_

#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>

template<typename T>
class SyncQueue {
public:
    SyncQueue(int max_size) : max_size_(max_size), need_stop_(false) {
    }

    void Put(const T& x) {
        Add(x);
    }

    void Put(T&& x) {
        Add(std::forward<T>(x));
    }

    void Take(std::list<T>& list) {
        std::unique_lock<std::mutex> locker(mutex_);
        not_empty_.wait(locker, [this]{ return need_stop_ || NotEmpty(); });
        if (need_stop_)
            return;
        list = std::move(queue_);
        not_full_.notify_one();
    }

    void Take(T& t) {
        std::unique_lock<std::mutex> locker(mutex_);
        not_empty_.wait(locker, [this]{ return need_stop_ || NotEmpty(); });
        if (need_stop_)
            return;
        t = queue_.front();
        queue_.pop_front();
        not_full_.notify_one();
    }

    void Stop() {
        {
            std::lock_guard<std::mutex> locker(mutex_);
            need_stop_ = true;
        }
        not_full_.notify_all();
        not_empty_.notify_all();
    }

    bool Empty() {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.empty();
    }

    bool Full() {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.size() == max_size_;
    }

    size_t Size() {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.size();
    }

private:

    bool NotEmpty() {
        return !queue_.empty();
    }

    bool NotFull() {
        return queue_.size() < max_size_;
    }

    template<typename F>
    void Add(F&& x) {
        std::unique_lock<std::mutex> locker(mutex_);
        not_full_.wait(locker, [this]{ return need_stop_ || NotFull(); });
        if (need_stop_) 
            return;
        queue_.push_back(std::forward<F>(x));
        not_empty_.notify_one();
    }

private:
    std::mutex mutex_;
    std::list<T> queue_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    int max_size_;
    bool need_stop_;
};

#endif