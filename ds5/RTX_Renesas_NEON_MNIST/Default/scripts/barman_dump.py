#
# Copyright (C) 2017 ARM Limited. All rights reserved.
#
from arm_ds.debugger_v1 import Debugger
from arm_ds.debugger_v1 import DebugException
from javax.imageio import ImageIO


def parseDScmdRetStr(retval):
    # extract the path from '$10 = "path"' like format
    s = retval.find("\"")    # detect first character of "
    e = retval.rfind("\"")    # detect char " from the end
    return retval[s+1:e]

def getImageDir(ec):
    # get image folder from CurrentExecutionContext
    idir = ec.executeDSCommand('print $idir')
    dir = parseDScmdRetStr(idir)
    # replace \ to /
    dir = dir.replace('\\', '/')
    return dir

def main():

    # Obtain the first execution context
    debugger = Debugger()
    ec = debugger.getCurrentExecutionContext()

    # Assumed the image file is located on the project folder
    imgDir = getImageDir(ec)

    #--- store image ---
    s_adr = 0x20500000     # BARMAN_BUFFER 0x20500000 to 0x20800000 size 0x300000

    size = 0x300000

    #--- save stored parameters to binary file ---
    dscmd = 'dump memory "%s/barman.raw" S:0x%08x +S:0x%08x' % (imgDir, s_adr, size)
    ec.executeDSCommand(dscmd)




if __name__ == '__main__':
    main() 
