# csci499_DylanAhTeck

This repository contains my implementation of the Function-as-a-Service Flatform *Func* and the *Warble* social network platform it services. Func is intended to provide a very basic FaaS platform like AWS Lambda or Google Cloud Functions, and Warble is intended to have the same basic functionality of Twitter.

The service operator can register a function **f** and its associated event type **t**, and when the FaaS platform receives the request that matches event type **t** it will pass that request to **f** for processing.  Functions that are executed in this manner have no persistent state, so Func provides access to a key-value store for the functions to store their data.

This FaaS functionality can be accesseded through a command-line interface, which will enable Twitter-like functionality.  Users of Warble can register for user accounts on Warble, post “warbles” (akin to tweets), follow other users, and be followed by other users.  They can also reply to warbles (either their own or those of other users).

## Installing Project and Dependencies

Follow the steps below to install the project and all required dependencies on your machine. 

### Clone git repository and open Vagrant virtual machine

Clone git repository
```bash
$ git clone https://github.com/DylanAhTeck/csci499_DylanAhTeck.git
```
Navigate into csci499_DylanAhTeck root directory
```bash
$ cd csci499_DylanAhTeck
```

With vagrant installed on Host OS, run vagrant machine and navigate to shared folder
```bash
$ vagrant up
$ vagrant ssh
$ cd ../../vagrant
```

### Install Make, CMake and other useful tools

Make and CMake
```bash
$ sudo apt-get install build-essential autoconf libtool pkg-config cmake
$ sudo apt-get update
```

### Clone and install GRPC (from source)

```bash
$ sudo git clone -b v1.27.3 https://github.com/grpc/grpc
$ cd grpc
$ sudo git submodule update --init
```

Build with CMake
```bash
$ cmake .
$ sudo make install
```

Install protobuf
```bash
$ cd third_party/protobuf
$ sudo make install
```

### Clone and install gtest (Google Tests)

Download repository and install in root directory

```bash
$ wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz
$ tar xzf release-1.8.0.tar.gz
$ cd googletest-release-1.8.0
$ cmake -DBUILD_SHARED_LIBS=ON .
$ make
$ sudo make install
```

### Clone and install gflag (Google Flags) and glog (Google Log)

Download repository and install in root directory

```bash
wget https://github.com/schuhschuh/gflags/archive/v2.2.1.tar.gz
wget https://github.com/google/glog/archive/v0.3.5.tar.gz
tar -xvzf v0.3.5.tar.gz
tar -xvzf v2.2.1.tar.gz
```

From root directory, navigate to glog-0.3.5 directory and run CMake to build glog

```bash
$ cd glog-0.3.5
$ cmake .
$ sudo make install
```

From root directory, cd to gflags-2.2.1 directory and run CMake to build gflag

```bash
$ cd gflags-2.2.1
$ cmake .
$ sudo make install
```

### Clone and install Boost (for command-line argument parsing)

```bash
$ sudo apt install libboost-all-dev
```

## Compiling and running services

Navigate to source directory and build executables using Make

```bash
$ cd src
$ make
```

To run func service:
```bash
$ ./func_server
```

To run key-value store service:
```bash
$ ./kvstore_server
```

To run command-line client:
```bash
$ ./clclient [flags]
```

**Note:Set the appropriate glog flags via command-line environment variables before the executable for logging.**

For example:
```bash
GLOG_v=7 GLOG_logtostderr=1 ./func_server
```

## Compiling and running Google Tests

The command-line client, func service and key-value store service each have a suite of google tests that 
is automatically compiled using the make command. 

**Note: each test requires a *new* instance of the other services to be running **

To run func service test:
```bash
$ ./func_server_test
```

To run key-value store test:
```bash
$ ./kvstore_server_test
```

To run command-line client test:
```bash
$ ./clclient_test
```
