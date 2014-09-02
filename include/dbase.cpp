// db.h
// MySQL database connections and DB search/parsing.
// the final step of the data flow - everything ends up in an indexable mysql database in this module.

#include<iostream>
#include<sstream>
#include<string>
#include<mysql++/mysql++.h>
#include<vector>
#include<iomanip>
#include"dbase.h"
#include"log.h"
//#include"serial.h"

bool dbase::runDriveQuery( const std::vector<std::string> &qvec ) {
	bool ex = checkExists( qvec[0], "gd_drives", "host_shortname", "drive_label", qvec[1]);
	std::stringstream qqquery;
	if( ex ) {
		qqquery << "UPDATE `" << dbname << "`.`gd_drives` SET spool_number=" << mysqlpp::quote_only << qvec[2];
		qqquery << ", drive_status=" << mysqlpp::quote_only << qvec[3] <<", spool_status=" <<mysqlpp::quote_only;
		qqquery << ", pending_sectors=" << mysqlpp::quote_only << qvec[6];
		qqquery << ", reallocated_sectors=" << mysqlpp::quote_only << qvec[7];
		qqquery << ";";
		std::string fquery = qqquery.str();
		qqquery.str("");
		GDLogger.log( fquery, 1 );
		update( fquery );
		return true;
	}
	else {
		qqquery << "INSERT INTO `" << dbname << "`.`gd_drives` VALUES ( " << mysqlpp::quote_only << "Hard Drive";
		qqquery << ", " << "'" << qvec[0] << "/" << qvec[1] << "', " << mysqlpp::quote_only << inval[5];
		qqquery << ", " << mysqlpp::quote_only << qvec[0] << ", " << mysqlpp::quote_only << qvec[1];
		qqquery << ", " << mysqlpp::quote_only << "0";
		qqquery << ", " << mysqlpp::quote_only << qvec[2];
		qqquery << ", " << mysqlpp::quote_only << qvec[3];
		qqquery << ", " << mysqlpp::quote_only << qvec[5] << ", " << mysqlpp::quote_only << qvec[4];
		qqquery << ", " << mysqlpp::quote_only << qvec[6] << ", " << mysqlpp::quote_only << qvec[7];
		qqquery << ", " << mysqlpp::quote_only << "0" << ", '1'";
		qqquery << ");";
		std::string ffquery = qqquery.str();
		qqquery.str("");
		GDLogger.log( ffquery, 1 );
		insert( ffquery );
		return true;
	}
}


void dbase::runQuery() {
	GDLogger.log( "runQuery() called.", 0 );
	bool ex = checkExists( inval[1], "gd_servers", "hostname", "0", "0" );
	if ( ex == true ) {
		updateServer();
	}
	else {
		inServer();
	}
	GDLogger.log( "runQuery() completed.", 0 );
}
bool dbase::checkExists( const std::string &data, const std::string &table, const std::string &field, const std::string &f2, const std::string &d2 ) { // check if the received data matches anything already in the database
	GDLogger.log( "checkExists() called.", 0 );
	GDLogger.log ( "Checking if host already in Database...", 1 );
	mysqlpp::Connection con3( false );
	if ( con3.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream queryss;
		queryss<< "SELECT " << field << " FROM `" << dbname << "`.`" << table << "` WHERE " << field << " = " << mysqlpp::quote_only << data;
		if( f2 == "0" ) {
			queryss  << ";";
		}
		else {
			queryss << "AND " << f2 << "=" << mysqlpp::quote_only << d2 << ";";
		}
		std::string qstr = queryss.str();
		GDLogger.log( qstr, 1 );
		mysqlpp::Query query = con3.query( qstr );
		//std::cout << qstr << std::endl;
		mysqlpp::StoreQueryResult res = query.store();
		std::stringstream res1;
		for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
			mysqlpp::Row row = *it;
			res1 << row[0];
		}
		std::string result = res1.str();
		GDLogger.log( result, 1 );

		if ( result == inval[1] ) {
			GDLogger.log( "Host exists in Database.", 1 );
			return true;
		}
		else {
			GDLogger.log( "Host does not exist in Database.", 1 );
			return false;
		}
	}
	GDLogger.log( "checkExists() completed.", 0 );
}

