#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <iterator>
#include <exception>
#include <algorithm>
#include <boost/thread.hpp>
#include <iomanip>
#include <stdlib.h>
#include "log.h"
#include "mmapper.h"
#define _logger mlog
mmapper::mmapper() {
	createDriveLetterMap();
	createSiteMap();
	createTypeMap();
}
std::map<DLpair, std::string> mmapper::Ddlabel;
std::map<TPCodeUidPair, std::string> mmapper::cachedData;
std::map<RgCodeUidPair, std::string> mmapper::siteMap;
std::string mmapper::getKeyFromMap( const int &key1, const int &key2, const int &mid ) {
	//logger _logger;
	switch( mid ) {
		case 1: {
			_logger.logstream << "Retrieving key values for Server Type.";
			_logger.log( 1 );
			auto it = cachedData.find(std::make_pair(key1, key2));
			if( it != cachedData.end() ) {
				_logger.logstream << "Successfully retrieved key value";
		_logger.log( 1 );
				return it->second;
			}
			else {
				_logger.logstream << "Failed to retrieve key.";
		_logger.log( 3 );
				return "ERR";
			}
			break;
		}
		case 2: {
			_logger.logstream << "Retrieving key values for Server Site.";
			_logger.log( 1 );
			auto it = siteMap.find(std::make_pair(key1, key2));
			if( it != siteMap.end() ) {
				_logger.logstream << "Successfully retrieved key value";
				_logger.log( 1 );
				return it->second;
			}
			else {
				_logger.logstream << "Failed to retrieve key.";
				_logger.log( 3 );
				return "ERR";
			}
			break;
		}
		case 3: {
			_logger.logstream << "Retrieveing key values for drive label";
				_logger.log( 1 );
			auto it = Ddlabel.find( std::make_pair( key1, key2 ));
			if( it != Ddlabel.end() ) {
				_logger.logstream << "Successfully retrieved key value";
				_logger.log( 1 );
				return it->second;
			}
			else {
				_logger.logstream << "Failed to retrieve key.";
		_logger.log( 3 );
				return "ERR";
			}
			break;
		}
		default:
			_logger.logstream << "Invalid value passed to mapper! Err 3";
			_logger.log( 5 );
			return "ERR";
	}
}
void mmapper::createDriveLetterMap() {
		//logger _logger;
		_logger.logstream << "createDriveLetterMap() called.";
		_logger.log( 0 );
		_logger.logstream << "Filling drive port:path map.";
		_logger.log( 1 );
	if( Ddlabel.empty() ) {
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 0 ), "sda" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 1 ), "sdb" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 2 ), "sdc" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 3 ), "sdd" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 4 ), "sde" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 5 ), "sdf" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 6 ), "sdg" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 7 ), "sdh" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 8 ), "sdi" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 0, 9 ), "sdj" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 0 ), "sdk" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 1 ), "sdl" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 2 ), "sdm" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 3 ), "sdn" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 4 ), "sdo" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 5 ), "sdp" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 6 ), "sdq" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 7 ), "sdr" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 8 ), "sds" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 1, 9 ), "sdt" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 0 ), "sdu" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 1 ), "sdv" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 2 ), "sdw" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 3 ), "sdx" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 4 ), "sdy" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 5 ), "sdz" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 6 ), "sdaa" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 7 ), "sdab" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 8 ), "sdac" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 2, 9 ), "sdad" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 0 ), "sdae" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 1 ), "sdaf" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 2 ), "sdag" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 3 ), "sdah" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 4 ), "sdai" ) );
		Ddlabel.insert( std::make_pair( std::make_pair( 3, 5 ), "sdaj" ) );
	}
}

