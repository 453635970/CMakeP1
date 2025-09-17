


#include <iostream>
#include <memory>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;



// 会话类，处理与单个客户端的通信
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket) : socket_(std::move(socket)) {}

	// 开始会话
	void start()
	{
		std::cout << "New client connected: "
			<< socket_.remote_endpoint().address().to_string() << std::endl;
		do_read();
	}

private:
	// 异步读取数据
	void do_read()
	{
		/*async_read_some 参数1 存储读取数据的缓冲区
		 参数2 回调函数 异步读取完成后触发的回调函数，用于处理读取结果
		[捕获列表](boost::system::error_code ec, std::size_t bytes_transferred) {
			 处理逻辑
		}
		第一个参数 ec：boost::system::error_code 类型，标识读取操作是否成功：
		!ec 表示成功，bytes_transferred 为实际读取的字节数。
		常见错误：ec == boost::asio::error::eof（客户端正常关闭连接）、ec == boost::asio::error::operation_aborted（操作被取消）等。
		第二个参数 bytes_transferred：std::size_t 类型，实际读取到的字节数（成功时有效）。*/

		//shared_from_this()作用：返回一个指向当前对象（*this）的 std::shared_ptr<T> 智能指针。
		//前提：当前对象必须已被 std::shared_ptr 管理（即通过 std::make_shared 创建），否则调用会抛出异常。
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "Received from " << socket_.remote_endpoint().address().to_string()
					<< ": " << std::string(data_, length) << std::endl;
				//回声响应：将接收到的数据原样返回
				do_write(length);
			}
			else
			{
				std::cout << "Client disconnected: "
					<< socket_.remote_endpoint().address().to_string() << std::endl;
			}
		});
	}

	// 异步写入数据
	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				do_read(); // 继续读取下一次数据
			}
			else
			{
				std::cout << "Write error: " << ec.message() << std::endl;
			}
		});
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};



// 服务器类，负责接受客户端连接
class Server
{
public:
	Server(boost::asio::io_context& io_context, short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	{
		do_accept();
	}

private:
	// 异步接受客户端连接
	void do_accept()
	{
		//async_accept 的 lambda 表达式需要接收两个参数
		//tcp::socket socket 参数是由 Boost.Asio 内部自动赋值的，无需你手动初始化
		acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				// 创建新会话并启动
				std::make_shared<Session>(std::move(socket))->start();
			}
			else
			{
				std::cerr << "Accept error: " << ec.message() << std::endl;
			}
			// 继续接受下一个连接
			do_accept();
		});
	}
	//用于服务器端，监听并接受 incoming 的 TCP 连接
	tcp::acceptor acceptor_;
};
