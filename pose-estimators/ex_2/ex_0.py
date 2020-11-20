import pyrealsense2 as rs
import numpy as np
import matplotlib

# Recorder is just saving the raw data, converting it to color/grayscale is a post-processing step. 
# If you want this capability from rs-convert you need to modify it a bit, or just write a short python 
# script that does what you want. Something like:

pipe = rs.pipeline()
config = rs.config()
config.enabe_device_from_file("filename.bag")
prof = pipe.start(config)
dev = prof.get_device()
playback = dev.as_playback_device()
playback.set_real_time(false); # to avoid dropping frames
clr = rs.colorizer()
clr.set_option(rs.option.color_scheme, 3.0)
while True:
     fs = pipe.wait_for_frames()
     df = fs.get_depth_frame()
     colorized = clr.colorize(df)
     data = colorized.get_data()
     array = np.asanyarray(data)
     matplotlib.image.imsave('name.png', array)