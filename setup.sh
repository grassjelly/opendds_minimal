#!/usr/bin/env bash
echo ""
echo "Have you source your OpenDDS env ($ source OpenDDS/setenv.sh) ? "
echo "Press [ENTER] to continue"
read

./clean.sh

#scrape all files and replace with idl filename
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

#Build and Compile
echo "Building Now"
mkdir build
cp ./run_test.pl ./build
cd build
cmake ..
make
cd ..
