CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++0x -Wall -Wextra -g -I/usr/include/mysql++ -I/usr/include/mysql -I/usr/include/ -I/usr/include/boost_1_56_0/ -I/home/ianc/www/coding/GDAgent/include/

LDFLAGS=-g -L/usr/lib/mysql/ -L/usr/lib/ -L/obj/ -L/usr/lib64/ -L/usr/include/boost_1_56_0/stage/lib/
LDLIBS=-lmysqlpp -lnsl -lz -lm -lboost_thread -lboost_system
OBJS=obj/$(asio.o, )

all: serial main dbase log mmapper drive globals pool
	g++ $(CPPFLAGS) -o GDAgent obj/pool.o obj/log.o obj/mmapper.o obj/main.o obj/serial.o obj/dbase.o obj/drive.o obj/globals.o $(LDFLAGS) $(LDLIBS)

main: main.cpp include/serial.h  include/dbase.h include/log.h include/ConfigFile.h include/globals.h
	g++ --std=c++0x -c main.cpp -o obj/main.o -I/usr/include/boost_1_56_0/ -I/usr/include/mysql -I/usr/include/mysql++/ -I/usr/include/ -lmysqlpp -lnsl -lz -lm -L/usr/lib64/ -L/usr/lib/mysql/ -L/usr/lib64/ -L/usr/include/boost_1_56_0/stage/lib/ -lboost_thread -lboost_system

dbase: include/dbase.cpp include/dbase.h include/serial.h include/log.h include/globals.h
	g++ --std=c++0x -o obj/dbase.o -c include/dbase.cpp -g $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

serial: include/serial.cpp include/serial.h include/dbase.h include/globals.h
	g++ $(CPPFLAGS) -o obj/serial.o  -c include/serial.cpp $(LDFLAGS) $(LDLIBS)

config: include/ConfigFile.cpp include/ConfigFile.h obj/convert.o
	g++ --std=c++0x -c include/ConfigFile.cpp -o obj/config.o

convert: include/convert.cpp include/convert.h
	g++ --std=c++0x -c include/convert.cpp -o obj/convert.o

log: include/log.cpp include/log.h include/convert.h 
	g++ --std=c++0x -c include/log.cpp -o obj/log.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

mmapper: include/mmapper.cpp include/mmapper.h include/globals.h
	g++ --std=c++0x -c include/mmapper.cpp -o obj/mmapper.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

drive: include/drive.cpp include/drive.h include/globals.h
	g++ --std=c++0x -c include/drive.cpp -o obj/drive.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

globals:
	g++ --std=c++0x -c include/globals.cpp -o obj/globals.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

pool: include/pool.cpp include/pool.h
	g++ $(CPPFLAGS) -c include/pool.cpp -o obj/pool.o
