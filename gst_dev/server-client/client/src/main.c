#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "client.h"
#include "playback.h"

int main(int argc, char *argv[]) {
    // Parse command-line options
    int c;
    char *protocol = NULL;
    int data_rate = 0;
    int qos = 0;

    while ((c = getopt(argc, argv, "p:d:q:")) != -1) {
        switch (c) {
            case 'p':
                protocol = optarg;
                break;
            case 'd':
                data_rate = atoi(optarg);
                break;
            case 'q':
                qos = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-p protocol] [-d data_rate] [-q qos]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Initialize GStreamer
    gst_init(&argc, &argv);

    // Create the pipeline
    GstElement *pipeline = setup_pipeline(protocol, data_rate, qos);

    // Start the main loop
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    // Clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
