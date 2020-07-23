# Mini Backup Server

## Description

Server that can be used for file backup

Built using socket socket programming

## Features implemented

Booth server and client use asynchronous IO from the keyboard and the socket via the select syscall

Creates new files on server and client side

File with the same name will be overwriten

## Client

Commands
* UPLOAD FILENAME
* DOWNLOAD FILENAME

## Server

Has the exit command that force clients to stop

Other commnads will result in an error message being displayed

Also the port must be passed as an argument

Usage
```
./server
Usage: server server_port

./server PORT
command
The only command possible for server is exit
exit
sending exit message
```

## Objects and architecute

Data structures used :
* linked lists
Functions created :
* recv_file
* send_file

```
./client
UPLOAD file.txt
```
Client side : ```send_file```
Server side : ```recv_file```

```
./client
DOWNLOAD file.txt
```
Client side : ```recv_file```
Server side : ```send_file```

