import pyrealsense2 as rs
import numpy as np

rs.pipeline()
    rs.pipeline.try_wait_for_frames()
    rs.pipeline.wait_for_frames()
rs.pipeline_profile()
    rs.pipeline_profile.get_device()
    rs.pipeline_profile.get_stream()
    rs.pipeline_profile.get_streams()
rs.pipeline_wrapper()
rs.playback()
rs.playback_status()
rs.points()
rs.pose()
rs.pose_frame()
rs.pose_stream_profile()

rs.pipeline
rs.pipeline.get_active_profile()
rs.pipeline.poll_for_frames()
rs.pipeline.start()
rs.pipeline.stop()
rs.pipeline.wait_for_frames()
rs.pipeline.try_wait_for_frames()
pipe = rs.pipeline()

config = rs.config()
config.enable_stream
config.enable_all_streams
config.enable_device
config.enable_device_from_file
config.enable_record_to_file
config.resolve




