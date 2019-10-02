

"""
Small collection of function to open, modify and save fits file. Meant to be use with > python 3.5.
Function included are:
  * loadIm - Load a fits file
  * saveIm - save a fits file
  * getHeader - return the value of a header keyword
  * changeHeader - change a keyword value in a header
  

Author: Jonathan St-antoine, 2019-08-14
"""

from astropy.io import fits
from numpy import float16,float32,float64,int64,int32,int16,asarray
from os.path import isfile


def loadIm(fname,**kwargs):
    '''
    **Description**:
        This function will load an image a return a numpy 32-bits array.
    **Option**: 
        header: [true/false] Will retuen the header as well. \n
        path: if a path is specified, the file path+fname will be loaded.\n
        optimize: [true/false] if set the function will not verify if the image exist before trying to open it\n
        type:   if 'u16-bits', the data will be return has 16 bits unsigned integer
    **Return**:
        return a numpy 32-bits array and more.
    '''
    if 'path' in kwargs:
        fname = kwargs.get('path')+fname
    if not (kwargs.get('optimized')):
        if not (isfile(fname)):
            print("The file does not exist")
            exit(1)
            return -1
    hdulist = fits.open(fname)
    data = hdulist[0].data
    
    if 'header' in kwargs and kwargs.get('header'):    
        header = hdulist[0].header
        hdulist.close()
        if 'type' in kwargs and kwargs.get('type')=='u16-bits':
            return data,header
        else:
            return float32(data),header
    else:
        hdulist.close()
        if 'type' in kwargs and kwargs.get('type')=='u16-bits':
            return data,header
        else:
            return float32(data)
def get_full_header(fname,**kwargs):
    '''
    **Description**:
        This function will fetch the header of a file
    **Option**: 
        header: [true/false] Will retuen the header as well. \n
        path: if a path is specified, the file path+fname will be loaded.\n
        optimize: [true/false] if set the function will not verify if the image exist before trying to open it\n
    **Return**:
        return the header of fname
    '''
    if 'path' in kwargs:
        fname = kwargs.get('path')+fname
    if not (kwargs.get('optimized')):
        if not (isfile(fname)):
            print("The file does not exist")
            exit(1)
            return -1
    hdulist = fits.open(fname)
    header = hdulist[0].header
    hdulist.close()
    return header

def saveIm(data,fname,**kwargs):
    '''
    **Description**:
        This function will save an array as a .fits file. Array can be an 2d or 3d array and name is the name of the file. By default it save the image in the current folder
    **Option**:
        header: the header table  \n
        path: if specified, the file will be saved as path_fname.fits \n
        overwrite: [true/false] by default the files can be overwritten. 
        verify: if set to true, the header will be check to see if it strickly comply with the fits format. By default the option if False. 
        dtype: by default the data type if float32. Valid dtype are float16,float32,float64,int16,int32 and int64\n
    '''
    verif = False
    if 'verify' in kwargs:
        verif = kwargs.get('verify') 
    if 'dtype' in kwargs:
        if kwargs.get('dtype') == 'int16': data = asarray(data,dtype=int16)
        if kwargs.get('dtype') == 'uint16': data = data
        if kwargs.get('dtype') == 'int32': data = asarray(data,dtype=int32)
        if kwargs.get('dtype') == 'int64': data = asarray(data,dtype=int64)
        if kwargs.get('dtype') == 'float16': data = asarray(data,dtype=float16)
        if kwargs.get('dtype') == 'float32': data = asarray(data,dtype=float32)
        if kwargs.get('dtype') == 'float64': data = asarray(data,dtype=float64)
        
    else:
        data = asarray(data,dtype=float32)
    if 'header' in kwargs:
        hdu = fits.PrimaryHDU(data,header=kwargs.get('header'))
    else:
        hdu = fits.PrimaryHDU(data,header=None)
    hdulist = fits.HDUList([hdu])
    if 'path' in kwargs:
        p = kwargs.get('path')
        if p[-1]!='/': p+='/'
        fname = p+fname
    if not '.fits' in fname:
        fname+='.fits'
    if 'overwrite' in kwargs: hdulist.writeto(fname,overwrite=kwargs.get('overwrite'),output_verify=verif)
    else: hdulist.writeto(fname,overwrite=True,output_verify=verif)
    hdulist.close()

def update_header(ff,kword,value,**kwargs):
    '''
    **Description**:
        This function will update the header keyword kword with the new value. By default value will be seen as a string 
    **Options**:
        dtype: data type. Can be float, int16, int32 or string.
    **Return**:
        return 0 if successful, 1 if not
    '''
    if not isfile(ff):
        print('file %s does not exist.'%ff)
        return 1
    
    hdulist = fits.open(ff)
    if 'dtype' in kwargs:
        t = kwargs.get('dtype')
        if t=='int16':
            hdulist[0].header[kword] = int16(value)
        elif t=='int32':
            hdulist[0].header[kword] = int32(value)
        elif t=='float':
            hdulist[0].header[kword] = float(value)
        elif t=='string':
            hdulist[0].header[kword] = str(value)
        else:
            return 1
    else:
        hdulist[0].header[kword] = value
    hdulist.writeto(ff,overwrite=True,output_verify='ignore')
    hdulist.close()
    return 0    
