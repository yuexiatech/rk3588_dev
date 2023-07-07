#include "protocols.h"
#include <gst/gst.h>

GstElement* rtmp_create_sink() {
  GstElement *sink = gst_element_factory_make("rtmpsink", "rtmp-sink");
  return sink;
}

GstElement* rtp_create_sink() {
  GstElement *sink = gst_element_factory_make("rtpsink", "rtp-sink");
  return sink;
}

GstElement* rtsp_create_sink() {
  GstElement *sink = gst_element_factory_make("rtspsink", "rtsp-sink");
  return sink;
}