# Features

- [x] Server supports multiple connections concurrently
  
  This is done via spawning a thread for each incoming connection to the chosen IPC method, which is UDS.
  
- [x] IPC Portion of the task may be decoupled:
  
  One needs to implement a concrete IPCChannel and IPCServer derivative classes.
  Currently, I chose to perform it using UDSChannel and UDSServer.
  
- [x] No external libraries are used (except the STL).

# System architecture & Components Design

The system outputs two targets, `client` and `server`.
These two targets communicate with each other using **MTP** (**M**essage **T**ransfer **P**rotocol):
An MTP message has the following syntax:
`type` `content` `delimiter`

* `type` and `delimiter` are both 1-byte long.

* `content` must be non-empty.

* Current supported message types are :

  * Client -> Server: APPEND_STRING, REQUEST_CHUNK, 
  * Server -> CLient: RESPONSE_CHUNK.

  Accordingly, the content s different for each message type:

  * APPEND_STRING -> the content is the string to be appended within the server's storage.
  * REQUEST_CHUNK -> the content is the size of the requested chunk.
  * RESPONSE_CHUNK -> the content is the requested chunk, whose size was requested by the client beforehand.

The communication between those endpoints is performed through multiple interfaces:

* `IPCChannel` is an interface for an IPC-based communication channel.
  This is the main class used for handling a specific `client<->server` connection.
* `IPCServer` is an interface for an IPC-based server that can handle connections, and output an `IPCChannel` for each new connection.
* `UDSChannel` and `UDSServer` both implement the corresponding interfaces, and handle most of the low-level network operations.
* `MessageHandler` is a static class that externalizes methods of generating MTP messages and parsing their components.
* Finally, `MTPServer` is the class that runs the whole show: It utilizes the previously-mentioned interfaces and facilitates a uniform way of handling a server that communicates with its clients using **MTP**. Moreover, `MTPServer` uses an additional class named `UserDataHandler` to perform all of the data-related operations, and sticks with error-checking and handling the network operations.
* `UserDataHandler` aggregates data for each client using its `uid`. The `uid` can be extracted from every implemented `IPCChannel`.
  I chose not to include the UID within the content of the protocol, but to use characteristics of the IPC to acquire that data.
  Similar functionality can be useful with other forms of IPC -> for example, file-permissions can aid in setting/getting permissions of a named FIFO or SharedMem.

# Correctness, Robustness & Usability

The code uses several techniques to avoid synchronization problems, and ensure correct, sequential responses to every client:

- [x] A different thread handles each incoming connection.
- [x] User data access is protected by mutexes.
- [x] Clients can send multiple requests before trying to request their first chunk.
- [x] Error-checking is performed at every point within the code, so that illegal requests and situations raise exceptions.

