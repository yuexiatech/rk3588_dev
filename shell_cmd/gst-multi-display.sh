gst-launch-1.0  \
videomixer name=mix \
sink_0::xpos=0 sink_0::ypos=0 sink_0::alpha=1 \
sink_1::xpos=320 sink_1::ypos=0 sink_1::alpha=1 \
sink_2::xpos=0 sink_2::ypos=240 sink_2::alpha=1 \
sink_3::xpos=320 sink_3::ypos=240 sink_3::alpha=1 \
! autovideosink qos=true \
uridecodebin uri=http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_0 \
uridecodebin uri=http://vjs.zencdn.net/v/oceans.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_1 \
uridecodebin uri=https://media.w3.org/2010/05/sintel/trailer.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_2 \
uridecodebin uri=http://mirror.aarnet.edu.au/pub/TED-talks/911Mothers_2010W-480p.mp4 ! decodebin ! videoscale ! video/x-raw,width=320,height=240 ! mix.sink_3
