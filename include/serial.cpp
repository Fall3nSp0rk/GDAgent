// read.h
// contains functions for reading from buffer.
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<map>
#include"serial.h"
#include"log.h"
#include<iomanip>
#include<stdlib.h>
using std::vector;
const int bsize = 1024;

const int Sserverstart = 6;
const int Smacstart = 18;
const int Sdbitstart = 590;
const int Sslen = 9;
const int Shlen = 4;
const int Sdlen = 612;
const int Smlen = 18;
const int Sblen = 1024;

typedef std::pair<int, int> TPCodeUidPair;
typedef std::pair<int, int> RgCodeUidPair;
logger serlogger;

serial::serial(){ // overrides default constructor
	serlogger.log( "serial object initialized.", 0 );
	Snbuff.resize(Sblen);
	Ssbits.resize(Sslen);
	Shbits.resize(Shlen);
	Sdbits.resize(Sdlen);
	Smbits.resize(Smlen);
}
void serial::readBits( const std::vector<int> &buff ){
	serlogger.log( "Called readBits", 0);
	for ( int i = 0; i < 1024; i++ ) {
		Snbuff[i] = buff[i];
	}
	int h = 1;
	for ( int i = 0; i < 5; i++ ) { // map buffer to smaller vectors for portability
		Shbits[i] = buff[h];
		h++;
	}
	int j = Sserverstart;
	for ( int i = 0; i < Sslen; i++ ) {
		Ssbits[i] = buff[j];
		j++;
	}
	int f = Smacstart;
	for ( int i = 0; i < Smlen; i++ ) {
		Smbits[i] = buff[f];
		f++;
	}
	int m = Sdbitstart;
	for ( int i = 0; i < Sdlen; i++ ) {
		Sdbits[i] = buff[m];
		m++;
	}
	serlogger.log( "readBits() exited.", 0 );
}
std::string serial::giveStrVal( std::string cval ) {
	serlogger.log( "giveStrVal() called.", 0 );
	std::string aval = cval;
	return aval;
}
int serial::giveIntVal( int cint ) { //for copying values
	serlogger.log( "giveIntVal() called.", 0 );
	int aint = cint;
	return aint;
}


// I have no idea if this will work
void serial::deSerialize() {
	serlogger.log( "deSerialize() called.", 0 );
	Sstype = readSType( Ssbits );
	readGC( Ssbits );
	readRecover( Ssbits );
	getHostName( Ssbits, Shbits );
	readDNum( Ssbits );
	readMac( Smbits );
	readAType( Ssbits );
	debuf();
	serlogger.log( "deSerialize() completed.", 0 );
}

void serial::debuf() {
	serlogger.log( "debuf() called.", 0);
	std::cout<< Ssite << std::endl;
	std::cout<< Sstype << std::endl;
	std::cout<< Shname << std::endl;
}
// function definitions for serial class follow here

void serial::readMac( const vector<int> &mbits ) {
	serlogger.log( "readMac() called.", 0 );
	std::ostringstream strm;
	char macbit[3];
	std::stringstream m;
	for( int i = 0; i < 18; i++ ){
		m<<mbits[i];
	}
	std::string mm = m.str();
	m.str("");
	serlogger.log( mm, 1 );
	for(int nMAC = 0; nMAC < 6; nMAC++) {
		int f = 0;
		std::string fmac = "";
		for( int i = 0; i < 3; i++ ) {
			macbit[i] = mbits[f];
			f++;
		}
		int num = atoi( macbit );
		strm << std::setfill('0') << std::setw(2) << std::hex << num;
		if( nMAC < 5 ) {
			strm<< ":";
		}
	}
	Smac = strm.str();
	strm.str("");
	std::stringstream lgmsg;
	lgmsg << "Mac address of server " << Shname << " should be " << Smac << ".\n";
	std::string ll = lgmsg.str();
	lgmsg.str("");
	serlogger.log( ll, 1 );
}

void serial::readGC( const vector<int> &fsbits ) {
	serlogger.log( "readGC() called.", 0);

	switch ( fsbits[2] ) {
		case 0:
			Sservices = "Not Running";
			break;
		case 1:
			Sservices = "Running";
			break;
		case 2:
			Sservices = "Service Error";
			break;
		default:
			Sservices = "Err";
			serlogger.log( "error on line 147: serial.cpp: Invalid value received for Sservices.", 3);
			break;
	}
	//catch( int e ) {
	//	std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
	//}
	serlogger.log( "readGC() completed.", 0);
}

