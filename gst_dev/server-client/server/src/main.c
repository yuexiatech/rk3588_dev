#include "network.h"
#include "storage.h"
#include "playback.h"
#include "request_handler.h"

int main(int argc, char *argv[]) {
    // Initialize GStreamer
    gst_init(&argc, &argv);

    // Set up the GStreamer pipeline
    GstElement *pipeline = gst_pipeline_new("server-pipeline");
    GstElement *network = setup_network();
    GstElement *storage = setup_storage();
    GstElement *playback = setup_playback();

    gst_bin_add_many(GST_BIN(pipeline), network, storage, playback, NULL);
    gst_element_link_many(network, storage, playback, NULL);

    // Start the main loop
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    // Listen for incoming client requests
    while (TRUE) {
        char *request = listen_for_request();
        handle_request(request);
    }

    g_main_loop_run(loop);

    // Clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}