#!/bin/bash

cd /home/rainjoe/ownrIO/build
rm -rf ./*
cmake ..
make
cd ..

python write.py
python read.py
python test.py
