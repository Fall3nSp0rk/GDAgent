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
#include "globals.h"
#define _logger dlog
//#include"serial.h"
bool dbase::runDriveQuery( const std::vector<std::string> &qvec ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	bool ex = checkExists( qvec[0], "gd_drives", "host_shortname", "drive_label", qvec[1]);
	std::stringstream qqquery;
	if( ex ) {
		qqquery << "UPDATE `" << dbname << "`.`gd_drives` SET spool_number=" << mysqlpp::quote_only << qvec[2];
		qqquery << ", drive_status=" << mysqlpp::quote_only << qvec[3] <<", spool_status='" << mysqlpp::quote_only;
		qqquery << qvec[5];
		qqquery << "', pending_sectors=" << mysqlpp::quote_only << qvec[6];
		qqquery << ", reallocated_sectors=" << mysqlpp::quote_only << qvec[7];
		qqquery << "WHERE host_shortname=" << mysqlpp::quote_only << qvec[0];
		qqquery << " AND drive_label=" << mysqlpp::quote_only << qvec[1] << ";";
		std::string fquery = qqquery.str();
		qqquery.str("");
		_logger.logstream << "Calling SQL statement '" << fquery << "'.;";
		_logger.log( 1 );
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
		_logger.logstream << "Calling SQL Statement '" << ffquery << "'.";
		_logger.log( 1 );
		insert( ffquery );
		return true;
	}
}


void dbase::runQuery() {
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "RunQuery() called.";
	_logger.log( 0 );
	bool ex = checkExists( inval[1], "gd_servers", "hostname", "0", "0" );
	if ( ex == true ) {
		updateServer();
	}
	else {
		inServer();
	}
	_logger.logstream << "runQuery() completed.";
	_logger.log( 0 );
}
bool dbase::checkExists( const std::string &data, const std::string &table, const std::string &field, const std::string &f2, const std::string &d2 ) { // check if the received data matches anything already in the database
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "checkExists() called.";
	_logger.log( 0 );
	_logger.logstream << "Checking if host already in Database...";
	_logger.log( 1 );
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
		_logger.logstream << "Calling SQL Query '" << qstr << "'.";
		_logger.log( 1 );
		mysqlpp::Query query = con3.query( qstr );
		//std::cout << qstr << std::endl;
		mysqlpp::StoreQueryResult res = query.store();
		std::stringstream res1;
		for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
			mysqlpp::Row row = *it;
			res1 << row[0];
		}
		std::string result = res1.str();
		_logger.logstream >> result;
		_logger.log( 1 );

		if ( result == data ) {
			_logger.logstream << "Host exists in Database.";
			_logger.log( 1 );
			return true;
		}
		else {
			_logger.logstream << "Host does not exist in Database.";
			_logger.log( 1 );
			return false;
		}
	}
	_logger.logstream << "checkExists() completed.";
	_logger.log( 0 );
}

void dbase::updateServer() {
	logger _logger( glob.g_ll, glob.g_logfile );
	std::stringstream uss;
	uss<< " UPDATE `" << dbname << "`.`gd_servers` SET asset_type=" << mysqlpp::quote_only << inval[0] << ", recover_status=" << mysqlpp::quote_only << inval[10] << ", gc_status=" << mysqlpp::quote_only << inval[11] << ", var_fill=" << mysqlpp::quote_only << 0 << ", open_ticket=" << mysqlpp::quote_only << "0" << ", last_report = NOW() WHERE hostname = " << mysqlpp::quote_only << inval[1] << ";";
	std::string upstr = uss.str();
	_logger.logstream << "Calling SQL Query '" << upstr << "'.";
	_logger.log( 1 );
	update( upstr );
}

void dbase::update( const std::string &qquery ) { // update the selected entry in the database
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "update() called.";
	_logger.log( 0 );
	_logger.logstream << "Updating database entry.";
	_logger.log( 1 );
	mysqlpp::Connection con2( false );
	if ( con2.connect( dbname, dbhost, dbuser, dbpass ) ) {
		mysqlpp::Query query = con2.query( qquery );
		mysqlpp::SimpleResult res = query.execute();
		if ( res ) {
			_logger.logstream << "Record updated sucessfully.";
			_logger.log( 1 );
		}
		else {
			_logger.logstream << "Error updating record.";
			_logger.log( 3 );
		}
	}
	else {
		_logger.logstream << "Error establishing connection to database.";
		_logger.log( 3 );
	}
	_logger.logstream << "update() completed.";
	_logger.log( 0 );
}

