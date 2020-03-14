# csci499_DylanAhTeck

Steps:

- Clone git repository
- cd into csci499_DylanAhTeck root directry
- Run vagrant machine
  vagrant up
  vagrant ssh

- Find shared folder
  cd ../../vagrant

Installing dependencies

Make and CMake:
sudo apt-get install build-essential autoconf libtool pkg-config cmake
sudo apt-get update

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

tar xzf release-1.8.0.tar.gz
cd googletest-release-1.8.0

cd into gtest file and run:

sudo cmake CMakeLists.txt
sudo make install

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
 $ sudo make install

From root directory, cd to gflags-2.2.1 root directory and run CMake:

 $ cd gflags-2.2.1
 $ cmake .
 $ sudo make install

Boost:

From root directory, install boost:

sudo apt install libboost-all-dev


Running Program:

To set to log level 7 in command line for func and kv service:

GLOG_v=7 ./func_server

To set in clclient use -v 7 as part of command




To install pip for cpplint:

Start by updating the package list using the following command:

sudo apt update
Use the following command to install pip for Python 3:

sudo apt install python3-pip
The command above will also install all the dependencies required for building Python modules.

Once the installation is complete, verify the installation by checking the pip version:

pip3 --version

Then install cpplint:

pip3 install cpplint

GRPC:

Protobuf: