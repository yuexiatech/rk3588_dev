#include "pipeline.h"
#include "protocols.h"
#include "playback.h"
#include <gst/gst.h>

// Global variables for the main pipeline and its elements
GstElement *pipeline;
GstElement *rtp_src, *rtsp_src, *rtmp_src;
GstElement *playbin;

void pipeline_create() {
    // Create the main pipeline
    pipeline = gst_pipeline_new("client-pipeline");

    // Create the protocol-specific source elements
    rtp_src = rtp_create_src();
    rtsp_src = rtsp_create_src();
    rtmp_src = rtmp_create_src();

    // Create the playbin element
    playbin = playback_create_playbin();

    // Add elements to the pipeline and link them
    gst_bin_add_many(GST_BIN(pipeline), rtp_src, rtsp_src, rtmp_src, playbin, NULL);
}

void pipeline_start() {
    // Set the pipeline to PLAYING state
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void pipeline_stop() {
    // Set the pipeline to NULL state
    gst_element_set_state(pipeline, GST_STATE_NULL);
}

void pipeline_destroy() {
    // Destroy the main pipeline and its elements
    gst_object_unref(pipeline);
}