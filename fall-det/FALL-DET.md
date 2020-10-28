# end-to-end app to detect a fall, using different datasets and models

-------------------------------------------------------------------------------
## Fall-Detection-with-CNNs-and-Optical-Flow
- Use Optical Flow Images (sequence of frames contains a person falling) as input to the CNN.
- Evaluated across three datasets (URFD, FDD and Multicam), achieving state-of-the-art results.
- Allows you to use your own dataset along with instructions.
- Three-step training phase.

![image](https://static-01.hindawi.com/articles/wcmc/volume-2017/9474806/figures/9474806.fig.001.svgz)

Figure 1: The system architecture or pipeline: the RGB images are converted to optical flow images, then features are extracted with a CNN, and a FC-NN decides whether there has been a fall or not.

**THESIS RESEARCH**
Vision-Based Fall Detection with Convolutional Neural Networks:
[publication](https://www.hindawi.com/journals/wcmc/2017/9474806/)
- Works only on camera images, using few image samples (10) to determine the occurrence of a fall.
To bring Fall detection to real world deployment:
- Further research on transfer learning with fall detection datasets is needed.
- Using Optical Flow Images involves a heavy computational burden of processing consecutive images.

[github](https://github.com/AdrianNunez/Fall-Detection-with-CNNs-and-Optical-Flow)

-------------------------------------------------------------------------------
## Fall Detection at the Edge

Jetson TX2 Module: ($400)
- Technical Specifications:
- GPU: 256-core NVIDIA Pascal™ GPU architecture with 256 NVIDIA CUDA cores
- CPU: Dual-Core NVIDIA Denver 2 64-Bit CPU
- Quad-Core ARM® Cortex®-A57 MPCore
- Memory: 8GB 128-bit LPDDR4 Memory
- 1866 MHx - 59.7 GB/s
- Storage: 32GB eMMC 5.1
- Power: 7.5W / 15W

Fall Detection at the Edge:
[medium](https://medium.com/test-pub-zorian-yu/fall-detection-at-the-edge-c1a026c02318)

[github](https://github.com/tyu0912/falling-net/)

-------------------------------------------------------------------------------
# Real-time Vision-based Fall Detection
In the paper [20], researchers presented a real-time vision-based fall detection solution. The system captures temporal features using motion history images, that are used as input to a deep convolutional neural network. 

Read Conclusion to this Paper and cite it for Thesis.

[20] Real-time Vision-based Fall Detection
[20] http://www.diva-portal.org/smash/get/diva2:1254131/FULLTEXT01.pdf

-------------------------------------------------------------------------------
# Fall-Detection-PyTorch


[medium](https://medium.com/diving-in-deep/fall-detection-with-pytorch-b4f19be71e80)
[github](https://github.com/nithiroj/Fall-Detection-PyTorch)
[dataset](http://le2i.cnrs.fr/Fall-detection-Dataset?lang=fr)

-------------------------------------------------------------------------------
# SisFall: A fall and movement dataset
Compares different 


[1] SisFall: A fall and movement dataset
Research on fall and movement detection with wearable devices has witnessed promising growth. However, there are few publicly available datasets, all recorded with smartphones, which are insufficient for testing new proposals due to their absence of objective population, lack of performed activities, and limited information.

This research present a dataset of falls and activities of daily living (ADLs) acquired with a self-developed device composed of two types of accelerometer and one gyroscope. It consists of 19 ADLs and 15 fall types performed by 23 young adults, 15 ADL types performed by 14 healthy and independent participants over 62 years old, and data from one participant of 60 years old that performed all ADLs and falls. These activities were selected based on a survey and a literature analysis. We test the dataset with widely used feature extraction and a simple to implement threshold based classification, achieving up to 96% of accuracy in fall detection. An individual activity analysis demonstrates that most errors coincide in a few number of activities where new approaches could be focused. Finally, validation tests with elderly people significantly reduced the fall detection performance of the tested features. This validates findings of other authors and encourages developing new strategies with this new dataset as the benchmark.
[dataset](http://sistemic.udea.edu.co/en/investigacion/proyectos/english-falls/)
[publication](https://www.mdpi.com/1424-8220/17/1/198/htm#B22-sensors-17-00198)

-------------------------------------------------------------------------------
## (uci) Simulated Falls and Daily Living Activities Data Set Data Set: 214 GB
Abstract: 20 falls and 16 daily living activities were performed by 17 volunteers with 5 repetitions while wearing 6 sensors (3.060 instances) that attached to their head, chest, waist, wrist, thigh and ankle.

Data Set Characteristics: Time-Series
Date Donated 2018-06-06
Associated Tasks: Classification

[dataset](https://archive.ics.uci.edu/ml/datasets/Simulated+Falls+and+Daily+Living+Activities+Data+Set#)
-------------------------------------------------------------------------------

## cloning a repo inside a repo
rm -rf test2  
git clone https://github.com/eromoe/test2  
git add test2/  
git commit -am 'add files from test2 repo to test repo'  
git push 
--------------------------------------------------------------------------------
old readme.md
# Vision-based fall detection system [5], [6]
- The objective is to build model which is able to detect falls from just a simple plain video.
- Using Fall Detection Dataset (FDD) [8]
    - Consists of videos from a single camera in separate locations-Home, Coffee room, Office, and Lecture room. 
    - The frame rate is 25 frames/s and the resolution is 320x240 pixels.


# (FDD) Fall Detection Dataset [8]
Fall Detection Dataset: Feb. 27 2013
[8](http://le2i.cnrs.fr/Fall-detection-Dataset)

# (URFD) University of Rzeszow Fall Detection Dataset [11]
- This dataset contains 70 (30 falls + 40 activities of daily living) sequences.
- Fall events are recorded with 2 Microsoft Kinect cameras and corresponding accelerometric data. 
- ADL events are recorded with only one device (camera 0) and accelerometer. Sensor data was collected using PS Move (60Hz) and x-IMU (256Hz) devices.
- The dataset is organized as follows. Each row contains sequence of depth and RGB images for camera 0 and camera 1 (parallel to the floor and ceiling mounted, respectively), synchronization data, and raw accelerometer data. 
- Each video stream is stored in separate zip archive in form of png image sequence. Depth data is stored in PNG16 format and should be rescaled

# CVonline: Image Databases
CVonline: Image Databases:
http://homepages.inf.ed.ac.uk/rbf/CVonline/Imagedbase.htm

[6](Real-time Vision-based Fall Detection with Motion History Images and Convolutional Neural Networks, 2018, T. HARALDSSON)

Multicam Dataset:
[9](http://www.iro.umontreal.ca/~labimage/Dataset/)

UR Fall Detection Dataset:
[11](http://fenix.univ.rzeszow.pl/~mkepski/ds/uf.html)

DeepLens Fall Detection Project:
[10](https://aws.amazon.com/blogs/publicsector/automating-fall-detection-with-aws-deeplens/)