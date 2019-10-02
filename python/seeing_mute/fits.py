

"""
Small collection of function to open, modify and save fits file.
Function included are:
  * loadIm - Load a fits file
  * saveIm - save a fits file
  * getHeader - return the value of a header keyword
  * changeHeader - change a keyword value in a header
  

Author: Jonathan St-antoine, 2018-03-25
"""

from astropy.io import fits
import numpy as np
import os.path


def loadIm(name,path='',head=False,Tstamp=False,Tstamp_name="CTRLTIME"):
    '''Description:\n This function will load an image a return a numpy 32-bits array.
    \nOption:\n if head the header will be return as well, if Tstamp the time stamp will be return (Pesto)
        \nReturn:\n Return an 32 bit array,[Tstamp and header](optional)
        \nAuthor:\n Jonathan St-Antoine, 2018-03-25'''
    name,ext = os.path.splitext(name)
    if os.path.splitext(name)[1]==".fits":
        name,ext = os.path.splitext(name)
    if os.path.isfile(path+name+".fits"): hdulist = fits.open(name+'.fits')
    elif os.path.isfile(path+name+".fits.gz"): hdulist = fits.open(name+'.fits.gz')
    data = hdulist[0].data    
    header = hdulist[0].header
    if Tstamp: 
        Time = header[Tstamp_name]
    hdulist.close()
    if not Tstamp and not head:return np.float32(data)
    if Tstamp and head:return np.float32(data),Time,header
    if Tstamp and not head:return np.float32(data),Time 
    if not Tstamp and head:return np.float32(data),header
def loadImMemmap(name,xmin,xmax,ymin,ymax):
    """
    **Description**:
        This function open only a section of the fits file in memory defined by xmin,xmax,ymin and ymax. 
    **special consideration**:
        This function is primarly used to speed up for loops. Consequently, extra cares need to be taken.\n
        **BZERO**:
            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 32768.
        **BSCALE**:
            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 1.
        **name**:
            the naming convention is /path/to/file/nameOfFile.fits
    **Return**:
        return a numpy array with dtype=int16 with the BZERO and BSCALE applied.
        """
    hdulist = fits.open(name,memmap=True,do_not_scale_image_data=True)
    data = hdulist[0].data[ymin:ymax,xmin:xmax] 
    hdulist.close()
    return np.int32(data+32768)
def loadImMemmapCube(name,xmin,xmax,ymin,ymax):
    """
    **Description**:
        (only to be used in photon.py the data value must be <2**15) This function open only a section of the fits file in memory defined by xmin,xmax,ymin and ymax. 
    **special consideration**:
        This function is primarly used to speed up for loops. Consequently, extra cares need to be taken.\n
        **BZERO**:
            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 32768.
        **BSCALE**:
            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 1.
        **name**:
            the naming convention is /path/to/file/nameOfFile.fits
    **Return**:
        return a numpy array with dtype=int16 with the BZERO and BSCALE applied.
        """
    hdulist = fits.open(name,memmap=True,do_not_scale_image_data=True)
    data = hdulist[0].data[:,ymin:ymax,xmin:xmax] 
    hdulist.close()
    return data
    

def saveIm(array,name,header=0,path=''):
    '''Description:\n This function will save an array as a .fits file. Array can be an 2d or 3d array and name is the name of the file. By default it save the image in the current folder
    \nOption:\n A header can be added    
    \nReturn:\n This function will return 0
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25'''
    array = np.float32(np.array(array)) 
    
    if header !=0: hdu = fits.PrimaryHDU(array,header=header)
    else: hdu = fits.PrimaryHDU(array,header=None)
    hdulist = fits.HDUList([hdu])
    
    hdulist.writeto(path+name+'.fits',overwrite=True)

    hdulist.close()
    return 0

def getHeader(keyWord,name,path=''):
    """Description:\n This function return the keyword in a fits file name
    \nReturn:\n Return the keyword. if the keyword does not exist the funtion will return -1
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
    name,ext = os.path.splitext(name)
    if os.path.splitext(name)[1]==".fits":
        name,ext = os.path.splitext(name)
    if os.path.splitext(name)[1]==".fits":
        name,ext = os.path.splitext(name)
   
    if os.path.isfile(path+name+".fits"):hdulist = fits.open(path+name+'.fits')
    elif os.path.isfile(path+name+".fits.gz"):
        
        hdulist = fits.open(path+name+'.fits.gz')
    else: return -1
    try:    
        key = hdulist[0].header[keyWord]
        hdulist.close()
        
        return key
    except:
        hdulist.close()
        
        return -1
    
def changeHeader(name,keyword,newValue,path=''):
    """Description:\n This function modify a specific keyword in a fits file name name
    \nReturn:\n Return 0 if successful, -1 if the keyword does not exist
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
    name,ext = os.path.splitext(name)
    
    if os.path.splitext(name)[1]==".fits":
        name,ext = os.path.splitext(name)
    if os.path.isfile(path+name+".fits"):data, header = fits.getdata(path+name+".fits",0, header=True)
    elif os.path.isfile(path+name+".fits.gz"):data, header = fits.getdata(path+name+".fits.gz",0, header=True)
    try:
        header[keyword] = newValue
        fits.writeto(path+name+".fits", data, header=header,output_verify="silentfix",overwrite=True)
        return 0
    except: return -1
def history(name,history,path=''):
    """Description:\n This function add an history line in the fits header name name
    \nReturn:\n Return 0 if successful, -1 unable to add history
    \nAuthor:\n Jonathan St-Antoine, 2018-08-20"""
    if os.path.splitext(name)[1]==".fits":
        name,ext = os.path.splitext(name)
    if os.path.isfile(path+name+".fits"):data, header = fits.getdata(path+name+".fits",0, header=True)
    elif os.path.isfile(path+name+".fits.gz"):data, header = fits.getdata(path+name+".fits.gz",0, header=True)
    try:
        header['history'] = history
        fits.writeto(path+name+".fits", data, header=header,output_verify="silentfix",overwrite=True)
        return 0
    except: return -1