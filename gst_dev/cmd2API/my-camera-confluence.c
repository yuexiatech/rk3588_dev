#include <gst/gst.h>

/*
gst-launch-1.0 -e \
    v4l2src device=/dev/video31 ! video/x-raw,format=NV12,width=800,height=600,framerate=30/1 ! videoconvert ! queue ! videomixer name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=800 sink_1::ypos=0 ! x264enc ! mp4mux ! filesink location=/tmp/output.mp4 \
    v4l2src device=/dev/video41 ! image/jpeg,width=800,height=600,framerate=30/1 ! jpegdec ! queue ! mix.
*/

int main(int argc, char *argv[]) {
  GstElement *pipeline, *source1, *source2, *filter1, *filter2, *converter, *queue1, *queue2, *mixer, *encoder, *muxer, *sink;
  GstCaps *filter1_caps, *filter2_caps;
  GstBus *bus;
  GstMessage *msg;

  gst_debug_set_default_threshold(GST_LEVEL_INFO);

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source1 = gst_element_factory_make ("v4l2src", "source1");
  source2 = gst_element_factory_make ("v4l2src", "source2");
  filter1 = gst_element_factory_make ("capsfilter", "filter1");
  filter2 = gst_element_factory_make ("capsfilter", "filter2");
  converter = gst_element_factory_make ("videoconvert", "converter");
  queue1 = gst_element_factory_make ("queue", "queue1");
  queue2 = gst_element_factory_make ("queue", "queue2");
  mixer = gst_element_factory_make ("videomixer", "mixer");
  encoder = gst_element_factory_make ("x264enc", "encoder");
  muxer = gst_element_factory_make ("mp4mux", "muxer");
  sink = gst_element_factory_make ("filesink", "sink");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source1 || !source2 || !filter1 || !filter2 || !converter || !queue1 || !queue2 || !mixer || !encoder || !muxer || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set the device property on the v4l2src element */
  g_object_set (source1, "device", "/dev/video31", NULL);
  g_object_set (source2, "device", "/dev/video32", NULL);

  /* Set the location property on the filesink element */
  g_object_set (sink, "location", "/tmp/output.mp4", NULL);

  /* Set the caps for the capsfilter elements */
  filter1_caps = gst_caps_from_string("video/x-raw,format=NV12,width=800,height=600,framerate=30/1");
  g_object_set (G_OBJECT (filter1), "caps", filter1_caps, NULL);
  filter2_caps = gst_caps_from_string("video/x-raw,format=NV12,width=800,height=600,framerate=30/1");
  g_object_set (G_OBJECT (filter2), "caps", filter2_caps, NULL);

  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source1, source2, filter1, filter2, converter, queue1, queue2, mixer, encoder, muxer, sink, NULL);
  
if (gst_element_link_many (source1, filter1, converter, queue1,mixer,NULL) != TRUE) 
    {
        g_printerr ("Elements0 could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

if(gst_element_link_many (source2, filter2,mixer,NULL) != TRUE )
{
    g_printerr ("Elements1 could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}

if(gst_element_link_many (mixer , encoder,muxer,sink,NULL) != TRUE)
{
    g_printerr ("Elements2 could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}
/* Set properties on mixer pads */
GstPad *mixer_sink_pad_0 = gst_element_get_static_pad(mixer,"sink_0");
g_object_set(mixer_sink_pad_0,"xpos",0,"ypos",0,NULL);
gst_object_unref(mixer_sink_pad_0);

GstPad *mixer_sink_pad_1 = gst_element_get_request_pad(mixer,"sink_%u");
g_object_set(mixer_sink_pad_1,"xpos",800,"ypos",0,NULL);
gst_object_unref(mixer_sink_pad_1);

/* Start playing */
gst_element_set_state(pipeline,GST_STATE_PLAYING);

/* Wait until error or EOS */
bus = gst_element_get_bus(pipeline);
msg = gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

/* Free resources */
if(msg != NULL)
    gst_message_unref(msg);
gst_object_unref(bus);
gst_element_set_state(pipeline,GST_STATE_NULL);
gst_object_unref(pipeline);

return 0;
}

