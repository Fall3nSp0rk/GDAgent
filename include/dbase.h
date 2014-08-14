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
		bool getQueryData( std::string Frstate, std::string Fstype, std::string Fsite, std::string Fhost, std::string Fatype, std::string Fsstate, std::string Fanum, std::string Fshanme, int vfill, int dnum );
	private:
		bool sQuery( std::string data, std::string table, std::string column );
		bool checkExists( std::string site );
		void storeQueryData( std::string fsval, int posid ); // written
		std::vector<std::string> inval;
		std::string getLocFromSite( std::string site );
		void insert();
		void update();

};
