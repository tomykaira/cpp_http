CXXFLAGS=-I/usr/local/include -std=gnu++11

all: net-cpp-sync net-cpp-async microhttpd

net-cpp-sync: net-cpp-sync.o worker.o
	g++ $^ -o $@ -L/usr/local/lib -lhiredis -lboost_system

net-cpp-async: net-cpp-async.o worker.o
	g++ $^ -o $@ -L/usr/local/lib -lhiredis -lboost_system -lpthread -lboost_thread -lcppnetlib-server-parsers

microhttpd: microhttpd.o worker.o
	g++ $^ -o $@ -lhiredis -lmicrohttpd -lboost_system -lboost_thread

worker.o: worker.cpp worker.hpp
