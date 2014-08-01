// db.h
// MySQL database connections and DB search/parsing.

#include<iostream>
#include<sstream>
#include<string>
#include<mysql++/mysql++.h>
#include<vector>
#include<iomanip>

const char dbuser[] = "GigaDash";
const char dbpass[] = "GigaDash123";
const char dbname[] = "gigadash";
const char dbhost[] = "localhost";

class dbase {
	public:
		dbase();
		void runQuery();
		bool getQueryData( std::string Frstate, std::string Fstype, std::string Fsite, std::string Fhost, std::string Fatype, std::string Fsstate, std::string Fanum, int vfill, int dnum );
	private:
		bool sQuery( std::string data, std::string table, std::string column );
		bool checkExists( std::string site );
		void storeQueryData( std::string fsval, int posid ); // written
		std::vector<std::string> inval;
		std::string getLocFromSite( std::string site );
		void insert();
		void update();

}thread;
void dbase::runQuery() {
	if ( checkExists( inval[1] ) == true ) {
		update();
	}
	else {
		insert();
	}
}
bool dbase::checkExists( std::string data ) {
	mysqlpp::Connection con3( false );
	if ( con3.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream queryss;
		queryss<< "SELECT hostname FROM `" << dbname << "`.`gd_servers` WHERE hostname = " << mysqlpp::quote_only << data << ";";
		std::string qstr = queryss.str();
		mysqlpp::Query query = con3.query( qstr );
		//std::cout << qstr << std::endl;
		mysqlpp::StoreQueryResult res = query.store();
		std::stringstream res1;
		for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
			mysqlpp::Row row = *it;
			res1 << row[0] << std::endl;
		}
		std::string result = res1.str();
		std::cout<< result << std::endl;

		if ( result == inval[1] ) {
			return true;
		}
		else {
			return false;
		}
	}
}

void dbase::update() {
	mysqlpp::Connection con2( false );
	if ( con2.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream uss;
		uss<< " UPDATE '" << dbname << "`.`gd_servers` SET asset_type=" << mysqlpp::quote_only << inval[2] << ", recover_status=" << mysqlpp::quote_only << inval[10] << ", gc_status=" << mysqlpp::quote_only << inval[11] << ", var_fill=" << mysqlpp::quote_only << inval[12] << ", open_ticket=" << mysqlpp::quote_only << "0" << ", last_report = NOW() WHERE hostname = " << mysqlpp::quote_only << inval[1] << ";";
		std::string upstr = uss.str();
		mysqlpp::Query query = con2.query( upstr );
		mysqlpp::SimpleResult res = query.execute();
		if ( res ) {
			std::cout<< "Record updated sucessfully." << std::endl;
		}
		else {
			std::cerr<< "Error updating record." << std::endl;
		}
	}
	else {
		std::cerr<< "Error establishing connection to database." << std::endl;
	}
}

std::string dbase::getLocFromSite( std::string site ) {
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream lfs;
		lfs<< "SELECT srg_code FROM `" << dbname << "`.`gd_sites` WHERE site_name = " << mysqlpp::quote_only << site << ";";
		std::string loc = lfs.str();
		mysqlpp::Query query = conn.query( loc );
		if (mysqlpp::StoreQueryResult res = query.store() ) {
			std::stringstream res1;
			for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
				mysqlpp::Row row = *it;
				res1 << row[0] << std::endl;
			}
			std::string result = res1.str();

			return result;
		} 
		if ( conn.errnum() ) {
			std::cerr << "Error Received in fetching a row: " << conn.error() << std::endl;
			return "ERR";
		}
		else {
			std::cerr << "Unknown Error Occurred or Unhandled Exception. " << std::endl;
			return "ERR";
		}
	}
	else {
		std::cerr << "Database connection failed: " << conn.error() << std::endl;
		return "ERR";
	}
}

