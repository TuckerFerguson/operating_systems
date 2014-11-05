#!/bin/bash
echo "old version definition `tail -n1 shell.h`"
version=`git rev-parse HEAD`
sed -ie '$d' shell.h
echo "#define VERSION_NUMBER \"$version\"" >> shell.h
echo "new version definition `tail -n1 shell.h`"