#include "protocol.h"

void setup_protocol(GstElement *appsrc, GstElement *appsink) {
    // Set up communication with the server using the desired protocol

    // For example, for RTP:
    setup_rtp(appsrc, appsink);
}

void setup_rtp(GstElement *appsrc, GstElement *appsink) {
    // Set up RTP communication with the server

    // Set the appsrc and appsink properties for RTP
    g_object_set(appsrc, "stream-type", GST_APP_STREAM_TYPE_STREAM, NULL);
    g_object_set(appsink, "sync", FALSE, NULL);

    // Set up the RTP sink and source elements
    GstElement *rtpsink = gst_element_factory_make("udpsink", "rtpsink");
    GstElement *rtpsrc = gst_element_factory_make("udpsrc", "rtpsrc");

    // Set the RTP sink and source properties
    g_object_set(rtpsink, "host", SERVER_HOST, "port", SERVER_PORT, NULL);
    g_object_set(rtpsrc, "port", CLIENT_PORT, NULL);

    // Link the appsrc and appsink elements to the RTP sink and source elements
    gst_element_link(appsrc, rtpsink);
    gst_element_link(rtpsrc, appsink);
}
