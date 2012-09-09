/*
 * MultithreadQueue.hpp
 *
 *  Created on: Sep 7, 2012
 *      Author: ricardo
 */

#ifndef MULTITHREADQUEUE_HPP_
#define MULTITHREADQUEUE_HPP_

#include <queue>
#include <thread>

template <class T>
class MultithreadQueue {
private:
	std::queue<T> mtQueue;
	std::mutex mtMutex;
public:
	MultithreadQueue(	);
	~MultithreadQueue();
	void push(T elem) {
		mtMutex.lock();
		mtQueue.push(elem);
		mtMutex.unlock();
	}
	T pop() {
		T elem;
		mtMutex.lock();
		elem = mtQueue.front();
		mtQueue.pop();
		mtMutex.unlock();
		return elem;
	}
	bool empty() {
		bool isEmpty;
		mtMutex.lock();
		isEmpty = mtQueue.empty();
		mtMutex.unlock();
		return isEmpty;
	}
};

#endif /* MULTITHREADQUEUE_HPP_ */
