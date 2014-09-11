// Drive.cpp
// Methods and definitions for the ddrive class
// subclass of Serial
//

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "log.h"
#include <iomanip>
#include <iterator>
#include <exception>
#include <stdlib.h>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include "util.h"
#include "dbase.h"
#include "mmapper.h"
#include "drive.h"
#include "globals.h"
#include "pool.h"
#define _logger ddlog
using std::vector;
const int maxlen = 576; // Maximum size of the ddrive bit vector
const int vdlen = 16; // constant value. Drive descriptors should always be 16 bytes.

ddrive::ddrive( int dnum, std::string hsname, vector<int> ddata, const std::string &fsite ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "ddrive object initialized.";
	_logger.log( 0 );
	_logger.logstream << "Setting Default Values and reading data into object.";
	_logger.log( 0 );
	Dsite = fsite;
	Ddnum = dnum;
	Ddvblen = ddata.size();
	qdata.resize( 9 );
	Dhsname = hsname;
	Dspnum = 0;
	Dspfill = 0;
	Dpsec = 0;
	Drsec = 0;
	Ddpath = "/dev/sda";
	Dhstatus = "Failed";
	Dspstate = "NO";
	_logger.logstream << "Drive Data bits: ";
	for( unsigned int i = 0; i < ddata.size(); i++ ) { // read the ddrive bits in
		Ddrivebits.push_back( ddata[i] );
		_logger.logstream << Ddrivebits[i];
	}
	_logger.log(  0 );
	_logger.logstream << "Drive Object Initialization Completed.";
	_logger.log( 0 );
}

// Destructor for ddrive class

bool ddrive::readDriveData( const vector<int> &data1, dbase db ) {
	mmapper::mmapper mapp;
	logger _logger( glob.g_ll, glob.g_logfile );
	_logger.logstream << "readDriveData() called.";
	_logger.log( 0 );
	int SequenceNumber = 0;
	vector<bool> validated;
	vector<bool> dbadded;
	vector<bool> rval;
	_logger.logstream << "Resizing vectors.";
	_logger.log( 0 );
	for( int i = 0; i < Ddnum; i++ ) {
		_logger.logstream << "Setting rval to false...";
		_logger.log( 0 );
		rval.push_back( false );
	}
	Ddcode.resize( vdlen );
	dbadded.resize( Ddnum );
	for( int i = 0; i < Ddnum; i++ ) {
		for( int j = 0; j < vdlen; j++ ) {
			Ddcode[j] = data1[SequenceNumber];
			_logger.logstream << "SN: "<< SequenceNumber << " " << Ddcode[j];
			_logger.log( 0 );
			SequenceNumber++;
		}
		_logger.logstream << "Sequence Number: " << SequenceNumber;
		_logger.log( 0 );
		Ddpath = mapp.getKeyFromMap( Ddcode[0], Ddcode[1], 3 );
		Dspnum = readSpoolNumber( Ddcode[2], Ddcode[3] );
		Dhstatus = readDHState( Ddcode[4] );
		Dspfill = read3DigitVals( Ddcode[5], Ddcode[6], Ddcode[7] );
		Dspstate = readSpoolState( Ddcode[8] );
		Dpsec = read3DigitVals( Ddcode[9], Ddcode[10], Ddcode[11] );
		Drsec = read3DigitVals( Ddcode[12], Ddcode[13], Ddcode[14] );
		bool DDValid = true;
		if( DDValid ) {
			validated.push_back( true );
			_logger.logstream << "Converting data to strings for database insertion.";
			_logger.log( 1 ); 
			std::stringstream qq;
			qq << Ddcode[2] << Ddcode[3];
			std::string qspnum = qq.str();
			_logger.logstream << "Drive spool number: " << qspnum;
			_logger.log(  0 );
			qq.str("");
			qq << Dspfill;
			std::string qspfill = qq.str();
			_logger.logstream << "Drive spool fill percentage: " << qspfill;
			_logger.log( 0 );
			qq.str("");
			qq << Dpsec;
			std::string qpsec = qq.str();
			_logger.logstream << "Drive Pending Sectors: " << qpsec;
			_logger.log( 0 );
			qq.str("");
			qq << Drsec;
			std::string qrsec = qq.str();
			_logger.logstream << "Drive Reallocated Sectors: " << qrsec;
			_logger.log( 0 );
			qq.str("");
			_logger.logstream << "Drive Host Shortname: " << Dhsname;
			_logger.log( 0 );
			qdata[0] = Dhsname;
			qdata[1] = Ddpath;
			qdata[2] = qspnum;
			qdata[3] = Dhstatus;
			qdata[4] = qspfill;
			qdata[5] = Dspstate;
			qdata[6] = qpsec;
			qdata[7] = qrsec;
			qdata[8] = Dsite;
			_logger.logstream << "Drive full host/path: " << Dhsname << "/" << Ddpath;
			_logger.log( 0 );
			dbpool.run_task( boost::bind( ttask::dbdrivetask, qdata ) );
		}
		else {
			_logger.logstream << "Drive datastream rejected by server.";
			_logger.log( 3 );
			validated[i] = false;
		}
	}
	bool valid = true;
	bool dbadd = true;
	_logger.logstream << "Validating that operation performed successfully.";
	_logger.log( 1 );
	for( int i = 0; i < Ddnum; i++ ) {
		if( !validated[i] ) {
			valid = false;
		}
		if( !dbadded[i] ) {
			dbadd = false;
		}
	}
	if( dbadd && valid ) {
		return true;
	}
	return false;
}

int ddrive::readSpoolNumber( const int &snfd, const int &snsd ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	char spoolchar[2]; // going to char because atoi accepts char*
	spoolchar[0] = snfd;
	spoolchar[1] = snsd;
	int rval = atoi( spoolchar );
	return rval;
}

int ddrive::read3DigitVals( const int &fd, const int &sd, const int &td ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	char fillchar[3];// converting int to char because atoi only accepts char*
	fillchar[0] = fd;
	fillchar[1] = sd;
	fillchar[2] = td;
	int rval = atoi( fillchar );
	return rval;
}

std::string ddrive::readDHState( const int &dhc ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	switch( dhc ) {
		case 0:
			return "Failed";
			break;
		case 1:
			return "Healthy";
			break;
		case 2:
			return "Degraded";
			break;
		case 3:
			return "Critical";
			break;
		default:
			return "ERR";
			break;
	}
	return "ERR";
}

std::string ddrive::readSpoolState( const int &sval ) {
	logger _logger( glob.g_ll, glob.g_logfile );
	switch( sval ) {
		case 0:
			return "OFF";
			break;
		case 1:
			return "RW";
			break;
		case 2:
			return "NO/ERROR";
			break;
		case 3:
			return "RO";
			break;
		case 4:
			return "NO";
			break;
		default:
			return "ERR";
			break;
	}
	return "ERR";
}
#undef _logger
