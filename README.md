# Purpose
The purpose of this project is to implement a Chat server in C based on our [RFC](https://docs.google.com/document/d/1iAAdjcAEl3txNKCionDErgmqmkE0AVlfWcdrcx9XCHk/edit?usp=sharing) This server will handle a “Message” (“Network Dispatch”, “Dispatch”) specified in the RFC. Additionally, it stores all the data into ndbm databases. Also, another purpose of this project is to implement a db_viewer which shows the contents of the database containing key-value pair data. It uses Ncurses UI, ndbm database.

# Intended Users
This guide is intended for users who have basic experience with the POSIX system.
* Users who can run programs using a terminal on a POSIX system
* Users who know what the command line arguments and flags are
* Users who know how to use cmake.

# Software Requirements
* Clang-tidy
* Cmake
* Dot (This is used by Doxygen)
* Doxygen (Although this is not necessary to run the program, it is intended to be used when building this program, so if you want to build without creating a Doxygen file, edit CMakeLists.txt)
* Standard C17 compiler (e.g. GCC, Clang)
* DC libraries (BCIT DataComm libraries)
* Cgreen (cgreen is a framework for unit testing. It's not essential for running both servers)
* **Ncurses**
* **Ndbm** (Gdbm for linux-based OS)
* uuid library


# Platform
We tested programs on macOS and Fedora Linux (Fedora 37).

# Installing
## Obtaining
Clone this project
```
https://github.com/hurshik7/COMP4985_ChatServer.git
```
Then, you can see the `source` directory.

## Building and Installing
First, go into the `source/` directory and build the Chat server with the following commands.
```
cd source/
cmake -S . -B build
cmake --build build
```

(If the build fails due to missing libraries, please install those libraries first. For example, on Fedora OS, if you don’t have gdbm or ncurses, or uuid libraries please run following code.) \
```
sudo dnf update
sudo dnf install gdbm-devel
sudo dnf install ncurses-devel
sudo dnf install libuuid libuuid-devel
```


Note: The compiler can be specified by passing one of the following to cmake when configure the project:
* `-DCMAKE_C_COMPILER="gcc"`
* `-DCMAKE_C_COMPILER="clang"`

(For example, `cmake -S . -B build -DCMAKE_C_COMPILER="gcc")`

## Running
* Chat Server, DB_Viewer
  You are able to configure the port number by passing arguments. Let's assume the current directory is `source/build` when running the Chat server. Also, If you have a firewall turned on, it is recommended to turn it off. The default port number is 5050.
  `./chat-server`

```
./chat-server -p <port number>
```

## Environment Variable

If you are using a **Mac** and the following message appears when you run the program either Server or Client, set the following environment variable.

`malloc: nano zone abandoned due to inability to preallocate reserved vm space.`
<table>
  <tr>
   <td>
<strong>Variable</strong>
   </td>
   <td><strong>Value</strong>
   </td>
  </tr>
  <tr>
   <td>MallocNanoZone
   </td>
   <td>0
   </td>
  </tr>
</table>
Then, it does not show the message any more. The above message does not cause any problems running these programs, it only appears on Macs among POSIX systems. For more details and solutions, please refer to the following. 
[https://stackoverflow.com/questions/64126942/malloc-nano-zone-abandoned-due-to-inability-to-preallocate-reserved-vm-space](https://stackoverflow.com/questions/64126942/malloc-nano-zone-abandoned-due-to-inability-to-preallocate-reserved-vm-space)

# Features
## Chat Server
* When you run the server you can see a message with the port number. The default port number is `5050`.

```
Listening on port 5050
```

* The server continues to listen for the client unless it is forcibly shut down or press ‘q’.
* The server reads a request from a client, and it will send a response to the request to the client. This server can handle multiple clients at a time.
* The server handles Messages specified in the RFC.
* The server stores the necessary data in the ndbm database according to the client's request.
* The creation and reading path for all DB files is the home directory. For example, `/Users/hurshik/user_account_info.db`
* The necessary and creatable DB files while running this server are as follows:
    * `user_login_info.db`: stores the user login information (login token, password, and UUID)
    * `display_names.db`: stores display names of all users
    * `user_account_info.db`: stores the information of all users
    * `channel_info.db`: stores the information of all channels
    * `messages_info.db`: stores the information of all messages
* The DB viewer shows data of all the databases.
* The program uses Ncurses UI. 
