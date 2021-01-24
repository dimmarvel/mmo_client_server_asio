#pragma once

#include "net_common.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete; //delete copy constructor
			virtual ~tsqueue() { clear(); };
		public:
			const T& front()
			{
				std::scoped_lock lock(mtxQueue);
				return deqQueue.front();
			}
			
			// Returns and maintains item at back of Queue
			const T& front()
			{
				std::scoped_lock lock(mtxQueue);
				return deqQueue.back();
			}

			//Adds an item to back of Queue
			void push_back(const T& item)
			{
				std::scoped_lock lock(mtxQueue);
				return deqQueue.emplace_back(std::move(item);
			}

			//Adds an item to front of Queue
			void push_front(const T& item)
			{
				std::scoped_lock lock(mtxQueue);
				return deqQueue.emplace_front(std::move(item);
			}

			bool empty()
			{
				std::scoped_lock lock(mtxQueue);
				return deqQueue.empty();
			}

			void clear()
			{
				std::scoped_lock lock(mtxQueue);
				deqQueue.clear();
			}

			// Remove and returns item from back of Queue
			T pop_back()
			{
				std::scoped_lock lock(mtxQueue);
				auto t = std::move(deqQueue.back());
				deqQueue.pop_back();
				return t;
			}

			// Remove and returns item from front of Queue
			T pop_front()
			{
				std::scoped_lock lock(mtxQueue);
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}

		protected:
			std::mutex mtxQueue;
			std::deque<T> deqQueue;
		};
	}
}