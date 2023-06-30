```
cd /home/rpdzkj/Downloads/rk3588_dev/

gcc my_double_camera.c -o my_double_camera ` pkg-config --cflags --libs gstreamer-1.0` 

```

gcc my_video31_play.c -o my_video31_play ` pkg-config --cflags --libs gstreamer-1.0`