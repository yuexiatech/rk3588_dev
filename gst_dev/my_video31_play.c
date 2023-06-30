#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline, *source, *convert, *scale, *capsfilter, *sink;
  GstCaps *caps;
  GstBus *bus;
  GstMessage *msg;

  gst_debug_set_default_threshold(GST_LEVEL_WARNING);
  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source = gst_element_factory_make ("v4l2src", "source");
  convert = gst_element_factory_make ("videoconvert", "convert");
  scale = gst_element_factory_make ("videoscale", "scale");
  capsfilter = gst_element_factory_make ("capsfilter", "capsfilter");
  sink = gst_element_factory_make ("autovideosink", "sink");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source || !convert || !scale || !capsfilter || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set the device property on the v4l2src element */
  g_object_set (G_OBJECT (source), "device", "/dev/video31", NULL);

  /* Set the caps on the capsfilter element */
  caps = gst_caps_from_string ("video/x-raw, width=800, height=600", pixel-aspect-ratio=1/1");
  g_object_set (G_OBJECT (capsfilter), "caps", caps, NULL);
  gst_caps_unref (caps);

  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source, convert, scale, capsfilter, sink, NULL);
  if (gst_element_link_many (source, convert, scale, capsfilter, sink, NULL) != TRUE) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
}