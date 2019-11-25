import numpy as np
from Pesto import Pesto as pesto
import sys
import file as fi
import fits as fits
import redux as red
from astropy.modeling import models, fitting
import warnings
from tqdm import tqdm
from matplotlib import pyplot as plt
pesto = pesto()
pxScale = 0.466

def fwhm(im,x,y):
    yMax,xMax = np.shape(im)
    subIm = np.copy(im[max(int(y)-30,0):min(int(y)+30,yMax),max(int(x)-30,0):min(int(x)+30,xMax)])
    g =  models.Gaussian2D(amplitude=np.amax(subIm), x_mean=x,y_mean=y, x_stddev=4,y_stddev=4)
    fit_p = fitting.LevMarLSQFitter()
    yf, xf = np.mgrid[:im.shape[0], :im.shape[1]]
    with warnings.catch_warnings():
        warnings.simplefilter('ignore')
        p = fit_p(g, xf, yf, im)
    return pxScale*(p.x_fwhm+p.y_fwhm)/2.


def analyse(im,maxStars):
    im-=red.sigma_clipped_stats(im)[0]
    L = red.listStars(im,fwhm=10)
    if len(L)>maxStars:
        L=L[0:maxStars]
    if len(L)>=1:
        FWHM = [fwhm(im,stars[1],stars[2]) for stars in tqdm(L)]
        if (len(FWHM)>1):    
            return np.mean(np.array(FWHM))
        if (len(FWHM)==1):
            return FWHM
        else:
            return -1
    else:
        return -1


nbIm = int(sys.argv[1])
size = int(sys.argv[2])
nbStars = int(sys.argv[3])


print("Making sure the temporary folder is empty...")
pesto.eraseTmpImages()


current_position = pesto.get_foyer()
print("The current position of the focus is %d. Do you want to proceed?"%current_position)
IN = sys.stdin.readline()
if not "yes" in IN:
    sys.exit(0)

position = np.asarray(np.linspace(current_position-size/2.,current_position+size/2.,10),dtype=int)

pesto.open_shutter()
pesto.stop()



X = [];   
Y = []; 
for pos in position:
    print("Set the focuser to position %d please wait..."%pos)
    if (pesto.set_foyer(pos)==-1):
        print("Unable to reach focuse position %d. The current position if %d. Do you want to proceed with this position?"%(pos,pesto.get_foyer()))
        IN = sys.stdin.readline()
        if not "yes" in IN:
            sys.exit(0)
        
            
        
    X.append(pesto.get_foyer())
    pesto.takeTmpImages(nbIm)
    L = fi.listDir(pesto.path_tmp())
    laList = [analyse(fits.loadIm(name),nbStars) for name in tqdm(L)]
    
    if len(laList)>1:
        Y.append(np.mean(laList))
    else:
        Y.append(laList)
    pesto.eraseTmpImages()

X = np.array(X)
Y = np.array(Y)
X =X[~np.isnan(Y)]
Y = Y[~np.isnan(Y)]

plt.figure()
plt.plot(X,Y,'ro')
plt.xlabel("Position ",fontsize=18)
plt.ylabel("FWHM",fontsize=18)
a,b,c = np.polyfit(X,Y,2)
x = np.linspace(X[0],X[-1],100)
plt.plot(x,a*x**2+b*x+c,'b')
bestSeeing = np.amin(a*x**2+b*x+c)
bestPos = x[np.argmin(a*x**2+b*x+c)]
plt.title('Best position is %d @ fwhm=%f'%(bestPos,bestSeeing),fontsize=24)
plt.show()
print("Best fit is %d at %d. Do you want to go to this position?"%(bestSeeing,bestPos))
IN = sys.stdin.readline()
if not "yes" in IN:
    sys.exit(0)
else:
    pesto.set_foyer(int(bestPos))
    


