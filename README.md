#### what is usb_hub_go?
Monitoring usb serial port connect or disconnect.


#### How to build?
extra packages
```
sudo apt-get install python-dev 
```

```bash
mkdir build && cd build
cmake .. -DPYTHON_INCLUDE_PATH=/usr/include/python2.7/
make
```

#### How to run?
1. python goStudy.py [for study mode]
2. python goTest.py [for test mode]
