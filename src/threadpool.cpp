
  #include "threadpool/threadpool.h"

#include <memory>

// for test output
#include <iostream>

  namespace threadpool
  {
	  threadpool::threadpool(unsigned int max_threads)
	  {
		  m_max_threads = max_threads;

		  for (int i = 0; i < m_max_threads; ++i)
		  {
			  m_pool.push_back(
				  std::thread([=](){

					  while (true) // always looping, running tasks.
					  {
						  //get next task
						  std::function<void()> task;

						  auto lck = std::unique_lock<std::mutex>(m_queue_mtx);
						  // wait until notified for end,  or when task needs to be processed
						  m_awake.wait(lck,[=]()->bool{ return m_request_end || !m_task_queue.empty(); });
						  {
						    //if notified on end and queue empty, terminates.
						    if ( m_request_end && m_task_queue.empty() ) return;
						    //else process next task
						    task = m_task_queue.front();
						    m_task_queue.pop();
						  }

						  //process it
						  task();
					  }

				  })
			  );
		  }
	  }

	  std::future<float> threadpool::enqueue(std::function<float()> tfun)
	  {
	  
	      auto tskr = new std::packaged_task<float()>(tfun);
	      std::shared_ptr < std::packaged_task<float()> > task( tskr);
	      {
		std::unique_lock<std::mutex>(m_queue_mtx);

		if ( ! m_request_end )
		{
		  //add to queue
		  m_task_queue.push( [task](){ 
		    (*task)();
		  });
		  
		  //awake one thread
		  m_awake.notify_one();
		  
		}
		else
		{
		  //cannot enqueue
		  //doing it right now
		  (*task)();		  
		}
	      }
	      
	      return task->get_future();

	  }
	  
	  threadpool::~threadpool()
	  {
     {
std::unique_lock<std::mutex> lock(m_queue_mtx);
m_request_end= true;
}
m_awake.notify_all();
// wait for all threads to end
for(std::thread & t: m_pool)
t.join();
   }

  }//  namespace threadpool
