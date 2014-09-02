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
		log( "No Logfile specified in cfg file. Using default.", 1 );
	}
	if( conf.keyExists( "DebugLevel" ) ) {
		int debuglevel = conf.getValueOfKey<int>( "DebugLevel" );
		std::stringstream d;
		d << debuglevel;
		std::string dd = d.str();
		char dl = dd[0];
		int dlvl = ccInt( dl );
		switch( dlvl ) {
			case 0:
				logger::log( "Logging level set to [DEBUG] (0) from config file.", 1 );
				break;
			case 1:
				logger::log( "Logging level set to [INFO] (1) from config file.", 1 );
				break;
			case 2:
				logger::log( "Logging level set to [WARN] (2) from config file.", 1 );
				break;
			case 3:
				logger::log( "Logging level set to [ERR] (3) from config file.", 1 );
				break;
			case 4:
				logger::log( "Logging level set to [FATAL] (4) from config file.", 1 );
				break;
			case 5:
				logger::log( "Logging level set to [EXCEPTION] (5) from config file.", 1);
				break;
			default:
				log( "Logging level set to  [ALL] (0) due to invalid config file entry.", 2 );
				break;
		}
		Llvl = dlvl;
	}
	if( conf.keyExists( "ListenPort" ) ) {
		Llport = conf.getValueOfKey<int>( "ListenPort" );
		log( "Listen port Read from cfg file.", 1 );

		
	}
	else {
		Llport = 8000;
		log( "No value found for listen port. Using default of 8000.", 1 );
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

void logger::log( const std::string &text, int mlvl ) {
	if( mlvl >= Llvl ){
		std::stringstream logtext;
		std::string logoutput;
		switch( mlvl ) {
			case 0:
				logtext<< "[DEBUG] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				break;
			case 1:
				logtext<< "[INFO] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				break;
			case 2:
				logtext<< "[WARN] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				break;
			case 3:
				logtext<< "[ERR] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				break;
			case 4:
				logtext<< "[CRITICAL] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				break;
			case 5:
				logtext<< "[EXCEPTION] " << text << std::endl;
				logoutput = logtext.str();
				logtext.str("");
				writeLog( logoutput );
			default:
				logtext<< "[GENERAL] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				logtext.str("");
				writeLog( "[ERROR] Invalid identifier given to logger.\n" );
				break;
		}
		
	}
	
}
logger GDLogger;
