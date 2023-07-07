| | | |
|---|---|---|
| 序号 | 功能点 | 功能描述 |
| 1 | **多路音视频传输编解码软件** |  |
| 1.1 | 音视频数据通信 | 实现编码板与解码板之间的音视频数据通信。 |
| 1.2 | 多通讯协议支持 | 信令与数据通信协议，支持RTP、RTSP和RTMP协议的存储、推拉流和点播功能。 |
| 1.3 | 多码率视频转换与生成 | 多码率视频转换与生成。 |
| 1.4 | 带宽探测与码率选择 | 网络带宽探测与码率选择（QoS）。 |

1)客户端全中文界面；
2)服务器端支持linux操作系统，客户端同时支持中标麒麟等国产操作系统以及Windows操作系统。
3)视频分辨率为4K向下兼容，编码方式为H 264/H 265；
4)点播服务软件支持多点访问，支持不少于32个用户同时在线异步回放。

**Request:**
1.  **Implementing communication between an encoder and a decoder**: You can use GStreamer’s `appsrc` and `appsink` elements to facilitate the transfer of audio and video data between the encoder and decoder boards. These elements allow you to push data into a pipeline or pull data from a pipeline from your application code.
    
2.  **Supporting multiple communication protocols**: GStreamer has built-in support for many communication protocols, including RTP, RTSP, and RTMP. You can use the appropriate elements (such as `rtpsrc`, `rtspsrc`, `rtmpsrc`, etc.) to receive or send data using these protocols.
    
3.  **Implementing storage, streaming, and video-on-demand**: You can use GStreamer’s `filesink` element to save the encoded data to a file for later playback. For streaming, you can use the appropriate protocol-specific sink elements (such as `rtmpsink`, `udpsink`, etc.) to send the data to a server or another client. For video-on-demand, you can use the `playbin` element to play back the stored files.
    
4.  **Implementing video conversion and generation at multiple bitrates**: You can use GStreamer’s encoding elements (such as `x264enc`, `x265enc`, etc.) to encode the raw video data at different bitrates. You can also use the `videorate` element to change the framerate of the video, and the `videoscale` element to change its resolution.
    
5.  **Implementing network bandwidth detection and bitrate selection**: You can use GStreamer’s QoS (Quality of Service) events to monitor the network conditions and adjust the bitrate of the encoded video accordingly. The QoS events provide information about the amount of buffering, packet loss, and jitter in the pipeline, which you can use to estimate the available bandwidth.
    
6.  **Command-line control for client and server**: You can create command-line interfaces for both the client and server applications using standard C libraries such as `getopt` or third-party libraries such as `argp`. These interfaces would allow users to control the applications by passing command-line arguments when starting them.
    
7.  **Support for 4K resolution and H.264/H.265 encoding**: GStreamer has built-in support for both H.264 and H.265 encoding through elements such as `x264enc` and `x265enc`. You can use these elements to encode the video data at 4K resolution or lower.
    
8.  **Support for multiple users accessing video-on-demand**: You can use GStreamer’s `playbin` element in combination with a server application to implement a video-on-demand service that supports multiple users. The server application would need to handle incoming requests from clients, manage the available resources, and create new instances of the `playbin` element as needed to serve the requested content.



