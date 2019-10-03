from fwhm import analyse as seeing
from file import find_last
from stack_stars import stack_stars
from fits import loadIm
import sys
from astropy.stats import sigma_clipped_stats as sc
'''
This script will find the last image written on disk (on lyra), 
detect 11 stars bwtween 100 adu and 13,000adu and compute the seeing
'''
try:
	if len(sys.argv)==2:
	    root_path = sys.argv[1]
	else:
	    root_path = '/home/data/' 

	seeing = seeing()
	seeing.plate_scale = 0.44
	im_name = find_last(root_path)

	if not im_name:
	#    print("Unable to find the last file written on disk")
	    exit(1)
	#load the last image    
	iimm = loadIm(im_name)#[200:-200,200:-200]

	#remove pedestal and subtract the sky
	iimm-=300
	iimm-=sc(iimm)[0]
	#stack 11 stars
	stack = stack_stars(iimm,pedestal=0,size_of_kernel=25,threshold=100,max_threshold=13000.0,max_stars=11)

	if len(stack)<=0:
		exit(1)


	#compute the model and fwhm
	model = [seeing.fit(im, only_gaussian=True) for im in stack]
	fwhm = [seeing.get_fwhm(G,'gaussian',in_arcsec=True) for G in model]

	#for i in range(len(fwhm)):
	#    print("Star #%2.2d: %2.2f\""%(i+1,fwhm[i]))
	from numpy import mean,std
	print("%2.2f"%(mean(fwhm)),end='')
	exit(0)

except:
	print("-1",end='')
	exit(1)


