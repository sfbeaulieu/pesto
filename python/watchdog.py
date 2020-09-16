# -*- coding: utf-8 -*-
from astropy.io import fits
from astropy.time import Time,TimeDelta
import sys
import file as f
import os
import numpy as np 

if len(sys.argv) > 1 :
	path = sys.argv[1]
else:
	path = "/home/data" 
last_path = f.find_last(path)
if (last_path == "-1"):
	sys.stdout.write(last_path)
	sys.exit()
	
data,header = fits.getdata(last_path,header=True)


#sys.stdout.write(str(int(np.median(data)))+","+str(int(np.amax(data))))

sys.stdout.write(";DATE;" + header["DATE"] + ";\n")
sys.stdout.write(";AMP_TYPE;" + header["AMP_TYPE"] + ";\n")
sys.stdout.write(";V_FREQ;" + str(header["V_FREQ"])+";\n")
sys.stdout.write(";H_FREQ;" + str(header["H_FREQ"])+";\n")
sys.stdout.write(";EXPOSURE;" + str(header["EXPOSURE"])+";\n")
sys.stdout.write(";ANA_GAIN;" + str(header["ANA_GAIN"])+";\n")
sys.stdout.write(";SHUTTER;" + str(header["SHUTTER"])+";\n")
sys.stdout.write(";ROI_NBR;" + str(header["ROI_NBR"])+";\n")
sys.stdout.write(";ROI_Y_1;" + str(header["ROI_Y_1"])+";\n")
sys.stdout.write(";ROI_H_1;" + str(header["ROI_H_1"])+";\n")
sys.stdout.write(";OBJECT;" + str(header["OBJECT"])+";\n")
sys.stdout.write(";RA;" + str(header["RA"])+";\n")
sys.stdout.write(";HA;" + str(header["HA"])+";\n")
sys.stdout.write(";DEC;" + str(header["DEC"])+";\n")
sys.stdout.write(";EPOCH;" + str(header["EPOCH"])+";\n")
sys.stdout.write(";AIRMASS;" + str(header["AIRMASS"])+";\n")
sys.stdout.write(";ST;" + str(header["ST"])+";\n")
sys.stdout.write(";UT;" + str(header["UT"])+";\n")
sys.stdout.write(";YEAR;" + str(header["YEAR"])+";\n")
sys.stdout.write(";FOCUS;" + str(header["FOCUS"])+";\n")
sys.stdout.write(";ROTATOR;" + str(header["ROTATOR"])+";\n")
sys.stdout.write(";DOME;" + str(header["DOME"])+";\n")
sys.stdout.write(";PESTOROT;" + str(header["PESTOROT"])+";\n")
sys.stdout.write(";PESTOMIR;" + str(header["PESTOMIR"])+";\n")
sys.stdout.write(";OBSERVER;" + str(header["OBSERVER"])+";\n")
sys.stdout.write(";OPERATOR;" + str(header["OPERATOR"])+";\n")
sys.stdout.write(";T1;" + str(header["T1"])+";\n")
sys.stdout.write(";FILTER;" + str(header["FILTER"])+";\n")
sys.stdout.write(";BIAS;" + str(header["BIAS"])+";\n")
sys.stdout.write(";FWOVER;" + str(header["FWOVER"])+";\n")
try:
	sys.stdout.write(";CTRLTIME;" + str(header["CTRLTIME"])+";\n")
	sys.stdout.write(";CTRLTMFL;" + str(header["CTRLTMFL"])+";\n")
except:
	sys.stdout.write(";CTRLTIME;" + "-1"+";\n")
	sys.stdout.write(";CTRLTMFL;" + "-1"+";\n")

