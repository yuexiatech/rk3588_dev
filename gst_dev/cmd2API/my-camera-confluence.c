#include <gst/gst.h>
/*
gst-launch-1.0 -e \
    v4l2src device=/dev/video31 num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! x264enc ! mp4mux reserved-moov-update-period=10 ! filesink location=/tmp/output.mp4 \
    v4l2src device=/dev/video32 num-buffers=100 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! mix.

gcc my-camera-confluence.c -o my-camera-confluence ` pkg-config --cflags --libs gstreamer-1.0 `
*/
int main(int argc, char *argv[]) {
  GstElement *pipeline, *source1, *source2, *filter1, *filter2, *converter1, *converter2, *queue1, *queue2, *mixer, *encoder, *muxer, *sink;
  GstCaps *caps;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source1 = gst_element_factory_make ("v4l2src", "source1");
  source2 = gst_element_factory_make ("v4l2src", "source2");
  filter1 = gst_element_factory_make ("capsfilter", "filter1");
  filter2 = gst_element_factory_make ("capsfilter", "filter2");
  converter1 = gst_element_factory_make ("videoconvert", "converter1");
  converter2 = gst_element_factory_make ("videoconvert", "converter2");
  queue1 = gst_element_factory_make ("queue", "queue1");
  queue2 = gst_element_factory_make ("queue", "queue2");
  mixer = gst_element_factory_make ("videomixer", "mixer");
  encoder = gst_element_factory_make ("x264enc", "encoder");
  muxer = gst_element_factory_make ("mp4mux", "muxer");
  sink = gst_element_factory_make ("filesink", "sink");

  /* Set the device property on the source elements */
  g_object_set (source1, "device", "/dev/video31", NULL);
  g_object_set (source2, "device", "/dev/video32", NULL);

  /* Set the num-buffers property on the source elements */
  g_object_set (source1, "num-buffers", 100, NULL);
  g_object_set (source2, "num-buffers", 100, NULL);

  /* Create the caps */
  caps = gst_caps_new_simple ("video/x-raw",
                              "format", G_TYPE_STRING, "NV12",
                              "width", G_TYPE_INT, 800,
                              "height", G_TYPE_INT, 600,
                              "framerate", GST_TYPE_FRACTION, 30, 1,
                              NULL);

  /* Set the caps on the capsfilter elements */
  g_object_set (filter1, "caps", caps, NULL);
  g_object_set (filter2, "caps", caps, NULL);

  /* Set the reserved-moov-update-period property on the mp4mux element */
  g_object_set (muxer, "reserved-moov-update-period", 10, NULL);

  /* Set the location property on the filesink element */
  g_object_set (sink, "location", "/tmp/output.mp4", NULL);

  /* Create the pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source1 || !source2 || !filter1 || !filter2 || !converter1 || !converter2 || !queue1 || !queue2 || !mixer || !encoder || !muxer || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source1, filter1, converter1, queue1, mixer, encoder, muxer, sink, source2, filter2, converter2, queue2, NULL);
  
  if (!gst_element_link_many (source1, filter1, converter1, queue1, mixer, encoder, muxer, sink, NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }
  
  if (!gst_element_link_many (source2, filter2, converter2, queue2, mixer, NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }

  /* Set the xpos and ypos properties on the videomixer pads */
  GstPad *mixer_sink_pad_0 = gst_element_get_static_pad(mixer,"sink_0");
  GstPad *mixer_sink_pad_1 = gst_element_get_static_pad(mixer,"sink_1");
  
  g_object_set(mixer_sink_pad_0,"xpos",0,"ypos",0,NULL);
  g_object_set(mixer_sink_pad_1,"xpos",800,"ypos",0,NULL);

  gst_object_unref(mixer_sink_pad_0);
  gst_object_unref(mixer_sink_pad_1);

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
    
  gst_caps_unref(caps);
  
  gst_object_unref(bus);
  
  gst_element_set_state(pipeline,GST_STATE_NULL);
  
  gst_object_unref(pipeline);
  
  return 0;
}
