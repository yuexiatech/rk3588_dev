#include "pipeline.h"
#include "cli.h"

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    cli_parse_args(argc, argv);

    // Initialize GStreamer
    gst_init(&argc, &argv);

    // Create and start the main pipeline
    pipeline_create();
    pipeline_start();

    // Run the main loop
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    // Clean up
    pipeline_stop();
    pipeline_destroy();
    g_main_loop_unref(loop);
    gst_deinit();

    return 0;
}