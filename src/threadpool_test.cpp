#include "threadpool/threadpool.h"

#include <random>
#include <functional>
#include <memory>
#include <deque>
#include <iostream>

int main(int argc, char* argv[])
{
  //TODO :add parameters to test CPU capabilities
	//number of threads could be parameter
	threadpool::threadpool tp(10);

	std::default_random_engine generator;
	//distribution mas could be parameter
	std::uniform_int_distribution<int> distribution(1, 100);
	auto randgen = std::bind(distribution,generator);
	//start processing all
	//number of computation could be parameter
	for (int i = 0; i < 1000; i++)
	{
		int roll = randgen();	  
		auto result = tp.enqueue([=]() -> float{
		      float res = sqrt(roll);
		      std::cout << roll << " -> " << res << std::endl;
		      return res;
		});
		//we can do something else with result here
	}
	
	//we could do something with results here.
	//It would be interesting to display then asynchronously, but we d need an UI
	
	//tp will be deleted here. tasks must all be done first
	return 0;
}


