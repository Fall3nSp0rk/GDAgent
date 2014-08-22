// read.h
// contains functions for reading from buffer.
using std::vector;
typedef std::pair<int, int> RgCodeUidPair;
class serial {
	public:
		serial();
		void readBits( const std::vector<int> &buff );
		void deSerialize();
		std::string giveStrVal( std::string cval );
		int giveIntVal( int cint );
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
	
};
