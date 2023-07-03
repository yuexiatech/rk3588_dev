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

*   The server program would be responsible for receiving incoming streams from clients, storing them, and making them available for playback.
*   The server would have a pipeline that uses `appsrc` and `appsink` elements to receive data from clients and send it to the storage component.
*   The storage component would use `filesink` and `filesrc` elements to save and retrieve data from disk.
*   The server would also have a pipeline for handling playback requests from clients. This pipeline would use `filesrc`, decoding, encoding, and muxing elements to read data from disk, transcode it into the desired format, and send it to clients.
*   The server would support multiple communication protocols, such as RTP, RTSP, and RTMP, for receiving data from clients and sending data to them.

**Client side**:

*   The client program would be responsible for capturing audio and video data, encoding it, and sending it to the server.
*   The client would have a pipeline that uses `v4l2src` or other source elements to capture audio and video data from the local system.
*   The captured data would be encoded using encoding elements such as `x264enc` or `vp8enc`, and then sent to the server using `appsrc` and `appsink` elements.
*   The client would support multiple communication protocols, such as RTP, RTSP, and RTMP, for sending data to the server and receiving data from it.
*   The client could also have a playback component that uses `appsrc`, decoding, and sink elements to receive data from the server and play it back locally.
