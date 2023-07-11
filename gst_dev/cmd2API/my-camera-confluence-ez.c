#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  GError *error = NULL;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  pipeline = gst_parse_launch ("v4l2src device=/dev/video31  num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! x264enc ! mp4mux reserved-moov-update-period=10 ! filesink location=/tmp/output.mp4 \
    v4l2src device=/dev/video32  num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! mix.", &error);

  if (!pipeline) {
    g_printerr ("Failed to parse launch: %s\n", error->message);
    g_error_free (error);
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
  
  return 0;
}
