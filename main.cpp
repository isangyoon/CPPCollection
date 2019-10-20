#include <iostream>
#include "Concurrency/Queue/queue.hpp"

auto main(int argc, char **argv) -> int
{
    queue<int> q;

    for (auto i = 0; i < 10; ++i)
    {
        q.push(i + 1);
    }

    return EXIT_SUCCESS;
}