// asio.h
//
// written using examples at: 
// http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
//
#include<cstdlib>
#include<iostream>
#include<boost/bind.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/asio.hpp>
#include<boost/thread/thread.hpp>
#include"read.h"
#include"db.h"

using boost::asio::ip::tcp;

const int max_length = 1024;


typedef boost::shared_ptr<tcp::socket> socket_ptr;



void handle_data( int buffer_data[max_length] ) {
	serial buffer;
	buffer.readBits( buffer_data );
	buffer.deSerialize();
	dbase thread;
	thread.getQueryData( buffer.Srstate, buffer.Sstype, buffer.Ssite, buffer.Shname, buffer.Satype, buffer.Sservices, buffer.Sanum, buffer.Svarfill, buffer.Sdnum );
	thread.runQuery();
}


void session( socket_ptr sock ) {
	try {
		for( ; ; ) {
			char data[max_length];
			boost::system::error_code error;
			size_t length = sock->read_some( boost::asio::buffer( data ), error );
			if( error == boost::asio::error::eof ) { // in original program, this is supposed to close the thread, but I'll have it pass it's data onto another class. thread will terminate AFTER completion
				break; // connection closed cleanly by peer
			}
			else if( error ) {
				throw boost::system::system_error( error ); //some other error
			}
			boost::asio::write( *sock, boost::asio::buffer( data, length) );
			int data_stream[max_length];
			for ( int i = 0; i < max_length; i++ ) {
				data_stream[i] = int(data[i]);
			}
			serial buffer;
			buffer.readBits( data_stream );
			buffer.deSerialize();
			dbase db;
			db.getQueryData( buffer.Srstate, buffer.Sstype, buffer.Ssite, buffer.Shname, buffer.Satype, buffer.Sservices, buffer.Sanum, buffer.Svarfill, buffer.Sdnum );
			db.runQuery();
		}
	}
	catch( std::exception& e ) {
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void server( boost::asio::io_service& io_service, short port ) {
	tcp::acceptor a( io_service, tcp::endpoint( tcp::v4(), port ) );
	for( ; ; ) {
		socket_ptr sock( new tcp::socket( io_service ) );
		a.accept( *sock );
		boost::thread t( boost::bind( session, sock ) );
	}
}


