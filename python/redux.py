"""
Small collection of function to interact with user.
Function included are:
  * listStars - list a defined number of stars in the image
  * center - Given an approximate x and y position it will return the precise position of the star
  
Author: Jonathan St-antoine, 2018-03-25
"""
from photutils import DAOStarFinder
from astropy.stats import sigma_clipped_stats
import numpy as np
import photutils as PU

def listStars(Im,threshold=15,fwhm=2,rapid=False):
    '''Description:\n This function find as much stars as it can in the image.
    \nOptions:\n \n*threshold:\n detect only stars over how that amount of standard deviation \n*fwhm:\n approximate fwhm of the stars \n*rapid:\n reduce the number of iteration for the robust statistic of the image background  
    \nReturn:\n Return an numpy array of the form [peak(adu),x position,y position]'''
    if rapid:mean, median, std = sigma_clipped_stats(Im, sigma=3.0, iters=2)
    else:mean, median, std = sigma_clipped_stats(Im, sigma=3.0, iters=5)  
    daofind = DAOStarFinder(threshold*std,fwhm ) 
    Im-=median
    sources = daofind(Im)
    peak,xcent,ycent = sources['peak'],sources['xcentroid'],sources['ycentroid']
    arg = np.flipud(np.argsort(np.array(peak)))
    A = [[peak[i],xcent[i],ycent[i]] for i in arg]
    return np.array(A)
    
def center(x,y,Im,sizeOfSubImange=50,auto=False,threshold=25,rapid=False):
    """Description:\n Given an image and an approximate x and y this function return the coordinate of the true psf. The function first define a subregion default is 50, then it will assess if a com or 2d gaussian is more suitable for the job.  
    \nOptions:\n \n*\n auto: will assess if the com or 2dg is better for the centroids\n
    \n*\n threshold: Im[Im<=threshold*std]=0\n
    \n*\n rapid: calculate the robust stats of the image with only 2 iteration instead of 5\n    
    \nReturn:\n This function return the center of the psf (float)
    \nNote:\n Std is deprecated and will have no effect"""
    shapeY,shapeX = np.shape(Im)
    subIm =Im[max(0,y-sizeOfSubImange/2):min(shapeY,y+sizeOfSubImange/2+1),max(0,x-sizeOfSubImange/2):min(shapeX,x+sizeOfSubImange/2+1)] 
    if rapid: mean, median, std = sigma_clipped_stats(subIm, sigma=3.0, iters=2) 
    else:mean, median, std = sigma_clipped_stats(subIm, sigma=3.0, iters=5)     
    subIm = subIm-median
    subIm[subIm<5*std]=0
    if auto: 
        if (np.amax(subIm-median)>threshold*std):
            newx,newy = PU.centroid_2dg(subIm) 
            return int(x-sizeOfSubImange/2)+newx,int(y-sizeOfSubImange/2)+newy
    newx,newy = PU.centroid_com(subIm)
    
    return int(x-sizeOfSubImange/2)+newx,int(y-sizeOfSubImange/2)+newy