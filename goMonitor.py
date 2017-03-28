import sys
sys.path.append("./build")
import libUsbHubGo

def convertSeq2Int(seq):
	try:
		return int(seq)
	except:
		return -1

def help():
	print "----------------------------------------"
	print "  Enter a number to query the usb device name."
	print "  Enter dump for details of all the devices."
	print "  Enter API display the API of libUsbHubGo."
	print "  Enter help to display the help information."
	print "  Enter quit to exit."
	print "----------------------------------------"

def api():
	print "----------------------------------------"
	print "  API[1] goStudy:",libUsbHubGo.goStudy.__doc__
	print "  API[2] goMonitor:",libUsbHubGo.goMonitor.__doc__
	print "  API[3] getUsbDevName:",libUsbHubGo.getUsbDevName.__doc__
	print "  API[4] dumpUsbSeqMapForJson:",libUsbHubGo.dumpUsbSeqMapForJson.__doc__
	print "----------------------------------------"

libUsbHubGo.goMonitor()
	
print "*** Please enter help, if you do not know how to use me. ***\n" 

while 1:
	input = raw_input("> ")
	if input == "help":
		help()
	elif input == "dump":
		json = libUsbHubGo.dumpUsbSeqMapForJson()
		print '>', json
	elif input == "api":
		api()
	elif input == "quit":
		break
	else:
		nSeq = convertSeq2Int(input)
		usbDevName = libUsbHubGo.getUsbDevName(nSeq)
		print '>', usbDevName
