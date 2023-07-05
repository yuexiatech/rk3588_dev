#include "playback.h"
#include <gst/gst.h>

GstElement* playback_create_playbin() {
  GstElement *playbin = gst_element_factory_make("playbin", "playbin");
  return playbin;
}