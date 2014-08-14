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
#include"serial.h"
#include"dbase.h"

using boost::asio::ip::tcp;

const int max_length = 1024;


typedef boost::shared_ptr<tcp::socket> socket_ptr;



void handle_data( std::vector<int>& buffer_data ) {
	serial sbuff;
	sbuff.readBits( buffer_data );
	sbuff.deSerialize();
	dbase thread;
	thread.getQueryData( sbuff.Srstate, sbuff.Sstype, sbuff.Ssite, sbuff.Shname, sbuff.Satype, sbuff.Sservices, sbuff.Sanum, sbuff.Svarfill, sbuff.Sdnum );
	thread.runQuery();
}


int charToInt( const char c ) {
	switch (c) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return 0;
	}
}

void session( socket_ptr sock ) {
	try {
		for( ; ; ) {
			char data[max_length];
			boost::system::error_code error;
			size_t length = sock->read_some( boost::asio::buffer( data ), error );
			std::stringstream ss;
			for( int i = 0; i < max_length; i++ ) {
				ss << data[i];
			}
			std::vector<int> idata;
			std::string s2 = ss.str();
			s2.erase( 0, 241 );
			idata.resize( max_length );
			//std::cout<<s2;
			for (int i = 0; i < max_length; i++ ) {
				idata[i] = charToInt( s2[i] );
				//std::cout<<idata[i];
			}
			handle_data( idata );
			//for( int i =0; i < max_length; i++) {
			//	std::cout<< data[i];
			//}
			if( error == boost::asio::error::eof ) { // in original program, this is supposed to close the thread, but I'll have it pass it's data onto another class. thread will terminate AFTER completion
				break; // connection closed cleanly by peer
			}
			else if( error ) {
				throw boost::system::system_error( error ); //some other error
			}
			char cont[max_length] = { '/', '\r',  '1', '0', '0', ' ', 'C', 'o', 'n', 't', 'i', 'n', 'u', 'e', '\r', '\n' };

			size_t len = sizeof(cont);
			boost::asio::write( *sock, boost::asio::buffer( cont, len) );
			std::cout<< '\r' << '\n' << "Next" << '\n';
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

