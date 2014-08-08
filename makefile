CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++0x -g $(shell root-config --cflags) -I/usr/include/mysql -I/usr/lib/boost_1_47_0/
LDFLAGS=-g $(shell root-config --ldflags) -lmysqlpp -lnsl -lz -lm -lboost_thread -lboost_system
LDLIBS=$(shell root-config --libs) -L/usr/lib/ -L/usr/lib/mysql -I/usr/lib/boost_1_47_0/stage/lib/ 
OBJS=obj/$(subst .o)

all: GDAgent

GDAgent: $(OBJS)
	g++ $(CPPFLAGS) $(LDFLAGS) -o GDAgent $(OBJS) $(LDLIBS)

dbase.o: include/dbase.cpp include/dbase.h include/serial.h
	g++ $(CPPFLAGS $(LDFLAGS) -o dbase.o $(LDLIBS)

serial.o: include/serial.cpp include/serial.h include/asio.h include/dbase.h
	g++ $(CPPFLAGS) $(LDFLAGS) -o serial.o $(LDLIBS)

asio.o: include/asio.cpp include/asio.h include/serial.h include/dbase.h
	g++ $(CPPFLAGS) $(LDFLAGS) -o asio.o $(LDLIBS)
