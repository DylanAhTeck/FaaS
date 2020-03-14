# csci499_DylanAhTeck

Steps:

- Clone git repository
  \$ git clone https://github.com/DylanAhTeck/csci499_DylanAhTeck.git
- cd into csci499_DylanAhTeck root directry
  \$ cd csci499_DylanAhTeck
- Run vagrant machine
  $ vagrant up
$ vagrant ssh

- Find shared folder
  cd ../../vagrant

Installing dependencies

Make and CMake:
$ sudo apt-get install build-essential autoconf libtool pkg-config cmake
$ sudo apt-get update

Clone GRPC (from source):

$ sudo git clone -b v1.27.3 https://github.com/grpc/grpc
$ cd grpc
\$ sudo git submodule update --init

Build with CMake
$ cmake .
$ sudo make install

Install protobuf
$ cd third_party/protobuf
$ sudo make install

Gtests:

Download latest release from lates:
wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz

Run:

$ tar xzf release-1.8.0.tar.gz
$ cd googletest-release-1.8.0
$ cmake -DBUILD_SHARED_LIBS=ON .
$ make
\$ sudo make install

Glog and Gflags:

In root directory:

Download:
wget https://github.com/schuhschuh/gflags/archive/v2.2.1.tar.gz
wget https://github.com/google/glog/archive/v0.3.5.tar.gz

Unzip:
tar -xvzf v0.3.5.tar.gz
tar -xvzf v2.2.1.tar.gz

From root directory, cd to glog-0.3.5 root directory and run CMake:

$ cd glog-0.3.5
$ cmake .
\$ sudo make install

From root directory, cd to gflags-2.2.1 root directory and run CMake:

$ cd gflags-2.2.1
$ cmake .
\$ sudo make install

Boost:

From root directory, install boost:

sudo apt install libboost-all-dev

Compile program:

Run make in src directory
\$ make

To run func service:
\$ ./func_server

To run key-value store service:
\$ ./kvstore_server

To run command-line client:
\$ ./clclient [flags]

Flags and Logging:

Set the appropriate flags via command-line environment variables before the executable.

For example:

GLOG_v=7 GLOG_logtostderr=1 ./func_server
