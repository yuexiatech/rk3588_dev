/*
gst-launch-1.0 -e \
    v4l2src device=/dev/video31 num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! mpph264enc ! h264parse ! mp4mux ! filesink location=/tmp/output.mp4 \
    v4l2src device=/dev/video41 num-buffers=100 ! image/jpeg,width=800,height=600,framerate=30/1 ! jpegdec ! queue ! mix.

gcc my-video3141-confluence-mpp.c -o my-video3141-confluence-mpp ` pkg-config --cflags --libs gstreamer-1.0 `
*/