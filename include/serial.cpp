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
#include<boost/thread.hpp>
#include<exception>
using std::vector;
const int bsize = 1024;

const int Sserverstart = 6;
const int Smacstart = 18;
const int Sdbitstart = 590;
const int Shbitstart = 1;
const int Sslen = 9;
const int Shlen = 4;
const int Sdlen = 612;
const int Smlen = 18;
const int Sblen = 1024;

typedef std::pair<int, int> TPCodeUidPair;
typedef std::pair<int, int> RgCodeUidPair;
logger serlogger;

serial::serial(){ // overrides default constructor
	try {
		serlogger.log( "serial object initialized.", 0 );
		Snbuff.resize(Sblen);
		Ssbits.resize(Sslen);
		Shbits.resize(Shlen);
		Sdbits.resize(Sdlen);
		Smbits.resize(Smlen);
		for( int i = 0; i < Sblen; i++ ) { // Setting all values to a default value. this ensures memory regions are contiguos
			Snbuff[i] = 0;
		}
		for( int i = 0; i < Sslen; i++ ) {
			Ssbits[i] = 0;
		}
		for( int i = 0; i < Shlen; i++ ) {
			Shbits[i] = 0;
		}
		for( int i = 0; i < Sdlen; i++ ) {
			Sdbits[i] = 0;
		}
		//for( int i = 0; i < Smlen; i++ ) {
		//	Smbits[i] = 0;
		//}
		Srstate = "Not Running";
		Sstype = "r";
		Ssite = "AMS3";
		Shname = "r00.gc.AMS3.giganews.com";
		Smac = "aa:bb:cc:dd:ee:ff";
		Sshname = "r00.AMS3";
		Satype = "Server";
		Sservices = "Not Running.";
		Sanum = "5";
		Svarfill = 0;
		Sdnum = 12;
		Ssid = 0;
		Stld = "giganews.com";
	}
	catch( std::exception &e ) {
		std::stringstream errstream;
		errstream << "Exception in thread: " << e.what();
		std::string errstr = errstream.str();
		errstream.str("");
		serlogger.log( errstr, 5 );
		boost::this_thread::interruption_point();
	}
}
void serial::readBits( const std::vector<int> &buff ){
	try{
		serlogger.log( "Called readBits", 0);
		for ( int i = 0; i < 1024; i++ ) {
			Snbuff[i] = buff[i];
		}
		int h = Shbitstart;
		for ( int i = 0; i < 4; i++ ) { // map buffer to smaller vectors for portability
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
		/*for ( int i = 0; i < 10; i++ ) {
			Sdbits[i] = buff[m];
			m++;
		}*/ //commenting this out until I write the actual drive bit reading util.
	}
	catch( std::exception &e ) {
		std::stringstream errstream;
		errstream << "Exception in thread: " << e.what();
		std::string lmsg = errstream.str();
		errstream.str("");
		serlogger.log( lmsg, 5 );
	}
		serlogger.log( "readBits() exited.", 0 );
}
std::string serial::giveStrVal( const std::string &cval ) {
	serlogger.log( "giveStrVal() called.", 0 );
	std::string aval = cval;
	return aval;
}
int serial::giveIntVal( const int &cint ) { //for copying values
	serlogger.log( "giveIntVal() called.", 0 );
	int aint = cint;
	return aint;
}


bool serial::validateHost( const std::string &fstype, const int &fsid, const std::string &fsite, const std::string &ftld ) {
	bool passedUIDValidation = false; // as each segment is validated, these flags are set to true.
	bool passedSiteTLDValidation = false;
	bool passedTypeTLDValidation = false;
	bool passedSiteTypeValidation = false;
	bool passedMACFormatValidation = false;
	bool passedMACValidation = false;
	serlogger.log( "validateHost() called.", 0 );
	std::stringstream logstream;
	std::string lmsg;
	logstream << "Beginning data validation on received data for alleged host " << Shname << ".";
	vector<std::string> hexvals{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
	vector<std::string> gnsites{ "AMS", "AMS1", "AMS2", "AMS3", "DCA", "DCA1", "DCA2", "DCA3", "LAX", "HKG", "AUS2" };
	vector<std::string> gntypes{ "r", "t", "h", "xref", "num", "backlog", "border", "buffer" };
	vector<std::string> gftypes{ "vpn", "vpnhost", "vmhost" };
	vector<std::string> l4uids{ "db", "border", "backlog", "serv", "vpnhost", "t" };
	vector<std::string> l2uids{ "gcf", "num", "xref", "buffer", "gw", "bfs", "ax", "lb" };
	lmsg = logstream.str();
	logstream.str("");
	serlogger.log( lmsg, 1 );
	serlogger.log( "Validating UIDs...", 1);
	if ( fstype == "r" && fsid > 400 ) { // checks UIDs against a range they should appear in
		logstream << "Invalid UID for r-server of " << fsid << " passed. Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( fstype == "h" &&  fsid > 8 ) { 
		logstream << "Invalid UID for h-server of " << fsid << " passed. Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( (std::find( l4uids.begin(), l4uids.end(), fstype) != l4uids.end() ) && fsid > 4 )  {
		logstream << "Invalid UID for " << fstype << "-server of " << fsid << " passed. Rejecting data as invalid."; // some uids are max of 4 e.g. backlogs, borders.
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( (std::find( l2uids.begin(), l2uids.end(), fstype ) != l2uids.end() ) && fsid > 2 ) {
		logstream << "Invalid UID for " << fstype << "-server of " << fsid << " passed. Rejecting data as invalid."; // some uids have a max of 2, e.g. num, xref
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else { // if nothing matches, set UID flag to true
		logstream << "UID Validation Successful. UID " << Suid << " is valid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1 );
		passedUIDValidation = true;
	}
	serlogger.log( "Validating TLD against site...", 1 );
	if( ( std::find( gnsites.begin(), gnsites.end(), fsite ) != gnsites.end() ) && ftld != "giganews.com" ) {
		logstream << "TLD Validation against site Failed. TLD should be giganews.com, currently is " << ftld << ". Rejecting as invalid."; // if site isn't listed in gnsites list above, and TLD is giganews, reject as invalid.
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( ( std::find( gnsites.begin(), gnsites.end(), fsite ) == gnsites.end() ) && ftld != "goldenfrog.com" ) {
		logstream << "TLD Validation against site Failed. TLD should be goldenfrog.com, currently is " << ftld << ". Rejecting data as invalid."; // similar to before, but if it IS listed in gnsites, and TLD is gf.com, reject
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else { // if none of above match, set next flag to true
		logstream << "TLD Validation against Site Successful. TLD " << ftld << " is valid for site " << fsite << ".";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1 );
		passedSiteTLDValidation = true;
	}
	serlogger.log( "Validating TLD against Type", 1 ); // only certain kinds of servers are .gn
	if( ( std::find( gntypes.begin(), gntypes.end(), fstype ) != gntypes.end() ) && ftld != "giganews.com" ) {
		logstream << "TLD Validation against Type failed. TLD should be giganews.com, currently is " << ftld << ". Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( ( std::find( gftypes.begin(), gftypes.end(), fstype ) != gftypes.end() ) && ftld != "goldenfrog.com" ) {
		logstream << "TLD Validation against Type failed. TLD should be goldenfrog.com, currently is " << ftld << ". Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else {
		logstream << "TLD Validation against Type Successful. TLD " << ftld << " is valid for type " << fstype <<".";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1 );
		passedTypeTLDValidation = true;
	}
	serlogger.log( "TLD Validation complete. Beginning Type Validation against Site.", 1 ); // certain sites don't have certain server types. Compare, and if a match is found, reject
	if(  ( std::find( gntypes.begin(), gntypes.end(), fstype ) != gntypes.end()
				&& std::find( gnsites.begin(), gnsites.end(), fsite ) == gnsites.end() )
			|| ( std::find( gftypes.begin(), gftypes.end(), fstype) != gftypes.end()
				&& std::find( gnsites.begin(), gnsites.end(), fsite) != gnsites.end() ) ) {
		logstream << "Type Validation against site failed. Type " << fstype << " does not exist in site " << fsite << ". Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else if( (fstype == "cruncher" && std::find( gnsites.begin(), gnsites.end(), fsite) != gnsites.end() ) 
			|| ( fstype == "db" && ( fsite != "DCA" || fsite != "DCA1" ) ) ) {
		logstream << "Type Validation against site failed. Type " << fstype << " does not exist in site ";
		logstream << fsite << ". Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	}
	else {
		logstream << "Type Validation against Site Successful. Type " << fstype << " is valid for site ";
		logstream << fsite << ".";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1 );
		passedSiteTypeValidation = true;
	}
	serlogger.log( "Validating MAC address...", 1 );
	serlogger.log( "Validating MAC format...", 1 ); 
	// broken code for mac address validation below
	/* if( &Smac[2] != ":" || &Smac[5] != ":" || &Smac[8] != ":" || &Smac[11] != ":" || &Smac[14] != "\n" ) {
		logstream << "MAC Address Validation Failed. MAC " << Smac << " is of invalid format. Rejecting data as invalid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 3 );
		return false;
	} 
	else {
		logstream << "MAC Address " << Smac << " if of valid format.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1);
		passedMACFormatValidation = true;
	} */
	passedMACFormatValidation = true;
	/*
	serlogger.log( "Validating MAC Address Values...", 1);
	bool mval = true;
	for( int i = 0; i < 14; i++ ) {
		if( i != 2 && i != 5 && i != 8 && i != 11 ) {
			if( std::find( hexvals.begin(), hexvals.end(), Smac[i] ) == hexvals.end() ) {
				mval = false;	
			}
		}
	}
	*/
		// /broken code
	bool mval = true;
	if( mval == false ) { // sad day about this.
		serlogger.log( "MAC Address Validation Failed. One or more values is not a valid hex value.", 3 );
		return false;
	}
	else {
		logstream << "MAC Address Validation Successful. MAC Address " << Smac << " is valid.";
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 1 );
		passedMACValidation = true;
	}
	serlogger.log( "All Validation checks completed. Running final check now.", 1 ); // final test. If all flags are TRUE now, return true - data has passed validation check.
	logstream << passedUIDValidation << " " << passedSiteTLDValidation << " " << passedTypeTLDValidation;
	logstream << " " << passedSiteTypeValidation << " " << passedMACFormatValidation << " ";
	lmsg = logstream.str();
	logstream.str("");
	serlogger.log( lmsg, 0 );
	if( passedUIDValidation == false || passedSiteTLDValidation == false || passedTypeTLDValidation == false
			|| passedSiteTypeValidation == false || passedMACFormatValidation == false
			|| passedMACValidation == false ) {
		logstream << "Error performing final validation checks. One or more flags was not set. Dumping flags. ";
		logstream << passedUIDValidation << " " << passedSiteTLDValidation << " " << passedTypeTLDValidation;
		logstream << " " << passedSiteTypeValidation << " " << passedMACFormatValidation << " ";
		logstream << passedMACValidation;
		lmsg = logstream.str();
		logstream.str("");
		serlogger.log( lmsg, 4 );
		return false;
	}
	else {
		serlogger.log( "Validation Completed. Passing to database module.", 1 );
		return true;
	}
}

bool serial::deSerialize() { // wrapper function that calls deserialization functioons, then validates, and returns true if successful, false if not.
	serlogger.log( "deSerialize() called.", 0 );
	Sstype = readSType( Ssbits ); 
	Ssite = readSite( Ssbits );
	readGC( Ssbits );
	readRecover( Ssbits );
	getHostName( Shbits );
	readDNum( Ssbits );
	readMac( Smbits );
	readAType( Ssbits );
	//debuf();
	serlogger.log( "deSerialize() completed.", 0 );
	if( validateHost( Sstype, Ssid, Ssite, Stld ) ) {
		return true;
	}
	else {
		return false;
	}
}

void serial::debuf() { // deprecated debug portion
	serlogger.log( "debuf() called.", 0);
	std::cout<< Ssite << std::endl;
	std::cout<< Sstype << std::endl;
	std::cout<< Shname << std::endl;
}
// function definitions for serial class follow here

void serial::readMac( const vector<int> &mbits ) { // reads the 18 digit decimal-encoded mac address
	serlogger.log( "readMac() called.", 0 );
	std::ostringstream strm;
	char macbit[3];
	std::stringstream m;
	for( int i = 0; i < 18; i++ ){
		m<<mbits[i];
	}
	std::string mm = m.str(); // for debug purposes, we echo the received decimal digits to the logger
	m.str(""); // set the original stringstream to null, to free up the used memory
	serlogger.log( mm, 1 );
	int f = 0;
	for(int nMAC = 0; nMAC < 6; nMAC++) {
		std::string fmac = "";
		for( int i = 0; i < 3; i++ ) {
			macbit[i] = mm[f]; // move things from a vector<int> to a char* array, for atoi
			f++;
		}
		int num = atoi( macbit ); // atoi only accepts char*. converts a char array to a number.
		strm << std::setfill('0') << std::setw(2) << std::hex << num; // translate 3-digit decimal to 2-digit hex
		if( nMAC < 5 ) {
			strm<< ":"; //adds colons between each 2-digit hex value for all but last one.
		}
	}
	Smac = strm.str(); // convert from stringstream to string, store in class variable Smac
	strm.str(""); // set old stringstream to null to free memory used
	std::stringstream lgmsg;
	lgmsg << "Mac address of server " << Shname << " should be " << Smac << ".\n";
	std::string ll = lgmsg.str(); // logs what it thinks the mac is
	lgmsg.str("");
	serlogger.log( ll, 1 );
}

void serial::readGC( const vector<int> &fsbits ) {
	serlogger.log( "readGC() called.", 0); // reads the GC status bit (sbit[2]) and translates it to human

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
	//catch( int e ) { // need to add this back in for exception handling after I write custom excaption subclass
	//	std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
	//}
	serlogger.log( "readGC() completed.", 0);
}

void serial::readDNum( const vector<int> &sbits ) { // read the number of drives from sbit[4]
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

std::string serial::readType( const std::string &fstype ) { // takes the server type and determines the next part of its hostname
	serlogger.log( "readType() called.", 0 );
	if ( fstype == "r" || fstype == "h" || fstype == "t" || fstype == "serv" ) {
		serlogger.log( "Determined type to be 'gc'.", 1 ); // *.gc.*.*
		return "gc";
	}
	else if ( fstype == "backlog" || fstype == "border" || fstype == "num" || fstype == "xref" ) {
		serlogger.log( "Determined type to be 'nntp'.", 1 ); // *.nntp.*.*
		return "nntp";
	}
	else {
		serlogger.log( "No type found.", 1 ); // All the rest
		return "na";
	}
	serlogger.log( "readType() completed.", 0 );
}
std::string serial::readSType( const vector<int> &sbits ) {
	serlogger.log( "readSType() called.", 0 );
	int stcode = sbits[0];
	int uid = sbits[1];
	std::stringstream debugsbitsvalue; // uses a map:key pair
	debugsbitsvalue << sbits[0] << sbits[1];
	std::string tdebug = debugsbitsvalue.str();
	debugsbitsvalue.str("");
	// serlogger.log( tdebug, 0 ); 
	try {
		static std::map<TPCodeUidPair, std::string> cachedData; 
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
		auto it = cachedData.find(std::make_pair(stcode, uid));
		if (  it != cachedData.end()) {
			serlogger.log( "readSType() completed.", 0 );
			return it->second; // return a pointer to the value stored at the matching key
		}
	}
	catch( std::exception &e ) { // throw an exception if things go south. like memory failed to allocate
		std::stringstream errstream;
		errstream << "Exception in thread: " << e.what();
		std::string errstr = errstream.str();
		errstream.str("");
		serlogger.log( errstr, 5 );
		boost::this_thread::interruption_point(); // specifies this as aninterruption point, allowing boost::thread to gracfully end the thread/
		return "Err";
	}
	return "Err";
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

std::string serial::readID(  const vector<unsigned long int> &hbits, const std::string &fstype ) { 
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
	unsigned int idlen = idnum.length();
	char sid[5];
	for( unsigned int i = 0; i < idlen; i++ ) {
		sid[i] = idnum[i];
	}
	Ssid = atoi( sid ); // used for data validation later
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
	static std::map<RgCodeUidPair, std::string> siteMap; // need to put this into try blocks. similar to readSType, but with sites
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
	auto it = siteMap.find(std::make_pair(rgcode, uid));
	if ( it != siteMap.end() ) {
		serlogger.log( "readSite() executed successfully.", 0 );
		return it->second; // returns a pointer to the matching key value
	}
	else {
		return "ERR";
	}
}


std::string serial::getTLD( const std::string &fsite ) {
	serlogger.log( "getTLD() called.", 0 ); // if site is not in the GNsites list, it's gf, if it is, GN
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

void serial::getHostName( const vector<unsigned long int> &fhbits ) { // Assembles various bits into a coherent hostname
	serlogger.log( "getHostName() called.", 0 );
	std::string Stld = serial::getTLD( Ssite );
	std::string tid = serial::readType( Sstype );
	std::string Suid = serial::readID( fhbits, Sstype );
	std::stringstream sss;
	std::stringstream ss;
	if ( tid != "na" ) {
		ss << Suid << "." << tid << "." << Ssite << "." << Stld; // assembles hostname  from the return values 
		Shname = ss.str();
		ss.str("");
		std::stringstream lstream; // I really ought to declare lstream globally. Sigh.
		lstream << "Determined Hostname to be: " << Shname << ".";
		std::string lstr = lstream.str();
		lstream.str("");
		serlogger.log( lstr, 1 );
		sss << Suid << "." << Ssite; // host shortname, mostly for server + drive matching
		Sshname = sss.str();
		sss.str("");
		lstream << "Determined Host Shortname to be: " << Sshname << ".";
		lstr = lstream.str();
		lstream.str("");
		serlogger.log( lstr, 1 );
	}
	else {
		ss << Suid << "." << Ssite << "." << Stld; // for 'non-typed' servers *no .gc. or .nntp.
		Shname = ss.str();
		ss.str("");
		ss << "Determined Hostname to be: " << Shname <<". Unable to determine host Shortname.";
		std::string lmsg = ss.str();
		ss.str("");
		serlogger.log( lmsg, 2 );
	}
	serlogger.log( "getHostName() completed.", 0 );
}

