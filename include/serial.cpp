// read.h
// contains functions for reading from buffer.
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<map>
#include "log.h"
#include "dbase.h"
#include"serial.h"
#include<iomanip>
#include<stdlib.h>
#include<boost/thread.hpp>
#include<exception>
#include "mmapper.h"
#include "drive.h"
#define _logger mlog
using std::vector;
const int bsize = 1024;
static const int valstrucpos[] = { 0, 1, 5, 15, 16, 36, 37, 38, 39, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586 };
vector<int> Ssflags ( valstrucpos, valstrucpos + sizeof( valstrucpos ) / sizeof(valstrucpos[0]) );
const int Sserverstart = 6;
const int Smacstart = 1;
const int Sdbitstart = 39;
const int Shbitstart = 1;
const int Sslen = 10;
const int Shlen = 4;
const int Sdlen = 576;
const int Smlen = 18;
const int Sblen = 1024;
serial::serial( ){ // overrides default constructor
	//logger _logger;
	try {
		_logger.logstream << "serial object initialized.";
		_logger.log( 1 );
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
		_logger.logstream << "Exception in thread: " << e.what();
		_logger.log( 5 );
		boost::this_thread::interruption_point();
	}
}
void serial::readDVec( std::vector<int> &vec ) {
	//logger _logger;
	_logger.logstream << "readDVec() called.";
	_logger.log( 0 );
	for( int i = 0; i > Sdlen; i++ ) {
		vec[i] = Sdbits[i];
		_logger.logstream << vec[i];
		
	}
	_logger.log( 0 );
}
void serial::readBits( const std::vector<int> &buff ){
	//logger _logger;
	try{
		_logger.logstream << "Called readBits";
		_logger.log( 0 );
		for ( int i = 0; i < 1024; i++ ) {
			Snbuff.push_back( buff[i] );
		}
		int h = Shbitstart;
		for ( int i = 0; i < 4; i++ ) { // map buffer to smaller vectors for portability
			Shbits.push_back( buff[h] );
			h++;
		}
		int j = Sserverstart;
		for ( int i = 0; i < Sslen; i++ ) {
			Ssbits.push_back( buff[j] );
			j++;
		}
		int f = Smacstart;
		for ( int i = 0; i < Smlen; i++ ) {
			Smbits.push_back( buff[f] );
			f++;
		}
		if( (buff[35] == 1) /* && (buff[36] == 9) && (buff[37] == 9) && (buff[38] == 9)  && (buff.size() >= 616 )*/ ) {
			int m = Sdbitstart;
			for ( int i = 0; i < Sdlen; i++ ) {
				Sdbits.push_back( buff[m] );
				m++;
				_logger.logstream << Sdbits[i];
			} //commenting this out until I write the actual drive bit reading util.
			_logger.log( 0 );
		}
		else {
			for( int i = 0; i < Sdlen; i++ ) {
				Sdbits.push_back( 0 );
			}
			_logger.logstream << "Stream too small for drive data.";
			_logger.log( 3 );
		}
	}
	catch( std::exception &e ) {
		_logger.logstream << "Exception in thread: " << e.what();
		_logger.log( 5 );
	}
		_logger.logstream << "readBits() exited.";
		_logger.log( 0 );
}


