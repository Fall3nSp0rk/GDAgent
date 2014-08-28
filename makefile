CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++0x -Wall -Wextra -g -I/usr/include/mysql++ -I/usr/include/mysql -I/usr/include/ -I/usr/include/boost_1_56_0/ -I/home/ianc/www/coding/GDAgent/include/

LDFLAGS=-g -L/usr/lib/mysql/ -L/usr/lib/ -L/obj/ -L/usr/lib64/ -L/usr/include/boost_1_56_0/stage/lib/
LDLIBS=-lmysqlpp -lnsl -lz -lm -lboost_thread -lboost_system
OBJS=obj/$(asio.o, )

all: obj/serial.o obj/main.o obj/dbase.o obj/log.o obj/mmapper.o obj/drive.o
	g++ $(CPPFLAGS) -o GDAgent obj/serial.o obj/dbase.o obj/main.o obj/log.o obj/mmapper.o obj/drive.o $(LDFLAGS) $(LDLIBS)

obj/main.o: main.cpp include/serial.h  include/dbase.h include/log.h include/ConfigFile.h
	g++ --std=c++0x -c main.cpp -o obj/main.o -I/usr/include/boost_1_56_0/ -I/usr/include/mysql -I/usr/include/mysql++/ -I/usr/include/ -lmysqlpp -lnsl -lz -lm -L/usr/lib64/ -L/usr/lib/mysql/ -L/usr/lib64/ -L/usr/include/boost_1_56_0/stage/lib/ -lboost_thread -lboost_system

obj/dbase.o: include/dbase.cpp include/dbase.h include/serial.h include/log.h
	g++ --std=c++0x -o obj/dbase.o -c include/dbase.cpp -g -I/usr/include/mysql++ -I/usr/include/mysql -I/usr/lib/boost_1_47_0/ -lmysqlpp -lnsl -lz -lm -lboost_thread -L/usr/lib/ -L/usr/lib/mysql/

obj/serial.o: include/serial.cpp include/serial.h include/dbase.h
	g++ $(CPPFLAGS) -o obj/serial.o  -c include/serial.cpp $(LDFLAGS) $(LDLIBS)

obj/config.o: include/ConfigFile.cpp include/ConfigFile.h obj/convert.o
	g++ --std=c++0x -c include/ConfigFile.cpp -o obj/config.o

obj/convert.o: include/convert.cpp include/convert.h
	g++ --std=c++0x -c include/convert.cpp -o obj/convert.o

obj/log.o: include/log.cpp include/log.h 
	g++ --std=c++0x -c include/log.cpp -o obj/log.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

obj/mmapper.o: include/mmapper.cpp include/mmapper.h
	g++ --std=c++0x -c include/mmapper.cpp -o obj/mmapper.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

obj/drive.o: include/drive.cpp include/drive.h
	g++ --std=c++0x -c include/drive.cpp -o obj/drive.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)
