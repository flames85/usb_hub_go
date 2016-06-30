#import sys
#sys.path.append(r"./")

import libUsbHubGo
#import time

def convertSeq2Int(seq):
	try:
		return int(seq)
	except:
		return -1
print "----------------------------------------"
print "API[1] goStudy:",libUsbHubGo.goStudy.__doc__
print "API[2] goMonitor:",libUsbHubGo.goMonitor.__doc__
print "API[3] getUsbDevName:",libUsbHubGo.getUsbDevName.__doc__
print "API[4] dumpUsbSeqMapForJson:",libUsbHubGo.dumpUsbSeqMapForJson.__doc__
print "----------------------------------------"

libUsbHubGo.goMonitor()

while 1:
	# input port sequence with keyboard
	seq = raw_input(">")
	nSeq = convertSeq2Int(seq)
	usbDevName = libUsbHubGo.getUsbDevName(nSeq)
	print 'usb-dev-name:', usbDevName
	json = libUsbHubGo.dumpUsbSeqMapForJson()
	print 'dump-json:', json
	# time.sleep(1)

