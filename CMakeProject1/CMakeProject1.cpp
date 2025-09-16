// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;


using namespace std;

int main()
{

	try {
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);
		// 解析目标地址和端口
		auto endpoints = resolver.resolve("example.com", "80");

		tcp::socket socket(io_context);
		// 连接到服务器
		boost::asio::connect(socket, endpoints);

		// 发送 HTTP 请求
		std::string request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
		boost::asio::write(socket, boost::asio::buffer(request));

		// 接收响应
		boost::asio::streambuf buffer;
		boost::asio::read_until(socket, buffer, "\r\n");
		std::istream is(&buffer);
		std::string response;
		std::getline(is, response);
		std::cout << "Response: " << response << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}


	auto ll = spdlog::basic_logger_mt("qwlog", "call.log");
	ll->info("fdjlkgkkfgjlkjfgjlkj");
	cout << "Hello CMake." << endl;
	return 0;
}
