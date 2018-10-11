from datetime import datetime

def racine():
    A = datetime.today()
    h,mi,s = str(A.time()).split(':')
    y,m,d = str(A.date()).split('-')
    newDay = d if int(h)>=12 else str(int(d)-1)

    return "%s%s%s"%(y,m,newDay)

def nameOfFile():
    A = datetime.today()
    
    return str(A.date())+".log"
def nameOfFileFoyer():
    A = datetime.today()
    return str(A.date())+".autofocus"
def timeNow():
    A = datetime.today()
    return str(A.time())
path = '/home/pesto/jo/version.6.1/fonction/log/'



def log(text):
    with open(path+nameOfFile(), "a") as myfile: 
        
       myfile.write(timeNow() +'\t'+text+"\n")

def logFoyer(text,pathFoyer=''):
    with open(pathFoyer+nameOfFileFoyer(), "a") as myfile: 
        
       myfile.write(timeNow() +'\t'+text+"\n")

def logCommand(text):
        commandPath = path+'commande/'
        with open(commandPath+"command.log", "a") as myfile:
            myfile.write(timeNow() +'\t'+text+"\n") 
#def logInfo(text):
    




