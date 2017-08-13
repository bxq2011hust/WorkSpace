#include "block_queue.h"
#include <iostream>
/**
 * g++ main.cpp -lpthread
 */
using namespace std;

BlockingQueue<int> queue(3);

void *push(void *)
{
    for (int i = 0; i < 5; ++i)
    {
        queue.push(i);
    }
    return NULL;
}

void *get(void *)
{
    for (int i = 0; i < 5; ++i)
        cout << queue.get() << endl;
    return NULL;
}

int main()
{

    pthread_t producer, comsumer;
    pthread_create(&producer, NULL, push, 0);
    pthread_create(&comsumer, NULL, get, 0);

    void *retval;
    pthread_join(producer, &retval);
    pthread_join(comsumer, &retval);

    return 0;
}
