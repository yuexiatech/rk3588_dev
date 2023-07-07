#include "bandwidth.h"
#include <gst/gst.h>

void bandwidth_add_elements(GstElement *pipeline) {
  GstElement *qos = gst_element_factory_make("qos", "qos");

  g_signal_connect(qos, "qos", G_CALLBACK(bandwidth_on_qos), NULL);

  gst_bin_add(GST_BIN(pipeline), qos);
  gst_element_link(qos, pipeline);
}

void bandwidth_on_qos(GstElement *qos, guint64 processed, guint64 dropped) {
  // Adjust the bitrate of the encoded video based on network conditions
}