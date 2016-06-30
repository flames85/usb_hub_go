#import sys
#sys.path.append(r"./")

import libUsbHubGo
import time
print 'go test'

def convertSeq2Int(seq):
	try:
		return int(seq)
	except:
		return -1

libUsbHubGo.goMonitor()

while 1:
	seq = raw_input(">")
	nSeq = convertSeq2Int(seq)
	devName = libUsbHubGo.getDevName(nSeq)
	print 'dev-name:',str(devName)
	# time.sleep(1)

print 'exit'
