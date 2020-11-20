# important directories
/usr/bin/wrModels
/usr/src/wrnchAI/wrSamples/python

# Intel RealSense D455
- realsense-viewer
- v.5.12.9.0
- pip3 install pyrealsense2
- sudo apt install python-usb # used to reset usb
- pip search pyrealsense2
    pyrealsense2 (2.40.0.2483) - Python Wrapper for Intel Realsense SDK 2.0.

# install wrnch requirements
pip3 install visualizer
pip3 install Pillow>=6.2.0
pip3 install numpy==1.16.3
pip3 install protobuf==3.6.1
opencv-python==4.1.0.25
# wrnch
- wrdemo
- [install wrnch](https://devportal.wrnch.ai/wrnchai_edge/user_guides/wrnchai_engine/getting_started/creating_samples.html)
- Once install, they suggest copy wrnch sample code to home dir
  + cp -r /usr/src/wrnchAI/wrSamples/ ~
- The samples can then be built with the following commands
   + cd ~/wrSamples
   + mkdir build
   + cd build
   + cmake ..
   + make
  The built samples will be placed in build/bin
- The models, which are the <model path> command-line argument for some samples, 
  are located in /usr/bin/wrModels

# if install doesn't upgrade then add '--upgrade' flag
pip3 install Pillow>=6.2.0 --upgrade

# if need be, update pip
pip3 install --user pip==20.2.4

# ERROR:
$ pip3 --version
Traceback (most recent call last):
  File "/usr/bin/pip3", line 7, in <module>
    from pip._internal import main
ModuleNotFoundError: No module named 'pip._internal'

# FIX 3:
python3.7 get-pip.py --force-reinstall
$ pip -V
    pip 20.2.4 from /home/ecg/.local/lib/python3.7/site-packages/pip (python 3.7)

# FIX 2 (didn't work):
sudo apt update
sudo apt upgrade
sudo apt install python2.7
# Then Install pip for python 2:
sudo apt install python-pip

# FIX 1 (worked once): 


# ISSUE: 
Traceback (most recent call last):
  File "realsense.py", line 6, in <module>
    frames = pipe.wait_for_frames()
RuntimeError: Frame didn't arrive within 5000

# FIX:
sudo apt install python-usb
sudo apt install python3-usb