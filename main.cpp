// main.cpp
//
// GDAgent v. 0.0.1 alpha, by Ian Clauser
// Copyright  2014 Giganews.

#include<cstdlib>
#include<iostream>
#include<boost/bind.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/asio.hpp>
#include<boost/thread/thread.hpp>
#include"asio.h"

using boost::asio::ip::tcp;
int main( ) {
	try{
		char listen_port[] = "8000";
		boost::asio::io_service io_service;
		using namespace std; // for atoi
		server( io_service, atoi( listen_port ) );
	}
	catch( std::exception& e ) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
