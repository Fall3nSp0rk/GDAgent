// asio.h
//
// written using examples at: 
// http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
//
#include<cstdlib>
#include<iostream>
#include<boost/bind.hpp>
#include<boost/smart_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include<boost/thread.hpp>
#include"include/serial.h"
#include"include/dbase.h"
#include<syslog.h>
#include<sys/types.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<ctime>
#include"include/log.h"
#include"include/util.h"
#include<algorithm>
#include<exception>
using boost::asio::ip::tcp;

const int max_length = 1024;
logger GDLogger;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

bool handle_data( std::vector<int>& buffer_data ) {
	GDLogger.log( "handle_data() called.", 0 );
	try {
		boost::system::error_code ec;
		serial sbuff;
		sbuff.readBits( buffer_data );
		dbase thread;
		bool dsvalidated = sbuff.deSerialize();
		if(!dsvalidated) {
			GDLogger.log( "Deserialized data validation returned as false.", 3 );
		}
		else {
			thread.getQueryData( sbuff.Srstate, sbuff.Sstype, sbuff.Ssite, sbuff.Shname, sbuff.Satype, sbuff.Sservices, sbuff.Sanum, sbuff.Sshname, sbuff.Smac, sbuff.Svarfill, sbuff.Sdnum ); // handing all the data off to the database module.
			thread.runQuery();
		}
		if( !ec ) {
			GDLogger.log( "handle_data() call completed without error.", 0 );
			return true;
		}
		else {
			GDLogger.log( "handle_data() call unsuccsessful, errors occurred.", 3 );
			return true;
		}
	}
	catch( std::exception& e ) {
		std::stringstream errstr;
		errstr << "Exception in thread: " << e.what();
		std::string logentry = errstr.str();
		errstr.str("");
		GDLogger.log( logentry, 3 );
	}
}

std::string timestamp() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime( buf, sizeof(buf), "%Y-%m-%d %X", &tstruct );
	return buf;
}

std::vector<int> sanData( char bdata[max_length] ) {
	std::stringstream ss( bdata );
	std::string s2 = ss.str();
	ss.str("");
	std::string contlen = s2;
	size_t f = contlen.find("Content-Length: ");
	size_t p = s2.find( "\r\n\r\n" );
	int h = f + 16;
	size_t contend = contlen.length();
	contlen.erase( p, contend);
	contlen.erase( 0, h );
	char cc[4];
	for( int i = 0; i < 5; i++ ) {
		cc[i] = contlen[i];
	}
	delNonNumericChar( cc );
	std::string bbsize;
	for( int i = 0; i < 4; i++ ) {
		bbsize[i] = cc[i];
	}
	using namespace std;
	int bsize = atoi( bbsize.c_str() );
	using boost::asio::ip::tcp;
	p = p + 4;
	int eob = p + bsize;
	int eos = s2.length();
	s2.erase( eob, eos);
	s2.erase( 0, p );
	int s2l = s2.length();
	std::vector<int> odata;
	odata.resize( max_length );
	GDLogger.log( s2, 0 );
	for (int i = 0; i < max_length; i++ ) {
		odata[i] = ccInt( s2[i] );
	}
	return odata;
}
bool valSerialData( const vector<int> &vdata ) {
	if( (vdata[0] == 0 && vdata[1] == 0 )
			&& ( vdata[5] == 1 && vdata[13] == 1)
			//&& ( vdata[14] == 0 && vdata[15] == 1)
			) {
		GDLogger.log( "Datastream Validated.", 1 );
		return true;
	}
	else {
		return false;
	}
}

