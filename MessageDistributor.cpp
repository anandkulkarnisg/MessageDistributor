#include<iostream>
#include<string>

#include "MessageDistributor.h"

using namespace std;

// First implement the Constructor for the MessageDistributor.
template<typename T> MessageDistributor<T>::MessageDistributor(const int& participantCount)
{
	// Takes the count of number of consumer threads needing the message. Producer is considered and counted implcitly below.
	m_preBarrierPtr.reset(new FlexBarrier(participantCount+1));
	m_postBarrierPtr.reset(new FlexBarrier(participantCount+1));
}

// Implement a publish method for the MessageDistributor.
template<typename T> void MessageDistributor<T>::publish(const T& inputItem)
{
	// Internally just call the exchange method. As a producer we need to exchange the item and get nothing in return.
	T temp = exchange(inputItem, MessageDistributorType::producer);
}

// Implement the recieve method using the exchange method.As a consumer we need to recieve the item and send in nothing.
template<typename T> T MessageDistributor<T>::recieve()
{
	T returnMessage = exchange(std::string(""), MessageDistributorType::consumer);
	return(returnMessage);
}

// Implement the Exchange method that actually does the syncing of the producer and consumer thread calls.
template<typename T> T MessageDistributor<T>::exchange(const T& inputItem, const MessageDistributorType& type)
{
	try
	{
		m_preBarrierPtr->await();
	}
	catch(const std::exception& e)
	{
		throw;	// Pass on the exceptions as is for further analysis or debugging.
	}

	// We have come here means barrier is broken and we can either set the data item as producer or consume the item as a consumer.
	if(type == MessageDistributorType::producer)
	{
		m_MessageItem = inputItem;
	}

	// We sync up again using post exchange barrier.
	try
	{
		m_postBarrierPtr->await();
	}
	catch(const std::exception& e)
	{
		throw;  // Pass on the exceptions as is for further analysis or debugging.
	}

	// Return the items to respective other threads.
	if(type == MessageDistributorType::consumer)
		return(m_MessageItem);
	else
		return(T());
}

template<typename T> void MessageDistributor<T>::reset()
{
	if(m_preBarrierPtr->isBroken())
		m_preBarrierPtr->reset();
	if(m_postBarrierPtr->isBroken())
		m_postBarrierPtr->reset();	
}

template class MessageDistributor<std::string>;
