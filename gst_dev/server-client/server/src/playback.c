#include "storage.h"

data_t get_playback_data() {
    // Retrieve data from storage component
    data = retrieve_data_from_disk();
    // Transcode data into desired format using decoding, encoding, and muxing elements
    // ...
}