import fits as fits
import redux as red
import file as fi
from matplotlib import pyplot as plt
from astropy.modeling import models, fitting
import warnings
import photutils as PU
import numpy as np
from astropy.visualization import (ImageNormalize, SqrtStretch,ZScaleInterval)
import sys

class starStats:
    """\nDescription:\n This class is used to get stats of stars selected by the user.\n 
    * dsiplay - Display the last image.
    
    """
    def __init__(self,path):
        self.gain = 5.305
        self.ro = 8.17
        self.path = path
        self.im = np.zeros((10,10))
        self.pxScale = 0.466
        self.fig = plt.figure(1)
        self.ax = self.fig.add_subplot(111)
        
    def SN(self,im,x,y,r,gain,ro):
        """Description:\n Calculate the signal to noise ratio given an image and the position of the star.
        \nReturn:\n Return the signal to noise and the signal i.e., return S/N,S"""
        star = PU.CircularAperture([x,y],r=r)
        sky = PU.CircularAnnulus([x,y],r+5,r+10) 
        phot_table = PU.aperture_photometry(im,[PU.CircularAperture([x,y],r=r),PU.CircularAnnulus([x,y],r+5,r+10) ])
        N = np.sqrt(np.float32(phot_table['aperture_sum_0']/1.)+np.float32(np.float32(phot_table['aperture_sum_1']/sky.area())*star.area())/sky.area()+star.area()*ro**2 *(1.+1./sky.area()))
        S = float(gain)*float(np.float32(phot_table['aperture_sum_0']/1.)-np.float32(np.float32(phot_table['aperture_sum_1']/sky.area())*star.area()))
        return float(S/N),S
    def fwhm(self,imO,x,y):
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
        SNR,S = self.SN(imO,xfit,yfit,r,self.gain,self.ro)
        return p,SNR,S,r
    def onclick(self,event):
        global ix, iy
        ix, iy = int(event.xdata), int(event.ydata)
        p,snr,S,r = self.fwhm(self.im[max(iy-30,0):min(iy+30,self.im.shape[0]),max(ix-30,0):min(ix+30,self.im.shape[1])],ix,iy)
        FWHM = self.pxScale*(p.x_fwhm+p.y_fwhm)/2.
        C = plt.Circle((ix,iy),r,facecolor='None', edgecolor='r')
        self.ax.add_artist(C)
        self.ax.figure.canvas.draw()   
        D = plt.Circle((ix,iy),r+5,facecolor='None', edgecolor='b')
        self.ax.add_artist(D)
        self.ax.figure.canvas.draw() 
        E = plt.Circle((ix,iy),r+10,facecolor='None', edgecolor='b')
        self.ax.add_artist(E)
        self.ax.figure.canvas.draw() 
        amp = p.amplitude.value
        #text = "FWHM:\t%.2f (\")\nAmplitude:\t%2.2f (adu)\nSNR:\t%2.2f\nSignal:\t%2.2f (elect.)\n"%(FWHM,amp,snr,S)        
        #print(text)
        text2 = "FWHM: %.2f (\")\nAmplitude: %2.2f (adu)\nSNR: %2.2f\nSignal: %2.2f (elect.)\n"%(FWHM,amp,snr,S)
        self.ax.text(ix+r+12,iy,text2)
        self.ax.figure.canvas.draw()
        return
    def display(self):
        """Description:\n Main function of this class. The function display the last image. The user can select a star to get the S/N ration, the signal, fwhm and Amplitude of the star.
        """
        fichier = fi.find_last(self.path)
        #open the file
        self.im = fits.loadIm(fichier)
        self.im = np.flipud(self.im)
        
        #subtract the background
        #self.im-=red.sigma_clipped_stats(self.im)[0]
        self.im-=300
        norm = ImageNormalize(self.im,interval=ZScaleInterval(),stretch=SqrtStretch()) 
        self.ax.imshow(self.im,cmap='Greys_r',norm=norm)
        cid = self.fig.canvas.mpl_connect('button_release_event', self.onclick)
        self.fig.canvas.mpl_connect('key_press_event', self.press)
        plt.show(1)
    def press(self,event):
        #print('press', event.key)
        sys.stdout.flush()
        if event.key == 'c':
            self.ax.cla() 
            
            norm = ImageNormalize(self.im,interval=ZScaleInterval(),stretch=SqrtStretch())
            self.ax.imshow(self.im,cmap='Greys_r',norm=norm)
            self.ax.figure.canvas.draw()
SS = starStats("/home/jonathan/")

SS.display()
