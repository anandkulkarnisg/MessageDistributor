#include<iostream>
#include<string>
#include<memory>
#include<mutex>
#include<thread>
#include<CyclicBarrier.h>
#include<boost/core/noncopyable.hpp>

#ifndef MessageDistributor_H
#define MessageDistributor_H

enum class MessageDistributorType { producer , consumer };

template<typename T> class MessageDistributor : private boost::noncopyable
{
	private:
		std::unique_ptr<CyclicBarrier> m_preBarrierPtr;						// We attempt to use a Flex Barrier to implement an MessageDistributor item.
		std::unique_ptr<CyclicBarrier> m_postBarrierPtr;						// We attempt to use a pair of pre and post barriers to sync up exchanging threads [ Always 2 of them ].
		T m_MessageItem;													// Underlying data of the MessageDistributor.

	public:
		MessageDistributor(const int&);										// Default Constructor. Takes number of parties.
		void publish(const T&);												// Publishes a given item to the waiting consumer threads over the barrier sync.
		T recieve();														// Recieves the published item in all the reciever threads over the barrier sync.
		void reset();														// The barriers can go broken due to timeouts. Hence need a reset interface.
};

#endif
