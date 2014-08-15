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
		log( "GDAgent starting. Loading cfg file.", 1 );
		log( "Write to logfile success.", 1 );
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
				logger::log( "Logging level set to [CRITICAL] (4) from config file.", 1 );
				break;
			default:
				log( "Logging level set to [GENERAL] (0) due to invalid config file entry.", 2 );
		}
		Llvl = dlvl;
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
	std::ofstream lmsg( Lfile, std::ofstream::app );
	std::string ts = tStamp();
	lmsg << ts << " [" << Lpid << "] (" << Lthread << ") " << message;
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
				break;
			case 1:
				logtext<< "[INFO] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				break;
			case 2:
				logtext<< "[WARN] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				break;
			case 3:
				logtext<< "[ERR] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				break;
			case 4:
				logtext<< "[CRITICAL] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				break;
			default:
				logtext<< "[GENERAL] " << text << std::endl;
				logoutput = logtext.str();
				writeLog( logoutput );
				writeLog( "[ERR] Invalid identifier given to logger.\n" );
				break;
		}
		
	}
	
}
