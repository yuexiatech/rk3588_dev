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


**Server side**:

* Implement a command-line interface for controlling the server.
* Implement support for multiple communication protocols, including RTP, RTSP, and RTMP for signaling and data communication.
* Implement storage, push/pull streaming, and video-on-demand (VOD) functionality.
* Implement multi-bitrate video transcoding and generation.
* Implement network bandwidth detection and bitrate selection (QoS).
* Support downward compatibility with 4K video resolution and H.264/H.265 encoding.
* Support multi-point access and asynchronous playback for at least 32 concurrent users.

**Client side**:

* Implement a command-line interface for controlling the client.
* Implement support for multiple communication protocols, including RTP, RTSP, and RTMP for signaling and data communication.
* Implement functionality to receive and decode audio and video data from the server.
* Support downward compatibility with 4K video resolution and H.264/H.265 decoding.
* Implement network bandwidth detection and bitrate selection (QoS).

Both the server and client programs will need to use GStreamer elements such as rtpbin, rtspclientsink, rtmpsink, decodebin, x264enc, x265enc, videorate, videoscale, etc., as well as custom code to handle the command line interface, network communication, and other features.