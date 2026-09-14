CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: server client rdma

server: TCPTransportServer.cpp DTransport.cpp
	$(CXX) $(CXXFLAGS) TCPTransportServer.cpp -o server

client: TCPTransportClient.cpp DTransport.cpp
	$(CXX) $(CXXFLAGS) TCPTransportClient.cpp -o client

rdma: RDMATransport.cpp DTransport.cpp
	$(CXX) $(CXXFLAGS) RDMATransport.cpp -o rdma

clean:
	rm -f server client tcp_res.txt