#include <gst/gst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//element [qtdemux] dynamic pad connection 等待qtdemux新引脚添加后，连接qtdemux和后一元素
void qtdemux_pad_added_handler(GstElement *qtdemux, GstPad *pad, GstElement *nextelement){
    gst_element_link_pads(qtdemux, GST_PAD_NAME (pad), nextelement, NULL);
}
//element [decodebin] dynamic pad connection
void decodebin_pad_added_handler(GstElement *decodebin, GstPad *pad, GstElement *nextelement){
    gst_element_link_pads(decodebin, GST_PAD_NAME (pad), nextelement, NULL);
}

//pipeline save : set state PLAYING
void mux_video_audio_save(GstElement *pipeline){
  GstBus *bus;
  GstMessage *msg;
  
  // 设置管道状态为PLAYING
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to start pipeline to save. Exiting.\n");
    // gst_object_unref(pipeline);
  }
  
  //对于本地视频要在播放结束后设置状态NULL
  // // 等待管道的EOS事件
  // bus = gst_element_get_bus(pipeline);
  // //调试代码
  // g_print("holly shit0!\n");
  // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  // //调试代码
  // g_print("holly shit1!\n");
  // // 处理管道状态变化的消息
  // if (msg != NULL) {
  //   GError *err;
  //   gchar *debug_info;

  //   switch (GST_MESSAGE_TYPE(msg)) {
  //     case GST_MESSAGE_ERROR:
  //       gst_message_parse_error(msg, &err, &debug_info);
  //       g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
  //       g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
  //       g_clear_error(&err);
  //       g_free(debug_info);
  //       break;
  //     case GST_MESSAGE_EOS:
  //       g_print("End-Of-Stream reached.\n");
  //       gst_element_set_state(pipeline, GST_STATE_NULL);
  //       break;
  //     default:
  //       g_printerr("Unexpected message received.\n");
  //       break;
  //   }
  //   gst_message_unref(msg);
  // }
  // //调试代码
  // g_print("holly shit!2\n");
  }

  //pipeline save : set state NULL
  void mux_video_audio_stopsave(GstElement *pipeline){
  GstBus *bus;
  GstMessage *msg;
  
  // 设置管道状态为NULL
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_NULL);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to stop pipeline to save. Exiting.\n");
    // gst_object_unref(pipeline);
  }
  }

//pipeline play : set state PLAYING
void mux_video_audio_play(GstElement *pipeline){
  GstBus *bus;
  GstMessage *msg;
  
  // 设置管道状态为PLAYING
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to start pipeline to play. Exiting.\n");
    // gst_object_unref(pipeline);
  }
  
  //对于本地视频要在播放结束后设置状态NULL
  // // 等待管道的EOS事件
  // bus = gst_element_get_bus(pipeline);
  // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  // // 处理管道状态变化的消息
  // if (msg != NULL) {
  //   GError *err;
  //   gchar *debug_info;

  //   switch (GST_MESSAGE_TYPE(msg)) {
  //     case GST_MESSAGE_ERROR:
  //       gst_message_parse_error(msg, &err, &debug_info);
  //       g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
  //       g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
  //       g_clear_error(&err);
  //       g_free(debug_info);
  //       gst_element_set_state(pipeline, GST_STATE_NULL);
  //       break;
  //     case GST_MESSAGE_EOS:
  //       g_print("End-Of-Stream reached.\n");
  //       gst_element_set_state(pipeline, GST_STATE_NULL);
  //       break;
  //     default:
  //       g_printerr("Unexpected message received.\n");
  //       break;
  //   }
  //   gst_message_unref(msg);
  // }
}

//pipeline play : set state NULL
void mux_video_audio_stopplay(GstElement *pipeline){
  // 设置管道状态为NULL
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_NULL);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to stop pipeline to play. Exiting.\n");
    // gst_object_unref(pipeline);
  }
}

