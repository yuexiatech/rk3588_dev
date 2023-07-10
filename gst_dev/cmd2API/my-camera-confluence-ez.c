#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline, *v4l2src1, *v4l2src2;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  pipeline = gst_parse_launch ("gst-launch-1.0 -e \
    v4l2src name=src1 device=/dev/video31 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! x264enc ! mp4mux name=mux ! filesink location=/tmp/output.mp4 \
    v4l2src name=src2 device=/dev/video32 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! mix.", NULL);

  /* Set group-id property of v4l2src elements */
  v4l2src1 = gst_bin_get_by_name (GST_BIN (pipeline), "src1");
  g_object_set (v4l2src1, "group-id", 1, NULL);
  gst_object_unref (v4l2src1);

  v4l2src2 = gst_bin_get_by_name (GST_BIN (pipeline), "src2");
  g_object_set (v4l2src2, "group-id", 1, NULL);
  gst_object_unref (v4l2src2);

  /* Set reserved-moov-update-period property of mp4mux */
  GstElement *mp4mux = gst_bin_get_by_name (GST_BIN (pipeline), "mux");
  g_object_set (mp4mux, "reserved-moov-update-period", 1000000000, NULL);
  gst_object_unref (mp4mux);

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
