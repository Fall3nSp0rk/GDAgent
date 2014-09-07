#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
boost::asio::io_service ioService;
boost::thread_group tpool;

boost::asio::io_service::work work(ioService);

