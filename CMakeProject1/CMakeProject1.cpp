// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include " _Asio.hpp"
//#include <memory>
//#include <boost/asio.hpp>
//using boost::asio::ip::tcp;
using namespace std;




int main()
{

	//try {
	//	boost::asio::io_context io_context;
	//	tcp::resolver resolver(io_context);
	//	// 解析目标地址和端口
	//	auto endpoints = resolver.resolve("example.com", "80");

	//	tcp::socket socket(io_context);
	//	// 连接到服务器
	//	boost::asio::connect(socket, endpoints);

	//	// 发送 HTTP 请求
	//	std::string request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
	//	boost::asio::write(socket, boost::asio::buffer(request));

	//	// 接收响应
	//	boost::asio::streambuf buffer;
	//	boost::asio::read_until(socket, buffer, "\r\n");
	//	std::istream is(&buffer);
	//	std::string response;
	//	std::getline(is, response);
	//	std::cout << "Response: " << response << std::endl;
	//}
	//catch (std::exception& e) {
	//	std::cerr << "Error: " << e.what() << std::endl;
	//}


	//auto ll = spdlog::basic_logger_mt("qwlog", "call.log");
	//ll->info("fdjlkgkkfgjlkjfgjlkj");
	//cout << "Hello CMake." << endl;
	//return 0;


	try
	{
		// 获取端口号
		short port =666;

		// 创建IO上下文，boost::asio在Windows上默认使用IOCP
		boost::asio::io_context io_context;

		// 创建服务器实例
		Server server(io_context, port);

	//	std::cout << "Server started on port " << port << std::endl;
	//	std::cout << "Using IOCP: " << (boost::asio::detail::is_iocp_io_context<boost::asio::io_context>::value ? "Yes" : "No") << std::endl;

		// 获取处理器核心数量
		std::size_t num_cores = std::thread::hardware_concurrency();
		if (num_cores == 0) num_cores = 1;

		std::cout << "Starting " << num_cores << " worker threads..." << std::endl;

		// 创建工作线程池，通常与CPU核心数量相同
		std::vector<std::thread> threads;
		for (std::size_t i = 0; i < num_cores; ++i)
		{
			threads.emplace_back([&io_context]() { io_context.run(); });
		}

		// 等待所有线程完成
		for (auto& t : threads)
		{
			t.join();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}



//int main(int argc, char* argv[]) {
//	try {
//		if (argc != 4) {
//			std::cerr << "用法: " << argv[0] << " <监听端口> <目标主机> <目标端口>\n";
//			return 1;
//		}
//
//		// 解析命令行参数
//		short listen_port = std::atoi(argv[1]);
//		std::string target_host = argv[2];
//		short target_port = std::atoi(argv[3]);
//
//		std::cout << "端口转发启动: 监听 " << listen_port
//			<< " -> 转发到 " << target_host << ":" << target_port << std::endl;
//
//		// 创建IO上下文并运行服务器
//		boost::asio::io_context io_context;
//		Server server(io_context, listen_port, target_host, target_port);
//		io_context.run();
//	}
//	catch (std::exception& e) {
//		std::cerr << "异常: " << e.what() << "\n";
//	}
//
//	return 0;
//}