#!/bin/bash

cd /home/rainjoe/ownrIO/build
rm -rf ./*
cmake ..
make
cd ..
python write.py