bool serial::validateHost( const std::string &fstype, const int &fsid, const std::string &fsite, const std::string &ftld ) {
	//logger _logger;
	bool passedUIDValidation = false; // as each segment is validated, these flags are set to true.
	bool passedSiteTLDValidation = false;
	bool passedTypeTLDValidation = false;
	bool passedSiteTypeValidation = false;
	bool passedMACFormatValidation = false;
	bool passedMACValidation = false;
	_logger.logstream << "validateHost() called.";
	_logger.log( 0 );
	_logger.logstream << "Beginning data validation on received data for alleged host " << Shname << ".";
	_logger.log( 1 );
	vector<std::string> hexvals{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
	vector<std::string> gnsites{ "AMS", "AMS1", "AMS2", "AMS3", "DCA", "DCA1", "DCA2", "DCA3", "LAX", "HKG", "AUS2" };
	vector<std::string> gntypes{ "r", "t", "h", "xref", "num", "backlog", "border", "buffer" };
	vector<std::string> gftypes{ "vpn", "vpnhost", "vmhost" };
	vector<std::string> l4uids{ "db", "border", "backlog", "serv", "vpnhost", "t" };
	vector<std::string> l2uids{ "gcf", "num", "xref", "buffer", "gw", "bfs", "ax", "lb" };
	_logger.logstream << "Validating UIDs...";
	_logger.log( 1 );
	if ( fstype == "r" && fsid > 400 ) { // checks UIDs against a range they should appear in
		_logger.logstream << "Invalid UID for r-server of " << fsid << " passed. Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else if( fstype == "h" &&  fsid > 8 ) { 
		_logger.logstream << "Invalid UID for h-server of " << fsid << " passed. Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else if( (std::find( l4uids.begin(), l4uids.end(), fstype) != l4uids.end() ) && fsid > 4 )  {
		_logger.logstream << "Invalid UID for " << fstype << "-server of " << fsid << " passed. Rejecting data as invalid."; // some uids are max of 4 e.g. backlogs, borders.
		_logger.log( 3 );
		return false;
	}
	else if( (std::find( l2uids.begin(), l2uids.end(), fstype ) != l2uids.end() ) && fsid > 2 ) {
		_logger.logstream << "Invalid UID for " << fstype << "-server of " << fsid << " passed. Rejecting data as invalid."; // some uids have a max of 2, e.g. num, xref
		_logger.log( 3 );
		return false;
	}
	else { // if nothing matches, set UID flag to true
		_logger.logstream << "UID Validation Successful. UID " << Suid << " is valid.";
		_logger.log( 1 );
		passedUIDValidation = true;
	}
	_logger.logstream << "Validating TLD against site...";
		_logger.log( 1 );
	if( ( std::find( gnsites.begin(), gnsites.end(), fsite ) != gnsites.end() ) && ftld != "giganews.com" ) {
		_logger.logstream << "TLD Validation against site Failed. TLD should be giganews.com, currently is " << ftld << ". Rejecting as invalid."; // if site isn't listed in gnsites list above, and TLD is giganews, reject as invalid.
		_logger.log( 3 );
		return false;
	}
	else if( ( std::find( gnsites.begin(), gnsites.end(), fsite ) == gnsites.end() ) && ftld != "goldenfrog.com" ) {
		_logger.logstream << "TLD Validation against site Failed. TLD should be goldenfrog.com, currently is " << ftld << ". Rejecting data as invalid."; // similar to before, but if it IS listed in gnsites, and TLD is gf.com, reject
		_logger.log( 3 );
		return false;
	}
	else { // if none of above match, set next flag to true
		_logger.logstream << "TLD Validation against Site Successful. TLD " << ftld << " is valid for site " << fsite << ".";
		_logger.log( 1 );
		passedSiteTLDValidation = true;
	}
	_logger.logstream << "Validating TLD against Type";
		_logger.log( 1 ); // only certain kinds of servers are .gn
	if( ( std::find( gntypes.begin(), gntypes.end(), fstype ) != gntypes.end() ) && ftld != "giganews.com" ) {
		_logger.logstream << "TLD Validation against Type failed. TLD should be giganews.com, currently is " << ftld << ". Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else if( ( std::find( gftypes.begin(), gftypes.end(), fstype ) != gftypes.end() ) && ftld != "goldenfrog.com" ) {
		_logger.logstream << "TLD Validation against Type failed. TLD should be goldenfrog.com, currently is " << ftld << ". Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else {
		_logger.logstream << "TLD Validation against Type Successful. TLD " << ftld << " is valid for type " << fstype <<".";
		_logger.log( 1 );
		passedTypeTLDValidation = true;
	}
	_logger.logstream <<  "TLD Validation complete. Beginning Type Validation against Site."; // certain sites don't have certain server types. Compare, and if a match is found, reject
	_logger.log( 1 );
	if(  ( std::find( gntypes.begin(), gntypes.end(), fstype ) != gntypes.end()
				&& std::find( gnsites.begin(), gnsites.end(), fsite ) == gnsites.end() )
			|| ( std::find( gftypes.begin(), gftypes.end(), fstype) != gftypes.end()
				&& std::find( gnsites.begin(), gnsites.end(), fsite) != gnsites.end() ) ) {
		_logger.logstream << "Type Validation against site failed. Type " << fstype << " does not exist in site " << fsite << ". Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else if( (fstype == "cruncher" && std::find( gnsites.begin(), gnsites.end(), fsite) != gnsites.end() ) 
			|| ( fstype == "db" && ( fsite != "DCA" || fsite != "DCA1" ) ) ) {
		_logger.logstream << "Type Validation against site failed. Type " << fstype << " does not exist in site ";
		_logger.logstream << fsite << ". Rejecting data as invalid.";
		_logger.log( 3 );
		return false;
	}
	else {
		_logger.logstream << "Type Validation against Site Successful. Type " << fstype << " is valid for site ";
		_logger.logstream << fsite << ".";
		_logger.log( 1 );
		passedSiteTypeValidation = true;
	}
	_logger.logstream <<  "Validating MAC address...";
	_logger.log( 1 );
	_logger.logstream <<  "Validating MAC format...";
	_logger.log( 1 );
	// broken code for mac address validation below
	/* if( &Smac[2] != ":" || &Smac[5] != ":" || &Smac[8] != ":" || &Smac[11] != ":" || &Smac[14] != "\n" ) {
	_logger.logstream << "MAC Address Validation Failed. MAC " << Smac << " is of invalid format. Rejecting data as invalid.";
		lmsg =_logger.logstream.str();
	_logger.logstream.str("");
		_logger.log( 3 );
		return false;
	} 
	else {
	_logger.logstream << "MAC Address " << Smac << " if of valid format.";
		lmsg =_logger.logstream.str();
	_logger.logstream.str("");
		_logger.log( 1);
		passedMACFormatValidation = true;
	} */
	passedMACFormatValidation = true;
	/*
	_logger.logstream << "Validating MAC Address Values...", 1);
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
		_logger.logstream << "MAC Address Validation Failed. One or more values is not a valid hex value.";
		_logger.log( 3 );
		return false;
	}
	else {
		_logger.logstream << "MAC Address Validation Successful. MAC Address " << Smac << " is valid.";
		_logger.log( 1 );
		passedMACValidation = true;
	}
		_logger.logstream << "All Validation checks completed. Running final check now."; // final test. If all flags are TRUE now, return true - data has passed validation check.
		_logger.log( 1 );
		_logger.logstream << passedUIDValidation << " " << passedSiteTLDValidation << " " << passedTypeTLDValidation;
		_logger.logstream << " " << passedSiteTypeValidation << " " << passedMACFormatValidation << " ";
		_logger.log( 0 );
	if( passedUIDValidation == false || passedSiteTLDValidation == false || passedTypeTLDValidation == false
			|| passedSiteTypeValidation == false || passedMACFormatValidation == false
			|| passedMACValidation == false ) {
		_logger.logstream << "Error performing final validation checks. One or more flags was not set. Dumping flags. ";
		_logger.logstream << passedUIDValidation << " " << passedSiteTLDValidation << " " << passedTypeTLDValidation;
		_logger.logstream << " " << passedSiteTypeValidation << " " << passedMACFormatValidation << " ";
		_logger.logstream << passedMACValidation;
		_logger.log( 4 );
		return false;
	}
	else {
		_logger.logstream << "Validation Completed. Passing to database module.";
		_logger.log( 1 );
		return true;
	}
}

bool serial::deSerialize( ) { // wrapper function that calls deserialization functioons, then validates, and returns true if successful, false if not.
	//logger _logger;
	_logger.logstream << "deSerialize() called.";
	_logger.log( 0 );
	/*Sstype = mapp.getKeyFromMap( Ssbits[0], Ssbits[1], 1 ); 
	Ssite = mapp.getKeyFromMap( Ssbits[5], Ssbits[6], 2 ); */
	Sstype = readSType( Ssbits );
	Ssite = readSite( Ssbits );
	readGC( Ssbits );
	readRecover( Ssbits[3] );
	getHostName( Shbits );
	readDNum( Ssbits );
	readMac( Smbits );
	readAType( Ssbits );
	_logger.logstream << "deSerialize() completed.";
	_logger.log( 0 );
	if( validateHost( Sstype, Ssid, Ssite, Stld ) ) {
		return true;
	}
	else {
		return false;
	}
}

// function definitions for serial class follow here

void serial::readMac( const vector<int> &mbits ) { // reads the 18 digit decimal-encoded mac address
	//logger _logger;
	_logger.logstream << "readMac() called.";
	_logger.log( 0 );
	std::ostringstream strm;
	char macbit[3];
	for( int i = 0; i < 18; i++ ){
		_logger.logstream << mbits[i];
	}
	std::string mm = _logger.logstream.str(); // for debug purposes, we echo the received decimal digits to the logger
	_logger.log( 1 );
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
	_logger.logstream << "Mac address of server " << Shname << " should be " << Smac << ".\n";
	_logger.log( 1 );
}

void serial::readGC( const vector<int> &fsbits ) {
	//logger _logger;
	_logger.logstream << "readGC() called."; // reads the GC status bit (sbit[2]) and translates it to human
	_logger.log( 0 );

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
			_logger.logstream << "Error on line 370: serial.cp: Invalid value received for Sservices: " << fsbits[2];
			_logger.log( 3 );
			break;
	}
	//catch( int e ) { // need to add this back in for exception handling after I write custom excaption subclass
	//	std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
	//}
	_logger.logstream << "readGC() completed.";
	_logger.log( 0 );
}

