"""
Small collection of function to interact with user.
Function included are:
  * showImage - display an image using matplotlib
  * getInput - get input of x nb of click
  * multipleInput - same as GetInput but red circle will be draw on the image
  * command - launch a system command.
  * createTmpFolder - creates a temporary folder.
Author: Jonathan St-antoine, 2018-03-25
"""

import subprocess           
import types



def command(CMD,arg,command_root="/bin/"):
    '''Description:\n This function launch a system command using Popen. 
    \nCMD (string):\n The terminal command 
    \narg (list)\n A list of variable that will be added as argument of the command
    \ncommand_root:\n A string to be place in front of the command 
    \nReturn:\n Return the stdout. If there is no stdout the return value is -2.
    \nAuthor:\n Jonathan St-Antoine, 2018-03-25'''
    if not isinstance(arg,types.ListType): 
         A = [];
         A.append(arg)
         arg = A
    arg = [str(a) for a in arg]

    if command_root[-1]!='/':command_root+='/'
    command_root+='./'+CMD
    arg.insert(0,command_root)
    maCommand = " ".join(arg)
    proc = subprocess.Popen([maCommand], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()    
    if not out:	
        return "-2"
    else:
        return out
def createTmpFolder(path=None):
    '''Description:\n This function create an tmp folder in the current directory if path is not set
    \nReturn:\n Return 0
    \nAuthor:\n Jonathan St-Antoine, 2018-05-09'''
    if not path: command("mkdir",["tmp/"])
    else:
        if path[-1]!='/': path+='/'
        if not os.path.isdir(path): command("mkdir",[path+"tmp/"])
    return 0

    