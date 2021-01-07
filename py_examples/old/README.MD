# Motion Analysis Lambda

This is a lambda refactor of himms-demo.

# HIMMS Demo

This Get Up and Go Demo uses Wrnch for skeletal tracking. This demo is mostly self contained. 
It does require wrnch to be installed, however the models are part of this repo. This be fixed in the future.

## System Requirements
- Ubuntu 18.04
- wrnch
- CUDA Toolkit (for GPU models)
- Python 3.6
    - opencv-python
    - pandas 
    - numpy
    - matplotlib 
    - pyrealsense2
    - boto3
- libx264-dev

## Installation
1. Clone this repo
2. Run ``bash bootstrap/setupSystem.sh``

This will install all dependencies and libraries. If you have nvidia drivers installed, it will install the GPU model.

## Bootstrap notes:

### Save all modules in lib

1. source venv/bin/activate
2. mkdir -p lib && cd lib
3. pip install -t opencv-python numpy matplotlib pandas pyrealsense ./
4. in the python file:
    - from lib import cv2
### Change import path in all files importing from lib

This can be used when lib is in the same directory as the import
```
import sys
import os

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path + "/lib")
```

This can be used for a lib path outside the lambda file tree 
```
import sys
sys.path.append("/gg/lib")
```