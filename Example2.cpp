#include<iostream>
#include<string>
#include<thread>
#include<vector>
#include<algorithm>

#include "MessageDistributor.h"

using namespace std;

const int numConsumerThreads = 10;
MessageDistributor<std::string> distributor(1); // Remember you need to pass the number of consumers waiting for producer.
std::mutex cout_mutex;
std::mutex consumer_mutex;
const long runDemoLoop = 10000;
int consumerLoopCount=0;

void printOut(const std::string& str1, const std::string& str2, const std::thread::id& id)
{
	std::unique_lock<std::mutex> lock(cout_mutex);
	std::cout << str1 << id << str2 << std::endl;
	lock.unlock();
}

void producerThread(const std::vector<std::string>& dataSource)
{
	printOut("I am producer thread currently running from thread id =", "",  std::this_thread::get_id());
	int i=0;

	while(i<runDemoLoop)
	{
		try
		{
			distributor.publish(dataSource[i]);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		++i;
	}
}

void consumerThread()
{

	printOut("I am consumer thread currently running from thread id =", "",  std::this_thread::get_id());
	std::string returnString;

	while(consumerLoopCount<runDemoLoop)
	{
		try
		{
			unique_lock<mutex> exclusiveLock(consumer_mutex);
			if(consumerLoopCount<runDemoLoop)
			{
				returnString = distributor.recieve();         
				++consumerLoopCount;
				printOut("I am consumer thread currently running from thread id =", ".The value i am having now is = " + returnString, std::this_thread::get_id()); 
			}
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}


// This is a demo of the producer and consumer threads written using MessageDistributor. The producer thread produces messages and wait for consumer thread on Exchanger synchronization.
// When they meet the Producer sends the message and recives nothing which is fine. The consumer gets the message and sends a blank string which producer can ignore.

// This implementation of a MessageDistributor can be used to implement either of the following.
// 1. If you want a single message processed only 1 time by a consumer thread then simply create more consumer threads and implement a single consumer thread MessageDistributor.
//    This is in principle equivalent to the multi threaded subscriber design.
// 2. If you want the message to be consumed by all consumer threads then simply pass the number of consumerThreads to the MessageDistributors at the start of construction.

int main(int argc, char* argv[])
{
	std::vector<std::string> dataSource;
	dataSource.reserve(runDemoLoop);

	for(unsigned int i=0; i<runDemoLoop; ++i)
		dataSource.emplace_back("Sample Message : " + std::to_string(i));

	// This is the producer thread.
	std::thread produceThread(&producerThread, std::cref(dataSource));

	// Create a vector of consumer threads.	
	std::vector<std::thread> consumerThreads;
	for(unsigned int i=0; i<numConsumerThreads; ++i)
		consumerThreads.emplace_back(std::thread(&consumerThread));

	// Upon finish join the producer thread.
	produceThread.join();

	// Join the consumer threads.
	std::for_each(consumerThreads.begin(), consumerThreads.end(), [&](std::thread& t){ t.join();});

	return(0);
}
