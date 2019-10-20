#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class queue
{
public:
    queue() = default;
    queue(const queue &) = delete;
    queue &operator=(const &queue) = delete;

    T pop()
    {
        std::unique_lock<std::mutex> mlock{mutex_};
        while (queue_.empty())
        {
            cv.wait(mlock);
        }
        auto value = std::move(queue_.front());
        queue_.pop();

        return value;
    }

    void pop(T &item)
    {
        std::unique_lock<std::mutex> mlock{mutex_};
        while (queue_.empty())
        {
            cv.wait(mlock);
        }
        item = queue_.front();
        queue_.pop();
    }

    void push(const T &item)
    {
        std::lock_guard<std::mutex> lock{mutex};
        queue_.push(item);
        cv.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock{mutex};

        return queue_.empty();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex;
    std::condition_variable cv;
};
