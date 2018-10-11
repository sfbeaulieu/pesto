import utils as utils
import os
import datetime as DT
import file as fi
import redux as red
import time
class Pesto:
    """Description:\n Methods to communicate with the Nuvu hnu1024 camera.\n
    *set_tstamp->sets the time stamp.\n
    *set_obj->sets the object name in the header.\n
    *bias->create and apply a bias fram to every subsequent frame.\n
    *get_ccdt->retrieve the actual ccd temperature.\n
    *set_ccdt-> sets the ccd temperature.\n
    *close->close the camera.\n
    *stop->stops an acquisition.\n
    *set_expt->sets the exposure time.\n
    *go->start an acquisition sequece.\n
    *set_gain->set the em gain.\n
    *get_gain->retrieve the Em gain.\n
    *open_shutter->Open the camera shutter.\n
    *close_shutter->close the camera shutter.\n
    *set_ro->sets the readout mode.\n
    *fw->trigger a change of filter.\n
    *set_obs->sets the observer keyword in the headers.\n
    *set_operator->sets the operator keyword in the headers.\n
    *set_roi->set the roi.\n
    *get_bias_status->retrieve the bias status of the camera.\n
    *get_exp->retrieve the exposure time.\n
    *get_romode->retrieve the readout mode of the camera.\n
    *path_tmp->return the path of the tmp folder.\n
    *path->return the path of the night folder.\n
    *ds9->display image in ds9.\n
    *listDir->list all .fits file in a folder.\n
    *takeTmpImages->takes temporaru images.\n
    *eraseTmpImages->erase the tmp images.\n
    *mvImages-> move images from path1 to path 2.\n
    *cpImages-> copy images from path1 to path2.\n
    *stats->return the stats of an images.\n
    *findFilesWith->find all files with a specific keyword in the header.\n
    *get_foyer-> get the focuser position of the telescope\n.
    *set_foyer->Set the position of the focuser."""
    def __init__(self):
        
        self.pathBase = "/home/data/"
        
        
        
    def set_tstamp(self,mode):
        """Descrition:\n This function set the time stamp.
        \nmode:\n (1)->gps, (2)->internal time stamp,(3)->No timestamp
        \nReturn:\n 0: success.\n -1:Unable to set the timestamp"""
        return utils.command("SET_TSTAMP",[mode])
    def set_obj(self,name):
        """Descrition:\n This function will set the name of the object in the header. It is also used to create the appropriat folder.
        \nname:\n Name of the object
        \nReturn:\n 0: success.\n -1:Unable to set the timestamp"""
        return utils.command("SET_OBJ",[name])
    def bias(self):
        """Descrition:\n This function subtract a bias frame to the scientific images.
        \nReturn:\n 0: success.\n -1:Unable to set the timestamp\n -2:There is an running acquisition (stop and retry)"""
        return utils.command("BIAS",[])
    def get_ccdt(self):
        """Desciption:\nThis function retrieve the current temperature of the EMCCD.
        \nReturn:\n if successful, returns the temperature, -1: unsuccessful."""
        return float(utils.command("GET_CCDT",[]))
    def set_ccdT(self,temp):
        """Description:\nThis function set the EMCCD temperature set point.
        \ntemp:\n temperature in degree celcius. 
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_CCDT",[temp])
    def close(self):
        """Description:\n Close the camera.
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("CLOSE",[])
    def stop(self):
        """Description:\n Stop an ongoing acquisition.
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("STOP",[])
    def set_expT(self,time):
        """Description:\n This function set the EMCCD exposure time in ms. If the exposure time is shorter than the readout time, the effective exposure time will be the readout time.
        \ntime:\n Exposure time in ms. 
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_EXPTIME",[time])
    def go(self,mode_a,nbExp,mode_b,mode_c):
        """Description:\n This function trigger a serie of scientific exposure. 
        \nmode_a:\n 1->infinite acquisition, 2-> finite acquisition
        \nmode_b:\n 1-> the command will return after the acquisition is stoped or completed, 2-> the command will return immediately
        \nmode_c:\n 1->Target, 2->Sky flat, 3->DomeFlat, 4->Dark, 5->Video  
        \nnbExp:\n number of expositions
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("GO",[mode_a,nbExp,mode_b,mode_c])
    def set_gain(self,gain):
        """Description:\n This function sets the EM gain.
        \ngain:\n Em gain [1..5000]
        \nReturn:\n 0:successful, -1: unsuccesfull, -2: value is out of range"""
        return utils.command("SET_GAIN",[gain])
    def get_gain(self):
        """Description:\n This function retrieve the EM gain of the camera.
        \nReturn:\n gain:successful, -1: unsuccesfull"""
        return int(utils.command("GET_GAIN",[]))
    def open_shutter(self):
        """Description:\n This function opens the shutter of the camera.
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("OPEN_SHUTTER",[])
    def close_shutter(self):
        """Description:\n This function closes the shutter of the camera.
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("CLOSE_SHUTTER",[])
    def set_romode(self,mode):
        """Description:\n This function sets the readout mode of the camera. A description of the readout mode can be found in the documentation or online on the instrument webpage.
        \nmode:\n readout mode. (1->std. EM mode, 8->std. conventional mode).        
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_RO",[mode])
    def fw(self,position):
        """Description:\nThis function querry a change of position on the filter wheel.
        \npostion:\n The position are H,1,2,3,4,5,+,?
        \nReturn:\n current position: succesfull, -1: unsuccessful."""
        return utils.command("FW",[position])
    def set_obs(self,observer):
        """Description:\n This function sets observer keyword in the header.
        \nobserver:\n name of the observer.        
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_OBS",[observer])
    def set_operator(self,operator):
        """Description:\n This function sets operator keyword in the header.
        \noperator:\n name of the operator.        
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_OPERATOR",[operator])
    def set_roi(self,offset,height):
        """Description:\n This function sets a new region of interest (roi). A maximum of 1 roi can be set in conventional and 8 in Em mode. By default all roi are 1024 in width.
        \noffset:\n offset in Y of the roi.   
        \nheight:\n height of the roi.
        \nReturn:\n 0:successful, -1: unsuccesfull"""
        return utils.command("SET_ROI",[offset,height])
    def get_bias_status(self):
        """Description:\n This function will return the staus of the bias. It will take into consideration the timestamp, readout mode, and ROI.
        \nwarning:\n This function will not be triggered if the temperature of the detector change due to thermomechanical problem. If it does, the bias should be updated manually.
        \nReturn:\n 0:bias is out of date, 1: bias is up to date."""
        return int(utils.command("GET_BIAS_STATUS",[]))
    def get_exp(self):
        """Description:\n This function retrive the exposure time of the camera.
        \nReturn:\n exposure time in ms : successful, -1: unsuccesfull"""
        return utils.command("GET_EXP",[])
    def get_romode(self):
        """Description:\n This function retrive the readout mode of the camera.
        \nReturn:\n readout mode: successful, -1: unsuccesfull"""
        return int(utils.command("GET_ROMODE",[]))
    def path_tmp(self):
        """Description:\n return the path of the tmp folders where images for further analysis are stored. For example before saving the flats ones might want to analyse the flux level.
        \nReturn:\n path name"""
        date = str(DT.datetime.today().date()).split('-')
        time = str(DT.datetime.today().time()).split(':')
        name = "%2.2d%2.2d%2.2d" % (int(date[0])-2000,int(date[1]),int(date[2])) if int(time[0])>=12 else "%2.2d%2.2d%2.2d" % (int(date[0])-2000,int(date[1]),int(date[2])-1)
        return self.pathBase+name+'/Other/'
    def path(self):
        """Description:\n return the path of the night folders where images for further analysis are stored. For example before saving the flats ones might want to analyse the flux level.
        \nReturn:\n path name"""
        date = str(DT.datetime.today().date()).split('-')
        time = str(DT.datetime.today().time()).split(':')
        name = "%2.2d%2.2d%2.2d" % (int(date[0])-2000,int(date[1]),int(date[2])) if int(time[0])>=12 else "%2.2d%2.2d%2.2d" % (int(date[0])-2000,int(date[1]),int(date[2])-1)
        return self.pathBase+name+'/'
    def ds9(self,f=" "):
        """Description:\ndisplay file f in ds9. If f="", it will display the last file written in /home/data/.
        \nNote:\n the file f must be preceded by the path i.e., /home/data/file.fits.
        \nNote:\n It is note necessary to add the extention (.fits/.fits.gz).
        \nReturn:\n void"""
        if (f==" "):
            f = fi.find_last("/home/data/")
        utils.command("ds9 ",[f],command_root="/usr/bin/")
        return 
    def listDir(self,path):
        """Description:\n List all the fits file in folder (path).
        \nreturn:\n return an array of strings."""
        return fi.listDir(path)
    def takeTmpImages(self,nbFrame):
        """Description:\n will trigger the go commande with nbFrame images and save them in /home/data/YYMMDD/Other/.
        \nReturn:\n return once the images are all saved."""
        self.go(2,nbFrame,1,6)
        return 
    def eraseTmpImages(self):
        """Description:\n erase all the fits files in /home/data/YYMMDD/Other/.
        \nReturn:\n void"""
        utils.command("rm "+self.path_tmp()+"*.fits",[])
        return 
    def mvImages(self,path1,path2):
        """Description:\n move images from path1 to path2.
        \nNote:\n if path1 is equal to 0 i.e., path1='0', path1 will be set to /home/data/YYMMDD/Other/.
        \nReturn:\n void."""
        if path1=='0':path1 = self.path_tmp()
        if path1[-1]!='/': path1+='/'
        if path2[-1]!='/': path2+='/'
        if not os.path.isdir(path2):
            os.mkdir(path2)
        else: 
            utils.command("mv "+path1+"*.fits "+path2,[])
            utils.command("rm "+path1+"*.fits",[])
    def cpImages(self,path1,path2):
        """Description:\n copy images from path1 to path2.
        \nNote:\n if path1 is equal to 0 i.e., path1='0', path1 will be set to /home/data/YYMMDD/Other/.
        \nReturn:\n void."""
        if path1=='0':path1 = self.path()
        if path1[-1]!='/': path1+='/'
        if path2[-1]!='/': path2+='/'
        if not os.path.isdir(path2):
            os.mkdir(path2)
        else: 
            utils.command("mv "+path1+"*.fits "+path2,[])
    def stats(self,Im):
        """Description:\n Give an image array, this function return the robust mean,median and std dev.
        \nReturn:\n mean,median,std."""
        return red.sigma_clipped_stats(Im)
    def findFilesWith(self,path,keyword):
        """Description:\n this function will list all the file with the keyword "keyword" in the header inside folder path and its subdirectory.
        \nExample:\n keyword="Ha'"
        \nReturn:\n returns an array of strings."""
        return fi.searchHeader(path,keyword)
    def get_foyer(self):
        """Description:\n This function retrieve the position of the telescope focus.
        \nReturn:\n Return the focuser position of the telescope."""
        return int(utils.command("getfoyer",[]))
    def set_foyer(self,pos):
        """Description:\n This function will set the position of the focus. if after 30 sec the position is not reached, the function will return -1.
        \nReturn:\n Return the current position of the telescope."""
        timeOut = 0
        utils.command("telfoyer",[pos])
        while(1):
            time.sleep(1)
            if (self.get_foyer()==pos):
                return pos
            elif (timeOut>30):
                return -1
            timeOut+=1
        
    
        
        