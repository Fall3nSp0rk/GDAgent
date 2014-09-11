#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <map>
#include "convert.h"
#include "util.h"
#include "ConfigFile.h"
#include "globals.h"

globals::globals() {
	g_cfg="./cfg/GDAgent.conf";
	readCfg();
}

void globals::readCfg() {
	cfg::cfg conf( g_cfg );
	g_ll = 0;

	if( conf.keyExists( "logfile" ) ){
		g_logfile = conf.getValueOfKey<std::string>( "logfile" );
	}
	else {
		g_logfile = "/var/log/GDlog";
	}
	if( conf.keyExists( "DebugLevel" ) ) {
		int debuglevel = conf.getValueOfKey<int>( "DebugLevel" );
		std::stringstream d;
		d << debuglevel;
		std::string dd = d.str();
		char dl = dd[0];
		int dlvl = ccInt( dl );
		g_ll = dlvl;
	}
	if( conf.keyExists( "ListenPort" ) ) {
		g_lport = conf.getValueOfKey<int>( "ListenPort" );
	}
	else {
		g_lport = 8000;
	}
}

globals glob;
