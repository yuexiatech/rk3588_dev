#include "protocol.h"

void setup_protocol(GstElement *appsrc, GstElement *appsink, char *protocol) {
    // Set up communication with the server using the specified protocol

    if (protocol == NULL || strcmp(protocol, "rtp") == 0) {
        setup_rtp(appsrc, appsink);
    } else if (strcmp(protocol, "rtsp") == 0) {
        setup_rtsp(appsrc, appsink);
    } else if (strcmp(protocol, "rtmp") == 0) {
        setup_rtmp(appsrc, appsink);
    } else {
        g_printerr("Unsupported protocol: %s\n", protocol);
        exit(EXIT_FAILURE);
    }
}

void setup_rtp(GstElement *appsrc, GstElement *appsink) {

}
