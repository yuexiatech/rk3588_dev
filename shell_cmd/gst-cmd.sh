# play
gst-launch-1.0 v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=800, height=600 ! autovideosink -v 

# camera video push
gst-launch-1.0 -v v4l2src device=/dev/video31 ! video/x-raw,width=640,height=480,framerate=30/1 ! x264enc ! rtph264pay ! udpsink host=192.168.8.90 port=5000
# camera video  recieve
gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, encoding-name=H264,payload=96 ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink

# .mp4 push 
gst-launch-1.0 -v filesrc location = Tennis1080p.mp4 ! decodebin ! x264enc ! rtph264pay ! udpsink host=192.168.8.90 port=5000
# .mp4 recv
gst-launch-1.0 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! decodebin ! videoconvert ! autovideosink


# format transforming(Not working)
gst-launch-1.0 filesrc location=Tennis1080p.h264 ! video/x-h264 ! h264parse ! mp4mux ! filesink location=Tennis1080p.mp4