void dbase::insert() {
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream iss;
		iss<< "INSERT INTO `" << dbname << "`.`gd_servers` VALUES( ";
		for( int i = 0; i < 13; i++ ) {
			iss<< mysqlpp::quote_only << inval[i] << ", ";
		}
		iss<< mysqlpp::quote_only << "0" << ", NOW() );";
		std::string is = iss.str();
		mysqlpp::Query query = conn.query( is );
		std::cout<< is << std::endl;
		mysqlpp::SimpleResult res = query.execute();
		if( res ) {
			std::cout<< "Data inserted successfully." << std::endl;
		}
		else {
			std::cerr<< "Failed to add server to database." << std::endl;
		}
	}
	else {
		std::cerr<< "Failed to connect to database " << dbname << ". Please ensure mysql is running, and that your credentials are correct." << std::endl;
	}
}

bool dbase::getQueryData( std::string Frstate, std::string Fstype, std::string Fsite, std::string Fhost, std::string Fatype, std::string Fsstate, std::string Fanum, int vfill, int dnum ) {
	/* std::string Frstate = buffer.giveStrVal( buffer.Srstate ); // recovery state
	std::string Fstype = buffer.giveStrVal( buffer.Sstype ); // server type
	std::string Fsite = buffer.giveStrVal( buffer.Ssite ); // site
	std::string Fhost = buffer.giveStrVal( buffer.Shname ); // hostname
	std::string Fatype = buffer.giveStrVal( buffer.Satype ); // asset type
	std::string Fsstate = buffer.giveStrVal( buffer.Sservices ); // service status
	std::string Fanum = buffer.giveStrVal( buffer.Sanum ); // number of arrays */
	std::string Fshorthost = "herp"; // host shortname
	std::string Floc = getLocFromSite( Fsite ); // location, queried from database based on string
	std::string FOrt = "false"; // true/false value for whether there is an open RT for the server. default is false.
	std::string Fmac = "aa:bb::cc:dd:ee:ff";
	std::stringstream ssfill;
	ssfill << vfill; // /ivar fill percentage
	std::string Fvfill = ssfill.str();
	std::stringstream ssdnum;
	ssdnum <<  dnum; // number of hard drives
	std::string Fdnum = ssdnum.str();
	std::string uid= ""; // queried from database, default is 0;
	if ( Fstype == "r" || Fstype == "h" || Fstype == "t" ) {
		std::stringstream ss;
		ss << Fstype << mysqlpp::quote << "_server";
		std::string sstype = ss.str();
		storeQueryData( sstype, 4 );
	}
	else {
		storeQueryData( Fstype, 5 );
	}
	int num = 5; // this is  temporary until I write it's hook in the serial class.
	std::stringstream anum1;
	anum1 << num;
	std::string tanum = anum1.str();
	storeQueryData( Fatype, 0 );
	storeQueryData( Fhost, 1 );
	storeQueryData( Fshorthost, 2 );
	storeQueryData( uid, 3 );
	//storeQueryData( Fstype, 4 );
	storeQueryData( Fsite, 5 );
	storeQueryData( Floc, 6 );
	storeQueryData( Fmac, 7 );
	storeQueryData( Fdnum, 8 );
	storeQueryData( tanum , 9 );
	storeQueryData( Frstate, 10 );
	storeQueryData( Fsstate, 11 );
	storeQueryData( Fvfill, 12 );
	storeQueryData( FOrt, 13 );
	 /* for ( int i = 0; i < 14; i++ ) {
		std::cout<< inval[i];
	}
	std::cout<< std::endl << Fhost << std::endl; */
	std::cout << buffer.giveStrVal( buffer.Shname ) << std::endl;
}

void dbase::storeQueryData( std::string fsval, int posid ) {
	inval[posid] = fsval;
}

dbase::dbase() {
	// initialize the size of the query data storage vector
	inval.resize(16);
	// getQueryData(); // retrieve data to be entered into the database
}

bool dbase::sQuery( std::string data, std::string table, std::string column ) {
	
	mysqlpp::Connection con( false ); // establish connection to db server
	if ( con.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream qss;
		qss  << "SELECT * FROM `" << table << "`.`" << dbname << "` WHERE " << column << " = ' " << data << "';";
		std::string qs = qss.str();
		mysqlpp::Query query = con.query( qs );
		mysqlpp::StoreQueryResult res = query.store();
		// return true  if the value was found, false if the result set was of size sero.
		if ( res.size() < 1 ) { 
			return false;
		}
		else {
			return true;
		}
	}
	else {
		std::cerr<< "Failed to retrieve database information." << std::endl;
		return false;
	}
}

