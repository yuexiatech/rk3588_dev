//
// Created by root on 17-11-1.
//

#ifndef MPP_LINUX_C_CODEC_H
#define MPP_LINUX_C_CODEC_H

#include <iostream>
#include "../rkrga/RGA.h"

extern "C" {
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "vpu.h"
#include "rk_mpi.h"
#include "rk_type.h"
#include "vpu_api.h"
#include "mpp_err.h"
#include "mpp_task.h"
#include "mpp_meta.h"
#include "mpp_frame.h"
#include "mpp_buffer.h"
#include "mpp_packet.h"
#include "rk_mpi_cmd.h"

#include "../rkdrm/bo.h"
#include "../rkdrm/dev.h"
#include "../rkdrm/modeset.h"
};
#define SZ_1K                   (1024)
#define SZ_2K                   (SZ_1K*2)
#define SZ_4K                   (SZ_1K*4)
#define SZ_8K                   (SZ_1K*8)
#define SZ_16K                  (SZ_1K*16)
#define SZ_32K                  (SZ_1K*32)
#define SZ_64K                  (SZ_1K*64)
#define SZ_128K                 (SZ_1K*128)
#define SZ_256K                 (SZ_1K*256)
#define SZ_512K                 (SZ_1K*512)
#define SZ_1M                   (SZ_1K*SZ_1K)
#define SZ_2M                   (SZ_1M*2)
#define SZ_4M                   (SZ_1M*4)
#define SZ_8M                   (SZ_1M*8)
#define SZ_16M                  (SZ_1M*16)
#define SZ_32M                  (SZ_1M*32)
#define SZ_64M                  (SZ_1M*64)
#define SZ_80M                  (SZ_1M*80)
#define SZ_128M                 (SZ_1M*128)
#define PKT_SIZE    SZ_4K
#define CODEC_ALIGN(x, a)   (((x)+(a)-1)&~((a)-1))

class Codec {
public:
    Codec();
    ~Codec();
    int init(const char *file_input,
             const char *file_output, MppCodingType type,
             int src_w, int src_h, int display);
    int deinit();
    int decode_one_pkt(char *buf, int size, MppFrame *srcFrm, MppFrame *dstFrm);
    int decode();
    int dump_mpp_frame_to_file(MppFrame frame, FILE *fp);
    int drm_show_frmae(MppFrame frame);

    double get_frm_rate() {
        return mFrmRate;
    }
private:
    int mFps;
    int mEos;
    int mID;
    int mDisPlay;
    int mFrmCnt;
    int srcW;
    int srcH;
    int dstW;
    int dstH;

    RK_S64 mTimeS;
    RK_S64 mTimeE;
    RK_S64 mTimeDiff;
    double mFrmRate;

    FILE *mFin;
    FILE *mFout;
    char *mPktBuf;

    MppCtx mCtx;
    MppApi *mApi;
    MppPacket mPkt;
    MppBuffer mBuffer;
    MppBufferGroup mFrmGrp;

    RGA *mRGA;

    sp_dev *mDev;
    sp_plane **mPlanes;
    sp_crtc *mCrtc;
    sp_plane *mTestPlane;
};


#endif //MPP_LINUX_C_CODEC_H
