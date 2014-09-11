#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <boost/asio/io_service.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <stdio.h>
#include <map>
#include <sstream>
#include "log.h"
#include "util.h"
#include <unistd.h>
#include <time.h>

logger::logger( int ll, std::string lf ) {
	Llvl = ll;
	Lfile = lf;
}

std::string logger::tStamp(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime( buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct );
	return buf;
}
unsigned long logger::getThread() {
	std::string threadID = boost::lexical_cast<std::string>(boost::this_thread::get_id() );
	unsigned long threadNumber = 0;
	sscanf( threadID.c_str(), "%lx", &threadNumber );
	return threadNumber;
}

pid_t logger::getPID() {
	Lpid = getpid();
	return Lpid;
}

bool logger::writeLog( const std::string &message ) {
	int slen = message.length();
	if( slen > 10 ) {
		Lpid = getPID();
		Lthread = getThread();
		std::string ts = tStamp();
		std::ofstream lmsg( Lfile, std::ofstream::app );
		lmsg << ts << " [" << Lpid << "] (" << Lthread << ") " << message << std::endl;
		lmsg.close();
		return true;
	}
	else {
		return false; // if incoming message is too small, discard and do nothing.
	}
}

void logger::log( int mlvl ) {
	boost::mutex::scoped_lock lock( _logmute ); // lock to this thread, for safety reasons.
	if( mlvl >= Llvl ){ // check if message level provided is greater than or equal to logging level specified in config file
		std::string lin = logstream.str(); // flush logstream object to a string so it can be used again
		logstream.str(""); // ensure it's empty again for next step
		switch( mlvl ) { // switch to input the proper message level header in front of stream
			case 0:
				logstream << "[DEBUG] "; 
				break;
			case 1:
				logstream<< "[INFO] ";
				break;
			case 2:
				logstream<< "[WARN] ";
				break;
			case 3:
				logstream<< "[ERROR] ";
				break;
			case 4:
				logstream<< "[EXCEPTION] ";
				break;
			case 5:
				logstream<< "[FATAL] ";
				break;
			default:
				logstream<< "[GENERAL] ";
				writeLog( "[ERROR] Invalid identifier given to logger.\n" );
				break;
		}
		logstream << lin;
		lin = logstream.str();
		writeLog( lin );
		logstream.str("");
		
	}
	else {
		logstream.str(""); // if logging level is not greater than logging level specified in config file, just flush buffer and ignore.
	}
}
