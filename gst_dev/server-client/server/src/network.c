#include "storage.h"

void receive_data_from_client() {
    // Receive data from client using appsrc element
    // ...
    // Send data to storage component
    save_data_to_disk(data);
}

void send_data_to_client() {
    // Retrieve data from playback component
    data = get_playback_data();
    // Send data to client using appsink element
    // ...
}