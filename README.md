# csci499_DylanAhTeck

Installing dependencies

Boost:

sudo apt install libboost-all-dev

Google Test:

Glog:

In root directory:

Download:
wget https://github.com/schuhschuh/gflags/archive/v2.2.1.tar.gz
wget https://github.com/google/glog/archive/v0.3.5.tar.gz

Unzip:
tar -xvzf v0.3.5.tar.gz
tar -xvzf v2.2.1.tar.gz

cd to glog-0.3.5 Directory
Run:
cmake .

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

Running Program:

To set to log level 7 in command line for func and kv service:

GLOG_v=7 ./func_server

To set in clclient use -v 7 as part of command