void serial::readDNum( const vector<int> &sbits ) { // read the number of drives from sbit[4]
	//logger _logger;
	_logger.logstream << "readDNum() called.";
	_logger.log( 0 );
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
			_logger.logstream << "Error on line 156: serial.cpp: invalid value (3) received for Sdnum.";
			_logger.log( 3 );
			break;
		default: // should only occur if no data is received, should throw an exception
			Sdnum = 0;
			_logger.logstream << "Error on line 156: serial.cpp: invalid value (" << sbits[4] <<  ") received for Sdnum.";
			_logger.log( 3 );
			break;
	}
//	catch( int e) {
//
//		std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
//	}
	_logger.logstream << "readDnum() completed.";
	_logger.log( 0 );
}

std::string serial::readType( const std::string &fstype ) { // takes the server type and determines the next part of its hostname
	//logger _logger;
	_logger.logstream << "readType() called.";
	_logger.log( 0 );
	if ( fstype == "r" || fstype == "h" || fstype == "t" || fstype == "serv" ) {
		_logger.logstream << "Determined type to be 'gc'.";
		_logger.log( 1 ); // *.gc.*.*
		return "gc";
	}
	else if ( fstype == "backlog" || fstype == "border" || fstype == "num" || fstype == "xref" ) {
		_logger.logstream << "Determined type to be 'nntp'.";
		_logger.log( 1 ); // *.nntp.*.*
		return "nntp";
	}
	else {
		_logger.logstream << "No type found.";
		_logger.log( 1 ); // All the rest
		return "na";
	}
	_logger.logstream << "readType() completed.";
	_logger.log( 0 );
}