#def loadImMemmap(name,xmin,xmax,ymin,ymax):
#    """
#    **Description**:
#        This function open only a section of the fits file in memory defined by xmin,xmax,ymin and ymax. 
#    **special consideration**:
#        This function is primarly used to speed up for loops. Consequently, extra cares need to be taken.\n
#        **BZERO**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 32768.
#        **BSCALE**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 1.
#        **name**:
#            the naming convention is /path/to/file/nameOfFile.fits
#    **Return**:
#        return a numpy array with dtype=int16 with the BZERO and BSCALE applied.
#        """
#    hdulist = fits.open(name,memmap=True,do_not_scale_image_data=True)
#    data = hdulist[0].data[ymin:ymax,xmin:xmax] 
#    hdulist.close()
#    
#    return np.int32(data+32768)
#def loadImMemmapData(name,xmin,xmax,ymin,ymax):
#    """
#    **Description**:
#        This function open only a section of the fits file in memory defined by xmin,xmax,ymin and ymax. 
#    **special consideration**:
#        This function is primarly used to speed up for loops. Consequently, extra cares need to be taken.\n
#        **BZERO**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 32768.
#        **BSCALE**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 1.
#        **name**:
#            the naming convention is /path/to/file/nameOfFile.fits
#    **Return**:
#        return a numpy array with dtype=int16 with the BZERO and BSCALE applied.
#        """
#    hdulist = fits.open(name,memmap=True,do_not_scale_image_data=True)
#    data = hdulist[0].data[ymin:ymax,xmin:xmax] 
#    hdulist.close()
#    
#    return 1.0*data
#def loadImMemmapCube(name,xmin,xmax,ymin,ymax):
#    """
#    **Description**:
#        (only to be used in photon.py the data value must be <2**15) This function open only a section of the fits file in memory defined by xmin,xmax,ymin and ymax. 
#    **special consideration**:
#        This function is primarly used to speed up for loops. Consequently, extra cares need to be taken.\n
#        **BZERO**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 32768.
#        **BSCALE**:
#            Unfortunatly, version <3.0.5 of astropy does not support memory mapping and scaling. Therefore, the BZERO is set automatically to 1.
#        **name**:
#            the naming convention is /path/to/file/nameOfFile.fits
#    **Return**:
#        return a numpy array with dtype=int16 with the BZERO and BSCALE applied.
#        """
#    hdulist = fits.open(name,memmap=True,do_not_scale_image_data=True)
#    data = hdulist[0].data[:,ymin:ymax,xmin:xmax] 
#    hdulist.close()
#    return data
#    
#
#def saveIm(array,name,header=0,path=''):
#    '''Description:\n This function will save an array as a .fits file. Array can be an 2d or 3d array and name is the name of the file. By default it save the image in the current folder
#    \nOption:\n A header can be added    
#    \nReturn:\n This function will return 0
#    \nAuthor:\n Jonathan St-Antoine, 2018-03-25'''
#    array = np.float32(np.array(array)) 
#    
#    if header !=0: hdu = fits.PrimaryHDU(array,header=header)
#    else: hdu = fits.PrimaryHDU(array,header=None)
#    hdulist = fits.HDUList([hdu])
#    
#    hdulist.writeto(path+name+'.fits',overwrite=True)
#
#    hdulist.close()
#    return 0
#
#def getHeader(keyWord,name,path=''):
#    """Description:\n This function return the keyword in a fits file name
#    \nReturn:\n Return the keyword. if the keyword does not exist the funtion will return -1
#    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
#    name,ext = os.path.splitext(name)
#    if os.path.splitext(name)[1]==".fits":
#        name,ext = os.path.splitext(name)
#    if os.path.splitext(name)[1]==".fits":
#        name,ext = os.path.splitext(name)
#   
#    if os.path.isfile(path+name+".fits"):hdulist = fits.open(path+name+'.fits')
#    elif os.path.isfile(path+name+".fits.gz"):
#        
#        hdulist = fits.open(path+name+'.fits.gz')
#    else: return -1
#    try:    
#        key = hdulist[0].header[keyWord]
#        hdulist.close()
#        
#        return key
#    except:
#        hdulist.close()
#        
#        return -1
#    
#def changeHeader(name,keyword,newValue,path=''):
#    """Description:\n This function modify a specific keyword in a fits file name name
#    \nReturn:\n Return 0 if successful, -1 if the keyword does not exist
#    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
#    name,ext = os.path.splitext(name)
#    
#    if os.path.splitext(name)[1]==".fits":
#        name,ext = os.path.splitext(name)
#    if os.path.isfile(path+name+".fits"):data, header = fits.getdata(path+name+".fits",0, header=True)
#    elif os.path.isfile(path+name+".fits.gz"):data, header = fits.getdata(path+name+".fits.gz",0, header=True)
#    try:
#        header[keyword] = newValue
#        fits.writeto(path+name+".fits", data, header=header,output_verify="silentfix",overwrite=True)
#        return 0
#    except: return -1
#def history(name,history,path=''):
#    """Description:\n This function add an history line in the fits header name name
#    \nReturn:\n Return 0 if successful, -1 unable to add history
#    \nAuthor:\n Jonathan St-Antoine, 2018-08-20"""
#    if os.path.splitext(name)[1]==".fits":
#        name,ext = os.path.splitext(name)
#    if os.path.isfile(path+name+".fits"):data, header = fits.getdata(path+name+".fits",0, header=True)
#    elif os.path.isfile(path+name+".fits.gz"):data, header = fits.getdata(path+name+".fits.gz",0, header=True)
#    try:
#        header['history'] = history
#        fits.writeto(path+name+".fits", data, header=header,output_verify="silentfix",overwrite=True)
#        return 0
#    except: return -1

