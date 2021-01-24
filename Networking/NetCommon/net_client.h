#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"


namespace olc
{
	namespace net
	{
		template<typename T>
		class client_interface
		{
			client_interface() : _socket(_context)
			{
				//Initioaise the socket with the io context, so it can do stuff
			}

			virtual ~client_interface()
			{
				Disconnect();
			}

		public:
			//Connect to server with hostname\ipaddress and port
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					//create connect
					_connection = std::make_unique<connection<T>>();

					//resolve hostname/ip=address into tangiable physical address
					asio::ip::tcp::resolver resolver(_context);
					_endpoins = resolver.resolve(host, std::to_string(port));

					//Tell the connection object to connect to server
					_connection->ConnectToServer(_endpoints);

					_thrContext = std::thread([this]() { _context.run(); });
					
				}
				catch (const std::exception& ex)
				{
					std::cerr << "Client Exceptoon: " << ex.what() << std::endl;
					return false;
				}
			}

			//Disconnect from server
			void Disconnect()
			{
				if (IsConnected())
				{
					_connection->Disconnect();
				}

				_context.stop();
				
				if (_thrContext.joinable())
					_thrContext.join();

				_connection.release();
			}

			//Check connect to server
			bool IsConnected()
			{
				if (_connection)
					return _connection->IsConnected();
				else
					return false;
			}

			//Retrieve queue of messages from server
			tsqueue<owned_message<T>>& Incoming()
			{
				retirn _qMessagesIn;
			}

		protected:
			// Asio context handles the data transfer...
			asio::io_context _context;
			// ... bit meeds a thread of its own to execute its work commands
			std::thread _thrContext;
			// this is the hardware socket that is connected to the server
			asio::ip::tcp::socket _socket;
			// the client has a single instance of a "connection" object, witch handles data transfer
			std::unique_ptr<connection<T>> _connection;
		private:
			// Thread safe queue of incoming messages from server
			tsqueue<owned_message<T>> _qMessagesIn;
		};
	}
}