std::string dbase::getLocFromSite( const std::string &site ) { // use the site that is transmitted to determine what the region is
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "getLocFromSite() called.";
	_logger.log( 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		std::stringstream lfs;
		lfs << "SELECT srg_code FROM `" << dbname << "`.`gd_sites` WHERE site_name = " << mysqlpp::quote_only << site << ";";
		std::string loc = lfs.str();
		_logger.logstream << "SQL Statement '" << loc << "' called.";
		_logger.log( 1 );
		mysqlpp::Query query = conn.query( loc );
		if (mysqlpp::StoreQueryResult res = query.store() ) {
			std::stringstream res1;
			for ( std::vector<mysqlpp::Row>::iterator it= res.begin(); it != res.end(); ++it ) {
				mysqlpp::Row row = *it;
				res1 << row[0] << std::endl;
			}
			std::string result = res1.str();
			_logger.logstream << result;
			_logger.log( 1 );
			return result;
		} 
		if ( conn.errnum() ) {
			_logger.logstream << "Error Received in fetching a row.";
			_logger.log( 3 );
			return "ERR";
		}
		else {
			_logger.logstream << "Unknown Error Occurred or Unhandled Exception.";
			_logger.log( 4 );
			return "ERR";
		}
	}
	else {
		_logger.logstream << "Unable to connect to database!";
		_logger.log( 3 );
		return "ERR";
	}
	_logger.logstream << "getLocFromSite() completed.";
	_logger.log( 0 );
}

void dbase::inServer() {
	logger _logger( glob.g_ll, glob.g_logfile );
	std::stringstream iss;
	iss<< "INSERT INTO `" << dbname << "`.`gd_servers` VALUES( ";
	for( int i = 0; i < 13; i++ ) {
		iss << mysqlpp::quote_only << inval[i] << ", ";
	}
	iss << mysqlpp::quote_only << "0" << ", NOW() );";
	std::string is = iss.str();
	_logger.logstream << is;
	_logger.log( 1 );
	insert( is );
}

void dbase::insert( const std::string &qstring ) { // insert server info into the mysql database (only if not already present)
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "insert() called.";
	_logger.log( 0 );
	mysqlpp::Connection conn( false );
	if ( conn.connect( dbname, dbhost, dbuser, dbpass ) ) {
		mysqlpp::Query query = conn.query( qstring );
		mysqlpp::SimpleResult res = query.execute();
		if( res ) {
			_logger.logstream << "Data inserted successfully.";
			_logger.log( 1 );
		}
		else {
			_logger.logstream << "Failed to add data to database.";
			_logger.log( 3 );
		}
	}
	else {
		_logger.logstream << "Failed to connect to database. Please ensure mysql is running, and that your credentials are correct.";
		_logger.log( 3 );
	}
	_logger.logstream << "insert() completed.";
	_logger.log( 0 );
}

bool dbase::getQueryData( const std::string &Frstate, const std::string &Fstype, const std::string &Fsite, const std::string &Fhost, const std::string &Fatype, const std::string &Fsstate, const std::string &Fanum, const std::string &Fshname, const std::string &Fmac, const int &vfill, const int &dnum ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "getQueryData() called."; // takes all the deserialized data from the serial class and sorts them into containers in preparation of db injection
	_logger.log( 0 );
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
	_logger.logstream <<  "Retrieving values for storage in database.";
	_logger.log( 1 );
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
	_logger.logstream << "getQueryData() completed.";
	_logger.log( 0 );
}

void dbase::storeQueryData( const std::string &fsval, const int &posid ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "storeQueryData() called.";
	_logger.log( 0 );
	inval[posid] = fsval;
}

dbase::dbase() { // overrides default constructor for dbase class
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "Initializing Database Module.";
	_logger.log( 1 );
	_logger.logstream << "dbase() called.";
	_logger.log( 0 );
	// initialize the size of the query data storage vector
	inval.resize(16);
	// getQueryData(); // retrieve data to be entered into the database
}
#undef _logger