void serial::readDNum( const vector<int> &sbits ) {
	serlogger.log( "readDNum() called.", 0 );
	switch( sbits[4] ) {
		case 0:
			Sdnum = 12;
			break;;
		case 1:
			Sdnum = 36;
			break;
		case 2:
			Sdnum = 4;
			break;
		case 3:
			Sdnum = 0; // should throw an exception
			serlogger.log( "Error on line 156: serial.cpp: invalid value (3) received for Sdnum.", 3 );
			break;
		default: // should only occur if no data is received, should throw an exception
			Sdnum = 0;
			serlogger.log( "Error on line 156: serial.cpp: invalid value (?) received for Sdnum.", 3 );
			break;
	}
//	catch( int e) {
//
//		std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
//	}
	serlogger.log( "readDnum() completed.", 0 );
}

std::string serial::readType( std::string fstype ) {
	serlogger.log( "readType() called.", 0 );
	if ( fstype == "r" || fstype == "h" || fstype == "t" || fstype == "serv" ) {
		serlogger.log( "Determined type to be 'gc'.", 1 );
		return "gc";
	}
	else if ( fstype == "backlog" || fstype == "border" || fstype == "num" || fstype == "xref" ) {
		serlogger.log( "Determined type to be 'nntp'.", 1 );
		return "nntp";
	}
	else {
		serlogger.log( "No type found.", 1 );
		return "na";
	}
	serlogger.log( "readType() completed.", 0 );
}
std::string serial::readSType( const vector<int> &sbits ) {
	serlogger.log( "readSType() called.", 0 );
	int stcode = sbits[0];
	int uid = sbits[1];
	std::stringstream debugsbitsvalue;
	debugsbitsvalue << sbits[0] << sbits[1];
	std::string tdebug = debugsbitsvalue.str();
	debugsbitsvalue.str("");
	// serlogger.log( tdebug, 0 ); 
	static std::map<TPCodeUidPair, std::string> cachedData;
	if ( cachedData.empty() ) {
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
	auto it = cachedData.find(std::make_pair(stcode, uid));
	if (  it != cachedData.end()) {
		serlogger.log( "readSType() completed.", 0 );
		return it->second;
	}
}


void serial::readAType( const vector<int> &sbits ) {
	serlogger.log( "Determined asset-type to be 'server'.", 1 );
	Satype = "server";
	// Asset type isn't implemented. YET.
	
}

void serial::readRecover( const vector<int> &sbits ) {
	serlogger.log( "readRecover() called.", 0 );
	int rcode = sbits[3];
	switch( rcode ) { // reads whether or not the server is currently recovering.
		case 0:
			Srstate = "Not running";
			break;
		case 1:
			Srstate = "Running";
			break;
		case 2:
			Srstate = "Infected";
			break;
		case 3:
			Srstate = "Defunct";
			break;
		default:
			serlogger.log( "Error on line 243: serial.cpp: Invalid data (?) received for Srstate.", 2 );
			break;
	}
	serlogger.log( "readRecover() completed.", 0 );
}

std::string serial::readID(  vector<unsigned long int> hbits, std::string fstype ) {
	serlogger.log( "readID() called.", 0 );
	std::stringstream result;
	std::copy(hbits.begin(), hbits.end(), std::ostream_iterator<int>(result, ""));
	std::string idnum = result.str();
	result.str("");
	//std::cout<<idnum;
	//remove leading zeroes from identifier....
	std::string::size_type pos = idnum.find_first_not_of( "0", 0 );
	if ( pos > 0 ) 
		idnum.erase( 0, pos );
	// end of removing leading zeroes from identifier
	std::stringstream ss;
	std::string id;
	int idlen = idnum.length();
	if ( idlen < 2 && ( fstype == "h" || fstype == "r" || fstype == "t" ) ) { // and add them back in if it's a single-digit t, h, or r server
		ss << fstype << "0" << idnum;
		id = ss.str();
		ss.str("");
		serlogger.log("readID() completed.", 0);
		return id;
	}
	else {
		ss << fstype << idnum;
		id = ss.str();
		ss.str("");
		serlogger.log( "readID() completed.", 0);
		return id;
	}
}

// Reads the site identifier code
std::string serial::readSite( const vector<int> &fsbits ) {
	serlogger.log( "readSite() called.", 0 );
	// reads the only two numbers we care about out of the server information bits
	int rgcode = fsbits[5];
	int uid = fsbits[6];
	static std::map<RgCodeUidPair, std::string> cachedData;
	if ( cachedData.empty() ) {
		cachedData.insert(std::make_pair(std::make_pair(0, 0), "AUS"));
		cachedData.insert(std::make_pair(std::make_pair(0, 1), "AUS2"));
		cachedData.insert(std::make_pair(std::make_pair(1, 0), "DCA"));
		cachedData.insert(std::make_pair(std::make_pair(1, 1), "DCA1"));
		cachedData.insert(std::make_pair(std::make_pair(1, 2), "DCA2"));
		cachedData.insert(std::make_pair(std::make_pair(1, 3), "DCA3"));
		cachedData.insert(std::make_pair(std::make_pair(1, 4), "DCAg"));
		cachedData.insert(std::make_pair(std::make_pair(1, 4), "LAX"));
		cachedData.insert(std::make_pair(std::make_pair(1, 5), "MIA"));
		cachedData.insert(std::make_pair(std::make_pair(1, 6), "TOR"));
		cachedData.insert(std::make_pair(std::make_pair(1, 7), "NYC"));
		cachedData.insert(std::make_pair(std::make_pair(2, 0), "AMS"));
		cachedData.insert(std::make_pair(std::make_pair(2, 1), "AMS1"));
		cachedData.insert(std::make_pair(std::make_pair(2, 2), "AMS2"));
		cachedData.insert(std::make_pair(std::make_pair(2, 3), "AMS3"));
		cachedData.insert(std::make_pair(std::make_pair(2, 4), "LON"));
		cachedData.insert(std::make_pair(std::make_pair(2, 5), "ZUR"));
		cachedData.insert(std::make_pair(std::make_pair(2, 6), "WAR"));
		cachedData.insert(std::make_pair(std::make_pair(3, 0), "HKG"));
		cachedData.insert(std::make_pair(std::make_pair(3, 1), "SGP"));
		cachedData.insert(std::make_pair(std::make_pair(3, 2), "TOK"));
		cachedData.insert(std::make_pair(std::make_pair(3, 3), "SYD"));
	}
	auto it = cachedData.find(std::make_pair(rgcode, uid));
	if ( it != cachedData.end()) {
		serlogger.log( "readSite() executed successfully.", 0 );
		return it->second;
	}
}


std::string serial::getTLD( std::string fsite ) {
	serlogger.log( "getTLD() called.", 0 );
	vector<std::string> gnsites{ "AMS", "AMS1", "AMS2", "AMS3", "DCA", "DCA1", "DCA2", "DCA3", "LAX", "HKG", "AUS2" };
	if ( std::find(gnsites.begin(), gnsites.end(), fsite) != gnsites.end()) { // gives the top level domain for the server.
		serlogger.log( "Determined TLD to be giganews.com.", 1 );
		return "giganews.com";
	}
	else {
		serlogger.log( "Determined TLD to be goldenfrog.com.", 1 );
		return "goldenfrog.com";
	}
}

void serial::getHostName( vector<int> &fsbits, vector<unsigned long int> fhbits ) {
	serlogger.log( "getHostName() called.", 0 );
	Ssite = serial::readSite( fsbits );
	std::string tld = serial::getTLD( Ssite );
	std::string tid = serial::readType( Sstype );
	std::string uid = serial::readID( fhbits, Sstype );
	std::stringstream sss;
	std::stringstream ss;
	if ( tid != "na" ) {
		ss << uid << "." << tid << "." << Ssite << "." << tld; // stringstream ftw
		Shname = ss.str();
		ss.str("");
		std::stringstream lstream;
		lstream << "Determined Hostname to be: " << Shname << ".";
		std::string lstr = lstream.str();
		lstream.str("");
		serlogger.log( lstr, 1 );
		sss << uid << "." << Ssite;
		Sshname = sss.str();
		sss.str("");
		lstream << "Determined Host Shortname to be: " << Sshname << ".";
		lstr = lstream.str();
		lstream.str("");
		serlogger.log( lstr, 1 );
	}
	else {
		ss << uid << "." << Ssite << "." << tld;
		Shname = ss.str();
	}
	serlogger.log( "getHostName() completed.", 0 );
}

