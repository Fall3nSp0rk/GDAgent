#include<algorithm>
#include<stdio.h>
#include<iostream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<iomanip>

int main() {
	std::stringstream strm;
	std::string mac;
	char macbit[3];
	char mbits[100];
	std::cout<< "Enter a number: ";
	std::cin>>mac;
	std::cout<<mac << "\n";

	std::stringstream ttest;
	int f=0;
        for(int nMAC = 0; nMAC < 6; nMAC++) {
                std::string fmac = "";
                for( int i = 0; i < 3; i++ ) {
                        macbit[i] = mac[f];
			std::cout<<macbit[i];
                        f++;
                }
                int num = atoi( macbit );
		std::cout<< "num" << num << "\n";
                strm << std::setfill('0') << std::setw(2) << std::hex << num;
                if( nMAC < 5 ) {
                        strm<< ":";
                }
        }
	std::string test = strm.str();
	std::cout<< test;
}
