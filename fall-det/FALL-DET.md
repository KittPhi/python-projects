# Senior Thesis: Vision-Based Fall Detection on IoT and offloading computation onto Super Computer.
> Goal is to create a vision-based fall detection methods (without accelerometers) onto an IoT device (low computation) and offload to super computer to do the heavy computation to create an affordable IoT, yet effective Fall Detection solution. 
> Purely vision-based approaches focuses on the frames of video to detect a fall.

1. What image type to use as input?
2. Which Image Dataset to use?
3. (Server) Handles heavy computational Machine Learning Training
    - what type of model?
    - what type of input into CNN?
    - what algorithms to process image?
    - what type of output to send to (IoT)?
    - hardware specs?
4. (IoT) Handles video capture
    - what type of model?
    - what type of input?
    - what type of output?
    - hardware specs?

5. Best results for realtime fall detection?
    - what type of data to use?
    - what type of hardware?

## Fall-Detection-with-CNNs-and-Optical-Flow
- Use Optical Flow Images (sequence of frames contains a person falling) as input to the CNN.
- Evaluated across three datasets (URFD, FDD and Multicam), achieving state-of-the-art results.
- Allows you to use your own dataset along with instructions.
- Three-step training phase.

![image](https://static-01.hindawi.com/articles/wcmc/volume-2017/9474806/figures/9474806.fig.001.svgz)

Figure 1: The system architecture or pipeline: the RGB images are converted to optical flow images, then features are extracted with a CNN, and a FC-NN decides whether there has been a fall or not.

**THESIS RESEARCH**

6. Best results using just a camera?
    - what type of data to use?
    - what type of input?
    - what is the output?
    - what hardware?

Previous approaches based on wearable sensors have provided high detection rates, however potential users are reluctant to wear such a device on an everyday basis, which in parrallel prevented them from becoming popularized. As a consequence, an alternative approach using vision-based methods, a IoT device with camera, has emerged. I believe that the driving reason which lead to the merger of Arm CPU and NVIDIA GPU was caused by the need for a better hardware needed for Internet of Things. They are forcasting that the sales of IoT would skyrocket, with it allowing homes to become smart environment through IoT devices becoming more affordable. Together with the increasing number of cameras in a home would allow an optimal environment for vision-based systems.

In the 2017 publication "Vision-Based Fall Detection with Convolutional Neural Networks", the authors used a vision-based solution using Convolutional Neural Networks to detect if a sequence of frames contained a person failing.

Summary of the paper "Vision-Based Fall Detection with Convolutional Neural Networks":
A. Optical flow images as input into the CNN.
    - Convolutional Neural Netowrks (CNN) was used for fall detection. More accurately, a CNN that learns how to detect falls from "optical flow iamges". 
    - The CNN model is sequentially trained on different datasets.
    - The purpose of using "optical flow images" as input to the network is to allow independence from environment features.
    - The "optical flow images" only represent the motion of the consecutive video frames while ignoring any appearance related information such as color, brightness or contrast. Thus generally using a CNN approach to fall detection.
B. Three-step training phase
    1. Firstly, they train the model on the Imagenet dataset [] to acquire relevant features for image recognition.
    2. Then by following the approach of [], they train the CNN on the UCF101 action dataset []. 
       For that reason, the "optical flow images" of consecutive frames are calculated and then used to teach the network to detect the actions.
    3. Finally, "transfer learning" is applied by reusing the network weights and fine-tunning the classification layers, so the network focuses on the binary problem of fall detection.
C. Model is evaluated in three public image datasets
    - At the time, this was the first time "transfer learning" was applied from the action recognition domain to fall detection. And because the public fall detection databases only contained small samples, it is crucial to use "transfer learning" to address that problem.
D. Achieving state-of-the-art results on all three datasets. 

Sihouettes and Bounding Boxes:
> Researches use meaningful features such as silhouettes, a popular strategy [b] or bounding boxes are extracted from the frames in order to facilitate detection. Notible solutions use those features as input into a classifier. (e.g., Gaussian Mixture Model (GMM), SVM, and MLP) to automatically detect if a fall has occurred.
> A common technique to detect falls is to compute bounding boxes around objects to determine if  

Tracking Systems:
> The use of tracking systems was applied in many research in a close environment to detect falls [a].
> Researchers used connected-components labeling to compute the silhouette of a person and extracting features such as the spatial orientation of the center of the silhouette or its geometric orientation. By combining this information, they are able to detect positions and also falls. [a]

# Vision-Based Fall Detection with Convolutional Neural Networks:

[publication](https://www.hindawi.com/journals/wcmc/2017/9474806/)
- Works only on camera images, using few image samples (10) to determine the occurrence of a fall.

To bring Fall detection to real world deployment:
- Further research on transfer learning with fall detection datasets is needed.
- Using Optical Flow Images involves a heavy computational burden of processing consecutive images.

[github](https://github.com/AdrianNunez/Fall-Detection-with-CNNs-and-Optical-Flow)

[a](T. Lee and A. Mihailidis, “An intelligent emergency response system: Preliminary development and testing of automated fall detection,” Journal of Telemedicine and Telecare, vol. 11, no. 4, pp. 194–198, 2005.
[Google Scholar](https://scholar.google.com/scholar_lookup?title=An%20intelligent%20emergency%20response%20system:%20Preliminary%20development%20and%20testing%20of%20automated%20fall%20detection&author=T.%20Lee%20&author=A.%20Mihailidis&publication_year=2005)


[b](C. Rougier, J. Meunier, A. St-Arnaud, and J. Rousseau, “Robust video surveillance for fall detection based on human shape deformation,” IEEE Transactions on Circuits and Systems for Video Technology, vol. 21, no. 5, pp. 611–622, 2011.
[Google Scholar(https://scholar.google.com/scholar_lookup?title=Robust%20video%20surveillance%20for%20fall%20detection%20based%20on%20human%20shape%20deformation&author=C.%20Rougier&author=J.%20Meunier&author=A.%20St-Arnaud&author=&author=J.%20Rousseau&publication_year=2011)

[c]()
-------------------------------------------------------------------------------
# TODO: Performance Metric to detect a fall, using different datasets and models

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

[medium](https://medium.com/test-pub-zorian-yu/fall-detection-at-the-edge-c1a026c02318)
[github](https://github.com/tyu0912/falling-net/)

-------------------------------------------------------------------------------
# Real-time Vision-based Fall Detection
**ADD TO THESIS**
In the paper [20], researchers presented a real-time vision-based fall detection solution. The system captures temporal features using motion history images, that are used as input to a deep convolutional neural network. 

Read Conclusion to this Paper and cite it for Thesis.

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
## Simulated Falls and Daily Living Activities Data Set Data Set: (uci) 214 GB
Abstract: 20 falls and 16 daily living activities were performed by 17 volunteers with 5 repetitions while wearing 6 sensors (3.060 instances) that attached to their head, chest, waist, wrist, thigh and ankle.

- Data Set Characteristics: Time-Series
- Date Donated 2018-06-06
- Associated Tasks: Classification

[dataset](https://archive.ics.uci.edu/ml/datasets/Simulated+Falls+and+Daily+Living+Activities+Data+Set#)
<<<<<<< HEAD
-------------------------------------------------------------------------------
## Fall Detection using Pose Estimation
- used OpenPifPaf
    - pip install openpifpaf

Sept. 18 2020
[article](https://towardsdatascience.com/fall-detection-using-pose-estimation-a8f7fd77081d)
-------------------------------------------------------------------------------
## A Real-Time Patient Monitoring Framework for Fall Detection
- achieve 99% accuracy using wearable device.
- Conclusion: They propose a framework that uses edge computing where instead of sending data to the cloud, wearable devices send data to a nearby edge device like a laptop or mobile device for real-time analysis.

March 29 2019
[publication](https://www.hindawi.com/journals/wcmc/2019/9507938/)
=======

>>>>>>> 19c8bdac5400d5dca628a10dd0a150edf1c903c9
-------------------------------------------------------------------------------
## cloning a repo inside a repo
```python
rm -rf test2  
git clone https://github.com/eromoe/test2  
git add test2/  
git commit -am 'add files from test2 repo to test repo'  
git push 
```

--------------------------------------------------------------------------------

# Vision-Based Fall Detection with Convolutional Neural Networks
July 14 2017

[article](https://www.hindawi.com/journals/wcmc/2017/9474806/)

--------------------------------------------------------------------------------
# Vision-based fall detection system 

[5], [6]
- The objective is to build model which is able to detect falls from just a simple plain video.
- Using Fall Detection Dataset (FDD) [8]
    - Consists of videos from a single camera in separate locations-Home, Coffee room, Office, and Lecture room. 
    - The frame rate is 25 frames/s and the resolution is 320x240 pixels.

[6](Real-time Vision-based Fall Detection with Motion History Images and Convolutional Neural Networks, 2018, T. HARALDSSON)
--------------------------------------------------------------------------------
# (FDD) Fall Detection Dataset [8]

Fall Detection Dataset: Feb. 27 2013
[8](http://le2i.cnrs.fr/Fall-detection-Dataset)

--------------------------------------------------------------------------------
# (URFD) University of Rzeszow Fall Detection Dataset [11]
- This dataset contains 70 (30 falls + 40 activities of daily living) sequences.
- Fall events are recorded with 2 Microsoft Kinect cameras and corresponding accelerometric data. 
- ADL events are recorded with only one device (camera 0) and accelerometer. Sensor data was collected using PS Move (60Hz) and x-IMU (256Hz) devices.
- The dataset is organized as follows. Each row contains sequence of depth and RGB images for camera 0 and camera 1 (parallel to the floor and ceiling mounted, respectively), synchronization data, and raw accelerometer data. 
- Each video stream is stored in separate zip archive in form of png image sequence. Depth data is stored in PNG16 format and should be rescaled

UR Fall Detection Dataset:
[11](http://fenix.univ.rzeszow.pl/~mkepski/ds/uf.html)
--------------------------------------------------------------------------------
# CVonline: Image Databases

[CVonline: Image Databases](http://homepages.inf.ed.ac.uk/rbf/CVonline/Imagedbase.htm)

[9](http://www.iro.umontreal.ca/~labimage/Dataset/)Multicam Dataset:

[10](https://aws.amazon.com/blogs/publicsector/automating-fall-detection-with-aws-deeplens/)DeepLens Fall Detection Project: