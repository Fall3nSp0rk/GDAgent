#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<time.h>
#include<stdio.h>
#include<map>
#include<sstream>
#include"log.h"
#include"util.h"
#include<unistd.h>
#include<boost/lexical_cast.hpp>
#include<boost/thread.hpp>
#include<time.h>

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
	if( mlvl >= Llvl ){
		std::string lin = logstream.str();
		logstream.str("");
		switch( mlvl ) {
			case 0:
				logstream << "[DEBUG] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 1:
				logstream<< "[INFO] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 2:
				logstream<< "[WARN] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 3:
				logstream<< "[ERR] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 4:
				logstream<< "[CRITICAL] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 5:
				logstream<< "[EXCEPTION] " << lin;
				lin = logstream.str();
				logstream.str("");
				writeLog( lin );
			default:
				logstream<< "[GENERAL] " << lin;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				writeLog( "[ERROR] Invalid identifier given to logger.\n" );
				break;
		}
		
	}
	
}
