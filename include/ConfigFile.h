// ConfigFile.h
// header file for ConfigFile.cpp
// from http://www.dreamincode.net/forums/topic/183191-create-a-simple-configuration-file-parser/

#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<fstream>
#include"convert.h"
class cfg{
	public:
		cfg( const std::string &fName );
		bool keyExists( const std::string &key ) const;
		template <typename ValueType> ValueType getValueOfKey( const std::string &key, ValueType const &defaultValue = ValueType() ) const;
	private:
		bool validateLine( const std::string &line ) const;
		void extractKey( std::string &key, size_t const &sepPos, const std::string &line ) const;
		void extractValue( std::string &value, size_t const &sepPos, const std::string &line ) const;
		void extractContents( const std::string &line );
		void removeComment( std::string &line ) const;
		void parseLine( const std::string &line, size_t const lineNo );
		void extractKeys();
		bool onlyWhitespace( const std::string &line ) const;
		std::map<std::string, std::string> contents;
		std::string fName;
};
// configfile.cpp
// from http://www.dreamincode.net/forums/topic/183191-create-a-simple-configuration-file-parser/
void cfg::removeComment( std::string &line ) const {
	if( line.find('#') != line.npos ) { // if line has a "#" in it, erase everything from that to the end of the line
		line.erase( line.find('#') );
	}
}

bool cfg::onlyWhitespace( const std::string &line ) const {
	return ( line.find_first_not_of( ' ' ) == line.npos ); // returns false if a non-whitespace character is found.
}

bool cfg::validateLine( const std::string &line ) const{
	std::string tt = line;
	tt.erase( 0, tt.find_first_not_of( "\t ") );
	if( tt[0] == '=' ){
		return false;
	}
	for( size_t i = tt.find('=') + 1; i < tt.length(); i++ ){
		if( tt[i] != ' ' ) {
			return true;
		}
	}
	return false;
}

void cfg::extractKey( std::string &key, size_t const &sepPos, const std::string &line ) const {
	key = line.substr(0, sepPos);
	if( key.find('\t') != line.npos || key.find(' ') != line.npos ) {
		key.erase( key.find_first_not_of( "\t " ) );
	}
}

void cfg::extractValue( std::string &value, size_t const &sepPos, const std::string &line ) const {
	value = line.substr( sepPos + 1 );
	value.erase( 0, value.find_first_not_of( "\t " ) );
	value.erase( value.find_last_not_of( "\t " ) + 1 );
}

void cfg::extractContents( const std::string &line ) {
	std::string tt = line;
	//erase leading whitespace from line
	tt.erase( 0, tt.find_first_not_of( "\t ") );
	size_t sepPos = tt.find( '=' );
	std::string key, value;
	extractKey( key, sepPos, tt );
	extractValue( value, sepPos, tt );
	if( !keyExists( key ) ) {
		contents.insert(std::pair<std::string, std::string>(key, value));
	}
	else {
		exitWithError( "CFG: can only have unique key names!\n" );
	}	
}

void cfg::parseLine( const std::string &line, size_t const lineNo ) {
	if ( line.find( '=' ) == line.npos ) {
		exitWithError( "CFG: Couldn't find separator on line: " + Convert::T_to_string( lineNo ) + "\n" );
	}
	if( !validateLine( line ) ) {
		exitWithError( "CFG: Bad format for line: \n" );
	}
	extractContents( line );
}

void cfg::extractKeys() {
	std::ifstream file;
	file.open( fName.c_str() );
	if( !file ) {
		exitWithError( "CFG: File " + fName + " couldn't be found!\n" );
	}
	std::string line;
	size_t lineNo=0;
	while ( std::getline( file, line ) ) {
		lineNo++;
		std::string tt = line;
		if( tt.empty() ) {
			continue;
		}
		removeComment( tt );
		if( onlyWhitespace( tt ) ) {
			continue;
		}
		parseLine( tt, lineNo );
	}
	file.close();
}

cfg::cfg( const std::string &fName ) {
	this->fName = fName;
	extractKeys();
}

bool cfg::keyExists( const std::string &key ) const {
	return contents.find( key ) != contents.end();
}

template <typename ValueType> ValueType cfg::getValueOfKey( const std::string &key, ValueType const &defaultValue ) const{
	if( !keyExists( key ) ) {
		return defaultValue;
	}
	return Convert::string_to_T<ValueType> ( contents.find( key ) -> second );
}
