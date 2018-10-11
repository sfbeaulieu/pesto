
import sys
from astropy.io import fits
import numpy as np
import matplotlib.pyplot as plt
from astropy.visualization import (ImageNormalize, SqrtStretch,ZScaleInterval)
import file as fi
import fits as fits
import utils as util

#Description: Simple script that will display the last file in the path=path and send a command with the mouse click.
#The number of click on the image must be even and <=16. If not the script return -1. Once teh click is ok the window must
#be close and the command SET_MROI will be sent automatically with the proper argument

path = sys.argv[1]




#find the last file written in path
lfile = fi.find_last(path)

data = fits.loadIm(lfile)
norm = ImageNormalize(data,interval=ZScaleInterval(),stretch=SqrtStretch()) 

fig = plt.figure(1)
ax = fig.add_subplot(111)
ax.imshow(data,cmap='Greys_r',norm=norm)


# Simple mouse click function to store coordinates
def onclick(event):
    global ix, iy
    ix, iy = event.xdata, event.ydata
    
    data[int(iy)-5:int(iy)+5,:]=1e6
    ax.imshow(data,cmap='Greys_r',norm=norm)    
    ax.figure.canvas.draw()
    
    global coords
    coords.append((ix, iy))
    
    # Disconnect after 2 clicks
    if len(coords) == 8:
        fig.canvas.mpl_disconnect(cid)
        plt.close(1)
    return





coords = []

# Call click func
cid = fig.canvas.mpl_connect('button_press_event', onclick)

plt.show(1)

check = len(coords)

if (check%2)!=0:
    print -1
    sys.exit()
numROI = check/2

for i in range(0,numROI):
    ind = 2*i
    coord1 = np.array(coords[ind])
    coord2 = np.array(coords[ind+1])
    y1 = int(coord1[1])
    y2 = int(coord2[1])
    if y2>y1:
        height = y2-y1
        offset = y1
    else:
        height = y1-y2
        offset = y2
     
    if i!=numROI-1:
        sys.stdout.write(str(offset)+","+str(height)+",")        
    else:
        sys.stdout.write(str(offset)+","+str(height))

	
	