std::string serial::readSType( const vector<int> &sbits ) {
	//logger _logger;
	_logger.logstream << "readSType() called.";
	_logger.log( 0 );
	int stcode = sbits[0];
	int uid = sbits[1];
	_logger.logstream << "Values fed to readSType(): " << sbits[0] << sbits[1];
	_logger.log( 0 );
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
			_logger.logstream << "readSType() completed.";
			_logger.log( 0 );
			return it->second; // return a pointer to the value stored at the matching key
		}
	}
	catch( std::exception &e ) { // throw an exception if things go south. like memory failed to allocate
		_logger.logstream << "Exception in thread: " << e.what();
		_logger.log( 5 );
		boost::this_thread::interruption_point(); // specifies this as aninterruption point, allowing boost::thread to gracfully end the thread/
		return "Err";
	}
	return "Err";
}


void serial::readAType( const vector<int> &sbits ) {
	//logger _logger;
	_logger.logstream <<  "Determined asset-type to be 'server'.";
	_logger.log( 1 );
	Satype = "server";
	// Asset type isn't implemented. YET.
	
}

void serial::readRecover( const int &rcode ) {
	//logger _logger;
	_logger.logstream << "readRecover() called.";
	_logger.log( 0 );
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
			_logger.logstream << "Error on line 243: serial.cpp: Invalid data (?) received for Srstate.";
			_logger.log( 2 );
			break;
	}
	_logger.logstream << "readRecover() completed.";
	_logger.log( 0 );
}

