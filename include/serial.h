// read.h
// contains functions for reading from buffer.
#ifndef __SERIAL_H
#define __SERIAL_H
#define _logger slog
using std::vector;
class serial {
	public:
		serial();
		void readBits( const std::vector<int> &buff );
		bool deSerialize();
		void readDVec( std::vector<int> &vec );
//	protected:
		std::string Srstate;
		std::string Sstype;
		std::string Ssite;
		std::string Shname;
		std::string Smac;
		std::string Sshname;
		std::string Satype;
		std::string Sservices;
		std::string Sanum;
		std::string Suid;
		int Svarfill;
		int Sdnum;
		bool Svalidated;
		vector<int> Sdbits;
		vector<std::string> Sdbvec;
	private:
		friend class dbase;
		std::string  readID( const vector<unsigned long int> &hbits, const std::string &fstype ); // definition written
		void getHostName( const vector<unsigned long int> &fhbits ); // definition written
		vector<int> Snbuff;
		void readDNum( const vector<int>& sbits ); // definition written
		std::string readSType( const vector<int> &sbits ); // definition written
		void readAType( const vector<int> &sbits ); // not implemented in this release, defaults to 0"server"
		std::string readType( const std::string &fstype );
		void readGC( const vector<int> &sbits ); // definition written
		std::string readSite( const vector<int> &sbits ); // definition written
		void readMac( const vector<int> &mbits ); 
		void readRecover( const int &rcode ); // definition written
		std::string getTLD( const std::string &fsite ); // definition written
		void wrapData();
		bool validateHost( const std::string &fstype, const int &fsid, const std::string &fsite, const std::string &ftld );
		vector<unsigned long int> Shbits;
		vector<int> Ssbits;
		vector<int> Smbits;
		int Ssid;
		std::string Stld;
	
};
#undef _logger
#endif
