#include<chrono>
#include<iostream>
//15:00

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE //use ASIO without boost
#include<asio.hpp>
#include<asio/ts/buffer.hpp> //Handles moving memory
#include<asio/ts/internet.hpp> //ASIO prepares for work with network communication

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	asio::error_code ec;
	
	//Create a "context" - essentioally the platform specific interface
	asio::io_context context;

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
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
		
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		
		/*using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms); BAD IDEA*/

		socket.wait(socket.wait_read);

		size_t bytes = socket.available();
		std::cout << "Bytes Avaliable: " << bytes << std::endl;

		if (bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
			
			for (auto c : vBuffer)
				std::cout << c;
		}
	}

	system("pause");
	return 0;
}