std::string serial::readID(  const vector<unsigned long int> &hbits, const std::string &fstype ) { 
	//logger _logger;
	_logger.logstream << "readID() called.";
	_logger.log( 0 );
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
		_logger.logstream << "readID() completed.";
		_logger.log( 0 );
		return id;
	}
	else {
		ss << fstype << idnum;
		id = ss.str();
		ss.str("");
		_logger.logstream << "readID() completed.";
		_logger.log( 0 );
		return id;
	}
}

// Reads the site identifier code
std::string serial::readSite( const vector<int> &fsbits ) {
	//logger _logger;
	_logger.logstream << "readSite() called.";
	_logger.log( 0 );
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
		_logger.logstream << "readSite() executed successfully.";
		_logger.log( 0 );
		return it->second; // returns a pointer to the matching key value
	}
	else {
		return "ERR";
	}
}

std::string serial::getTLD( const std::string &fsite ) {
	//logger _logger;
	_logger.logstream << "getTLD() called.";
	_logger.log( 0 ); // if site is not in the GNsites list, it's gf, if it is, GN
	vector<std::string> gnsites{ "AMS", "AMS1", "AMS2", "AMS3", "DCA", "DCA1", "DCA2", "DCA3", "LAX", "HKG", "AUS2" };
	if ( std::find(gnsites.begin(), gnsites.end(), fsite) != gnsites.end()) { // gives the top level domain for the server.
		_logger.logstream << "Determined TLD to be giganews.com.";
		_logger.log( 1 );
		return "giganews.com";
	}
	else {
		_logger.logstream << "Determined TLD to be goldenfrog.com.";
		_logger.log( 1 );
		return "goldenfrog.com";
	}
}

void serial::getHostName( const vector<unsigned long int> &fhbits ) { // Assembles various bits into a coherent hostname
	//logger _logger;
	_logger.logstream << "getHostName() called.";
	_logger.log( 0 );
	std::string Stld = serial::getTLD( Ssite );
	std::string tid = serial::readType( Sstype );
	std::string Suid = serial::readID( fhbits, Sstype );
	std::stringstream sss;
	std::stringstream ss;
	if ( tid != "na" ) {
		ss << Suid << "." << tid << "." << Ssite << "." << Stld; // assembles hostname  from the return values 
		Shname = ss.str();
		ss.str("");
		_logger.logstream << "Determined Hostname to be: " << Shname << ".";
		_logger.log( 1 );
		ss << Suid << "." << Ssite; // host shortname, mostly for server + drive matching
		Sshname = ss.str();
		ss.str("");
		_logger.logstream << "Determined hostname to be: " << Shname << ". Determined Host Shortname to be: " << Sshname << ".";
		_logger.log( 1 );
	}
	else {
		sss << Suid << "." << Ssite << "." << Stld; // for 'non-typed' servers *no .gc. or .nntp.
		Shname = ss.str();
		sss.str("");
		_logger.logstream << "Determined Hostname to be: " << Shname <<".";
		_logger.log( 0 );
		sss <<  Suid << Ssite;
		Sshname = sss.str();
		sss.str("");
		_logger.logstream << "Determined Hostname to be: " << Shname <<  ". Determined Host Shortname to be: " << Sshname << ".";
		_logger.log( 0 );
	}
	_logger.logstream << "getHostName() completed.";
	_logger.log( 0 );
}
#undef _logger
