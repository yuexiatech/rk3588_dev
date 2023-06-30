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

int
main (int argc, char *argv[])
{
    // Create a pipeline and a videomixer element
    GstElement *pipeline = gst_pipeline_new ("pipeline");
    GstElement *mixer = gst_element_factory_make ("videomixer", "mix");
    GstBus *bus;
    GstMessage *msg;

    // Set the properties of the mixer
    g_object_set (mixer, "sink_0::xpos", 0, "sink_0::ypos", 0, "sink_0::alpha", 1, NULL);
    g_object_set (mixer, "sink_1::xpos", 0, "sink_1::ypos", 240, "sink_1::alpha", 1, NULL);

    // Add the mixer to the pipeline
    gst_bin_add (GST_BIN (pipeline), mixer);

    // Create and add the first video source
    GstElement *src1 = gst_element_factory_make ("v4l2src", "src1");
    g_object_set (src1, "device", "/dev/video31", NULL);
    GstElement *convert = gst_element_factory_make ("videoconvert", "convert");
    GstElement *scale = gst_element_factory_make ("videoscale", "scale");
    GstCaps *caps1 = gst_caps_from_string ("video/x-raw, width=320, height=240");
    gst_bin_add_many (GST_BIN (pipeline), src1, convert, scale, NULL);
    gst_element_link_filtered (src1, convert, caps1);
    gst_element_link (convert, scale);

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
    gst_element_link_pads (scale, "src1", mixer, GST_OBJECT_NAME (pad1));
    gst_element_link_pads (dec2, "src2", mixer, GST_OBJECT_NAME (pad2));

    // Create and add the video sink
    GstElement *sink = gst_element_factory_make ("autovideosink", "sink");
    g_object_set (sink, "qos", TRUE, NULL);
    gst_bin_add (GST_BIN (pipeline), sink);
    gst_element_link (mixer, sink);

    // Start playing the pipeline
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

