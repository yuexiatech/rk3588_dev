#include "vod.h"
#include <gst/gst.h>

void vod_add_elements(GstElement *pipeline) {
  GstElement *playbin = gst_element_factory_make("playbin", "playbin");

  gst_bin_add(GST_BIN(pipeline), playbin);
}