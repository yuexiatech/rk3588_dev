#include <gst/gst.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif


int
main (int argc, char *argv[])
{
// Create a pipeline and a videomixer element
GstElement *pipeline = gst_pipeline_new ("pipeline");
GstElement *mixer = gst_element_factory_make ("videomixer", "mix");

// Set the properties of the mixer
g_object_set (mixer, "sink_0::xpos", 0, "sink_0::ypos", 0, "sink_0::alpha", 1, NULL);
g_object_set (mixer, "sink_1::xpos", 0, "sink_1::ypos", 240, "sink_1::alpha", 1, NULL);

// Add the mixer to the pipeline
gst_bin_add (GST_BIN (pipeline), mixer);

// Create and add the first video source
GstElement *src1 = gst_element_factory_make ("v4l2src", "src1");
g_object_set (src1, "device", "/dev/video31", NULL);
GstElement *convert1 = gst_element_factory_make ("videoconvert", "convert1");
GstElement *scale1 = gst_element_factory_make ("videoscale", "scale1");
GstCaps *caps1 = gst_caps_from_string ("video/x-raw, width=320, height=240");
gst_bin_add_many (GST_BIN (pipeline), src1, convert1, scale1, NULL);
gst_element_link_filtered (src1, convert1, caps1);
gst_element_link (convert1, scale1);

// Create and add the second video source
GstElement *src2 = gst_element_factory_make ("v4l2src", "src2");
g_object_set (src2, "device", "/dev/video41", NULL);
GstElement *dec2 = gst_element_factory_make ("jpegdec", "dec2");
GstCaps *caps2 = gst_caps_from_string ("image/jpeg,width=320,height=240,framerate=30/1");
gst_bin_add_many (GST_BIN (pipeline), src2, dec2, NULL);
gst_element_link_filtered (src2, dec2, caps2);

// Link the sources to the mixer
GstPad *pad1 = gst_element_get_request_pad (mixer, "sink_%u");
GstPad *pad2 = gst_element_get_request_pad (mixer, "sink_%u");
gst_element_link_pads (scale1, "src", mixer, GST_OBJECT_NAME (pad1));
gst_element_link_pads (dec2, "src", mixer, GST_OBJECT_NAME (pad2));

// Create and add the video sink
GstElement *sink = gst_element_factory_make ("autovideosink", "sink");
g_object_set (sink, "qos", TRUE, NULL);
gst_bin_add (GST_BIN (pipeline), sink);
gst_element_link (mixer, sink);

// Start playing the pipeline
gst_element_set_state (pipeline, GST_STATE_PLAYING);

}

