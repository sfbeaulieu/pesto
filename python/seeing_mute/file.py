"""
Small collection of function to get information on the containing folders regarding fits file.
Function included are:
  * find_last - return the last fits file of the directory
  * listDir - return the list of fits file in a directory
  * firstFits - return the first fits of a directory
  
  

Author: Jonathan St-antoine, 2018-03-25
"""

import types
import os.path
from natsort import natsorted
import numpy as np
from tqdm import tqdm

def find_last(path):
    """Description:\n This fonction will find the last image written on disk under all directory path and all its subdirectory. 
    \nReturn:\n Return the path and name of the file"""
    files = [os.path.join(root,f) for root,_,the_files in os.walk(path) for f in the_files if f.lower().endswith(".fits")]
    if len(files)!=0:
        List = sorted(files,key=os.path.getctime)
        most_recent_file = str(List[-1])
    else:
        most_recent_file = "-1"
    return most_recent_file
  
def listDir(path,sub=True):
    """Description:\n This function will list all the .fits file in directory and subdirectory path.
    \nReturn:\n This function return a list of path+name of file
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
    if not sub:
        files = [f for f in os.listdir(path) if f.endswith('.fits')]
        files = natsorted(files)   
        return np.array(files)
    elif sub:
        L = [];
        for root,Dir,files in os.walk(path):
            for filename in files:
                L.append(os.path.join(root,filename))
        fichier = [f for f in L if f.endswith('.fits') or f.endswith('.fits.gz')]
        return  np.array(natsorted(np.array(fichier)))
        
    
    
def firstFits(path):
    """Description:\n This function will find the first image in a directory.
    \nReturn:\n This function return the path+name fo the first fits file
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
    files = [f for f in os.listdir(path) if f.endswith('.fits')]
    files = natsorted(files)   
    return files[0]
    
def searchHeader(path,keyword):  
    """Description:\n This function will look inside every file inside the path=path and all its subdirectory
    \n
    \nReturn:\n This function return the path+name fo the first fits file
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25"""
    import fits as fits
    if not isinstance(keyword,types.ListType): return -1
    files = listDir(path,sub=True)
    find = [ image for image in tqdm(files) for key in keyword if fits.getHeader(key,image)!=-1]
    return find
    