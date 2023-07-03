#include "client.h"
#include "protocol.h"

GstElement *setup_pipeline(char *protocol, int data_rate, int qos) {
    // Create the pipeline
    GstElement *pipeline = gst_pipeline_new("client-pipeline");

    // Create the source element
    GstElement *source = gst_element_factory_make("v4l2src", "source");

    // Create the encoding element
    GstElement *encoder = gst_element_factory_make("x264enc", "encoder");

    // Set the data rate if specified
    if (data_rate > 0) {
        g_object_set(encoder, "bitrate", data_rate, NULL);
    }

    // Create the appsrc element
    GstElement *appsrc = gst_element_factory_make("appsrc", "appsrc");

    // Set the QoS option if specified
    if (qos > 0) {
        g_object_set(appsrc, "qos", TRUE, NULL);
    }

    // Create the appsink element
    GstElement *appsink = gst_element_factory_make("appsink", "appsink");

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, encoder, appsrc, appsink, NULL);

    // Link elements
    gst_element_link_many(source, encoder, appsrc, appsink, NULL);

    // Set up communication with the server using the specified protocol
    setup_protocol(appsrc, appsink, protocol);

    return pipeline;
}

void handle_server_message(GstElement *appsrc, GstBuffer *buffer) {

}
