#ifndef BLOCK_QUEUE_H_
#define BLOCK_QUEUE_H_

#include <pthread.h>
#include <iostream>  //debug
using namespace std;  //debug
template <typename T>
class BlockingQueue
{
  public:
	BlockingQueue()
	{
		this->capacity = 8;
		queue = new T[capacity];
		head = 0, tail = 0;
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&notFull, NULL);
		pthread_cond_init(&notEmpty, NULL);
	}
	BlockingQueue(int capacity)
	{
		this->capacity = capacity;
		queue = new T[capacity];
		head = 0, tail = 0;
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&notFull, NULL);
		pthread_cond_init(&notEmpty, NULL);
	}
	~BlockingQueue()
	{
		this->capacity = 0;
		head = 0, tail = 0;
		delete[] queue;
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&notFull);
		pthread_cond_destroy(&notEmpty);
	}

	bool push(T &item)
	{
		pthread_mutex_lock(&mutex);
		while ((head + 1) % capacity == tail) //is full
		{
			//cout << "is full,wait..." << endl;  
			// push wait
			pthread_cond_wait(&notFull, &mutex);
			//cout << "not full,unlock" << endl;  
		}

		queue[head] = item;
		head = (head + 1) % capacity;
		//wake up get thread
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&notEmpty);
		

		return true;
	}
	T get()
	{
		pthread_mutex_lock(&mutex);
		T ret;
		while (head == tail) // is empty
		{
			//cout << "is empty,wait..." << endl;  
			//get wait
			pthread_cond_wait(&notEmpty, &mutex);
			//cout << "not empty,unlock..." << endl; 
		}

		ret = queue[tail];
		tail = (tail + 1) % capacity;
		//wake up push thread
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&notFull);
		
		return ret;
	}
	bool isEmpty_withoutLock()
	{
		return (head == tail);
	}

  private:
	int capacity;
	T *queue;
	int head, tail;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
};

#endif //BLOCK_QUEUE_H_