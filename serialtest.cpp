// serialtest.cpp
// Testing the deserialization functions 

#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include "read.h"
#include "db.h"

int netbuffer[1024] = {0,0,0,0,3,1,1,0,1,0,0,2,0,1,0,0,1,5,9,5,9,3,5,3,2,1,4,4,8,5,7,8,8,0,3,3,7,8,0,4,0,6,9,5,7,0,2,0,6,5,5,5,6,3,1,3,9,4,1,4,5,5,4,9,4,0,7,6,4,8,6,4,7,9,6,6,3,1,6,0,2,5,2,5,2,3,8,6,5,4,8,7,1,6,0,2,1,3,4,6,1,1,3,5,4,9,2,6,0,3,2,0,6,8,8,8,9,6,5,6,2,4,4,7,8,9,7,2,5,3,0,7,7,1,7,8,0,2,6,9,6,4,9,3,3,8,5,8,5,6,1,6,8,7,1,0,5,9,4,2,1,4,7,0,4,1,3,8,7,2,3,9,3,4,6,3,0,1,1,1,2,7,8,9,5,8,7,0,1,2,0,5,8,6,3,0,0,4,0,1,3,3,9,2,7,7,2,4,6,2,5,1,6,9,4,9,2,0,4,1,5,2,4,5,4,7,0,3,4,5,6,1,0,6,6,2,3,7,9,7,3,2,2,2,3,0,1,2,6,1,1,4,2,2,1,5,1,7,7,3,1,5,3,1,4,6,3,2,0,0,5,7,9,5,9,5,9,3,5,3,2,1,4,4,8,5,7,8,8,0,3,3,7,8,0,4,0,6,9,5,7,0,2,0,6,5,5,5,6,3,1,3,9,4,1,4,5,5,4,9,4,0,7,6,4,8,6,4,7,9,6,6,3,1,6,0,2,5,2,5,2,3,8,6,5,4,8,7,1,6,0,2,1,3,4,6,1,1,3,5,4,9,2,6,0,3,2,0,6,8,8,8,9,6,5,6,2,4,4,7,8,9,7,2,5,3,0,7,7,1,7,8,0,2,6,9,6,4,9,3,3,8,5,8,5,6,1,6,8,7,1,4,9,6,9,0,1,9,4,9,5,2,1,9,3,1,0,2,4,1,1,8,6,1,5,8,0,7,0,8,8,3,1,0,9,3,2,7,1,6,7,6,8,9,2,6,6,8,0,9,4,7,1,5,9,8,6,2,3,3,3,4,1,0,1,7,0,9,3,3,4,2,7,0,8,5,2,2,6,2,3,5,7,1,4,2,9,4,8,8,4,0,3,1,1,6,1,4,2,6,5,0,4,9,7,6,9,2,5,9,7,8,3,2,8,8,4,8,9,1,9,5,7,7,6,7,2,3,4,0,3,3,6,6,7,5,0,3,2,9,5,6,9,5,6,9,8,5,7,2,2,7,9,1,7,1,7,1,5,0,5,7,6,9,6,0,7,9,0,0,2,5,8,4,4,6,1,0,5,6,2,3,7,9,7,3,2,2,1,2,0,1,2,5,1,0,4,2,2,1,5,1,7,6,3,1,5,3,1,4,6,2,2,0,0,5,7,9,5,8,4,9,2,5,3,2,1,4,4,8,5,7,8,7,0,3,3,7,8,9,4,0,6,9,5,6,0,2,0,6,5,5,5,6,3,1,3,9,4,1,4,5,5,4,8,4,0,7,6,4,8,6,4,7,9,6,6,3,0,6,0,2,5,2,5,2,3,8,6,5,4,8,7,1,6,0,1,1,3,4,6,0,1,3,4,3,9,2,6,0,3,2,0,6,8,8,8,9,6,5,6,2,4,4,7,8,8,7,2,5,3,0,7,7,1,7,7,0,2,6,9,6,4,9,3,3,8,5,8,5,6,1,6,8,7,1,0,4,9,3,2,1,3,7,0,4,1,3,8,7,2,3,9,3,4,5,3,9,1,1,1,2,6,8,9,5,7,7,0,1,2,0,5,8,6,3,0,0,4,0,1,3,3,9,2,7,7,2,4,6,1,5,1,6,9,4,9,2,0,4,1,5,2,4,5,4,7,0,3,4,4,6,1,0,5,6,2,3,7,9,7,3,2,2,1,2,0,1,2,5,1,0,4,2,2,1,5,1,7,6,3,1,5,3,1,4,6,2,2,0,0,5,7,9,5,8,4,9,2,5,3,2,1,4,4,8,5,7,8,7,0,3,3,7,8,9,4,0,6,9,5,6,0,2,0,6,5,5,5,6,3,1,3,9,4,1,4,5,5,4,8,4,0,7,6,4,8,6,4,7,9,6,6,3,0,6,0,2,5,2,5,2,3,8,6,5,4,8,7,1,6,0,1,1,3,4,6,0,1,3,4,3,9,2,6,0,3,2,0,6,8,8,8,9,6,5,6,2,4,4,7,8,8,7,2,5,3,0,7,7,1,7,7,0,2,6,9,6,4,9,3,3,8,5,8,5,6,1,6,8,7,1,0,4 };

int main() {
	std::cout<<"Instantiating object serial buffer..." << std::endl;
	serial buffer;
	std::cout<< "Reading buffer..." << std::endl;
	buffer.readBits( netbuffer );
	std::cout<< "Deserializing..." << std::endl;
	buffer.deSerialize();
	std::cout<< "Reading Data..." << std::endl;
	std::string rstate = buffer.giveStrVal( buffer.Srstate );
	std::string site = buffer.giveStrVal( buffer.Ssite );
	std::string stype = buffer.giveStrVal( buffer.Sstype );
	std::string hname = buffer.giveStrVal( buffer.Shname );
	std::string mac = buffer.giveStrVal( buffer.Smac );
	std::string atype = buffer.giveStrVal( buffer.Satype );
	std::string services = buffer.giveStrVal( buffer.Sservices );
	int dnum = buffer.giveIntVal( buffer.Sdnum );
	std::cout<< "Printing Data:" << std::endl;
	std::cout<< "Recover State: " << rstate << std::endl;
	std::cout<< "Site: " << site << std::endl;
	std::cout<< "Server Type: " << stype << std::endl;
	std::cout<< "Hostname: " << hname << std::endl;
	std::cout<< "Mac Address: " << mac << std::endl;
	std::cout<< "Asset Type: " << atype << std::endl;
	std::cout<< "GC State: " << services << std::endl;
	std::cout<<"Number of Hard Drives: " << dnum << std::endl;
	dbase thread;
	thread.getQueryData( buffer.Srstate, buffer.Sstype, buffer.Ssite, buffer.Shname, buffer.Satype, buffer.Sservices, buffer.Sanum, buffer.Svarfill, buffer.Sdnum ); 
	thread.runQuery();
	char testchar;
	std::cin>> testchar;
}
