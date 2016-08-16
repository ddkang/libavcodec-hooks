#include "internal.h"
#include "avcodec.h"
#include "golomb.h"
#include "mpegutils.h"
#include "libavutil/avassert.h"
#include "coding_hooks.h"


#ifndef AVCODEC_H264_CAVLC_H
#define AVCODEC_H264_CAVLC_H

#define LEVEL_TAB_BITS 8
static int8_t cavlc_level_tab[7][1<<LEVEL_TAB_BITS][2];

static inline int get_level_prefix(GetBitContext *gb){
    if (gb->cavlc_hooks && gb->cavlc_hooks->get_level_prefix) {
        return gb->cavlc_hooks->get_level_prefix(gb->cavlc_hooks_opaque);
    }
    {
    unsigned int buf;
    int log;

    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf=GET_CACHE(re, gb);

    log= 32 - av_log2(buf);
#ifdef TRACE
    print_bin(buf>>(32-log), log);
    av_log(NULL, AV_LOG_DEBUG, "%5d %2d %3d lpr @%5d in %s get_level_prefix\n", buf>>(32-log), log, log-1, get_bits_count(gb), __FILE__);
#endif

    LAST_SKIP_BITS(re, gb, log);
    CLOSE_READER(re, gb);

    return log-1;
    }
}

#endif /* AVCODEC_H264_CAVLC_H */
