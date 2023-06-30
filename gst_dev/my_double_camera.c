#include <gst/gst.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif
/*
gst-launch-1.0  \
videomixer name=mix \
sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=1 \
sink_1::xpos=0 sink_1::ypos=240 sink_1::alpha=1 \
! autovideosink qos=true \
v4l2src device=/dev/video31 ! videoconvert! videoscale ! video/x-raw, width=320, height=240 ! mix.sink_0 \
v4l2src device=/dev/video41 ! image/jpeg,width=320,height=240,framerate=30/1 ! jpegdec ! mix.sink_1
*/

// gcc my_double_camera.c -o my_double_camera ` pkg-config --cflags --libs gstreamer-1.0`

int main(int argc, char *argv[]) {
  GstElement *pipeline, *mixer, *sink, *source1, *convert1, *scale1, *capsfilter1, *source2, *capsfilter2, *decoder;
  GstCaps *caps1, *caps2;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  mixer = gst_element_factory_make ("videomixer", "mixer");
  sink = gst_element_factory_make ("autovideosink", "sink");
  source1 = gst_element_factory_make ("v4l2src", "source1");
  convert1 = gst_element_factory_make ("videoconvert", "convert1");
  scale1 = gst_element_factory_make ("videoscale", "scale1");
  capsfilter1 = gst_element_factory_make ("capsfilter", "capsfilter1");
  source2 = gst_element_factory_make ("v4l2src", "source2");
  capsfilter2 = gst_element_factory_make ("capsfilter", "capsfilter2");
  decoder = gst_element_factory_make ("jpegdec", "decoder");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !mixer || !sink || !source1 || !convert1 || !scale1 || !capsfilter1 || !source2 || !capsfilter2 || !decoder) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set the device property on the v4l2src elements */
  g_object_set (G_OBJECT (source1), "device", "/dev/video31", NULL);
  g_object_set (G_OBJECT (source2), "device", "/dev/video41", NULL);

  /* Set the caps on the capsfilter elements */
  caps1 = gst_caps_from_string ("video/x-raw, width=320, height=240");
  g_object_set (G_OBJECT (capsfilter1), "caps", caps1, NULL);
  gst_caps_unref (caps1);

  caps2 = gst_caps_from_string ("image/jpeg,width=320,height=240,framerate=30/1");
  g_object_set (G_OBJECT (capsfilter2), "caps", caps2, NULL);
  gst_caps_unref (caps2);

  /* Set properties on the mixer pads */
  GstPad *mixer_sink_pad_0;
  mixer_sink_pad_0 = gst_element_get_request_pad(mixer, "sink_%u");
  g_object_set(mixer_sink_pad_0, "xpos", 0, "ypos", 0, "alpha", (gdouble)1.0, NULL);

  GstPad *mixer_sink_pad_1;
  mixer_sink_pad_1 = gst_element_get_request_pad(mixer, "sink_%u");
  g_object_set(mixer_sink_pad_1, "xpos", 0, "ypos", 240, "alpha", (gdouble)1.0, NULL);

/* Build the pipeline */
   gst_bin_add_many(GST_BIN(pipeline), mixer,sink,
       source1,capsfilter1,scale1,
       convert1,
       source2,capsfilter2,
       decoder,NULL);

   if(!gst_element_link_many(source1,capsfilter1,scale1,
       convert1,NULL)) {
     g_printerr("Failed to link source #1\n");
     return -1;
   }
   if(!gst_element_link_pads(convert1,"src",
       GST_ELEMENT_NAME(mixer),"sink_0")) {
     g_printerr("Failed to link mixer pad #0\n");
     return -1;
   }
   if(!gst_element_link_many(source2,capsfilter2,
       decoder,NULL)) {
     g_printerr("Failed to link source #2\n");
     return -1;
   }
   if(!gst_element_link_pads(decoder,"src",
       GST_ELEMENT_NAME(mixer),"sink_1")) {
     g_printerr("Failed to link mixer pad #0\n");
     return -1;
   }
   if(!gst_element_link_many(mixer,sink,NULL)) {
     g_printerr("Failed to link mixer and sink\n");
     return -1;
   }


   /* Start playing */
   gst_element_set_state(pipeline,GST_STATE_PLAYING);

   /* Wait until error or EOS */
   bus = gst_element_get_bus(pipeline);
   msg = gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,
       GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

   /* Free resources */
   if(msg != NULL)
     gst_message_unref(msg);
   gst_object_unref(bus);
   gst_element_set_state(pipeline,GST_STATE_NULL);
   gst_object_unref(pipeline);
   return 0;
}