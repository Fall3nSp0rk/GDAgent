// db.h
// MySQL database connections and DB search/parsing.

const char dbuser[] = "GigaDash";
const char dbpass[] = "GigaDash123";
const char dbname[] = "gigadash";
const char dbhost[] = "localhost";
//#include"serial.h"

class dbase{
	public:
		dbase();
		void runQuery();
		bool getQueryData( const std::string &Frstate, const std::string &Fstype, const std::string &Fsite, const std::string &Fhost, const std::string &Fatype, const std::string &Fsstate, const std::string &Fanum, const std::string &Fshanme, const std::string &Fmac, const int &vfill, const int &dnum );
	private:
		bool sQuery( const std::string &data, const std::string &table, const std::string &column );
		bool checkExists( const std::string &site );
		void storeQueryData( const std::string &fsval, const int &posid ); // written
		std::vector<std::string> inval;
		std::string getLocFromSite( const std::string &site );
		void insert();
		void update();

};
