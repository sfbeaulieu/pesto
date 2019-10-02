
import warnings

from astropy.utils.exceptions import AstropyDeprecationWarning
import numpy as np

#im = loadIm('/home/noboru/190710_0000001126.fits.gz')


warnings.simplefilter('ignore', category=AstropyDeprecationWarning)

def stack_stars(im,fwhm=5,threshold=100,pedestal=300,**kwargs):
    """
    **Description**:
        This function returns a stack of *max_stars* form an image
    **Options**:
        -fwhm: fwhm of the kernel in pixel\n
        -threshold: min peak of the stars\n
        -get_table: [true/false] will return the table as well\n
        -size_of_kernel: default is 20 \n
        -all IRAFStarFinder argument works here\n
        -name_plot: name of the plot\n
        -return_plot: [true/false] return fig\n
        -show: [true/false] show plot\n
        -get_only_table: will return the Iraf's star find table\n
        -max_threshold: peak value in adu\n
    """
    if 'size_of_kernel' in kwargs: kern = int(kwargs.get('size_of_kernel')/2.0)
    else: kern = 10    
    from photutils import IRAFStarFinder as ISF
    
    x,y = np.shape(im)
    im-=pedestal
    ISF = ISF(threshold,fwhm)
    ISF.minsep_fwhm = 10
    ISF.exclude_border = True
    if 'max_threshold' in kwargs:
        ISF.peakmax = kwargs.get('max_threshold')
    if 'sharplo' in kwargs:ISF.sharplo = kwargs.get('sharplo')
    if 'sharphi' in kwargs:ISF.sharphi = kwargs.get('sharphi')
    if 'roundlo' in kwargs:ISF.roundlo = kwargs.get('roundlo')
    if 'roundhi' in kwargs:ISF.roundhi = kwargs.get('roundhi')
    if 'max_stars' in kwargs:ISF.brightest =  kwargs.get('max_stars')
          
    ITable = ISF.find_stars(im)
    if 'get_only_table' in kwargs and kwargs.get('get_only_table'):
        
        if len(ITable['ycentroid'])<=0:
            return -1
        return ITable 
    stack = [ im[int(ITable['ycentroid'][i])-kern:int(ITable['ycentroid'][i])+kern,int(ITable['xcentroid'][i])-kern:int(ITable['xcentroid'][i])+kern] for i in range(len(ITable['xcentroid'])) if np.all([ITable['ycentroid'][i]-kern>=0,ITable['ycentroid'][i]+kern<y,ITable['xcentroid'][i]-kern>=0,ITable['xcentroid'][i]+kern<x])]
    nbStars = len(stack)
    if nbStars<=0:
        return -1
    if 'plot' in kwargs and kwargs.get('plot'):
        from matplotlib import pyplot as plt
        fig=plt.figure(figsize=(14, 14))
        if 'name_plot' in kwargs:
            plt.suptitle(kwargs.get('name_plot'),fontsize=18)
        columns = 3
        rows = 3
        for i in range(1, columns*rows +1):
            if nbStars==0:
                break
            fig.add_subplot(rows, columns, i)
            
            plt.imshow(stack[i-1])
            nbStars-=1
        if 'show' in kwargs and kwargs.get('show'):
            
            plt.show()
    if 'get_table' and kwargs.get('get_table'):
        if 'return_plot' and kwargs.get('return_plot'): 
            return np.asarray(stack),ITable,fig
        return np.asarray(stack),ITable
    else:
        if 'return_plot' and kwargs.get('return_plot'): 
            return np.asarray(stack),fig
        return np.asarray(stack)
    
#stack_stars(im,plot=True,size_of_kernel=40)

