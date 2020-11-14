source venv/bin/activate

# Compiling librealsense for Linux using Ubuntu
[ref](https://dev.intelrealsense.com/docs/compiling-librealsense-for-linux-ubuntu-guide)

#update ubuntu
sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade -y

# Update OS Boot and reboot to enforce the correct kernel selection with
sudo update-grub && sudo reboot

uname -r
- 5.4.0-48-generic

# Update Firmware
[ref](https://dev.intelrealsense.com/docs/firmware-update-tool)

rs-fw-update -l
- connected devices:
- 1) Name: Intel RealSense D455, serial number: 035622250880, update serial number: 030223060611, firmware version: 05.12.06.00, USB type: 3.2

Download firmware production version, then Extract
https://dev.intelrealsense.com/docs/firmware-releases

rs-fw-update -f ~/Downloads/D400_Series_Production_FW_5_12_7_100/Signed_Image_UVC_5_12_7_100.bin

------------------------------------------------------------------------------------------
# Linux Intel RealSense SDK 2.0, click "Learn more" below Linux Icon, not the word "Linux"
[link](https://www.intelrealsense.com/sdk-2/)

Installing the packages:
Add Intel server to the list of repositories :
echo 'deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial main' | sudo tee /etc/apt/sources.list.d/realsense-public.list

It is recommended to backup /etc/apt/sources.list.d/realsense-public.list file in case of an upgrade.
Register the server’s public key :
sudo apt-key adv --keyserver keys.gnupg.net --recv-key 6F3EFCDE

Refresh the list of repositories and packages available :
sudo apt-get update

In order to run demos install:
sudo apt-get install librealsense2-dkms
sudo apt-get install librealsense2-utils

The above two lines will deploy librealsense2 udev rules, kernel drivers, runtime library and executable demos and tools. Reconnect the Intel RealSense depth camera and run: realsense-viewer

Developers shall install additional packages:
sudo apt-get install librealsense2-dev
sudo apt-get install librealsense2-dbg

With dev package installed, you can compile an application with librealsense using 
`g++ -std=c++11 filename.cpp -lrealsense2` or an IDE of your choice.

Verify that the kernel is updated :
`modinfo uvcvideo | grep "version:"` should include realsense string

------------------------------------------------------------------------------------------