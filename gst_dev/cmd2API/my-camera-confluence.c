#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline, *source1, *source2, *filter1, *filter2, *decoder, *converter1, *converter2, *mixer, *queue1, *queue2, *encoder, *muxer, *sink;
  GstCaps *filtercaps1, *filtercaps2;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source1 = gst_element_factory_make ("v4l2src", "source1");
  source2 = gst_element_factory_make ("v4l2src", "source2");
  filter1 = gst_element_factory_make ("capsfilter", "filter1");
  filter2 = gst_element_factory_make ("capsfilter", "filter2");
  decoder = gst_element_factory_make ("jpegdec", "decoder");
  converter1 = gst_element_factory_make ("videoconvert", "converter1");
  converter2 = gst_element_factory_make ("videoscale", "converter2");
  mixer = gst_element_factory_make ("videomixer", "mixer");
  queue1 = gst_element_factory_make ("queue", "queue1");
  queue2 = gst_element_factory_make ("queue", "queue2");
  encoder = gst_element_factory_make ("x264enc", "encoder");
  muxer = gst_element_factory_make ("mp4mux", "muxer");
  sink = gst_element_factory_make ("filesink", "sink");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source1 || !source2 || !filter1 || !filter2 || !decoder || !converter1 || !converter2 || !mixer || !queue1 || !queue2 || !encoder || !muxer || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set the source properties */
  g_object_set (source1, "device", "/dev/video31", NULL);
  g_object_set (source1, "num-buffers", 100, NULL);
  
  g_object_set (source2, "device", "/dev/video41", NULL);

  /* Set the filter properties */
  filtercaps1 = gst_caps_from_string("video/x-raw,format=NV12,width=800,height=600,framerate=30/1");
  g_object_set (filter1, "caps", filtercaps1, NULL);
  
  filtercaps2 = gst_caps_from_string("image/jpeg,width=640,height=480,framerate=30/1");
  g_object_set (filter2, "caps", filtercaps2, NULL);

  /* Set the mixer properties */
  g_object_set (mixer, "sink_0::xpos", 0, NULL);
  g_object_set (mixer, "sink_0::ypos", 0, NULL);
  
    g_object_set (mixer, "sink_1::xpos",800 , NULL);
    g_object_set (mixer,"sink_1::ypos" ,0 , NULL);

  
    /* Set the sink properties */
    g_object_set (sink,"location","/tmp/output.mp4" ,NULL);

    /* Build the pipeline */
    gst_bin_add_many (GST_BIN (pipeline), source1, filter1, converter1, queue1,mixer ,encoder,muxer,sink,NULL);
    if (gst_element_link_many (source1 ,filter1 ,converter1 ,queue1 ,mixer,NULL) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    
    gst_bin_add_many(GST_BIN(pipeline), source2 ,filter2 ,decoder ,converter2 ,queue2,NULL);
    if(gst_element_link_many(source2 ,filter2 ,decoder ,converter2 ,queue2,mixer,NULL) != TRUE){
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    
    if(gst_element_link_many(mixer ,encoder,muxer,sink,NULL) != TRUE){
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Start playing */
    gst_element_set_state(pipeline,GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus =gst_element_get_bus(pipeline);
    msg=gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Free resources */
    if(msg != NULL)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}
