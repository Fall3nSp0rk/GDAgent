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
#include "util.h"
#include "dbase.h"
#include "mmapper.h"
#include "drive.h"
using std::vector;
const int maxvlen = 612; // Maximum size of the ddrive bit vector
const int vdlen = 16; // constant value. Drive descriptors should always be 16 bytes.

ddrive::ddrive( const int &dnum, const std::string &hsname, const vector<int> &ddata ) {
	GDLogger.log( "ddrive object initialized.", 0 );
	GDLogger.log( "Setting Default Values and reading data into object.", 0 );
	Ddnum = dnum;
	Ddvblen = ddata.size();
	Ddrivebits.resize( Ddvblen ); // apply the calculated ddrive vector size
	Ddcode.resize( vdlen ); // resize the ddrive code segment for the appropriate size
	qdata.resize( 8 );
	Dhsname = hsname;
	Dspnum = 0;
	Dspfill = 0;
	Dpsec = 0;
	Drsec = 0;
	Ddpath = "/dev/sda";
	Dhstatus = "Failed";
	Dspstate = "NO";
	for( int i = 0; i < Ddvblen; i++ ) { // read the ddrive bits in
		Ddrivebits[i] = ddata[i];
	}
	for( int i = 0; i < vdlen; i++ ) { // initialize Ddcode to a default value of 16 0s
		Ddcode[i] = 0;
	}
	GDLogger.log( "Drive Object Initialization Completed.", 0 );
}

// Destructor for ddrive class

bool ddrive::readDriveData() {
	GDLogger.log( "readDriveData() called.", 0 );
	int f = 0;
	vector<bool> validated;
	vector<bool> dbadded;
	vector<bool> rval;
	dbadded.resize( Ddnum );
	validated.resize( Ddnum );
	for( int i = 0; i < Ddnum; i++ ) {
		rval[i] = false;
	}
	for( int i = 0; i < Ddnum; i++ ) {
		for( int j = 0; j < vdlen; j++ ) {
			Ddcode[j] = Ddrivebits[f];
			f++;
		}
		Ddpath = mapp.getKeyFromMap( Ddcode[0], Ddcode[1], 3 );
		Dspnum = readSpoolNumber( Ddcode[2], Ddcode[3] );
		Dhstatus = readDHState( Ddcode[4] );
		Dspfill = read3DigitVals( Ddcode[5], Ddcode[6], Ddcode[7] );
		Dspstate = readSpoolState( Ddcode[8] );
		Dpsec = read3DigitVals( Ddcode[9], Ddcode[10], Ddcode[11] );
		Drsec = read3DigitVals( Ddcode[12], Ddcode[13], Ddcode[14] );
		bool DDValid = true;
		if( DDValid ) {
			validated[i] = true;
			GDLogger.log( "Converting data to strings for database insertion.", 1 ); 
			std::stringstream qq;
			qq << Dspnum;
			std::string qspnum = qq.str();
			GDLogger.log( qspnum, 0 );
			qq.str("");
			qq << Dspfill;
			std::string qspfill = qq.str();
			GDLogger.log( qspfill, 0 );
			qq.str("");
			qq << Dpsec;
			std::string qpsec = qq.str();
			GDLogger.log( qpsec, 0 );
			qq.str("");
			qq << Drsec;
			std::string qrsec = qq.str();
			GDLogger.log( qrsec, 0);
			qq.str("");
			qdata[0] = Dhsname;
			qdata[1] = Ddpath;
			qdata[2] = qspnum;
			qdata[3] = Dhstatus;
			qdata[4] = qspfill;
			qdata[5] = Dspstate;
			qdata[6] = qpsec;
			qdata[7] = qrsec;
			dbadded[i] = thread.runDriveQuery( qdata ); // still need  to write this  function
		}
		else {
			GDLogger.log( "Drive datastream rejected by server.", 3 );
			validated[i] = false;
		}
	}
	bool valid = true;
	bool dbadd = true;
	GDLogger.log( "Validating that operation performed successfully.", 1 );
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
}

int ddrive::readSpoolNumber( const int &snfd, const int &snsd ) {
	char spoolchar[2]; // going to char because atoi accepts char*
	spoolchar[0] = snfd;
	spoolchar[1] = snsd;
	int rval = atoi( spoolchar );
	return rval;
}

int ddrive::read3DigitVals( const int &fd, const int &sd, const int &td ) {
	char fillchar[3];// converting int to char because atoi only accepts char*
	fillchar[0] = fd;
	fillchar[1] = sd;
	fillchar[2] = td;
	int rval = atoi( fillchar );
	return rval;
}

std::string ddrive::readDHState( const int &dhc ) {
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
}

std::string ddrive::readSpoolState( const int &sval ) {
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
}

