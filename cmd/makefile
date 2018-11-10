

ALL: ./acquisition/acquisition ./setExposureTime/setExposureTime ./stopAcq/stopAcq ./close/close ./ccdTemp/ccdTemp ./getCcdTemp/getCcdTemp ./setTimeStamp/setTimeStamp\
	./bias/bias ./setObjName/setObjName ./setGain/setGain ./closeShut/closeShut ./openShut/openShut ./setRO/setRO ./getGain/getGain ./positionFW/positionFW\
	./connectFW/connectFW ./closeConnectFW/closeConnectFW ./SET_OBS/SET_OBS ./SET_OPE/SET_OPE ./printTCS/PRINT_TCS ./test/TEST ./getCCDTLoop/GET_CCDT_LOOP\
	./getBiasStatus/GET_BIAS_STATUS ./setROI/SET_ROI ./getExpT/GET_EXP ./getROMode/GET_ROMODE ./overideFW/overideFW ./stopOverideFW/stopOverideFW \
	./isacqongoing/isacqongoing ./stopThread/stopThread ./getInc/getInc

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
	g++ -I./lib -o/bin/SET_OPE ./lib/socket.cpp ./SET_OPE/main.cpp
./printTCS/PRINT_TCS:./printTCS/main.cpp
	g++ -I./lib -o/bin/PRINT_TCS ./lib/socket.cpp ./printTCS/main.cpp
./test/TEST:./test/main.cpp
	g++ -I./lib -o/bin/TEST ./lib/socket.cpp ./test/main.cpp
./getCCDTLoop/GET_CCDT_LOOP:./getCCDTLoop/main.cpp
	g++ -I./lib -o/bin/GET_CCDT_LOOP ./lib/socket.cpp ./getCCDTLoop/main.cpp
./getBiasStatus/GET_BIAS_STATUS:./getBiasStatus/main.cpp
	g++ -I./lib -o/bin/GET_BIAS_STATUS ./lib/socket.cpp ./getBiasStatus/main.cpp
./setROI/SET_ROI:./setROI/main.cpp
	g++ -I./lib -o/bin/SET_ROI ./lib/socket.cpp ./setROI/main.cpp
./getExpT/GET_EXP:./getExpT/main.cpp
	g++ -I./lib -o/bin/GET_EXP ./lib/socket.cpp ./getExpT/main.cpp
./getROMode/GET_ROMODE:./getROMode/main.cpp
	g++ -I./lib -o/bin/GET_ROMODE ./lib/socket.cpp ./getROMode/main.cpp
./overideFW/overideFW:./overideFW/main.cpp
	g++ -I./lib -o/bin/OVERIDEFW ./lib/socket.cpp ./overideFW/main.cpp
./stopOverideFW/stopOverideFW:./stopOverideFW/main.cpp
	g++ -I./lib -o/bin/STOPOVERIDEFW ./lib/socket.cpp ./stopOverideFW/main.cpp
./isacqongoing/isacqongoing:./isacqongoing/main.cpp
	g++ -I./lib -o/bin/ISACQONGOING ./lib/socket.cpp ./isacqongoing/main.cpp
./stopThread/stopThread:./stopThread/main.cpp
	g++ -I./lib -o/bin/STOPTHREAD ./lib/socket.cpp ./stopThread/main.cpp
./getInc/getInc:./getInc/main.cpp
	g++ -I./lib -o/bin/GETINC ./lib/socket.cpp ./getInc/main.cpp
