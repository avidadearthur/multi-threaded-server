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
  <img width="413" alt="image" src="https://github.com/avidadearthur/multi-threaded-server/assets/42184405/92eb83a7-1ab0-4306-ae28-4a4a39048ceb">
  <br>
  <em>Figure 2: Generic Double-Linked List diagram</em>
</p>

##  Data Manager - File I/O
Before handling sensor data, the data manager should first read a file called
'room_sensor.map' containing all room-sensor node mappings. The file is a text file
(i.e. you can open and modify the file in a standard editor) with every line having the
format:
<room ID><space><sensor ID><\n>
A room ID and sensor ID are both positive 16-bit integers (uint16_t).
The data manager organizes all sensor nodes in a pointer list data structure. Use the
implementation of a pointer list implemented in the previous exercises to do. An
element in this list maintains at least information on (i) sensor node ID, (ii) room ID,
(iii) data to compute a running average, and (iv) a last-modified timestamp that
contains the timestamp of the last received sensor data used to update the running
average of this sensor. Figure 2 visualizes this data structure
<p align="center">
  <img width="257" alt="image" src="https://github.com/avidadearthur/multi-threaded-server/assets/42184405/e4d36e40-46cd-447a-93d0-209876a61135">
  <br>
  <em>Figure 3: Data manager</em>
</p>
The data manager starts collecting sensor data and computes for every sensor node a
running average. We define a running average in this context as the average of the last
RUN_AVG_LENGTH sensor values. If this running average exceeds a minimum or
maximum temperature value, a log event (message sent to stderr) should be generated
indicating in which room it’s too cold or too hot.
RUN_AVG_LENGTH should be set at compile-time with the preprocessor directives
RUN_AVG_LENGTH=<some_value>. If this isn't done, the default value of 5 should
be used.
It is also possible to define the minimum and maximum temperature values at
compile-time with the preprocessor directives SET_MIN_TEMP=<some_value> and
SET_MAX_TEMP=<some_value> where <some_value> is expressed in degrees
Celsius. Compilation should fail and an appropriate error message should be printed
when these preprocessor directives are not set at compile-time. Sensor data is defined
as a struct with the following fields:
• sensor_id: a positive 16-bit integer;
• temperature: a double;
• timestamp: a time_t value;
The data manager reads sensor data from a binary file called 'sensor_data' with the
format:
<sensor ID><temperature><timestamp><sensor ID><temperature><timestamp> ...
Notice this is a binary file, not readable in a text editor, and spaces and newlines (\n)
have no meaning. 

## Storage Manager & Event Logging - Inter-process communication
Create a program such that a new log process is created as a child process, and a
communication pipe between the storage manager and the newly created log process
is installed. The log process receives log-events from the storage manager and writes
them to a text file (the log file).
A log-event contains an ASCII info message describing the type of event. A few
examples of log-events are:
 A new csv file is created or an existing file has been opened.
 Data insertion succeeded.
 An error occurred when writing to the csv file.
 The csv file has been closed.
For each log-event received, the log process writes an ASCII message of the format
 <sequence number> <timestamp> <log-event info message> to a new line on a log
file called “gateway.log”.
Try to provide a clean shutdown for both processes. Make sure you don’t end up with
orphan or zombie processes.
We will compile and test your solution against the header sensor_db.h specified
above, check if the logger is running as a separate process, and check if your
gateway.log contains our test operations. 

## A multi-threaded server
The scope of this lab is a server-side process with a connection manager that listens
for incoming sensor data from client-side sensor nodes and writes all incoming sensor
readings on the console (std out). TCP sockets are used for the communication
between the connection manager and the sensor nodes.
<p align="center">
  <img width="335" alt="image" src="https://github.com/avidadearthur/multi-threaded-server/assets/42184405/27629edb-41c7-4d4d-9eaf-88aace08271a">
  <br>
  <em>Figure 2: Generic Double-Linked List diagram</em>
</p>
