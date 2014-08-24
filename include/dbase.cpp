// db.h
// MySQL database connections and DB search/parsing.

#include<iostream>
#include<sstream>
#include<string>
#include<mysql++/mysql++.h>
#include<vector>
#include<iomanip>
#include"dbase.h"
#include"log.h"
//#include"serial.h"
logger dbaselogger;
void dbase::runQuery() {
	dbaselogger.log( "runQuery() called.", 0 );
	bool ex = checkExists( inval[1] );
	if ( ex == true ) {
		update();
	}
	else {
		insert();
	}
	dbaselogger.log( "runQuery() completed.", 0 );
}
bool dbase::checkExists( const std::string &data ) {
	dbaselogger.log( "checkExists() called.", 0 );
	dbaselogger.log ( "Checking if host already in Database...", 1 );
	mysqlpp::Connection con3( false );
	if ( con3.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream queryss;
		queryss<< "SELECT hostname FROM `" << dbname << "`.`gd_servers` WHERE hostname = " << mysqlpp::quote_only << data << ";";
		std::string qstr = queryss.str();
		dbaselogger.log( qstr, 1 );
		mysqlpp::Query query = con3.query( qstr );
		//std::cout << qstr << std::endl;
		mysqlpp::StoreQueryResult res = query.store();
		std::stringstream res1;
		for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
			mysqlpp::Row row = *it;
			res1 << row[0];
		}
		std::string result = res1.str();
		dbaselogger.log( result, 1 );

		if ( result == inval[1] ) {
			dbaselogger.log( "Host exists in Database.", 1 );
			return true;
		}
		else {
			dbaselogger.log( "Host does not exist in Database.", 1 );
			return false;
		}
	}
	dbaselogger.log( "checkExists() completed.", 0 );
}

void dbase::update() {
	dbaselogger.log( "update() called.", 0 );
	dbaselogger.log( "Updating database entry.", 1 );
	mysqlpp::Connection con2( false );
	if ( con2.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream uss;
		uss<< " UPDATE `" << dbname << "`.`gd_servers` SET asset_type=" << mysqlpp::quote_only << inval[0] << ", recover_status=" << mysqlpp::quote_only << inval[10] << ", gc_status=" << mysqlpp::quote_only << inval[11] << ", var_fill=" << mysqlpp::quote_only << 0 << ", open_ticket=" << mysqlpp::quote_only << "0" << ", last_report = NOW() WHERE hostname = " << mysqlpp::quote_only << inval[1] << ";";
		std::string upstr = uss.str();
		dbaselogger.log( upstr, 1 );
		mysqlpp::Query query = con2.query( upstr );
		mysqlpp::SimpleResult res = query.execute();
		if ( res ) {
			dbaselogger.log( "Record updated sucessfully.", 1 );
		}
		else {
			dbaselogger.log( "Error updating record.", 2 );
		}
	}
	else {
		dbaselogger.log( "Error establishing connection to database.", 3 );
	}
	dbaselogger.log( "update() completed.", 0 );
}

std::string dbase::getLocFromSite( const std::string &site ) {
	dbaselogger.log( "getLocFromSite() called.", 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream lfs;
		lfs<< "SELECT srg_code FROM `" << dbname << "`.`gd_sites` WHERE site_name = " << mysqlpp::quote_only << site << ";";
		std::string loc = lfs.str();
		dbaselogger.log( loc, 1 );
		mysqlpp::Query query = conn.query( loc );
		if (mysqlpp::StoreQueryResult res = query.store() ) {
			std::stringstream res1;
			for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
				mysqlpp::Row row = *it;
				res1 << row[0] << std::endl;
			}
			std::string result = res1.str();
			dbaselogger.log( result, 1 );
			return result;
		} 
		if ( conn.errnum() ) {
			dbaselogger.log( "Error Received in fetching a row.", 2 );
			return "ERR";
		}
		else {
			dbaselogger.log( "Unknown Error Occurred or Unhandled Exception.", 3 );
			return "ERR";
		}
	}
	else {
		dbaselogger.log( "Unable to connect to database!", 4 );
		return "ERR";
	}
	dbaselogger.log( "getLocFromSite() completed.", 0 );
}

