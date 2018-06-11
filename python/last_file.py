import os
import sys

if len(sys.argv) != 2:
  
  sys.stdout.write("-2")
  sys.exit()
path = str(sys.argv[1])

#path = '/home/jonathan/data/'
#function that extract a string between two character 
def find_between_r( s, first, last ):
    try:
        start = s.rindex( first ) + len( first )
        end = s.rindex( last, start )
        return s[start:end]
    except ValueError:
        return ""
        
#files = sorted([os.path.join(root,f) for root,_,the_files in os.walk(path) for f in the_files if f.lower().endswith(".fits")], key=os.path.getctime, reverse = True)
files = [os.path.join(root,f) for root,_,the_files in os.walk(path) for f in the_files if f.lower().endswith(".fits")]
if len(files)!=0:
    List = sorted(files,key=os.path.getctime)
    most_recent_file = find_between_r(str(List[-1]),"_",".fits")
    
      

else:
    most_recent_file = "-1"

sys.stdout.write(most_recent_file)
