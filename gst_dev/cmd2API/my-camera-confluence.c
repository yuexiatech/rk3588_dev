#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline, *source1, *source2, *filter1, *filter2, *converter, *queue1, *queue2, *mixer, *encoder, *muxer, *sink;
  GstCaps *filter1_caps, *filter2_caps;
  GstBus *bus;
  GstMessage *msg;

  gst_init (&argc, &argv);

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

  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source1 || !source2 || !filter1 || !filter2 || !converter || !queue1 || !queue2 || !mixer || !encoder || !muxer || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  g_object_set (source1, "device", "/dev/video31", NULL);
  g_object_set (source2, "device", "/dev/video41", NULL);
  filter1_caps = gst_caps_from_string ("video/x-raw,format=NV12,width=800,height=600,framerate=30/1");
  g_object_set (G_OBJECT (filter1), "caps", filter1_caps, NULL);
  filter2_caps = gst_caps_from_string ("image/jpeg,width=800,height=600,framerate=30/1");
  g_object_set (G_OBJECT (filter2), "caps", filter2_caps, NULL);
  g_object_set (G_OBJECT (mixer), "sink_0::xpos", 0, NULL);
  g_object_set (G_OBJECT (mixer), "sink_0::ypos", 0, NULL);
  g_object_set (G_OBJECT (mixer), "sink_1::xpos", 800, NULL);
  g_object_set (G_OBJECT (mixer), "sink_1::ypos", 0, NULL);
  g_object_set (G_OBJECT (sink), "location", "/tmp/output.mp4", NULL);

  gst_bin_add_many (GST_BIN (pipeline), source1, source2, filter1, filter2, converter, queue1, queue2, mixer, encoder, muxer, sink, NULL);
  
if (!gst_element_link_many (source1, filter1, converter, queue1,mixer,NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}
if (!gst_element_link_many (source2, filter2,gst_element_factory_make("jpegdec","jpegdec"), queue2,mixer,NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}
if (!gst_element_link_many(mixer , encoder , muxer , sink ,NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}

gst_caps_unref(filter1_caps);
gst_caps_unref(filter2_caps);

gst_element_set_state(pipeline,GST_STATE_PLAYING);

bus=gst_element_get_bus(pipeline);
msg=gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,GST_MESSAGE_ERROR|GST_MESSAGE_EOS);

if(msg!=NULL)
{
    GError*err;
    gchar*debug_info;

    switch(GST_MESSAGE_TYPE(msg))
    {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg,&err,&debug_info);
            g_printerr("Error received from element %s: %s\n",GST_OBJECT_NAME(msg->src),err->message);
            g_printerr("Debugging information: %s\n",(debug_info)?debug_info:"none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            g_printerr("Unexpected message received.\n");
            break;
    }
    gst_message_unref(msg);
}

gst_object_unref(bus);
gst_element_set_state(pipeline,GST_STATE_NULL);
gst_object_unref(pipeline);

return 0;
}
