#include<iostream>
#include<string>
#include<sstream>
#include"convert.h"
#include<typeinfo>

template <typename T> static std::string T_to_string( T const &val ) {
	std::ostringstream ostr;
	ostr << val;
	return ostr.str();
}

template <typename T> static T string_to_T( std::string const &val ){
	std::istringstream istr(val);
	T returnVal;
	if ( !( istr >> returnVal ) ) {
		exitWithError("CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n");
	}
	return returnVal;
}

static std::string string_to_T(std::string const &val) {
	return val;

}

void exitWithError( const std::string &error ) {
	std::cout <<error;
	std::cin.ignore();
	std::cin.get();
	exit(EXIT_FAILURE);
}
