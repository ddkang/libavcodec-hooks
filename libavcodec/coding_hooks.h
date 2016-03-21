/**
 * AVCodecCodingHooks: table of callbacks used to hook deeply into the decoding
 * stack of certain codecs. Used for recoding.
 */

#ifndef AVCODEC_CODING_HOOKS_H
#define AVCODEC_CODING_HOOKS_H


typedef enum CodingType {
    PIP_UNKNOWN,
    PIP_UNREACHABLE,
    PIP_SIGNIFICANCE_MAP,
    PIP_SIGNIFICANCE_EOB,
    PIP_RESIDUALS,
} CodingType;

struct CABACContext;


typedef struct ModelHooks_ {
  void (*frame_spec)(void *opaque, int frame_num, int mb_width, int mb_height);
  void (*mb_xy)(void *opaque, int x, int y);
  // for scan8index docu see h264.h line 981: Scan8 organization and cat docu in h264_cabac.c:1807
  void (*begin_sub_mb)(void *opaque,
                       int cat, int scan8index, int max_coeff, int is_dc, int chroma422);
  void (*end_sub_mb)(void *opaque,
                     int cat, int scan8index, int max_coeff, int is_dc, int chroma422);
   void (*begin_coding_type)(void *opaque, CodingType pp_type, int param0, int param1, int param2);
   void (*end_coding_type)(void *opaque, CodingType pp_type);
} ModelHooks;
typedef struct CABACCodingHooks {
  // Called by h264 decoder before decoding a CABAC-encoded block.
  // Returns a new opaque pointer which will be passed to other CABAC hooks.
  void* (*init_decoder)(void *opaque, struct CABACContext *c, const uint8_t *buf, int size);

  int (*get)(void *opaque, uint8_t *state);
  int (*get_bypass)(void *opaque);
  int (*get_terminate)(void *opaque);
  const uint8_t* (*skip_bytes)(void *opaque, int n);
} CABACCodingHooks;

typedef struct AVCodecHooks {
  // Passed to callbacks.
  void *opaque;

  // Hook into CABAC-encoded block decoding.
  CABACCodingHooks coding_hooks;
  ModelHooks model_hooks;
} AVCodecHooks;

#endif  // AVCODEC_CODING_HOOKS_H
