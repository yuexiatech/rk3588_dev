# rk 3588 ip 192.168.8.51
# rpdzkj ubuntu ip 192.168.8.90
# neccessary install
apt-get install gstreamer1.0-plugins-bad
apt-get install gstreamer1.0-plugins-good
apt-get install gst-rtsp-server

# ov13855
## stream
gst-launch-1.0 v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=800, height=600 ! autovideosink 

## stream with hardware
gst-launch-1.0 v4l2src device=/dev/video31 num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! mpph264enc ! h264parse ! mp4mux ! filesink location=/tmp/h264.mp4

## picture 
gst-launch-1.0 -v v4l2src device=/dev/video31 num-buffers=10 ! video/x-raw,format=NV12,width=800,height=600 ! mppjpegenc ! multifilesink location=/tmp/test.jpg

# eSP570
## stream
gst-launch-1.0 v4l2src device=/dev/video41 ! image/jpeg,width=640,height=480,framerate=30/1 ! jpegdec ! autovideosink
## record
gst-launch-1.0 v4l2src device=/dev/video41 ! image/jpeg,width=640,height=480,framerate=30/1 ! filesink location=video.mjpg

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

# RTSP
## RTSP server(Not working)
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! rtph264pay name=pay0 pt=96 ! gdppay ! tcpserversink port=8554
## RTSP connect(Not working)
gst-launch-1.0 rtspsrc location=rtsp://192.168.8.51:8554/test ! gdpdepay ! rtph264depay ! avdec_h264 ! autovideosink


# RTMP
export RTMP_SRC="rtmp://matthewc.co.uk/vod/scooter.flv"
gst-launch-1.0 playbin uri=$RTMP_SRC
## send to server (Not working)
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! flvmux streamable=true name=mux ! rtmpsink location="rtmp://172.24.80.1/live/test" audiotestsrc ! voaacenc ! mux.
## read from server(Not working)
gst-launch-1.0 rtmpsrc location="rtmp://192.168.1.100/live/test" ! flvdemux name=demux demux.video ! h264parse ! avdec_h264 ! autovideosink demux.audio ! aacparse ! avdec_aac ! autoaudiosink

# Storage 保存为多个TS文件，并生成一个M3U8文件
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! mpegtsmux name=mux ! hlssink max-files=5 playlist-root=http://192.168.1.100/ 

# Pull/Push
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! qtdemux ! h264parse ! mpegtsmux name=mux ! tcpserversink port=5000

# on-demand 
gst-launch-1.0 playbin uri=https://prod-streaming-video-msn-com.akamaized.net/a8c412fa-f696-4ff2-9c76-e8ed9cdffe0f/604a87fc-e7bc-463e-8d56-cde7e661d690.mp4

# format transform 重新编码成H.265格式，并保存为MP4文件
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! decodebin name=decode ! encodebin profile="video/x-h265" name=encode ! filesink location=Tennis1080p_h265.mp4

# rate conversion
gst-launch-1.0 filesrc location=Tennis1080p.mp4 ! decodebin name=decode ! tee name=t t. ! queue ! x264enc bitrate=1000 ! mp4mux name=mux1 ! filesink location=Tennis1080p_1000.mp4 t. ! queue ! videoscale ! "video/x-raw,width=640,height=480" ! x264enc bitrate=500 ! mp4mux name=mux2 ! filesink location=Tennis1080p_500.mp4 t. ! queue ! videoscale ! "video/x-raw,width=320,height=240" ! x264enc bitrate=200 ! mp4mux name=mux3 ! filesink location=Tennis1080p_200.mp4

# Qos
gst-launch-1.0 -v filesrc location=Tennis1080p.mp4 ! decodebin ! x264enc tune=zerolatency bitrate=1000 ! rtph264pay ! udpsink host=192.168.8.90 port=5000 qos=true

gst-launch-1.0 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! decodebin ! videoconvert ! autovideosink qos=true
    
# multi display
## qualdrable .mp4 video
gst-launch-1.0  \
videomixer name=mix \
sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=1 \
sink_1::xpos=320 sink_1::ypos=0 sink_1::alpha=1 \
sink_2::xpos=0 sink_2::ypos=240 sink_2::alpha=1 \
sink_3::xpos=320 sink_3::ypos=240 sink_3::alpha=1 \
! autovideosink qos=true \
uridecodebin uri=http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_0 \
uridecodebin uri=http://vjs.zencdn.net/v/oceans.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_1 \
uridecodebin uri=https://media.w3.org/2010/05/sintel/trailer.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_2 \
uridecodebin uri=http://mirror.aarnet.edu.au/pub/TED-talks/911Mothers_2010W-480p.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_3

# triple .mp4 video and one camera
gst-launch-1.0  \
videomixer name=mix \
sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=1 \
sink_1::xpos=320 sink_1::ypos=0 sink_1::alpha=1 \
sink_2::xpos=0 sink_2::ypos=240 sink_2::alpha=1 \
sink_3::xpos=320 sink_3::ypos=240 sink_3::alpha=1 \
! autovideosink qos=true \
filesrc location = Tennis1080p.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_0 \
filesrc location = Tennis1080p_200.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_1 \
filesrc location = Tennis1080p_500.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_2 \
v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=320, height=240 ! mix.sink_3

# double camera
gst-launch-1.0  \
videomixer name=mix \
sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=1 \
sink_1::xpos=0 sink_1::ypos=240 sink_1::alpha=1 \
! autovideosink qos=true \
v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=320, height=240 ! mix.sink_0 \
v4l2src device=/dev/video41 ! image/jpeg,width=320,height=240,framerate=30/1 ! jpegdec ! mix.sink_1

gst-launch-1.0 -e \
    v4l2src device=/dev/video31 num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! x264enc ! mp4mux ! filesink location=/tmp/output.mp4 \
    v4l2src device=/dev/video41 ! image/jpeg,width=640,height=480,framerate=30/1 ! jpegdec ! videoscale ! video/x-raw,width=800,height=600 ! queue ! mix.

