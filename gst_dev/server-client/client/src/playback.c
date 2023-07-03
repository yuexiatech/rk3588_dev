#include "playback.h"

void handle_playback_request(GstElement *appsrc, GstBuffer *buffer) {
    // Set up the playback pipeline
    GstElement *playback_pipeline = setup_playback_pipeline();

    // Push the buffer to the appsrc element of the playback pipeline
    gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer);

    // Start playback
    gst_element_set_state(playback_pipeline, GST_STATE_PLAYING);
}

GstElement *setup_playback_pipeline() {
    // Create the playback pipeline
    GstElement *pipeline = gst_pipeline_new("playback-pipeline");

    // Create the appsrc element
    GstElement *appsrc = gst_element_factory_make("appsrc", "appsrc");

    // Create the decoding element
    GstElement *decoder = gst_element_factory_make("decodebin", "decoder");

    // Create the sink element
    GstElement *sink = gst_element_factory_make("autovideosink", "sink");

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, decoder, sink, NULL);

    // Link elements
    gst_element_link_many(appsrc, decoder, sink, NULL);

    return pipeline;
}