//pipeline trans : set state PLAYING and listen to bus
void mux_video_audio_trans(GstElement *pipeline, GstElement *filesrc_video_trans, GstElement *filesink_trans){
  GstBus *bus;
  GstMessage *msg;
  while (1)
  {
  //设置读入文件名
    g_print("please enter filename need transformation\n");
    gchar input_filename[128];
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) 
    {
      continue;
    }
    int len_in = strcspn(input_filename, "\n");
    //debug代码
    // g_print("%d\n",len_in);
    // g_print("%c\n",input_filename[len_in]);

    if(len_in > 0 && input_filename[len_in] == '\n')
    {
      input_filename[len_in] = '\0';
      //debug代码
      // g_print("modification executed");
    }
    //debug代码
    // g_print("%s\n",input_filename);
    g_object_set(G_OBJECT(filesrc_video_trans), "location", input_filename, NULL);
  //设置输出文件名  
    g_print("please enter filename of output file\n");
    gchar output_filename[128];
    if (fgets(output_filename, sizeof(output_filename), stdin) == NULL) 
    {
      continue;
    }
    int len_out = strcspn(output_filename, "\n");
    //debug代码
    // g_print("%d\n",len_out);
    // g_print("%c\n",output_filename[len_out]);

    if(len_out > 0 && output_filename[len_out] == '\n')
    {
      output_filename[len_out] = '\0';
      // g_print("modification executed");
    }
    //debug代码
    // g_print("%s\n",output_filename);
    g_object_set(G_OBJECT(filesink_trans), "location", output_filename, NULL);
    break;
  }
  
  
  // 设置管道状态为PLAYING
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to start pipeline to transform video. Exiting.\n");
    // gst_object_unref(pipeline);
  }
  
  // 对于本地视频要在播放结束后设置状态NULL
  // 等待管道的EOS事件
  // bus = gst_element_get_bus(pipeline);
  // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  // // 处理管道状态变化的消息
  // if (msg != NULL) {
  //   GError *err;
  //   gchar *debug_info;

  //   switch (GST_MESSAGE_TYPE(msg)) {
  //     case GST_MESSAGE_ERROR:
  //       gst_message_parse_error(msg, &err, &debug_info);
  //       g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
  //       g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
  //       g_clear_error(&err);
  //       g_free(debug_info);
  //       gst_element_set_state(pipeline, GST_STATE_NULL);
  //       break;
  //     case GST_MESSAGE_EOS:
  //       g_print("End-Of-Stream reached.\n");
  //       gst_element_set_state(pipeline, GST_STATE_NULL);
  //       break;
  //     default:
  //       g_printerr("Unexpected message received.\n");
  //       break;
  //   }
  //   gst_message_unref(msg);
  // }

  //pipeline trans : set state NULL


}

void mux_video_audio_stoptrans(GstElement *pipeline){
  // 设置管道状态为NULL
  GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_NULL);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Failed to stop pipeline to trans. Exiting.\n");
    // gst_object_unref(pipeline);
  }
}

