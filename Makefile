CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: server client

server: TCPTransportServer.cpp DTransport.cpp
	$(CXX) $(CXXFLAGS) TCPTransportServer.cpp -o server

client: TCPTransportClient.cpp DTransport.cpp
	$(CXX) $(CXXFLAGS) TCPTransportClient.cpp -o client

clean:
	rm -f server client tcp_res.txt