void mmapper::createSiteMap() {
		//logger _logger;
		_logger.logstream << "Loading Site Map.";
		_logger.log( 0 );
	if ( siteMap.empty() ) {
		siteMap.insert(std::make_pair(std::make_pair(0, 0), "AUS"));
		siteMap.insert(std::make_pair(std::make_pair(0, 1), "AUS2"));
		siteMap.insert(std::make_pair(std::make_pair(1, 0), "DCA"));
		siteMap.insert(std::make_pair(std::make_pair(1, 1), "DCA1"));
		siteMap.insert(std::make_pair(std::make_pair(1, 2), "DCA2"));
		siteMap.insert(std::make_pair(std::make_pair(1, 3), "DCA3"));
		siteMap.insert(std::make_pair(std::make_pair(1, 4), "DCAg"));
		siteMap.insert(std::make_pair(std::make_pair(1, 4), "LAX"));
		siteMap.insert(std::make_pair(std::make_pair(1, 5), "MIA"));
		siteMap.insert(std::make_pair(std::make_pair(1, 6), "TOR"));
		siteMap.insert(std::make_pair(std::make_pair(1, 7), "NYC"));
		siteMap.insert(std::make_pair(std::make_pair(2, 0), "AMS"));
		siteMap.insert(std::make_pair(std::make_pair(2, 1), "AMS1"));
		siteMap.insert(std::make_pair(std::make_pair(2, 2), "AMS2"));
		siteMap.insert(std::make_pair(std::make_pair(2, 3), "AMS3"));
		siteMap.insert(std::make_pair(std::make_pair(2, 4), "LON"));
		siteMap.insert(std::make_pair(std::make_pair(2, 5), "ZUR"));
		siteMap.insert(std::make_pair(std::make_pair(2, 6), "WAR"));
		siteMap.insert(std::make_pair(std::make_pair(3, 0), "HKG"));
		siteMap.insert(std::make_pair(std::make_pair(3, 1), "SGP"));
		siteMap.insert(std::make_pair(std::make_pair(3, 2), "TOK"));
		siteMap.insert(std::make_pair(std::make_pair(3, 3), "SYD"));
	}
}
void mmapper::createTypeMap() {
		//logger _logger;
		_logger.logstream << "Loading Type Map.";
		_logger.log( 0 );
	try {
		if ( cachedData.empty() ) { // if the map isn't already in memory, load it now with these values
			cachedData.insert(std::make_pair(std::make_pair(0,0), "db")); // database server
			cachedData.insert(std::make_pair(std::make_pair(0,2), "cruncher")); // cruncher
			cachedData.insert(std::make_pair(std::make_pair(0,3), "serv")); //serv machine
			cachedData.insert(std::make_pair(std::make_pair(0,4), "lb")); // load-balancer. don't expect to ever actually use this
			cachedData.insert(std::make_pair(std::make_pair(1,0), "r")); // r-server
			cachedData.insert(std::make_pair(std::make_pair(1,1), "h")); // header server
			cachedData.insert(std::make_pair(std::make_pair(1,2), "t")); // text server
			cachedData.insert(std::make_pair(std::make_pair(2,0), "num")); // numbering server
			cachedData.insert(std::make_pair(std::make_pair(2,1), "border")); // border server
			cachedData.insert(std::make_pair(std::make_pair(2,2), "backlog")); //backlog server
			cachedData.insert(std::make_pair(std::make_pair(2,3), "gcf")); // gc feeder server
			cachedData.insert(std::make_pair(std::make_pair(2,4), "xref")); // xref server
			cachedData.insert(std::make_pair(std::make_pair(3,0), "vmhost")); // self explanatory.
			cachedData.insert(std::make_pair(std::make_pair(3,1), "vpnhost")); // also self explanatory
			cachedData.insert(std::make_pair(std::make_pair(3,2), "vpn")); // vpn virtual server
			cachedData.insert(std::make_pair(std::make_pair(4,0), "s")); // storage server for DumpTruck. Has special rules/
			cachedData.insert(std::make_pair(std::make_pair(5,0), "gw")); // gateway router
			cachedData.insert(std::make_pair(std::make_pair(5,1), "sw")); // regular switch
			cachedData.insert(std::make_pair(std::make_pair(5,2), "fw")); // firewall. Not used.
			cachedData.insert(std::make_pair(std::make_pair(5,3), "p")); // pdu
			cachedData.insert(std::make_pair(std::make_pair(6,0), "err")); // this returns if the server was unable to determine it's type. This will throw an exception, once I figure out how to make that happen.
		}
	}
	catch( std::exception &e ) {
		_logger.logstream << "Exception in thread: " << e.what();
		_logger.log( 5 );
		boost::this_thread::interruption_point();
	}
}
#undef _logger
