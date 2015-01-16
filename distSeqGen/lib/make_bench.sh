echo $1
cd $1
verilator --cc --coverage-line --prefix Vtop -CFLAGS -g --Wno-WIDTH $1.v
mv obj_dir cpp

cd cpp
make -f Vtop.mk
cd ../../

echo "END" $1

