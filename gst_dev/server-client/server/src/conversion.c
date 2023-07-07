#include "conversion.h"
#include <gst/gst.h>

void conversion_add_elements(GstElement *pipeline) {
  GstElement *encoder = gst_element_factory_make("x264enc", "encoder");
  GstElement *videorate = gst_element_factory_make("videorate", "videorate");
  GstElement *videoscale = gst_element_factory_make("videoscale", "videoscale");

  gst_bin_add_many(GST_BIN(pipeline), encoder, videorate, videoscale, NULL);
  gst_element_link_many(encoder, videorate, videoscale, NULL);
}