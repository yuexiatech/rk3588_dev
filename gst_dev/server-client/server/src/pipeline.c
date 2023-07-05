#include "pipeline.h"
#include "protocols.h"
#include "storage.h"
#include "conversion.h"
#include "bandwidth.h"
#include "vod.h"
#include <gst/gst.h>

// Global variables for the main pipeline and its elements
GstElement *pipeline;
GstElement *appsrc, *appsink;
GstElement *rtp_sink, *rtsp_sink, *rtmp_sink;
GstElement *filesink;

void pipeline_create() {
    // Create the main pipeline
    pipeline = gst_pipeline_new("server-pipeline");

    // Create the appsrc and appsink elements
    appsrc = gst_element_factory_make("appsrc", "appsrc");
    appsink = gst_element_factory_make("appsink", "appsink");

    // Create the protocol-specific sink elements
    rtp_sink = rtp_create_sink();
    rtsp_sink = rtsp_create_sink();
    rtmp_sink = rtmp_create_sink();

    // Create the filesink element
    filesink = storage_create_filesink();

    // Add elements to the pipeline and link them
    gst_bin_add_many(GST_BIN(pipeline), appsrc, appsink, rtp_sink, rtsp_sink, rtmp_sink, filesink, NULL);
    gst_element_link_many(appsrc, appsink, rtp_sink, rtsp_sink, rtmp_sink, filesink, NULL);

    // Add conversion and bitrate selection elements to the pipeline
    conversion_add_elements(pipeline);
    bandwidth_add_elements(pipeline);

    // Add video-on-demand service elements to the pipeline
    vod_add_elements(pipeline);
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