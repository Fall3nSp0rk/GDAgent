CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++0x -g $(shell root-config --cflags) -I/usr/include/mysql -I/usr/lib/boost_1_47_0/
LDFLAGS=-g $(shell root-config --ldflags) -lmysqlpp -lnsl -lz -lm -lboost_thread -lboost_system
LDLIBS=$(shell root-config --libs) -L/usr/lib/ -L/usr/lib/mysql -I/usr/lib/boost_1_47_0/stage/lib/ 
OBJS=obj/$(asio.o, )

all: main.o serial.o dbase.o
	g++ --std=c++0x -Wall -Wextra obj/main.o obj/dbase.o obj/serial.o -o GDAgent -g -L/obj -L/usr/lib/ -L/usr/lib/mysql/ -I/usr/include/mysql -lmysqlpp -lm -lnsl -lz -I/usr/include/mysql++/ -lmysqlpp -L/usr/lib/boost_1_47_0/ -lboost_thread-mt  -L/usr/lib/boost_1_47_0/ -lboost_system-mt

main.o: include/serial.h  include/dbase.h
	g++ --std=c++0x main.cpp -o GDAgent obj/serial.o obj/dbase.o obj/log.o -g -I/usr/include/mysql -I/usr/include/mysql++/ -I/usr/lib/boost_1_47_0/ -lmysqlpp -lnsl -lz -lm -L/usr/lib64/ -L/usr/lib/mysql/ -L/usr/lib64/ -lboost_thread -lboost_system

dbase.o: include/dbase.cpp include/dbase.h include/serial.h include/log.h
	g++ --std=c++0x -o obj/dbase.o -c include/dbase.cpp -g -I/usr/include/mysql++ -I/usr/include/mysql -I/usr/lib/boost_1_47_0/ -lmysqlpp -lnsl -lz -lm -lboost_thread -L/usr/lib/ -L/usr/lib/mysql/ -L/usr/lib/boost_1_47_0/stage/lib/ -lboost_system-mt

serial.o: include/serial.cpp include/serial.h include/dbase.h
	g++ --std=c++0x  -o obj/serial.o  -c include/serial.cpp -g -I/usr/include/mysql -I/usr/lib/boost_1_47_0/ -lmysqlpp -lnsl -lz -lm  -L/usr/lib/ -L/usr/lib/mysql/ -L/usr/lib/boost_1_47_0/stage/lib/  -lboost_thread -lboost_system

log.o: include/log.cpp include/log.h
	g++ --std=c++0x -c include/log.cpp -o obj/log.o
