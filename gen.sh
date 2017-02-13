#!/usr/bin/env bash

#scrape all files and replace with idl filename
rm -rf gen

idl_filename=$(basename ./idl/*.idl .idl)
perl -p -i -e "s/Template/${idl_filename}/g" `find ./ -name *.cpp`
perl -p -i -e "s/Template/${idl_filename}/g" `find ./ -name *.txt`
perl -p -i -e "s/Template/${idl_filename}/g" `find ./ -name *.h`

#generate codes
mkdir gen && cd idl
opendds_idl -o ../gen *.idl
tao_idl -I$DDS_ROOT -I$TAO_ROOT/orbsvcs -o ../gen ../gen/*.idl
tao_idl -I$DDS_ROOT -I$TAO_ROOT/orbsvcs -o ../gen *.idl
echo "Code Generated"
cd ..

cp ./run_test.pl ./build