void dbase::updateServer() {
	std::stringstream uss;
	uss<< " UPDATE `" << dbname << "`.`gd_servers` SET asset_type=" << mysqlpp::quote_only << inval[0] << ", recover_status=" << mysqlpp::quote_only << inval[10] << ", gc_status=" << mysqlpp::quote_only << inval[11] << ", var_fill=" << mysqlpp::quote_only << 0 << ", open_ticket=" << mysqlpp::quote_only << "0" << ", last_report = NOW() WHERE hostname = " << mysqlpp::quote_only << inval[1] << ";";
	std::string upstr = uss.str();
	GDLogger.log( upstr, 1 );
	update( upstr );
}

void dbase::update( const std::string &qquery ) { // update the selected entry in the database
	GDLogger.log( "update() called.", 0 );
	GDLogger.log( "Updating database entry.", 1 );
	mysqlpp::Connection con2( false );
	if ( con2.connect( dbname, dbhost, dbuser, dbpass ) ) {
		mysqlpp::Query query = con2.query( qquery );
		mysqlpp::SimpleResult res = query.execute();
		if ( res ) {
			GDLogger.log( "Record updated sucessfully.", 1 );
		}
		else {
			GDLogger.log( "Error updating record.", 2 );
		}
	}
	else {
		GDLogger.log( "Error establishing connection to database.", 3 );
	}
	GDLogger.log( "update() completed.", 0 );
}

std::string dbase::getLocFromSite( const std::string &site ) { // use the site that is transmitted to determine what the region is
	GDLogger.log( "getLocFromSite() called.", 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream lfs;
		lfs<< "SELECT srg_code FROM `" << dbname << "`.`gd_sites` WHERE site_name = " << mysqlpp::quote_only << site << ";";
		std::string loc = lfs.str();
		GDLogger.log( loc, 1 );
		mysqlpp::Query query = conn.query( loc );
		if (mysqlpp::StoreQueryResult res = query.store() ) {
			std::stringstream res1;
			for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
				mysqlpp::Row row = *it;
				res1 << row[0] << std::endl;
			}
			std::string result = res1.str();
			GDLogger.log( result, 1 );
			return result;
		} 
		if ( conn.errnum() ) {
			GDLogger.log( "Error Received in fetching a row.", 2 );
			return "ERR";
		}
		else {
			GDLogger.log( "Unknown Error Occurred or Unhandled Exception.", 3 );
			return "ERR";
		}
	}
	else {
		GDLogger.log( "Unable to connect to database!", 4 );
		return "ERR";
	}
	GDLogger.log( "getLocFromSite() completed.", 0 );
}

void dbase::inServer() {

	std::stringstream iss;
	iss<< "INSERT INTO `" << dbname << "`.`gd_servers` VALUES( ";
	for( int i = 0; i < 13; i++ ) {
		iss << mysqlpp::quote_only << inval[i] << ", ";
	}
	iss << mysqlpp::quote_only << "0" << ", NOW() );";
	std::string is = iss.str();
	GDLogger.log( is, 1 );
	insert( is );
}

void dbase::insert( const std::string &qstring ) { // insert server info into the mysql database (only if not already present)
	GDLogger.log( "insert() called.", 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		mysqlpp::Query query = conn.query( qstring );
		mysqlpp::SimpleResult res = query.execute();
		if( res ) {
			GDLogger.log( "Data inserted successfully.", 1 );
		}
		else {
			GDLogger.log( "Failed to add data to database.", 2 );
		}
	}
	else {
		GDLogger.log( "Failed to connect to database. Please ensure mysql is running, and that your credentials are correct.", 3 );
	}
	GDLogger.log("insert() completed.", 0 );
}

bool dbase::getQueryData( const std::string &Frstate, const std::string &Fstype, const std::string &Fsite, const std::string &Fhost, const std::string &Fatype, const std::string &Fsstate, const std::string &Fanum, const std::string &Fshname, const std::string &Fmac, const int &vfill, const int &dnum ) {
	GDLogger.log( "getQueryData() called.", 0 ); // takes all the deserialized data from the serial class and sorts them into containers in preparation of db injection
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
	GDLogger.log( "Retrieving values for storage in database.", 1 );
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
	GDLogger.log( "getQueryData() completed.", 0 );
}

void dbase::storeQueryData( const std::string &fsval, const int &posid ) {
	GDLogger.log( "storeQueryData() called.", 0 );
	inval[posid] = fsval;
}

dbase::dbase() { // overrides default constructor for dbase class
	GDLogger.log( "Initializing Database Module.", 1 );
	GDLogger.log( "dbase() called.", 0 );
	// initialize the size of the query data storage vector
	inval.resize(16);
	// getQueryData(); // retrieve data to be entered into the database
}
dbase thread;