//主函数
int main(int argc, char *argv[]) {
  //variables for mux video and audio and save
  GstElement *pipeline_save, *filesrc_video_save, *filesrc_audio_save, *qtdemux_save, *h264parse_save, *avdec_h264_save;
  GstElement *videoconvert_save, *mpph264enc_save, *mp4mux_save, *decodebin_save, *audioconvert_save, *voaacenc_save,  *filesink_save, *queue_save;
  //variables for mux video and audio and play
  GstElement *pipeline_play, *filesrc_video_play, *filesrc_audio_play, *qtdemux_play, *h264parse_play, *avdec_h264_play;
  GstElement *videoconvert_play, *autovideosink_play, *decodebin_play, *audioconvert_play, *autoaudiosink_play;
  //variables for trans
  GstElement *pipeline_trans, *filesrc_video_trans, *decodebin_trans, *mpph264enc_trans, *video_queue_trans,*audio_queue_trans, *mp4mux_trans, *filesink_trans;
  GstElement *audioconvert_trans, *lamemp3enc_trans; 

  GMainLoop *loop;
  /* 初始化GStreamer */
  gst_init(&argc, &argv);

//pipeline save: mux video and audio and save
  /* 创建各个元素 */
  pipeline_save = gst_pipeline_new("pipeline");
  filesrc_video_save = gst_element_factory_make("filesrc", "filesrc_video_save");
  g_object_set(G_OBJECT(filesrc_video_save), "location", "input_video1.mp4", NULL);
  filesrc_audio_save = gst_element_factory_make("filesrc", "filesrc_audio_save");
  g_object_set(G_OBJECT(filesrc_audio_save), "location", "input_audio1.wav", NULL);
  qtdemux_save = gst_element_factory_make("qtdemux", "qtdemux_save");
  h264parse_save = gst_element_factory_make("h264parse", "h264parse_save");
  avdec_h264_save = gst_element_factory_make("avdec_h264", "avdec_h264_save");
  videoconvert_save = gst_element_factory_make("videoconvert", "videoconvert_save");
  mpph264enc_save = gst_element_factory_make("mpph264enc", "mpph264enc_save");
  mp4mux_save = gst_element_factory_make("mp4mux", "mp4mux_save");
  decodebin_save = gst_element_factory_make("decodebin", "decodebin_save");
  audioconvert_save = gst_element_factory_make("audioconvert", "audioconvert_save");
  voaacenc_save = gst_element_factory_make("voaacenc", "voaacenc_save");
  filesink_save = gst_element_factory_make("filesink", "filesink_save");
  g_object_set(G_OBJECT(filesink_save), "location", "output.mp4", NULL);
  queue_save = gst_element_factory_make("queue", "queue_save");
    
    /* 检查所有元素是否创建成功 */
  if (!pipeline_save || !filesrc_video_save || !filesrc_audio_save || !qtdemux_save || !h264parse_save || !avdec_h264_save
      || !videoconvert_save || !mpph264enc_save || !mp4mux_save || !decodebin_save || !audioconvert_save || !queue_save
      || !voaacenc_save  || !filesink_save) {
      g_printerr("创建元素失败\n");
      return -1;
  }

  // 将元素添加到主管道
  gst_bin_add_many(GST_BIN(pipeline_save), filesrc_video_save, qtdemux_save, h264parse_save, avdec_h264_save,
                   videoconvert_save, mpph264enc_save, mp4mux_save, filesrc_audio_save, decodebin_save,
                   audioconvert_save, voaacenc_save, queue_save,  filesink_save, NULL);

  // 连接元素之间的always pads
  if (
    !gst_element_link_many(filesrc_video_save, qtdemux_save, NULL) ||
    !g_signal_connect (qtdemux_save, "pad-added", G_CALLBACK (qtdemux_pad_added_handler), h264parse_save)||  
    !gst_element_link(h264parse_save, avdec_h264_save) ||
    !gst_element_link(avdec_h264_save, videoconvert_save) ||
    !gst_element_link(videoconvert_save, mpph264enc_save) ||
    !gst_element_link(mpph264enc_save, mp4mux_save) ||
    !gst_element_link(filesrc_audio_save, decodebin_save) ||
    !g_signal_connect (decodebin_save, "pad-added", G_CALLBACK (decodebin_pad_added_handler), audioconvert_save)||
    !gst_element_link(audioconvert_save, voaacenc_save) ||
    !gst_element_link(voaacenc_save, queue_save) ||
    !gst_element_link(queue_save, mp4mux_save) ||
    !gst_element_link(mp4mux_save, filesink_save)
    ) 
  {
    g_printerr("Failed to link elements. Exiting.\n");
    gst_object_unref(pipeline_save);
    return -1;
  }

//pipeline play: mux a video and an audio and play
   /* 创建各个元素 */
  pipeline_play = gst_pipeline_new("pipeline");
  filesrc_video_play = gst_element_factory_make("filesrc", "filesrc_video_play");
  g_object_set(G_OBJECT(filesrc_video_play), "location", "input_video1.mp4", NULL);
  filesrc_audio_play = gst_element_factory_make("filesrc", "filesrc_audio_play");
  g_object_set(G_OBJECT(filesrc_audio_play), "location", "input_audio1.wav", NULL);
  qtdemux_play = gst_element_factory_make("qtdemux", "qtdemux_play");
  h264parse_play = gst_element_factory_make("h264parse", "h264parse_play");
  avdec_h264_play = gst_element_factory_make("avdec_h264", "avdec_h264_play");
  videoconvert_play = gst_element_factory_make("videoconvert", "videoconvert_play");
  autovideosink_play = gst_element_factory_make("autovideosink", "autovideosink_play");

  decodebin_play = gst_element_factory_make("decodebin", "decodebin_play");
  audioconvert_play = gst_element_factory_make("audioconvert", "audioconvert_play");
  autoaudiosink_play = gst_element_factory_make("autoaudiosink", "autoaudiosink_play");
    
    /* 检查所有元素是否创建成功 */
  if (!pipeline_play || !filesrc_video_play || !filesrc_audio_play || !qtdemux_play || !h264parse_play || !avdec_h264_play
      || !videoconvert_play || !autovideosink_play || !decodebin_play || !audioconvert_play || !autoaudiosink_play) 
      {
      g_printerr("创建元素失败\n");
      return -1;
  }

  // 将元素添加到主管道
  gst_bin_add_many(GST_BIN(pipeline_play), filesrc_video_play, qtdemux_play, h264parse_play, avdec_h264_play,
                   videoconvert_play, autovideosink_play, filesrc_audio_play, decodebin_play,
                   audioconvert_play, autoaudiosink_play, NULL);

  // 连接元素之间的always pads
  if (
    !gst_element_link_many(filesrc_video_play, qtdemux_play, NULL) ||
    !g_signal_connect (qtdemux_play, "pad-added", G_CALLBACK (qtdemux_pad_added_handler), h264parse_play)||  
    !gst_element_link(h264parse_play, avdec_h264_play) ||
    !gst_element_link(avdec_h264_play, videoconvert_play) ||
    !gst_element_link(videoconvert_play, autovideosink_play) ||
    !gst_element_link(filesrc_audio_play, decodebin_play) ||
    !g_signal_connect (decodebin_play, "pad-added", G_CALLBACK (decodebin_pad_added_handler), audioconvert_play)||
    !gst_element_link(audioconvert_play, autoaudiosink_play) 
    ) 
  {
    g_printerr("Failed to link elements. Exiting.\n");
    gst_object_unref(pipeline_play);
    return -1;
  }
  

//pipeline trans: video bitrate transform
  /* 创建各个元素 */
  pipeline_trans      = gst_pipeline_new("pipeline");
  filesrc_video_trans = gst_element_factory_make("filesrc","filesrc_video_trans");
  //filename 等待后续写入
  //g_object_set(G_OBJECT(filesrc_video_trans), "location", "input_video.mp4", NULL);
  decodebin_trans     = gst_element_factory_make("decodebin", "decodebin_trans");
  mpph264enc_trans       = gst_element_factory_make("mpph264enc", "mpph264enc_trans");
  video_queue_trans   = gst_element_factory_make("queue", "video_queue_trans");
  mp4mux_trans        = gst_element_factory_make("mp4mux", "mp4mux_trans");
  filesink_trans      = gst_element_factory_make("filesink", "filesink_trans");
  //filename 后续写入
  //g_object_set(G_OBJECT(filesink_save), "location", "output.mp4", NULL);
  audioconvert_trans  = gst_element_factory_make("audioconvert", "audioconvert_trans");
  lamemp3enc_trans    = gst_element_factory_make("lamemp3enc", "lamemp3enc_trans");
  audio_queue_trans   = gst_element_factory_make("queue", "audio_queue_trans");

  /* 检查所有元素是否创建成功 */
  if(!pipeline_trans || !filesrc_video_trans || !decodebin_trans || 
      !mpph264enc_trans || !mp4mux_trans || !filesink_trans
    || !audioconvert_trans || !lamemp3enc_trans
    || !video_queue_trans || !audio_queue_trans
    ) 
  {
    g_printerr("创建元素失败\n");
    return -1;
  }

  // 将元素添加到主管道
  gst_bin_add_many(GST_BIN(pipeline_trans), filesrc_video_trans, decodebin_trans, 
                  mpph264enc_trans,video_queue_trans,mp4mux_trans, filesink_trans,
                  audioconvert_trans,lamemp3enc_trans,
                  audio_queue_trans,NULL);

  // 连接元素之间的always pads
  if (
    !gst_element_link(filesrc_video_trans, decodebin_trans) ||
    !g_signal_connect(decodebin_trans, "pad-added", G_CALLBACK (decodebin_pad_added_handler), mpph264enc_trans)||
    !gst_element_link(mpph264enc_trans, video_queue_trans) ||
    !gst_element_link(video_queue_trans, mp4mux_trans) ||
    !gst_element_link(mp4mux_trans, filesink_trans) ||
    !g_signal_connect(decodebin_trans, "pad-added", G_CALLBACK (decodebin_pad_added_handler), audioconvert_trans)||
    !gst_element_link(audioconvert_trans, lamemp3enc_trans) ||
    !gst_element_link(lamemp3enc_trans, audio_queue_trans) ||
    !gst_element_link(audio_queue_trans, mp4mux_trans)
    ) 
  {
    g_printerr("Failed to link elements. Exiting.\n");
    gst_object_unref(pipeline_save);
    return -1;
  }

  //start main loop
  loop = g_main_loop_new(NULL, FALSE);

  // hint
  g_print("please enter command\nsave\t\t: save video,\nstopsave\t: stop save video,\nplay\t\t: play on screen,\nstopplay\t: stop play on screen,\ntrans\t\t: video bitrate transform,\nexit\t\t: exit the exe\n");
  
  // command acquisition loop
  while (1) {
    gchar cmd[128];

    if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
      continue;
    }

    if (strcmp(cmd, "save\n") == 0) {
      mux_video_audio_save(pipeline_save);
    }else if (strcmp(cmd, "stopsave\n") == 0) {
      mux_video_audio_stopsave(pipeline_save);
    }else if (strcmp(cmd, "play\n") == 0) {
      mux_video_audio_play(pipeline_play);
    }else if (strcmp(cmd, "stopplay\n") == 0) {
      mux_video_audio_stopplay(pipeline_play);
    }else if (strcmp(cmd, "trans\n") == 0) {
      mux_video_audio_trans(pipeline_trans,filesrc_video_trans,filesink_trans);
    }else if (strcmp(cmd, "stoptrans\n") == 0) {
      mux_video_audio_stoptrans(pipeline_trans);
    }else if (strcmp(cmd, "exit\n") == 0) {
      if (pipeline_save != NULL) {
        gst_element_set_state(pipeline_save, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline_save));
      }
      if (pipeline_play != NULL) {
        gst_element_set_state(pipeline_play, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline_play));
      }
      break;
    }
  }

  g_main_loop_unref(loop);

  return 0;
}