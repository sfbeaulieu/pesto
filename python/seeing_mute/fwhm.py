from astropy.modeling import models, fitting
from stack_stars import stack_stars
from fits import loadIm
from astropy.stats import sigma_clipped_stats as sc
import numpy as np
from matplotlib import pyplot as plt
from astropy.utils.exceptions import AstropyWarning
import warnings
warnings.simplefilter('ignore', category=AstropyWarning)
from photutils import CircularAperture as CA
from photutils import CircularAnnulus as CAN
from photutils import aperture_photometry as AP

class analyse:
    def __init__(self):
        self.gain = 5.001
        self.ron = 4.08
        self.plate_scale = 0.44#arcsec per pixels
        self.pedestal=300
        self.default_file = '/home/jonathan/Documents/OMM/Instrument/DRPM/DRPM/190805_0000001238.fits.gz'
    def __residual_star(self,im,f):
        '''
        **Description**:
            This function will return the residue of the image after a model has been subtracted
        **Return**:
            Residual image of size shape(im)
        '''
        y,x = np.shape(im)
        model_image = np.zeros((y,x))
        for Y in range(y):
            for X in range(x):
                model_image[Y,X] = f(Y,X)
        return im-model_image
    def fit(self,im,**kwargs):
        '''
        **Description**:
            This fonction fits a 2d Gaussian and 2d Moffat function on an image. Ideally, the image would be relativelly small and would contain only one star at its center. The amplitude is approximated using numpy amax function
        **Options**:
            -only_moffat: [true/false] return only the moffat function
            -only_gaussian: [true/false] return only the gaussian function
        **Return**:
            returns the moffat and the gaussian function
        '''
        moffat2d = models.Moffat2D()
        gauss2d = models.Gaussian2D()
        moffat2d.amplitude = np.amax(im)#guess the amplitude
        gauss2d.amplitude = np.amax(im)#guess the amplitude
        x,y = np.shape(im)
        moffat2d.x_0 = int(x/2)
        moffat2d.y_0 = int(y/2)
        gauss2d.x_mean = int(x/2)
        gauss2d.y_mean = int(y/2)
        fit_p = fitting.LevMarLSQFitter()
        x_grid,y_grid = np.mgrid[:x,:y]
        M = fit_p(moffat2d,x_grid,y_grid,im)
        G = fit_p(gauss2d,x_grid,y_grid,im)
        if 'only_moffat' and kwargs.get('only_moffat'):
            return M
        if 'only_gaussian' and kwargs.get('only_gaussian'):
            return G
        return M,G
    def get_fwhm(self,f_model,model_type,**kwargs):
        '''
        **Description**:
            Given a gaussian or moffat 2d model, this function return the fwhm.
        **options**:
            -in_arcsec: true/false
        '''            
            
        if model_type=='gaussian':
            if 'in_arcsec' in kwargs and kwargs.get('in_arcsec'):
                return ((f_model.x_fwhm+f_model.y_fwhm)/2.0)*self.plate_scale
            return (f_model.x_fwhm+f_model.y_fwhm)/2.0
        elif model_type == 'moffat':
            beta = f_model.alpha
            alpha = f_model.gamma
            if 'in_arcsec' in kwargs and kwargs.get('in_arcsec'):
                return (2*alpha[0]*np.sqrt(2**(1.0/(beta[0])) -1.0))*self.plate_scale
            return 2*alpha[0]*np.sqrt(2**(1.0/(beta[0])) -1.0)
        else:
            return -1
    def __plot(self,im,**kwargs):
        '''
        **Descriptions**:
            This function will  plot a cross section of an image as well as the gaussian 2d and Mofat 2d best fit curves.
        **options**:
            -in_arcsec: true/false
        **Return**:
            Return the residual of the Gaussian and the Moffat fit
        '''
        y,x = np.shape(im)    
        M,G = self.fit(im)# we fit a gaussian 2d and a moffat 2d
        y_for_fit = np.linspace(1-1,y-1,100)
        
        plt.plot(im[:,int(x/2.0)],'x',label='data')       
        plt.plot(y_for_fit,M(y_for_fit,int(x/2.0)),label='Moffat 2D')
        plt.plot(y_for_fit,G(y_for_fit,int(x/2.0)),label='Gaussian 2D')
        if 'in_arcsec' in kwargs and kwargs.get('in_arcsec'):
            Gfwhm = self.get_fwhm(G,'gaussian',in_arcsec=True)
            Mfwhm = self.get_fwhm(M,'moffat',in_arcsec=True)
            title = "FWHM MOFFAT: %2.2f\", Gaussian %2.2f\""%(Mfwhm,Gfwhm)
        else:
            Gfwhm = self.get_fwhm(G,'gaussian',in_arcsec=False)
            Mfwhm = self.get_fwhm(M,'moffat',in_arcsec=False)
            title = "FWHM MOFFAT: %2.2f, Gaussian %2.2f"%(Mfwhm,Gfwhm)
        plt.title(title)
        plt.legend()
        return self.__residual_star(im,G),self.__residual_star(im,M)


    def __model_test(self,stack,**kwargs):
        '''
        **Description:**
            This function is used to plot the figures in the test model method
        **Options**:
            -show: [True/False] shjow or don't show the plots
            -name_plot: name of the suptitle for the plot
        **Return**:
            Return the fig to be saved, and the stack of the residual for the gaussian and moffat fit
        '''
        columns = 3
        rows = 3
        fig=plt.figure(figsize=(14, 14))
        if 'name_plot' in kwargs:
            plt.suptitle(kwargs.get('name_plot'),fontsize=18)
        res_G = [];
        res_M = [];
        nbStars = len(stack)
        if nbStars>9:
            print("Only the first 9 stars will be displayed")
        for i in range(1, columns*rows +1):
            if nbStars==0:
                break
            fig.add_subplot(rows, columns, i)
            out = self.__plot(stack[i-1])
            res_G.append(out[0])
            res_M.append(out[1])
            nbStars-=1
        if 'show' in kwargs and kwargs.get('show'):
            plt.show()
        return fig,res_G,res_M
    def __name(self,f):
        '''
        Description:
            utility function to extract the name of the file from the path
        '''
        return f.split('/')[-1]


    def __plot_residual(self,res_stack,name,**kwargs):
        '''
        Description:
            this function will call iteratively the function plt.plot() to plot every images in the residual stack. 
        **name**:
            name of the plot suptitle. Usually Gaussian or Moffat.
        **res_stack**:
            Stack of the residual images
        **Options**:
            -robust: true/false-> will use robust statistic
            -show: true/false-> show or not the plots
        **Return**:
            return the fig to be saved.
        '''
        columns = 3
        rows = 3
        max_im = len(res_stack)
        fig=plt.figure(figsize=(14, 14))
        plt.suptitle("2D %s fit residual"%name,fontsize=18)
        for i in range(1, columns*rows +1):
            if max_im==0:
                break
            fig.add_subplot(rows, columns, i)
            if 'robust' in kwargs and kwargs.get('robust'):
                mn,_,st = sc(res_stack[i-1])
                plt.title('mean: %2.2f, std: %2.2f'%(mn,st))
            else:
                plt.title('mean: %2.2f, std: %2.2f'%(np.mean(res_stack[i-1]),np.std(res_stack[i-1])))
            plt.imshow(res_stack[i-1])
            max_im-=1
        if 'show' in kwargs and kwargs.get('show'):
            plt.show()
        return fig
    def sub_sky(self,im,**kwargs):
        '''
        Description:
            This funcltion subtract the sky from an image using sigma clipped stats.
        **Options**
            keep_sky: will return the image and the sky value
            sigma: number of sigma to used
        '''
        if 'sigma' in kwargs:
            sig = kwargs.get('sigma')
        else:
            sig = 5
        if 'keep_sky' in kwargs and kwargs.get('keep_sky'):
            bkp = sc(im,sigma=sig)[0]
            im-=bkp
            return im,bkp
        else:
            return im
    def test_fitting(self,**kwargs):
        '''
        **Description**:
            Given a path where a list of images are, this method will print out a document with the found stars, gaussian and moffat 2d function fitted, and residuals.
            
        **Options**:
            -show: show the plots as the script is executed
            -path: path where the list of images are
            -save_path: path where to save the report 
            -name_file: name of the report
        '''
        from file import listDir
        import matplotlib.backends.backend_pdf
        if 'name_file' in kwargs:
            name_file = kwargs.get('name_file')
        else:
            name_file  = 'rapport_fwhm'
        
        
        if 'path' in kwargs:
            path = kwargs.get('path')
        else:
            path = input('Enter the path to a list of image (.fits/.fits.gz)')
        if path[-1]!='/': path+='/'
        if 'save_path' in kwargs:
            save_path = kwargs.get('save_path')
        else:
            save_path = input("Path where the report will be saved:")
        if save_path[-1]!='/': save_path+='/'
        pdf = matplotlib.backends.backend_pdf.PdfPages("%s%s.pdf"%(save_path,name_file))
        llist = listDir('%s'%path)
        for f in llist:
            im = loadIm(f)
            print("Working on %s"%f)
            im = self.sub_sky(im)
            sstack,fig_stars = stack_stars(im,pedestal=0,threshold=50,max_stars=9,plot=True,name_plot=self.__name(f),return_plot=True,**kwargs)
            pdf.savefig( fig_stars)    
            fig_stars.savefig('/home/noboru/test.pdf')
            graph,resG,resM = self.__model_test(sstack,name_plot=self.__name(f),**kwargs)    
            pdf.savefig(graph)
            pdf.savefig(self.__plot_residual(resG,'Gaussian',robust=True,**kwargs))
            pdf.savefig(self.__plot_residual(resM,'Moffat',robust=True,**kwargs))
        pdf.close()
    
    def EE(self,im,coord,**kwargs):
        '''
        Description:
            Mesure the Encircled Energy given an image and a set of coordinate. Coordinate is a list (x_0,y_0)
        **Options**:
            -debug: will upload automatically an image with a star. no need to enter im and coord
        **Return**:
            return radius at 90 % of the flux
            
        '''
        if 'debug' in kwargs and kwargs.get('debug'):
            del im
            out = input("path+name of the file. (default: %s)"%self.default_file)            
            if not out:
                imm = loadIm(self.default_file)
            else:
                imm = loadIm(out)
            imm-=sc(imm)[0]
            stack = stack_stars(imm,pedestal=0)
            im = stack[0]
            M,G = self.fit(im)
            res_G = self.__residual_star(im,G)
            res_M = self.__residual_star(im,M)
            if sc(res_G)[2]<sc(res_M)[2]:
                x_0 = G.x_mean
                y_0 = G.y_mean
            else:
                x_0 = M.x_0[0]
                y_0 = M.y_0[0]
                
        else:
            x_0,y_0 = coord
            
        y,x = np.shape(im)
        r_max = np.amin([x-x_0,y-y_0])-0.5
        x_plot = np.linspace(0.5,r_max,30)
        y_plot = [AP(im,[CA((x_0,y_0),r)])['aperture_sum_0'] for r in x_plot]
        y_plot = y_plot[:-1]/y_plot[-1] *100.0
        x_plot = x_plot[:-1]
        ninty = x_plot[np.argmin(np.abs(y_plot-90.0))]
        
        if 'plot' in kwargs and kwargs.get('plot'):
            plt.figure()
            plt.plot(x_plot,y_plot,'x')
            plt.ylim([0,100])
            plt.xlabel('radius (pixel)',fontsize=16)
            plt.ylabel('EE (%)',fontsize=16)   
            plt.title('Encircled Energy',fontsize=16)
            plt.plot((ninty,ninty),(0,100),'--',label="r=%2.2f at ~90 percent"%ninty)
            plt.legend()
            plt.show()
        
        return ninty    
            
        
    def get_SN(self,im,coord,**kwargs):
        '''
        **Description:**\n
            image is not corrected 
        **Options**:\n
            -kernel_size: size of the kernel in pixels\n
            -only_fwhm: return only the fwhm in px\n
            -only_fwhm_arcsec: return only the fwhm in \"\n
            -no_fit: will no fit a Gaussian and moffat 2d. Instead it will use the coordinate
            -debug: will launch the test version of the function\n
        '''
        coord_o = np.copy(np.asarray(coord))
        coord = np.asarray(coord,dtype=int)
        if 'kernel_size' in kwargs:
            ks = int(kwargs.get('kernel_size')/2.0)
            
        else:
            ks=15
        if 'debug' in kwargs and kwargs.get('debug'):
            del im
            out = input("path+name of the file. (default is: %s"%self.default_file)            
            if not out:
                imm = loadIm(self.default_file)
            else:
                imm = loadIm(out)
            imm-=self.pedestal
            sky_bkp = sc(imm)[0]
            imm-=sky_bkp
            stack = stack_stars(imm,pedestal=0,size_of_kernel=2*ks)
            im = stack[0]   
        else:
            im-=self.pedestal
            im,sky_bkp = self.sub_sky(im,keep_sky=True) 
        
        y,x = np.shape(im)
        if 'debug' in kwargs and kwargs.get('debug'):
             kernel = im
        else:
            if not np.all([ks+coord[0]<x,coord[0]-ks>0,ks+coord[1]<y,coord[1]-ks>0]):
                print("coordinate is too close to the border")            
                return -1
            kernel = im[coord[1]-ks:coord[1]+ks,coord[0]-ks:coord[0]+ks]
        if 'no_fit' in kwargs and kwargs.get('no_fit'):
            x_0 = coord_o[0]-(coord[0]-ks)
            y_0 = coord_o[1]-(coord[1]-ks)
        else:
            M,G = self.fit(kernel)
            res_G = self.__residual_star(kernel,G)
            res_M = self.__residual_star(kernel,M)
            if sc(res_G)[2]<sc(res_M)[2]:
                fwhm = self.get_fwhm(G,'gaussian')
                x_0 = G.x_mean
                y_0 = G.y_mean
            else:
                fwhm = self.get_fwhm(M,'moffat')
                x_0 = M.x_0[0]
                y_0 = M.y_0[0]
            if 'only_fwhm' and kwargs.get('only_fwhm'):
                return fwhm
            if 'only_fwhm_arcsec' and kwargs.get('only_fwhm_arcsec'):
                return fwhm*self.plate_scale
        
        r = self.EE(kernel,(x_0,y_0))
        r_in = r+2
        r_out = r_in+2
        print(r,r_in,r_out)
        if r_out>y or r_out >x:
            r_out-=1
            if r_out>y or r_out >x:
                print("Error; kernel is too small to compute S/N")
                return -1
        kernel+=sky_bkp# image+sky
        star_ap = CA((x_0,y_0),r)
        sky_ap = CAN((x_0,y_0),r_in,r_out)
        Table = AP(kernel,[star_ap,sky_ap])
        #the given radius is for ~90% of the flux
        S_sky = Table['aperture_sum_1']*self.gain
        S_star = Table['aperture_sum_0']*1.1*self.gain - S_sky/sky_ap.area()#il faut arranger ça!!
        #print(Table)
        print("star: %2.2f, sky: %2.2f e$^-$"%(S_star,S_sky))
        if 'plot' in kwargs and kwargs.get('plot'):        
            fig,ax = plt.subplots(1)
            ax.set_aspect('equal')
            ax.imshow(kernel)
            from matplotlib.patches import Circle
            circ = Circle((x_0,y_0),r,fill=False)
            ax.add_patch(circ)
            circ = Circle((x_0,y_0),r_in,fill=False)
            ax.add_patch(circ)
            circ = Circle((x_0,y_0),r_out,fill=False)
            ax.add_patch(circ)
            plt.show()
        N,SN = self.noise_SN(Table,[star_ap,sky_ap])

        print("S/N: %2.2f"%(SN))
    def noise_SN(self,Table,apertures):
        '''
          Nt: number of elec. integrated in a na pixels aperture\n
          Ns: number of elec. integrated in a na pixels aperture intrinsict to the source\n
          Is: average sky level per pixel. \n
          na: number of pixels in the source aperture_photometry\n
          ns: number of pixels in the sky aperture\n
          sig_r: readout noise\n
          sig_Is: uncertainty on Is\n'''
        na = apertures[0].area()
        ns = apertures[1].area()
        Is = Table['aperture_sum_1']*self.gain / ns
        Ns = Table['aperture_sum_0']*self.gain-na*Is
        Nt = Table['aperture_sum_0']*self.gain
        #sig_Is = np.sqrt((Is+self.ron**2)/ns)
        #sig_Ns = np.sqrt(Nt+na*(sig_Is**2+self.ron**2))
        return np.sqrt(Nt+na*(Is+self.ron**2)/ns + na*self.ron**2),Ns/(np.sqrt(Nt+na*(Is+self.ron**2)/ns + na*self.ron**2))
    def test_SN(self,**kwargs):
        '''
        **Description**:
            Given a path where a list of images are, this method will print out a document with the found stars S/N.
            
        **Options**:
            -show: show the plots as the script is executed
            -path: path where the list of images are
            -save_path: path where to save the report 
            -name_file: name of the report
            -size_of_kernel: size of the kernel
        '''
        from file import listDir
        import matplotlib.backends.backend_pdf
        if 'name_file' in kwargs:
            name_file = kwargs.get('name_file')
        else:
            name_file  = 'rapport_fwhm'
        
        
        if 'path' in kwargs:
            path = kwargs.get('path')
        else:
            path = input('Enter the path to a list of image (.fits/.fits.gz)')
        if path[-1]!='/': path+='/'
        if 'save_path' in kwargs:
            save_path = kwargs.get('save_path')
        else:
            save_path = input("Path where the report will be saved:")
        if save_path[-1]!='/': save_path+='/'
        pdf = matplotlib.backends.backend_pdf.PdfPages("%s%s.pdf"%(save_path,name_file))
        llist = listDir('%s'%path)
        for f in llist:
            im = loadIm(f)
            print("Working on %s"%f)
            im_o = np.copy(im)
            im-=300
            im,sky_bkp = self.sub_sky(im,keep_sky=True)
            
            #sstack,fig_stars = stack_stars(im,pedestal=0,threshold=50,max_stars=9,plot=True,name_plot=self.__name(f),return_plot=True,**kwargs)
            #pdf.savefig(fig_stars)    
            
            Table= stack_stars(im,pedestal=0,threshold=50,max_stars=9,get_only_table=True,**kwargs)
            plt.figure()
            plt.imshow(im_o)
            print(Table['xcentroid'],Table['ycentroid'])
            plt.show()
            for i in range(len(Table['ycentroid'])):
                self.get_SN(im_o,[Table['xcentroid'][i],Table['ycentroid'][i]],plot=True,kernel_size=kwargs.get('size_of_kernel'))
            #graph,resG,resM = self.__model_test(sstack,name_plot=self.__name(f),**kwargs)    
            #pdf.savefig(graph)
            #pdf.savefig(self.__plot_residual(resG,'Gaussian',robust=True,**kwargs))
            #pdf.savefig(self.__plot_residual(resM,'Moffat',robust=True,**kwargs))
        pdf.close()

#analyse = analyse()

#analyse.test_fitting(path='/home/noboru/test_fwhm/',show=False,name_file='rapport_fwhm',save_path='/home/noboru/')

#analyse.get_SN(1,1,plot=True,debug=True,only_fwhm=False,kernel_size=30)

#analyse.EE(1,1,debug=True,plot=True)
#analyse.test_SN(path='/home/jonathan/Documents/OMM/Instrument/DRPM/DRPM/',size_of_kernel=30,save_path='/home/jonathan/',name_file='test.pdf')
