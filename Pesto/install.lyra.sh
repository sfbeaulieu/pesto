g++ -std=c++11 -c *.cpp
g++ -std=c++11 -L/opt/NuvuCameras/lib/ *.o -o Pesto -lpthread -lnuvu
rm *.o
echo "Please enter the root pwd:"
sudo cp Pesto /bin/
cd ../cmd/
sudo make
cd ../Pesto
