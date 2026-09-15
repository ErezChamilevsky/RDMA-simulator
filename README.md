# RDMA - simulator

## Intro

The idea of *RDMA*, *Remote Direct Memory Access* is simple - instead of giving the CPU to work on writing files that has been recieved, let other hardware component, *NIC*, do it.
With this method files can be transfered much faster - as can be seen in this [video]([https://www.example.com](https://www.youtube.com/watch?v=TR_nooHyA9Q)).

This simulator is just simulates the idea of it, but without using NIC the differnce between two methods (regular TCP connection, and RDMA) is not so clear. (There is rdma_rxe, where can 
write and test real RDMA applications over standard, non-RDMA Ethernet network cards but I decided to not use it)

## Summary

There are two methods that are been checkd - simple tcp connection, and mmap (that simulates the RDMA since there is no regular internet transfer). The simulate itself is not 
perfect as said above. The results also can reveal it:
[https://raw.githubusercontent.com/ErezChamilevsky/RDMA-simulator/refs/heads/main/results.png]

## Implementation

- Interface of transfering data called DTransport.
- TCPTransport cliend and server, implements the interface.
- RDMATRansport, using mmap, to simulate RDMA.
