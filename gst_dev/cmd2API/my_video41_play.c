#include <gst/gst.h>
// gst-launch-1.0 v4l2src device=/dev/video41 ! image/jpeg,width=640,height=480,framerate=30/1 ! jpegdec ! autovideosink
// gcc my_video41_play.c -o my_video41_play ` pkg-config --cflags --libs gstreamer-1.0`
int main(int argc, char *argv[]) {
  GstElement *pipeline, *source, *capsfilter, *decoder, *sink;
  GstCaps *caps;
  GstBus *bus;
  GstMessage *msg;

  gst_debug_set_default_threshold(GST_LEVEL_WARNING);
  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source = gst_element_factory_make ("v4l2src", "source");
  capsfilter = gst_element_factory_make ("capsfilter", "capsfilter");
  decoder = gst_element_factory_make ("jpegdec", "decoder");
  sink = gst_element_factory_make ("autovideosink", "sink");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source || !capsfilter || !decoder || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set the device property on the v4l2src element */
  g_object_set (G_OBJECT (source), "device", "/dev/video41", NULL);

  /* Set the caps on the capsfilter element */
  caps = gst_caps_from_string ("image/jpeg,width=640,height=480,framerate=30/1");
  g_object_set (G_OBJECT (capsfilter), "caps", caps, NULL);
  gst_caps_unref (caps);

  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source, capsfilter, decoder, sink, NULL);
  if (gst_element_link_many (source, capsfilter, decoder, sink, NULL) != TRUE) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
}