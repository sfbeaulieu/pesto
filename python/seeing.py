import fits as fits
import redux as red
import file as fi
import utils as utils
import numpy as np
from astropy.visualization import (MinMaxInterval,ImageNormalize, SqrtStretch,ZScaleInterval)
from matplotlib import pyplot as plt
from astropy.modeling import models, fitting
import warnings
import photutils as PU
from astropy.stats import gaussian_fwhm_to_sigma
#from Pesto import Pesto as pesto

pxScale = 0.466
ro = 32
gain = 1.65

def SN(im,x,y,r,gain,ro):
    star = PU.CircularAperture([x,y],r=r)
    sky = PU.CircularAnnulus([x,y],r+5,r+10) 
    #extract flux from the image
    apers = [star,sky]
    phot_table = PU.aperture_photometry(im,apers)
    sky_mean = np.float32(phot_table['aperture_sum_1']/sky.area())
    sky_sum = np.float32(sky_mean*star.area())  
    star_sum = np.float32(phot_table['aperture_sum_0']/1.)
    #using noise() to calculate the noise    
    N = np.sqrt(star_sum+sky_sum/sky.area()+star.area()*ro**2 *(1.+1./sky.area()))
    #Sn-sky
    S = float(gain)*float(star_sum-sky_sum)
    return float(S/N),S

def onclick(event):
    global ix, iy
    ix, iy = int(event.xdata), int(event.ydata)
    p,snr,S,r = fwhm(im[max(iy-30,0):min(iy+30,im.shape[0]),max(ix-30,0):min(ix+30,im.shape[1])],ix,iy)
    FWHM = pxScale*(p.x_fwhm+p.y_fwhm)/2.
    C = plt.Circle((ix,iy),r,facecolor='None', edgecolor='r')
    ax.add_artist(C)
    ax.figure.canvas.draw()   
    D = plt.Circle((ix,iy),r+5,facecolor='None', edgecolor='b')
    ax.add_artist(D)
    ax.figure.canvas.draw() 
    E = plt.Circle((ix,iy),r+10,facecolor='None', edgecolor='b')
    ax.add_artist(E)
    ax.figure.canvas.draw() 
    amp = p.amplitude.value
    print("FWHM:\t%.2f (\")\nAmplitude:\t%2.2f (adu)\nSNR:\t%2.2f\nSignal:\t%2.2f (elect.)\n"%(FWHM,amp,snr,S))
    return


def fwhm(imO,x,y):
    im = np.copy(imO)
    im[im<4*red.sigma_clipped_stats(im)[2]+red.sigma_clipped_stats(im)[0]]=0
    xfit,yfit = PU.centroid_com(im)
    g =  models.Gaussian2D(amplitude=np.amax(im), x_mean=xfit,y_mean=yfit, x_stddev=4,y_stddev=4)
    fit_p = fitting.LevMarLSQFitter()
    y, x = np.mgrid[:im.shape[0], :im.shape[1]]
    with warnings.catch_warnings():
        warnings.simplefilter('ignore')
        p = fit_p(g, x, y, im)
    r = (p.x_stddev+p.y_stddev)
    SNR,S = SN(imO,xfit,yfit,r,gain,ro)
    return p,SNR,S,r

#find the last file written on disk
fichier = fi.find_last("/home/jonathan/")
#open the file
im = fits.loadIm(fichier)
im = np.flipud(im)

#subtract the background
im-=red.sigma_clipped_stats(im)[0]
norm = ImageNormalize(im,interval=ZScaleInterval(),stretch=SqrtStretch()) 

fig = plt.figure(1)
ax = fig.add_subplot(111)
ax.imshow(im,cmap='Greys_r',norm=norm)


cid = fig.canvas.mpl_connect('button_release_event', onclick)


plt.show(1)
