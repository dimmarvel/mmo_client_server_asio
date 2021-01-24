#pragma once

#include"net_common.h"
#include"net_tsqueue.h"
#include"net_message.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			connection()
			{}

			virtual ~connection()
			{}
		
		public:
			bool ConnectToServer();
			bool Disconncet();
			bool IsConnected() const;
		
		public:
			bool Send(const message<T>& msg);
		
		protected:
			// Each connection has a unique socket to a remote
			asio::ip::tcp::socket _socket;
			
			// This context is shared with the whole asio instance
			asio::io_context& _asioContext;

			//This queue hold all messages to be sent to the remote side of this connection
			tsqueue<message<T>> _qMessagesOut;

			tsqueue<owned_message>& _qMessageIn;
		};
	}
}