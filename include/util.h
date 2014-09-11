#ifndef __UTIL_H
#define __UTIL_H
#include<iostream>
#include<algorithm>
#include<string.h>
#include<stdio.h>
inline int ccInt( const char c ) {
        switch (c) {
        case '0':
                return 0;
        case '1':
                return 1;
        case '2':
                return 2;
        case '3':
                return 3;
        case '4':
                return 4;
        case '5':
                return 5;
        case '6':
                return 6;
        case '7':
                return 7;
        case '8':
                return 8;
        case '9':
                return 9;
        default:
                return 0;
        }
}

using namespace std;
inline char *Shuffle( char *dest, const char *src ) {
	return ( char * ) memmove( dest, src, strlen(src) + sizeof(char));
}

inline bool IsNumeric( const char c ) {
	return isdigit(c) || c == '-' || c == '.';
}

inline void delNonNumericChar( char *s ) {
	for ( char *p = s + strlen(s) - 1; p >= s; p-- ) {
		if( !IsNumeric(*p) ) {
			Shuffle(p, p+1);
		}
	}
}
#endif