void session( socket_ptr sock ) {
	// boost::this_thread::disable_interruption di;
	logger GDLogger;
	bool inproc = false; // this will be set to true once input has been sucessfully processed.
	try {
		GDLogger.log( "Connection Established.", 1 );
		int icont = 0;
		for( ; ;) {
			char data[max_length];
			boost::system::error_code error;
			size_t length = sock->read_some( boost::asio::buffer( data ), error );
			char cdata[max_length];
			for( int i = 0; i < max_length; i++ ) {
				cdata[i] = data[i];
			}
			std::vector<int> idata;
			idata.resize( max_length );
			idata = sanData( cdata );
			/* std::stringstream ss( data );
			std::vector<int> idata;
			std::string s2 = ss.str();
			ss.str("");
			if( icont == 0 && s2.find("Expect: 100-continue") ){
				std::string continuemsg = "HTTP/1.1 100 CONTINUE \n";
				int m = continuemsg.size();
				char contin[m];
				for( int i = 0; i < m; i++ ) {
					contin[i] = continuemsg[i];
				}
				boost::asio::write( *sock, boost::asio::buffer( contin, m) );
			}
			std::string contlen = s2;
			size_t f = contlen.find("Content-Length: ");
			size_t p = s2.find( "\r\n\r\n" );
			int h = f + 16;
			size_t contend = contlen.length();
			contlen.erase( p, contend);
			contlen.erase( 0, h );
			char cc[4];
			for( int i = 0; i < 4; i++ ) {
				cc[i] = contlen[i];
			}
			delNonNumericChar( cc );
			std::string bbsize;
			for( int i = 0; i < 5; i++ ) {
				bbsize[i] = cc[i];
			}
			using namespace std;
			int bsize = atoi( bbsize.c_str() );
			using boost::asio::ip::tcp;
			p = p + 4;
			int eob = p + bsize;
			int eos = s2.length();
			s2.erase( eob, eos);
			s2.erase( 0, p );
			int s2l = s2.length(); 
			for ( int i = 0; i < max_length; i++ ) {
				idata[i] = ccInt( s2[i] );
				
			} */
			bool dvalid = valSerialData( idata );
			if( !dvalid ) {
				GDLogger.log("Data rejected by server.", 3 );
			}
			if( dvalid ) {
				GDLogger.log("Valid Datastream accepted. Processing.", 1 );
				inproc = handle_data( idata );
			}
			/* if( !error && icont == 0 ) {
				std::string curtime = timestamp();
				std::stringstream okmsg;
				okmsg << "HTTP/1.1 200 OK \n Date: " << curtime << "\n";
				std::string okm = okmsg.str();
				int oklen = okm.length();
				char okma[oklen];
				for( int i = 0; i < okm.length(); i++ ) {
					okma[i] = okm[i];
				}
				size_t oksize = sizeof(okma);
				boost::asio::write( *sock, boost::asio::buffer( okma, oksize) );
			}
			*/
			if( inproc == true && error == boost::asio::error::eof ) { // if EOF reached and 
				GDLogger.log( "Connection Terminated, data processed.", 1 );
				break;
			}
			else if( !inproc && error == boost::asio::error::eof  && icont > 0) {
				GDLogger.log( "Connection Terminated before data was successfully processed.", 2 );
				break;
			 }
			else if( error ) {
				throw boost::system::system_error( error ); //some other error
			}
			icont++;
		}
	}
	catch( std::exception& e ) {
		std::stringstream errstr;
		errstr << "Exception in thread: " << e.what() << "\n";
		std::string logentry = errstr.str();
		errstr.str("");
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
	boost::system::error_code er;
	logger MT;
	MT.log( "GDAgent v 0.5 Started. Initializing.", 1 );
	MT.log( "Daemonizing....", 1 );
	seedDaemon();
	if( !er ) {
		MT.log( "Daemonizing successful.", 1 );
	}
	else {
		MT.log( "Daemonizing failed.", 3 );
	}
	try{
		MT.log( "Starting IO service... ", 1 );
		using boost::asio::ip::tcp;
		char listen_port[] = "8000";
		boost::asio::io_service io_service;
		using namespace std; // for atoi
		server( io_service, atoi( listen_port ) );
	}
	catch( std::exception& e ) {
		std::stringstream errmsg;
		errmsg << "Exception: " << e.what() << "\n";
		std::string em = errmsg.str();
		errmsg.str("");
		MT.log( em, 3 );
	}
	return 0;
}
