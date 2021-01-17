#include<chrono>
#include<iostream>

#ifdef _WIN32 //For windows, clear warnings
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE //use ASIO without boost

#include<asio.hpp>
#include<asio/ts/buffer.hpp> //Handles moving memory
#include<asio/ts/internet.hpp> //ASIO prepares for work with network communication

std::vector<char> vBuffer(1 * 1024); // 1kb buff

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (int i = 0; i < length; i++)
					std::cout << vBuffer[i];

				GrabSomeData(socket);
			}
		});
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	asio::error_code ec;
	
	//Create a "context" - essentioally the platform specific interface
	asio::io_context context;
	
	//Give some fake task to asio so the context doesnt finish
	asio::io_context::work idleWork(context);

	//Start the context
	std::thread thrContext = std::thread([&]() { context.run(); });

	//Get the address of somewhere we wish to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

	//Create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	//Tell socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected." << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{
		GrabSomeData(socket);

		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
		
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms);

		context.stop();
		if (thrContext.joinable()) thrContext.join();
	}

	system("pause");
	return 0;
}
