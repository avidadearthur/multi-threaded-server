# Multi-Threaded Server
Project done for the Operating Systems class @ Group T - Faculty of Engineering Technology / academic year 2022-23

## Project Overview
The sensor monitoring system simulates temperature measurements using client-side sensor nodes. These nodes are responsible for measuring room temperature, 
while a central server-side sensor gateway accepts incoming network connections from multiple client devices to upload the sensor data. 
The connection manager handles these incoming connections and stores the data in a shared data structure. 
The data manager then processes the incoming data, comparing it with a running average for each room. The storage manager takes care of persisting 
the data into a comma-separated file (csv file). Additionally, an accompanying logging process tracks important events and errors, recording them in a log file. The diagram below illustrates the complete system architecture.

<p align="center">
  <img width="543" alt="image" src="https://user-images.githubusercontent.com/100540403/222375089-df535bc0-0ca3-4338-a7a8-5e4f5ec2fef0.png">
  <br>
  <em>Figure 1: Project block diagram</em>
</p>

## Shared Data Structure - Generic Doubly Linked List
Dynamic data structures such as linked lists are essential for operating systems and
software development. These data structures change at runtime in terms of size, and
we often don’t have any information on maximum size up front. The first step of this project was to write an implementation of a Generic Doubly linked list.

<p align="center">
  <img width="412" alt="image" src="https://github.com/avidadearthur/multi-threaded-server/assets/42184405/6598a341-a9c0-42b8-bdd4-a248e1033c6a">
  <br>
  <em>Figure 2: Generic Double-Linked List diagram</em>
</p>

##  Data Manager - File I/O

<p align="center">
  <img width="268" alt="image" src="https://github.com/avidadearthur/multi-threaded-server/assets/42184405/e216942d-0df1-4304-b54f-f00f0e8a09b5">
  <br>
  <em>Figure 3: Data manager</em>
</p>

## Storage Manager & Event Logging - Inter-process communication

