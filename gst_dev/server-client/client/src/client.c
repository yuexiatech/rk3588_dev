#include "client.h"
#include "protocol.h"

GstElement *setup_pipeline() {
    // Create the pipeline
    GstElement *pipeline = gst_pipeline_new("client-pipeline");

    // Create the source element
    GstElement *source = gst_element_factory_make("v4l2src", "source");

    // Create the encoding element
    GstElement *encoder = gst_element_factory_make("x264enc", "encoder");

    // Create the appsrc element
    GstElement *appsrc = gst_element_factory_make("appsrc", "appsrc");

    // Create the appsink element
    GstElement *appsink = gst_element_factory_make("appsink", "appsink");

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, encoder, appsrc, appsink, NULL);

    // Link elements
    gst_element_link_many(source, encoder, appsrc, appsink, NULL);

    // Set up communication with the server
    setup_protocol(appsrc, appsink);

    return pipeline;
}

void handle_server_message(GstElement *appsrc, GstBuffer *buffer) {
    // Handle messages from the server

    // If it's a playback request, send it to the playback component
    if (is_playback_request(buffer)) {
        handle_playback_request(appsrc, buffer);
    }
}
