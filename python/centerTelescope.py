import numpy as np
import fits as fits
import file as fi
from matplotlib import pyplot as plt
from astropy.visualization import (ImageNormalize, LinearStretch,ZScaleInterval)
import utils as utils


def GetInput(Im,nbOfClick=1,Title=''):
    """Description:\nThis function will display an image. After nbOfClick the window will close
    \nReturn:\n This function will return an array of np.shape(coord)=(nbOfPoint , 2) where x=[i,0] and y=[i,1]"""
    def onclick(event):
        '''Description:\n Function that is used with GetInput.
        \nReturn:\n NULL'''
        global ix, iy
        ix, iy = event.xdata, event.ydata
        coords.append((ix, iy))
        if len(coords) == nbOfClick:
            fig.canvas.mpl_disconnect(cid)
            plt.close(1)
        return  
    Title = 'Select %d Star(s)'%nbOfClick 
    #Im = np.flipud(Im)
    fig = plt.figure(1)
    ax = fig.add_subplot(111)
    norm = ImageNormalize(Im,interval=ZScaleInterval(),stretch=LinearStretch())
    ax.imshow(Im,cmap='Greys_r',origin='lower',norm=norm)
    ax.set_title(Title,fontsize=26)
    global coords        
    coords = [];
    # Call click func
    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    plt.show(1)
    centerCoordinate = [];
    coords = np.array(coords)
    for i in range(0,len(coords[:,0])):
        x,y = coords[i]
        centerCoordinate.append([x,y])
    return np.array(centerCoordinate)

#def findRot():
#    #need to stop the acquisition on the GUI
#    #Need to have an exposure time of less than 2 sec
#    ppl.command("GO",arg1='6',arg2='2',arg3='2')
#    time.sleep(2)
#    fileName = ppl.find_last('/home/data/')
#    Im1 = ppl.loadIm(fileName)
#    ppl.command("GO",arg1='6',arg2='2',arg3='2')
#    time.sleep(2)
#    fileName = ppl.find_last('/home/data/')
#    Im2 = ppl.loadIm(fileName)
#    print "finished"

def mouvement90(x,y,centerX,centerY):
    deltaRa = int(-1*(x-centerX)*pxScale)
    deltaDEC = int(-1*(centerY-y)*pxScale)
    utils.command("teloffset",[deltaRa,deltaDEC])

pxScale = 0.466 #arcsec/pixel

Im = fits.loadIm(fi.find_last("/home/data/"))
coord = GetInput(Im,Title="Click on the star you want to center")
x,y = coord[0]

mouvement90(x,y,int(Im.shape[1]/2.),int(Im.shape[0]/2.))
