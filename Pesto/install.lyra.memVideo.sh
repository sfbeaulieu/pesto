g++ -std=c++11 -I/usr/local/include/opencv4 -I/usr/local/include/opencv4/opencv2/ /usr/local/include -L/usr/local/lib -L/opt/opencv/release -c *.cpp
gcc -c zscale.c
g++ -std=c++11 -L/opt/NuvuCameras/lib/ -L/usr/local/lib -L/opt/opencv/release *.o -o Pesto -lpthread -lnuvu -lopencv_core -lopencv_highgui -lopencv_imgproc

rm *.o
 
echo "Please enter the root pwd:"
sudo rm /bin/Pesto
sudo cp Pesto /bin/
cd ../cmd/
sudo make
cd ../Pesto

