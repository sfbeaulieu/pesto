import file as fi
import sys
import utils as util
path = sys.argv[1]


lfile = fi.find_last(path)
util.command("ds9 "+lfile,[],command_root="/usr/bin/")


