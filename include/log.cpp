#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<time.h>
#include<stdio.h>
#include<map>
#include<sstream>
#include"log.h"
#include"ConfigFile.h"
#include"util.h"
#include<unistd.h>
#include<boost/lexical_cast.hpp>
#include<boost/thread.hpp>
#include<time.h>

logger::logger(){
	Lcfgfile="/home/ianc/www/coding/GDAgent/cfg/GDAgent.conf";
	readCfg();
}

std::string logger::tStamp(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime( buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct );
	return buf;
}
void logger::readCfg() {
	cfg::cfg conf( Lcfgfile );
	Llvl = 0;

	if( conf.keyExists( "logfile" ) ){
		Lfile = conf.getValueOfKey<std::string>( "logfile" );
	}
	else {
		Lfile = "/var/log/GDlog";
		/*logstream << "No Logfile specified in cfg file. Using default of " << Lfile << ".";
		log( 1 );*/
	}
	if( conf.keyExists( "DebugLevel" ) ) {
		int debuglevel = conf.getValueOfKey<int>( "DebugLevel" );
		std::stringstream d;
		d << debuglevel;
		std::string dd = d.str();
		char dl = dd[0];
		int dlvl = ccInt( dl );
		/*logstream << "Logging level set to [";
		switch( dlvl ) {
			case 0:
				logstream << "DEBUG] (0) from config file.";
				logger::log( 1 );
				break;
			case 1:
				logstream << "INFO] (1) from config file.";
				logger::log( 1 );
				break;
			case 2:
				logstream << "WARN] (2) from config file.";
				logger::log( 1 );
				break;
			case 3:
				logstream << "ERR] (3) from config file.";
				logger::log( 1 );
				break;
			case 4:
				logstream << "FATAL] (4) from config file.";
				logger::log( 1 );
				break;
			case 5:
				logstream << "EXCEPTION] (5) from config file.";
				logger::log( 1 );
				break;
			default:
				logstream << "ALL] (0) due to invalid config file entry.";
				log( 2 );
				break;
		}*/
		Llvl = dlvl;
	}
	if( conf.keyExists( "ListenPort" ) ) {
		Llport = conf.getValueOfKey<int>( "ListenPort" );
		/*logstream << "Listen port of " << Llport << " read from config file.";
		log( 1 );*/

		
	}
	else {
		Llport = 8000;
		/*logstream << "No entry found for listen port. Using default value of " << Llport << ".";
		log( 1 ); */
	}
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
	Lpid = getPID();
	Lthread = getThread();
	std::string ts = tStamp();
	std::ofstream lmsg( Lfile, std::ofstream::app );
	lmsg << ts << " [" << Lpid << "] (" << Lthread << ") " << message;
	lmsg.close();
	return true;
}

void logger::log( int mlvl ) {
	if( mlvl >= Llvl ){
		std::string lin = logstream.str();
		logstream.str("");
		switch( mlvl ) {
			case 0:
				logstream << "[DEBUG] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 1:
				logstream<< "[INFO] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 2:
				logstream<< "[WARN] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 3:
				logstream<< "[ERR] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 4:
				logstream<< "[CRITICAL] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				break;
			case 5:
				logstream<< "[EXCEPTION] " << lin << std::endl;
				lin = logstream.str();
				logstream.str("");
				writeLog( lin );
			default:
				logstream<< "[GENERAL] " << lin << std::endl;
				lin = logstream.str();
				writeLog( lin );
				logstream.str("");
				writeLog( "[ERROR] Invalid identifier given to logger.\n" );
				break;
		}
		
	}
	
}
logger mlog;
