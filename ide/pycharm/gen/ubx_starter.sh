#!/bin/bash

echo "Generating datafile ..."
cd gen && python gpsEmulator.py && cd ..
echo "Generating datafile ... done"

echo "Sending data ..."
cd sender && python dataSender.py && cd ..
echo "Sending data ...done"
