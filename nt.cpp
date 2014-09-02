#include <iostream>
#include <string>

using namespace std;

int main() {
	int AbsolutePos = 40;
	for( int i = 0; i < 18; i++ ) {
		cout<< "&& ( ";
		for( int f = 0; f < 2; f++ ) {
			
			for( int rp = 0; rp < 16; rp++ ) {
				AbsolutePos++;
			}
			cout<< "vdata[" << AbsolutePos << "] = 0 && ";
		}
		cout<<" )\n";
	}
}
