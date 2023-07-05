#include "protocols.h"
#include <gst/gst.h>

GstElement* rtmp_create_src() {
  GstElement *src = gst_element_factory_make("rtmpsrc", "rtmp-src");
  return src;
}

GstElement* rtp_create_src() {
  GstElement *src = gst_element_factory_make("rtpsrc", "rtp-src");
  return src;
}

GstElement* rtsp_create_src() {
  GstElement *src = gst_element_factory_make("rtspsrc", "rtsp-src");
  return src;
}