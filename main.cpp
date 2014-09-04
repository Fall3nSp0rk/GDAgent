// main.cpp
// Main file for GDAgent. All the magic happens here.
//
// written using examples at: 
// http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
//
//lot of includes here....
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
#include "include/drive.h"
#include "include/mmapper.h"
using boost::asio::ip::tcp;
#define _logger mlog
const int max_length = 1024;
// define the pointer to where socket data is being stored
typedef boost::shared_ptr<tcp::socket> socket_ptr;

bool handle_data( std::vector<int>& buffer_data ) { // data handling happens here
	_logger.logstream << "handle_data() called.";
	_logger.log( 0 );
	try {
		serial ser;
		boost::system::error_code ec; // declare an error object to collect exceptions
		ser.readBits( buffer_data ); //read from buffer into saod object
		std::vector<int> dbuff;
		dbase thread;
		if( buffer_data.size() >=616 ) {
			//ser.readDVec( dbuff );
			for( int i = 0; i < ser.Sdbits.size(); i++ ) {
				dbuff.push_back( ser.Sdbits[i] );
			
			}
		}
		bool dsvalidated = ser.deSerialize(); // deserialize data
		if(!dsvalidated) { // self explanatory
			_logger.logstream << "Deserialized data validation returned as false.";
			_logger.log( 3 ); //log an error
		}
		else { // I hate this function. Reads from the serial object to the DB object.
			int ddnum = ser.Sdnum;
			std::string dshname = ser.Sshname;
			ddrive dd( ddnum, dshname, dbuff );
			thread.getQueryData( ser.Srstate, ser.Sstype, ser.Ssite, ser.Shname, ser.Satype, ser.Sservices, ser.Sanum, ser.Sshname, ser.Smac, ser.Svarfill, ser.Sdnum ); // handing all the data off to the database module.
			thread.runQuery(); // runs all the queries necessary to update or insert
			dd.readDriveData( dbuff, thread );
			
		}
		if( !ec ) {
			_logger.logstream << "handle_data() call completed without error.";
			_logger.log( 0 ); // if ec is still empty, we're done!
			return true;
		}
		else {
			_logger.logstream << "handle_data() call unsuccsessful, errors occurred."; // things broke~somewhere~, but not fatally
			_logger.log( 3 );
			return true; 
		}
	}
	catch( std::exception& e ) { // catch allocation exceptions.
		_logger.logstream << "Exception in thread: " << e.what();
		_logger.log( 3 );
	}
}

std::string timestamp() { // needs to get moved to util.h as an inline
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime( buf, sizeof(buf), "%Y-%m-%d %X", &tstruct );
	return buf;
}

std::vector<int> sanData( char bdata[max_length] ) { // sanitizes and trims received data to prevent exceptions and buffer overflows
	std::stringstream ss( bdata );
	std::string s2 = ss.str();
	ss.str("");
	std::string contlen = s2; // make a copy of the buffer string, for retrieving the length from http headers
	size_t f = contlen.find("Content-Length: ");
	size_t p = s2.find( "\r\n\r\n" ); // start of the http header body marker
	int h = f + 16;
	size_t contend = contlen.length();
	contlen.erase( p, contend); // trim until just the number remains
	contlen.erase( 0, h );
	char cc[4];
	for( int i = 0; i < 5; i++ ) { // convert to char* for atoi and nonnumeric
		cc[i] = contlen[i];
	}
	delNonNumericChar( cc ); // delete anything that isn't a number
	std::string bbsize;
	for( int i = 0; i < 4; i++ ) {
		bbsize[i] = cc[i]; // stores in a string temporarily
	}
	int bsize = atoi( bbsize.c_str() ); // dumps the c_str of string, because atoi hates me
	p = p + 4; // now back to trimming the buffer
	int eob = p + bsize; // trim off the end according to bsize
	int eos = s2.length(); 
	s2.erase( eob, eos);
	s2.erase( 0, p );
	int s2l = s2.length();
	std::vector<int> odata;
	odata.resize( max_length ); // funnel int our vector<int>
	_logger.logstream << "Sanitized datastream: " << s2;
	_logger.log( 0 ); // log what we got 
	for (int i = 0; i < max_length; i++ ) {
		odata[i] = ccInt( s2[i] );
	}
	return odata; // return
}
bool valSerialData( const vector<int> &vdata ) { // validates that all data conforms to structures set out in inflow doc
	if( (vdata[0] == 0 && vdata[1] == 0 )
			&& ( vdata[5] == 1 && vdata[15] == 0)
			&& ( vdata[16] == 1 && vdata[35] == 1 )/*
			&& ( vdata[36] == 9 && vdata[37] == 9 )
			&& ( vdata[39] == 9 && vdata[40] == 0 )
			&& ( vdata[56] == 0 && vdata[72] == 0 )
			&& ( vdata[88] == 0 && vdata[104] == 0 )
			&& ( vdata[120] == 0 && vdata[136] == 0 )
			&& ( vdata[152] == 0 && vdata[168] == 0 )
			&& ( vdata[184] == 0 && vdata[200] == 0 )
			&& ( vdata[216] == 0 && vdata[232] == 0 )
			&& ( vdata[248] == 0 && vdata[264] == 0 )
			&& ( vdata[280] == 0 && vdata[296] == 0 )
			&& ( vdata[312] == 0 && vdata[328] == 0 )
			&& ( vdata[344] == 0 && vdata[360] == 0 )
			&& ( vdata[376] == 0 && vdata[392] == 0 )
			&& ( vdata[408] == 0 && vdata[424] == 0 )
			&& ( vdata[440] == 0 && vdata[456] == 0 )
			&& ( vdata[472] == 0 && vdata[488] == 0 )
			&& ( vdata[504] == 0 && vdata[520] == 0 )
			&& ( vdata[536] == 0 && vdata[552] == 0 )
			&& ( vdata[568] == 0 && vdata[584] == 0 )
			&& ( vdata[600] == 0 && vdata[616] == 0 )*/
			) {
		_logger.logstream << "Datastream Validated.";
		_logger.log( 1 );
		return true;
	}
	else {
		_logger.logstream << "Datastream REJECTED as Invalid.";
		_logger.log( 3 );
		return false;
	}
}

