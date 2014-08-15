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
#include"include/serial.h"
#include"include/dbase.h"
#include<syslog.h>
#include<sys/types.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include"include/log.h"
#include"include/util.h"
using boost::asio::ip::tcp;

const int max_length = 1024;
logger GDLogger;

typedef boost::shared_ptr<tcp::socket> socket_ptr;



void handle_data( std::vector<int>& buffer_data ) {
	GDLogger.log( "handle_data() called.", 0 );
	serial sbuff;
	sbuff.readBits( buffer_data );
	sbuff.deSerialize();
	dbase thread;
	thread.getQueryData( sbuff.Srstate, sbuff.Sstype, sbuff.Ssite, sbuff.Shname, sbuff.Satype, sbuff.Sservices, sbuff.Sanum, sbuff.Sshname, sbuff.Svarfill, sbuff.Sdnum );
	thread.runQuery();
}



void session( socket_ptr sock ) {
	boost::this_thread::disable_interruption di;
	logger GDLogger;
	try {
		GDLogger.log( "Connection Established.", 1 );
		int icont = 0;
		for( ; ; ) {
			
			char data[max_length];
			boost::system::error_code error;
			size_t length = sock->read_some( boost::asio::buffer( data ), error );
			std::stringstream ss( data );
			std::vector<int> idata;
			std::string s2 = ss.str();
			ss.str("");
			size_t p = s2.find( "\r\n\r\n" );
			p = p + 4;
			s2.erase( 0, p );
			idata.resize( max_length );
			GDLogger.log( s2, 1 );
			for (int i = 0; i < max_length; i++ ) {
				idata[i] = ccInt( s2[i] );
				//std::cout<<idata[i];
			}
			//for( int i =0; i < max_length; i++) {
			//	std::cout<< data[i];
			//}
			if( error == boost::asio::error::eof ) {
				break;
			}
			else if( error ) {
				throw boost::system::system_error( error ); //some other error
			}
			char cont[max_length] = { '/', '\r',  '1', '0', '0', ' ', 'C', 'o', 'n', 't', 'i', 'n', 'u', 'e', '\r', '\n' };
			if( idata[0] == 0 && idata[1] == 0 ) {
				handle_data( idata );
			}
			//sleep(500);
			size_t len = sizeof(cont);
			if( icont == 0 ) {
				boost::asio::write( *sock, boost::asio::buffer( cont, len) );
			}
			icont++;
		}
	}
	catch( std::exception& e ) {
		std::stringstream errstr;
		errstr << "Exception in thread: " << e.what() << "\n";
		std::string logentry = errstr.str();
		GDLogger.log( logentry, 3 );
	}
	GDLogger.log( "Thread Exited.", 0 );
}

void server( boost::asio::io_service& io_service, short port ) {
	tcp::acceptor a( io_service, tcp::endpoint( tcp::v4(), port ) );
	for( ; ; ) {
		socket_ptr sock( new tcp::socket( io_service ) );
		a.accept( *sock );
		boost::thread t( boost::bind( session, sock ) );
	}
}

bool seedDaemon() {
	using namespace std;
	int childpid = 0;
	pid_t pid = 0;
	if( ( childpid = fork() ) < 0 ) {
		return false;
	}
	else if( childpid > 0 ){
		exit(0);
	}
	setsid();
	umask(0);
	close( fileno(stderr) );
	close( fileno(stdout) );
	close( STDIN_FILENO );
	return true;
}


int main( ) {
	seedDaemon();
	try{
		using boost::asio::ip::tcp;
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
