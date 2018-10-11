import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np

from astropy.io import fits
from astropy.visualization import (MinMaxInterval,ImageNormalize,LinearStretch, SqrtStretch,ZScaleInterval)
import sys
import os
#Path = "/home/data/development/"
path = sys.argv[1]
refreshR = sys.argv[2]
#Path = "/home/data/"

def find_between_r( s, first, last ):
#function that extract a string from character 1 and character 2. 
    try:
        start = s.rindex( first ) + len( first )
        end = s.rindex( last, start )
        return s[start:end]
    except ValueError:
        return ""

def find_last(path):
	#find the last file written on disk in Path=path and all its subdirectory 
	files = [os.path.join(root,f) for root,_,the_files in os.walk(path) for f in the_files if f.lower().endswith(".fits")]
	if len(files)!=0:
		List = sorted(files,key=os.path.getctime)
		most_recent_file = str(List[-1])
	else:
		most_recent_file = "-1"
	return most_recent_file

last_file =  find_last(path)

if (last_file != "-1"):
	increment = int(find_between_r(last_file,"_",".fits"))
	name = find_between_r(last_file,"/","_")
	tup = os.path.split(last_file)
	path = tup[0]+"/"
else:
	increment = 1

fig = plt.figure()
plt.title('Pine Nuts Video V 2.0')
ax1 = plt.subplot2grid((5,5), (0,0), colspan=4,rowspan=4)
ax2 = plt.subplot2grid((5,5), (0,4), rowspan=4,sharey=ax1)
ax3 = plt.subplot2grid((5,5), (4, 0), colspan=4,sharex=ax1)
ax1.set(aspect=1)
plt.tight_layout()


#path: path where to find the last file, refreshR: The rate of refresh
current_file = find_last(path)
hdulist = fits.open(current_file)
data = hdulist[0].data
hdulist.close()
X = len(data[1,:])
Y = len(data[:,1])

norm = ImageNormalize(data,interval=ZScaleInterval(),stretch=LinearStretch())

previous_file = " "

def animate(i):
	current_file = find_last(path)
	if current_file==previous_file:
		i=i
	else:
		hdulist = fits.open(current_file)
		data = hdulist[0].data
		norm = ImageNormalize(data,interval=ZScaleInterval(),stretch=LinearStretch())
		hdulist.close()
		ax1.clear()
		#ax1.imshow(data,clim=(Mn-5*Std,Mn+5*Std))
		ax1.imshow(data,cmap='Greys_r',origin='lower',norm=norm)

ani = animation.FuncAnimation(fig,animate,interval=refreshR)
plt.show()

