#### 1. What is usb_hub_go?
Monitoring usb serial port connect or disconnect.


#### 2. How to build?
Install extra packages
```
sudo apt-get install python-dev 
```

Build c++ shared lib for python module
```bash
mkdir build && cd build
cmake .. -DPYTHON_INCLUDE_PATH=/usr/include/python2.7/
make
```

#### 3. How to use?
You must exec `goStudy.py` for the first time.
```bash
python goStudy.py
```
When you see `# Study ok! Bye`, you can run the `goMonitor.py` for __monitoring usb serial port__.

```bash
python goMonitor.py
```

