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
    queue(queue<T> const &) = delete;
    queue &operator=(const &queue<T>) = delete;

    void push(T value)
    {
        std::lock_guard<std::mutex> lock{mutex};
        data.push(std::move(value));
        cv.notify_one();
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock{mutex};
        cv.wait(lock, [this] { return !data.empty(); });

        value = std::move(data.front());
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

        value = std::move(data.front());
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

        return data.empty();
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock{mutex};

        return data.size();
    }

private:
    std::queue<T> data;
    mutable std::mutex mutex;
    std::condition_variable cv;
};
