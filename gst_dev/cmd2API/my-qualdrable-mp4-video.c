#include <gst/gst.h>

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data) {
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the decodebin sink pad */
  sinkpad = gst_element_get_static_pad (decoder, "sink");
  gst_pad_link (pad, sinkpad);
  gst_object_unref (sinkpad);
}

int main(int argc, char *argv[]) {
  GstElement *pipeline, *mixer, *sink, *src0, *src1, *src2, *src3, *dec0, *dec1, *dec2, *dec3, *scale0, *scale1, *scale2, *scale3;
  GstCaps *caps;
  GstBus *bus;
  GstMessage *msg;

  gst_debug_set_default_threshold(GST_LEVEL_WARNING);

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  mixer = gst_element_factory_make ("videomixer", "mix");
  sink = gst_element_factory_make ("autovideosink", "sink");
  src0 = gst_element_factory_make ("uridecodebin", "src0");
  src1 = gst_element_factory_make ("uridecodebin", "src1");
  src2 = gst_element_factory_make ("uridecodebin", "src2");
  src3 = gst_element_factory_make ("uridecodebin", "src3");
  dec0 = gst_element_factory_make ("decodebin", "dec0");
  dec1 = gst_element_factory_make ("decodebin", "dec1");
  dec2 = gst_element_factory_make ("decodebin", "dec2");
  dec3 = gst_element_factory_make ("decodebin", "dec3");
  scale0 = gst_element_factory_make ("videoscale", "scale0");
  scale1 = gst_element_factory_make ("videoscale", "scale1");
  scale2 = gst_element_factory_make ("videoscale", "scale2");
  scale3 = gst_element_factory_make ("videoscale", "scale3");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !mixer || !sink || !src0 || !src1 || !src2 || !src3 || !dec0 || !dec1 || !dec2 || !dec3 || !scale0 || !scale1 || !scale2 || !scale3) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Set up the pipeline */
  
  /* we set the input uri to the source element */
  g_object_set (G_OBJECT (src0), "uri", "http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4", NULL);
  g_object_set (G_OBJECT (src1), "uri", "http://vjs.zencdn.net/v/oceans.mp4", NULL);
  g_object_set (G_OBJECT (src2), "uri", "https://media.w3.org/2010/05/sintel/trailer.mp4", NULL);
  g_object_set (G_OBJECT (src3), "uri", "http://mirror.aarnet.edu.au/pub/TED-talks/911Mothers_2010W-480p.mp4", NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
   gst_object_unref (bus);

  /* we add all elements into the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), mixer, sink, src0, src1, src2, src3, dec0, dec1, dec2, dec3, scale0, scale1, scale2, scale3, NULL);

  g_signal_connect (src0, "pad-added", G_CALLBACK (on_pad_added), dec0);
  g_signal_connect (src1, "pad-added", G_CALLBACK (on_pad_added), dec1);
  g_signal_connect (src2, "pad-added", G_CALLBACK (on_pad_added), dec2);
  g_signal_connect (src3, "pad-added", G_CALLBACK (on_pad_added), dec3);
  
  /* we link the elements together */
  gst_element_link_many (src0, dec0, scale0, mixer, NULL);
  gst_element_link_many (src1, dec1, scale1, mixer, NULL);
  gst_element_link_many (src2, dec2, scale2, mixer, NULL);
  gst_element_link_many (src3, dec3, scale3, mixer, NULL);
  gst_element_link (mixer, sink);


  /* Set the pipeline to "playing" state*/
  g_print ("Now playing\n");
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