// db.h
// MySQL database connections and DB search/parsing.
#ifndef __DBASE_H
#define __DBASE_H
const char dbuser[] = "GigaDash";
const char dbpass[] = "GigaDash123";
const char dbname[] = "gigadash";
const char dbhost[] = "localhost";
//#include"serial.h"

class dbase{
	public:
		dbase();
		void runQuery();
		void runDriveQuery( const std::vector<std::string> &qvec );
		bool getQueryData( const std::vector<std::string> &fvec );
	private:
		void updateServer();
		void inServer();
		bool checkExists( const std::string &data, const std::string &table, const std::string &field, const std::string &f2, const std::string &d2 );
		void storeQueryData( const std::string &fsval, const int &posid ); // written
		std::vector<std::string> inval;
		std::string getLocFromSite( const std::string &site );
		void insert( const std::string &qstring );
		void update( const std::string &qquery );

};
class ttask {
	public:
		static void dbdrivetask( const std::vector<std::string> &qq );
		static void dbstask( const std::vector<std::string> &qq );
};
extern ttask tt;
//extern dbase thread;
#endif
