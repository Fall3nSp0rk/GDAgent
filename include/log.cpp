#include<fstream>
const std::string logfile = "/var/log/GDAgentLog";

void log( const std::string &text ) {
	std::ofstream log_file( logfile, std::ios_base::out | std::ios_base::app );
	log_file<< text << std::endl;
}