void session( socket_ptr sock ) { // the actual TCP session starts here
	// boost::this_thread::disable_interruption di;
	bool inproc = false; // this will be set to true once input has been sucessfully processed.
	try {
		_logger.logstream << "Connection Established.";
		_logger.log( 1 );
		int icont = 0;
		for( ; ;) {
			char data[max_length];
			boost::system::error_code error;
			size_t length = sock->read_some( boost::asio::buffer( data ), error ); // reads the length of the buffer, then reads the buffer into the char* data
			char cdata[max_length];
			for( int i = 0; i < max_length; i++ ) {
				cdata[i] = data[i];
			}
			std::vector<int> idata; // our hero, the vector going into handle_data
			idata.resize( max_length );
			idata = sanData( cdata ); // sanitize
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
			bool dvalid = valSerialData( idata ); // validate
			if( !dvalid ) {
				_logger.logstream << "Data rejected by server.";
				_logger.log( 3 );
			}
			if( dvalid ) {
				_logger.logstream << "Valid Datastream accepted. Processing.";
				_logger.log( 1 );
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
				_logger.logstream << "Connection Terminated, data processed."; // if handle_data success, close connection
				_logger.log( 1 );
				break;
			}
			else if( !inproc && error == boost::asio::error::eof  && icont > 0) {
				_logger.logstream << "Connection Terminated before data was successfully processed.";
				_logger.log( 2 );
				break;
			 }
			else if( error ) {
				throw boost::system::system_error( error ); //some other error
			}
			icont++; // to ensure that handle_data only gets called ONCE
		}
	}
	catch( std::exception& e ) {
		_logger.logstream << "Exception in thread: " << e.what() << "\n";
		_logger.log( 3 );
	}
	_logger.logstream << "Thread Exited.";
	_logger.log( 0 );
}

void server( boost::asio::io_service& io_service, short port ) {
	tcp::acceptor a( io_service, tcp::endpoint( tcp::v4(), port ) ); // starts 
	for( ; ; ) {
		socket_ptr sock( new tcp::socket( io_service ) ); // creates a new socket for every connection and accepts
		a.accept( *sock );
		boost::thread t( boost::bind( session, sock ) ); // starts session in a new thread
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
	logger mlog;
	boost::system::error_code er;
	std::string ver = "0.5 Alpha";
	mlog.logstream << "GDAgent version " << ver << " Initializing. Reading configuration from config file";
	mlog.log( 1 );
	mlog.logstream << "Daemonizing....";
	mlog.log( 1 );
	//seedDaemon();
	if( !er ) { // if EC is still empty, alls well
		mlog.logstream << "Daemonizing successful.";
		mlog.log( 1 );
	}
	else {
		mlog.logstream << "Daemonizing failed."; // if not, log an error, and continue
		mlog.log( 3 );
	}
	try{
		mlog.logstream << "Starting IO service... "; 
		using boost::asio::ip::tcp; 
		boost::asio::io_service io_service;
		using namespace std; // for atoi
		server( io_service, mlog.Llport ); //  listen porrt will eventually be read from config file, which reads as char* array.
	}
	catch( std::exception& e ) {
		mlog.logstream << "Exception: " << e.what() << "\n";
		mlog.log( 4 );
		exit(0);
	}
	return 0;
}
#undef _logger
