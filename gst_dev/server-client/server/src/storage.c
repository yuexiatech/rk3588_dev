#include "storage.h"
#include <gst/gst.h>

GstElement* storage_create_filesink() {
  GstElement *sink = gst_element_factory_make("filesink", "file-sink");
  return sink;
}