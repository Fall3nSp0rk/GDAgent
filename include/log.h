//log.h
// for logging and daemonizing functions.
//

class logger {
	public:
		logger();
		void log ( const std::string &text, int mlvl );
	private:
		void errException( const int &ecode, const int &line, const std::string &file );
		void readCfg();
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

extern logger GDLogger;
