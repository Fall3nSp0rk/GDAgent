#ifndef __DDRIVE_H
#define __DDRIVE_H
using std::vector;
class ddrive {
	public:
		static std::vector<int> DEFAULT_VECTOR;
		ddrive( const int &dnum = 36, const std::string &hsname = "r00.AMS", const vector<int> &ddata = DEFAULT_VECTOR ); 
		bool readDriveData();
		vector<std::string> qdata;
	protected:
		int Ddnum;
		int Dspnum; // spool number
		int Dspfill; // spool fill percentage
		int Dpsec; // drive pending sectors
		int Drsec; // drive reallocated sectors
		std::string Dhsname; // host shortname
		std::string Ddpath; // drive path, e.g. /dev/sda
		std::string Dhstatus; // drive health status, e.g. Failed, healthy, degraded, or critical
		std::string Dspstate; // spool status, e.g. OFF, RW, NO/ERROR, RO, or NO
		
	private:
		
		int readSpoolNumber( const int &snfd, const int &snsd );
		int read3DigitVals( const int &fd,  const int &sd, const int &tdtd );
		std::string readDHState( const int &dhc );
		std::string readSpoolState( const int &sval );
		std::string readDPath( const int &dpathfd, const int &dpathsd );
		int Ddvblen; // total length of drive bit segment
		vector<int> Ddrivebits; // entire drive description segment
		vector<int> Ddcode; // the vector that holds each drives data
};

extern ddrive dd;


#endif