void dbase::insert() {
	dbaselogger.log( "insert() called.", 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream iss;
		iss<< "INSERT INTO `" << dbname << "`.`gd_servers` VALUES( ";
		for( int i = 0; i < 13; i++ ) {
			iss<< mysqlpp::quote_only << inval[i] << ", ";
		}
		iss<< mysqlpp::quote_only << "0" << ", NOW() );";
		std::string is = iss.str();
		dbaselogger.log( is, 1 );
		mysqlpp::Query query = conn.query( is );
		mysqlpp::SimpleResult res = query.execute();
		if( res ) {
			dbaselogger.log( "Data inserted successfully.", 1 );
		}
		else {
			dbaselogger.log( "Failed to add server to database.", 2 );
		}
	}
	else {
		dbaselogger.log( "Failed to connect to database. Please ensure mysql is running, and that your credentials are correct.", 3 );
	}
	dbaselogger.log("insert() completed.", 0 );
}

bool dbase::getQueryData( const std::string &Frstate, const std::string &Fstype, const std::string &Fsite, const std::string &Fhost, const std::string &Fatype, const std::string &Fsstate, const std::string &Fanum, const std::string &Fshname, const std::string &Fmac, const int &vfill, const int &dnum ) {
	dbaselogger.log( "getQueryData() called.", 0 );
	/* std::string Frstate = buffer.giveStrVal( buffer.Srstate ); // recovery state
	std::string Fstype = buffer.giveStrVal( buffer.Sstype ); // server type
	std::string Fsite = buffer.giveStrVal( buffer.Ssite ); // site
	std::string Fhost = buffer.giveStrVal( buffer.Shname ); // hostname
	std::string Fatype = buffer.giveStrVal( buffer.Satype ); // asset type
	std::string Fsstate = buffer.giveStrVal( buffer.Sservices ); // service status
	std::string Fanum = buffer.giveStrVal( buffer.Sanum ); // number of arrays */
	std::string Fshorthost = Fshname; // host shortname
	std::string Floc = getLocFromSite( Fsite ); // location, queried from database based on string
	std::string FOrt = "false"; // true/false value for whether there is an open RT for the server. default is false.
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
		storeQueryData( Fstype, 4 );
	}
	int num = 5; // this is  temporary until I write it's hook in the serial class.
	std::stringstream anum1;
	anum1 << num;
	dbaselogger.log( "Retrieving values for storage in database.", 1 );
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
//	std::cout << buffer.giveStrVal( buffer.Shname ) << std::endl;
	dbaselogger.log( "getQueryData() completed.", 0 );
}

void dbase::storeQueryData( const std::string &fsval, const int &posid ) {
	dbaselogger.log( "storeQueryData() called.", 0 );
	inval[posid] = fsval;
}

dbase::dbase() {
	dbaselogger.log( "Initializing Database Module.", 1 );
	dbaselogger.log( "dbase() called.", 0 );
	// initialize the size of the query data storage vector
	inval.resize(16);
	// getQueryData(); // retrieve data to be entered into the database
}

bool dbase::sQuery( const std::string &data, const std::string &table, const std::string &column ) {
	dbaselogger.log( "sQuery() called.", 0 );
	mysqlpp::Connection con( false ); // establish connection to db server
	if ( con.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream qss;
		qss  << "SELECT * FROM `" << table << "`.`" << dbname << "` WHERE " << column << " = ' " << data << "';";
		std::string qs = qss.str();
		dbaselogger.log( qs, 1 );
		mysqlpp::Query query = con.query( qs );
		mysqlpp::StoreQueryResult res = query.store();
		// return true  if the value was found, false if the result set was of size sero.
		if ( res.size() < 1 ) {
			dbaselogger.log( "Value not found in database.", 1 );
			return false;
		}
		else {
			dbaselogger.log( "Value found in database.", 1 );
			return true;
		}
	}
	else {
		dbaselogger.log( "Failed to retrieve rows from database.", 2 );
		return false;
	}
}

