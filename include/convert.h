// convert class
#include<iostream>
#include<string>
#include<sstream>
#include<typeinfo>

class Convert {
	public:
	template <typename T> static std::string T_to_string( T const &val );
	template <typename T> static T string_to_T( std::string const &val );
	static std::string string_to_T( std::string const &val );
};

void exitWithError( const std::string &error );

template <typename T> std::string Convert::T_to_string( T const &val ) {
	std::ostringstream ostr;
	ostr << val;
	return ostr.str();
}

template <typename T> T Convert::string_to_T( std::string const &val ){
	std::istringstream istr(val);
	T returnVal;
	if ( !( istr >> returnVal ) ) {
		exitWithError("CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n");
	}
	return returnVal;
}

std::string Convert::string_to_T(std::string const &val) {
	return val;

}

void exitWithError( const std::string &error ) {
	std::cout <<error;
	std::cin.ignore();
	std::cin.get();
	exit(EXIT_FAILURE);
}
