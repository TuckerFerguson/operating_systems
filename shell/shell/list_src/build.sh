#!/bin/sh

export LD_LIBRARY_PATH="$(pwd)":$LD_LIBRARY_PATH;

cd src;
make; 
cp mydash ..;