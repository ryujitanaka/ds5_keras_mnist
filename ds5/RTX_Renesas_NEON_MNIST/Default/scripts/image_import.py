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

def loadImage(filepath):

    fp = open(filepath, 'r')
    image = ImageIO.read(fp)
    fp.close()
    
    return image

def storeImage(ec, store_adr, image):

    adr = store_adr

    # grayscale color image
    start_adr = adr
    for x in range(0, image.getWidth()):
        for y in range(0, image.getHeight()):
            pixel = image.getRGB(y, x) & 0xFF   # grayscale and transform to [row][col]
            dscmd = 'memory set_typed S:0x%08x (unsigned int) %d' % (adr, pixel)
            ec.executeDSCommand(dscmd)
            adr += 0x4

    #print ' image  S:0x%08x - S:0x%08x' % (start_adr, adr)

    return adr

def main():

    # Obtain the first execution context
    debugger = Debugger()
    ec = debugger.getCurrentExecutionContext()

    # Assumed the image file is located on the project folder
    imgDir = getImageDir(ec)

    image = loadImage(imgDir + '/scripts/test.jpg')

    #--- store image ---
    s_adr = 0x206C0000     # TESTDATA 0x206C0000 to 0x206C0C40 (size 0xC40)

    e_adr = storeImage(ec, s_adr, image)

    #--- save stored parameters to binary file ---
    dscmd = 'dump binary memory "%s/scripts/ds5_test.bin" S:0x%08x S:0x%08x' % (imgDir, s_adr, e_adr)
    ec.executeDSCommand(dscmd)




if __name__ == '__main__':
    main() 
