import file as f
import fits as fits
import sys
import numpy as np

path = sys.argv[1]

Im = fits.loadIm(f.find_last(path))

sys.stdout.write(str(int(np.median(Im)))+","+str(int(np.amax(Im))))
