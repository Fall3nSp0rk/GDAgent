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
		bool runDriveQuery( const std::vector<std::string> &qvec );
		bool getQueryData( const std::string &Frstate, const std::string &Fstype, const std::string &Fsite, const std::string &Fhost, const std::string &Fatype, const std::string &Fsstate, const std::string &Fanum, const std::string &Fshanme, const std::string &Fmac, const int &vfill, const int &dnum );
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
//extern dbase thread;
#endif
