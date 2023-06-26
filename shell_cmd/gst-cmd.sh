# rk 3588 ip 192.168.8.51
# rpdzkj ubuntu ip 192.168.8.90
# neccessary install
apt-get install gstreamer1.0-plugins-bad
apt-get install gstreamer1.0-plugins-good
apt-get install gst-rtsp-server

# play
gst-launch-1.0 v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=800, height=600 ! autovideosink -v 

# camera video push
gst-launch-1.0 -v v4l2src device=/dev/video31 ! video/x-raw,width=640,height=480,framerate=30/1 ! x264enc ! rtph264pay ! udpsink host=192.168.8.90 port=5000
# camera video  recieve
gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, encoding-name=H264,payload=96 ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink

# .mp4 push - RTP
gst-launch-1.0 -v filesrc location = Tennis1080p.mp4 ! decodebin ! x264enc ! rtph264pay ! udpsink host=192.168.8.90 port=5000
# .mp4 recv - RTP
gst-launch-1.0 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! decodebin ! videoconvert ! autovideosink

# format transforming(Not working)
gst-launch-1.0 filesrc location=Tennis1080p.h264 ! video/x-h264 ! h264parse ! mp4mux ! filesink location=Tennis1080p.mp4

# RTSP server(Not working)
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! rtph264pay name=pay0 pt=96 ! gdppay ! tcpserversink port=8554
# RTSP connect(Not working)
gst-launch-1.0 rtspsrc location=rtsp://192.168.8.51:8554/test ! gdpdepay ! rtph264depay ! avdec_h264 ! autovideosink

gst-launch-1.0 rtspsrc location=rtsp://192.168.8.51:8554/url ! fakesink

# RTMP: need flasher

gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! mpegtsmux name=mux ! hlssink max-files=5

