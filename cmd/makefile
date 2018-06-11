

ALL: ./acquisition/acquisition ./setExposureTime/setExposureTime ./stopAcq/stopAcq ./close/close ./ccdTemp/ccdTemp ./getCcdTemp/getCcdTemp ./setTimeStamp/setTimeStamp\
	./bias/bias ./setObjName/setObjName ./setGain/setGain ./closeShut/closeShut ./openShut/openShut ./setRO/setRO ./getGain/getGain ./positionFW/positionFW\
	./connectFW/connectFW ./closeConnectFW/closeConnectFW ./SET_OBS/SET_OBS ./SET_OPE/SET_OPE

./acquisition/acquisition:./acquisition/main.cpp
	g++ -I./lib -o/bin/GO ./lib/socket.cpp ./acquisition/main.cpp
./setExposureTime/setExposureTime:./setExposureTime/main.cpp
	g++ -I./lib -o/bin/SET_EXPTIME ./lib/socket.cpp ./setExposureTime/main.cpp
./stopAcq/stopAcq:./stopAcq/main.cpp
	g++ -I./lib -o/bin/STOP ./lib/socket.cpp ./stopAcq/main.cpp
./close/close:./close/main.cpp
	g++ -I./lib -o/bin/CLOSE ./lib/socket.cpp ./close/main.cpp
./ccdTemp/ccdTemp:./ccdTemp/main.cpp
	g++ -I./lib -o/bin/SET_CCDT ./lib/socket.cpp ./ccdTemp/main.cpp
./getCcdTemp/getCcdTemp:./getCcdTemp/main.cpp
	g++ -I./lib -o/bin/GET_CCDT ./lib/socket.cpp ./getCcdTemp/main.cpp
./setTimeStamp/setTimeStamp:./setTimeStamp/main.cpp
	g++ -I./lib -o/bin/SET_TSTAMP ./lib/socket.cpp ./setTimeStamp/main.cpp
./setObjName/setObjName:./setObjName/main.cpp
	g++ -I./lib -o/bin/SET_OBJ ./lib/socket.cpp ./setObjName/main.cpp
./bias/bias:./bias/main.cpp
	g++ -I./lib -o/bin/BIAS ./lib/socket.cpp ./bias/main.cpp
./setGain/setGain:./setGain/main.cpp
	g++ -I./lib -o/bin/SET_GAIN ./lib/socket.cpp ./setGain/main.cpp
./closeShut/closeShut:./closeShut/main.cpp
	g++ -I./lib -o/bin/CLOSE_SHUTTER ./lib/socket.cpp ./closeShut/main.cpp
./openShut/openShut:./openShut/main.cpp
	g++ -I./lib -o/bin/OPEN_SHUTTER ./lib/socket.cpp ./openShut/main.cpp
./setRO/setRO:./setRO/main.cpp
	g++ -I./lib -o/bin/SET_RO ./lib/socket.cpp ./setRO/main.cpp
./getGain/getGain:./getGain/main.cpp
	g++ -I./lib -o/bin/GET_GAIN ./lib/socket.cpp ./getGain/main.cpp
./closeConnectFW/closeConnectFW:./closeConnectFW/main.cpp
	g++ -I./lib -o/bin/CLOSE_CONNECTION_FW ./lib/socket.cpp ./closeConnectFW/main.cpp
./connectFW/connectFW:./connectFW/main.cpp
	g++ -I./lib -o/bin/CONNECT_FW ./lib/socket.cpp ./connectFW/main.cpp
./positionFW/positionFW:./positionFW/main.cpp
	g++ -I./lib -o/bin/FW ./lib/socket.cpp ./positionFW/main.cpp
./SET_OBS/SET_OBS:./SET_OBS/main.cpp
	g++ -I./lib -o/bin/SET_OBS ./lib/socket.cpp ./SET_OBS/main.cpp
./SET_OPE/SET_OPE:./SET_OPE/main.cpp
	g++ -I./lib -o/bin/SET_OPERATOR ./lib/socket.cpp ./SET_OPE/main.cpp

