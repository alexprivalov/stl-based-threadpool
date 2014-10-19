#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <future>
#include <vector>
#include <queue>
#include <chrono>

namespace threadpool
{
	class threadpool
	{
		//maximum number of threads
		unsigned int m_max_threads;
		unsigned int m_threads_count;

		//queue for tasks
		std::queue < std::function<void()> > m_task_queue;
		std::mutex m_queue_mtx;
		
		//pool to hold threads
		std::vector < std::thread > m_pool;
		std::condition_variable m_awake;

		bool m_request_end;
		
	public:
		threadpool(unsigned int max_threads = std::thread::hardware_concurrency() / 2); //divided by 2 to keep some space on CPU for other things by default.
		~threadpool();
		
		//add a new task to the queue
		std::future<float> enqueue(std::function<float()> tfun);		

	};
}//namespace threadpool

#endif
