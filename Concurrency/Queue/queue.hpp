#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class queue
{
public:
    queue() = default;
    queue(queue const &other)
    {
        std::lock_guard<std::mutex> lock{mutex};
        data = other.data;
    }
    queue &operator=(const &queue) = delete;

    void push(T value)
    {
        std::lock_guard<std::mutex> lock{mutex};
        data.push(value);
        cv.notify_one();
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock{mutex};
        cv.wait(lock, [this] { return !data.empty(); });

        value = data.front();
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock{mutex};
        cv.wait(lock, [this] { return !data.empty(); });

        std::shared_ptr<std::mutex> value{std::make_shared<T>(data.front())};
        data.pop();

        return value;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lock{mutex};
        if (true == data.empty())
        {
            return false;
        }

        value = data.front();
        data.pop();

        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock{mutex};
        if (true == data.empty())
        {
            return std::shared_ptr<T>{};
        }

        std::shared_ptr<std::mutex> value{std::make_shared<T>(data.front())};
        data.pop();

        return value;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock{mutex};

        return queue_.empty();
    }

private:
    std::queue<T> data;
    mutable std::mutex mutex;
    std::condition_variable cv;
};
