//log.h
// for logging and daemonizing functions.
//
#ifndef __LOG_H
#define __LOG_H
class logger {
	public:
		logger( int ll = 0, std::string lf = "/var/log/glog" );
		void log ( int mlvl );
		int Llport;
		std::stringstream logstream;
	private:
		void errException( const int &ecode, const int &line, const std::string &file );
		void setLogLevel( int llvl );
		pid_t getPID();
		unsigned long getThread();
		std::string tStamp();
		bool writeLog( const std::string &message );
		std::string Lcfgfile;
		std::string Lfile;
		unsigned long Lthread;
		pid_t Lpid;
		int Llvl;
};
#endif
