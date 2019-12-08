#include<iostream>
#include<string>

#include "MessageDistributor.h"
#include "MessageDistributorImpl.cpp"

using namespace std;

// First implement the Constructor for the MessageDistributor.
template<typename T> MessageDistributor<T>::MessageDistributor(const int& participantCount)
{
	// Takes the count of number of consumer threads needing the message. Producer is considered and counted implcitly below.
	m_preBarrierPtr.reset(new CyclicBarrier(participantCount+1));
	m_postBarrierPtr.reset(new CyclicBarrier(participantCount+1));
}

// Implement a publish method for the MessageDistributor.
template<typename T> void MessageDistributor<T>::publish(const T& inputItem)
{
	try
	{
		m_preBarrierPtr->await();
	}
	catch(const exception& e)
	{
		throw;  // Pass on the exceptions as is for further analysis or debugging.
	}

	m_MessageItem = inputItem;

	// We sync up again using post exchange barrier.
	try
	{
		m_postBarrierPtr->await();
	}
	catch(const exception& e)
	{
		throw;  // Pass on the exceptions as is for further analysis or debugging.
	}
}

// Implement the recieve method using the exchange method.As a consumer we need to recieve the item and send in nothing.
template<typename T> T MessageDistributor<T>::recieve()
{
	try
	{
		m_preBarrierPtr->await();
	}
	catch(const exception& e)
	{
		throw;  // Pass on the exceptions as is for further analysis or debugging.
	}

	// We sync up again using post exchange barrier.
	try
	{
		m_postBarrierPtr->await();
	}
	catch(const exception& e)
	{
		throw;  // Pass on the exceptions as is for further analysis or debugging.
	}

	return(m_MessageItem);
}

template<typename T> void MessageDistributor<T>::reset()
{
	if(m_preBarrierPtr->isBroken())
		m_preBarrierPtr->reset();
	if(m_postBarrierPtr->isBroken())
		m_postBarrierPtr->reset();	
}

