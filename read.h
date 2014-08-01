// read.h
// contains functions for reading from buffer.
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<map>

using std::vector;

const int bsize = 1024;

const int Sserverstart = 6;
const int Sdbitstart = 11;
const int Sslen = 9;
const int Shlen = 4;
const int Sdlen = 612;
const int Smlen = 24;
const int Sblen = 1024;

typedef std::pair<int, int> RgCodeUidPair;

class serial {
	public:
		serial(){ // overrides default constructor
			Snbuff.resize(Sblen);
			Ssbits.resize(Sslen);
			Shbits.resize(Shlen);
			Sdbits.resize(Sdlen);
			Smbits.resize(Smlen);
		};
		void readBits( int buff[1024] ){
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
			int m = Sdbitstart;
			for ( int i = 0; i < Sdlen; i++ ) {
				Sdbits[i] = buff[m];
				m++;
			}
		};
		void deSerialize();
		std::string giveStrVal( std::string cval ) {
			std::string aval = cval;
			return aval;
		};
		int giveIntVal( int cint ) { //for copying values
			int aint = cint;
			return aint;
		};
//	protected:
		std::string Srstate;
		std::string Sstype;
		std::string Ssite;
		std::string Shname;
		std::string Smac;
		std::string Satype;
		std::string Sservices;
		std::string Sanum;
		int Svarfill;
		int Sdnum;

	private:
		friend class dbase;
		std::string  readID(  vector<unsigned long int> hbits, std::string fstype ); // definition written
		void getHostName( vector<int> &fsbits, vector<unsigned long int> fhbits ); // definition written
		vector<int> Snbuff;
		void readDNum( const vector<int>& sbits ); // definition written
		std::string readSType( const vector<int> &sbits ); // definition written
		void readAType( const vector<int> &sbits ); // not implemented in this release, defaults to 0"server"
		std::string readType( std::string fstype );
		void readGC( const vector<int> &sbits ); // definition written
		std::string readSite( const vector<int> &sbits ); // definition written
		void readMac( const vector<int> &mbits ); 
		void readRecover( const vector<int> &sbits ); // definition written
		std::string getTLD( std::string fsite ); // definition written

		vector<unsigned long int> Shbits;
		vector<int> Ssbits;
		vector<int> Sdbits;
		vector<int> Smbits;
		void debuf();
	
} buffer;


// I have no idea if this will work
void serial::deSerialize() {
	Ssite = readSite( Ssbits );
	Sstype = readSType( Ssbits );
	readGC( Ssbits );
	readRecover( Ssbits );
	getHostName( Ssbits, Shbits );
	readDNum( Ssbits );
	readMac( Smbits );
	readAType( Ssbits );
	debuf();
}

void serial::debuf() {
	std::cout<< Ssite << std::endl;
	std::cout<< Sstype << std::endl;
	std::cout<< Shname << std::endl;
}
// function definitions for serial class follow here

void serial::readMac( const vector<int> &mbits ) {
	Smac = "Not Implemented"; // haven't implemented this yet.
}

void serial::readGC( const vector<int> &fsbits ) {
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
			//throw(1002); // throw an exception if it gets data other than what it expects.
			break;
	}
	//catch( int e ) {
	//	std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
	//}
}

void serial::readDNum( const vector<int> &sbits ) {
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
//			throw(1001);
			break;
		default: // should only occur if no data is received, should throw an exception
			Sdnum = 0;
			//std::throw(1000);
			break;
	}
//	catch( int e) {
//		std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
//	}
}

std::string serial::readType( std::string fstype ) {
	if ( fstype == "r" || fstype == "h" || fstype == "t" || fstype == "serv" ) {
		return "gc";
	}
	else if ( fstype == "backlog" || fstype == "border" || fstype == "num" || fstype == "xref" ) {
		return "nntp";
	}
	else {
		return "na";
	}
}
std::string serial::readSType( const vector<int> &sbits ) {
	 int stcode = sbits[0];
	int uid = sbits[1];
	static std::map<RgCodeUidPair, std::string> cachedData;
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
		return it->second;
	}
}


void serial::readAType( const vector<int> &sbits ) {
	Satype = "Server";
	// Asset type isn't implemented. YET.
	
}

void serial::readRecover( const vector<int> &sbits ) {
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
	//		throw(1003);
			break;
	}
	//catch( int e ) {
	//	std::cout<< "An Exception has occurred. Error " << e << "." << std::endl;
	//}
}

std::string serial::readID(  vector<unsigned long int> hbits, std::string fstype ) {
	std::stringstream result;
	std::copy(hbits.begin(), hbits.end(), std::ostream_iterator<int>(result, ""));
	std::string idnum = result.str();
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
		return id;
	}
	else {
		ss << fstype << idnum;
		id = ss.str();
		return id;
	}
}

// Reads the site identifier code
std::string serial::readSite( const vector<int> &fsbits ) {
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
		return it->second;
	}
}


std::string serial::getTLD( std::string fsite ) {
	vector<std::string> gnsites{ "ams", "ams1", "ams2", "ams3", "dca", "dca1", "dca2", "dca3", "lax", "hkg", "aus2" };
	if ( std::find(gnsites.begin(), gnsites.end(), fsite) != gnsites.end()) { // gives the top level domain for the server.
		return "giganews.com";
	}
	else {
		return "goldenfrog.com";
	}
}

void serial::getHostName( vector<int> &fsbits, vector<unsigned long int> fhbits ) {
	std::string site = serial::readSite( fsbits );
	std::string tld = serial::getTLD( site );
	std::string tid = serial::readType( Sstype );
	std::string uid = serial::readID( fhbits, Sstype );
	std::stringstream ss;
	if ( tid != "na" ) {
		ss << uid << "." << tid << "." << site << "." << tld; // stringstream ftw
		Shname = ss.str();
	}
	else {
		ss << uid << "." << site << "." << tld;
		Shname = ss.str();
	}
}

