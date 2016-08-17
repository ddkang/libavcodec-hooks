/**
 * AVCodecCodingHooks: table of callbacks used to hook deeply into the decoding
 * stack of certain codecs. Used for recoding.
 */

#ifndef AVCODEC_CODING_HOOKS_H
#define AVCODEC_CODING_HOOKS_H


#define EACH_PIP_CODING_TYPE(fn) \
    fn(PIP_UNKNOWN) \
    fn(PIP_UNREACHABLE) \
    fn(PIP_SIGNIFICANCE_MAP) \
    fn(PIP_SIGNIFICANCE_EOB) \
    fn(PIP_RESIDUALS) \
    fn(PIP_SIGNIFICANCE_NZ) \
    fn(PIP_INTRA_MB_TYPE) \
    fn(PIP_INTRA4X4_PRED_MODE) \
    fn(PIP_MB_CBP_LUMA) \
    fn(PIP_MB_MVD) \
    fn(PIP_MB_SKIP_FLAG) \
    fn(PIP_MB_CHROMA_PRE_MODE) \
    fn(PIP_MB_CBP_CHROMA) \
    fn(PIP_P_MB_SUB_TYPE) \
    fn(PIP_B_MB_SUB_TYPE) \
    fn(PIP_MB_REF) \
    fn(PIP_CODED_BLOCK)

#define ITEM_COMMA(a) a,

typedef enum CodingType {
    EACH_PIP_CODING_TYPE(ITEM_COMMA)
    PIP_NUM_TYPES,
} CodingType;

struct CABACContext;
struct GetBitContext;


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
   void (*copy_coefficients)(void *opaque, int16_t *block, int max_coeff);
   void (*set_mb_type)(void *opaque, int mb_type);
} ModelHooks;
typedef struct CABACCodingHooks {
  // Called by h264 decoder before decoding a CABAC-encoded block.
  // Returns a new opaque pointer which will be passed to other CABAC hooks.
  void* (*init_decoder)(void *opaque, struct CABACContext *c, const uint8_t *buf,
                        uint8_t *state_start, int size);

  int (*get)(void *opaque, uint8_t *state);
  int (*get_bypass)(void *opaque);
  int (*get_sign_bypass)(void *opaque);
  int (*get_terminate)(void *opaque);
  const uint8_t* (*skip_bytes)(void *opaque, int n);
} CABACCodingHooks;
typedef struct CAVLCCodingHooks {
  // Called by h264 decoder before decoding a CAVLC-encoded block.
  // Returns a new opaque pointer which will be passed to other CAVLC hooks.
  // Alright, how do I deal with this void pointer. It's actually GetBitContext
  void* (*init_decoder)(void *opaque, void* gb, const uint8_t *buf,
                        uint8_t *state_start, int size);
  // Golomb hooks
  int (*get_ue_golomb)(void *opaque);
  int (*get_ue_golomb_31)(void *opaque);
  unsigned (*get_ue_golomb_long)(void *opaque);
  int (*get_se_golomb)(void *opaque);
  // Get bit hooks
  unsigned int (*get_bits)(void *opaque, int n);
  unsigned int (*get_bits1)(void *opaque);
  // The type here is actually VLC_TYPE
  int (*get_vlc2)(void *opaque, int16_t (*table)[2], int bits, int max_depth);
  int (*get_level_prefix)(void *opaque);
  unsigned int (*show_bits)(void *opaque, int n);
  unsigned int (*recode_show_bits)(void *opaque, const int8_t table[256][2], int n);
  void (*skip_bits)(void *opaque, int n);
  void (*terminate)(void *opaque);
} CAVLCCodingHooks;

typedef struct AVCodecHooks {
  // Passed to callbacks.
  void *opaque;

  // Hook into CABAC-encoded block decoding.
  CABACCodingHooks coding_hooks;
  CAVLCCodingHooks cavlc_hooks;
  ModelHooks model_hooks;
} AVCodecHooks;

#endif  // AVCODEC_CODING_HOOKS_H
