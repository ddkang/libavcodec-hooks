/**
 * AVCodecCodingHooks: table of callbacks used to hook deeply into the decoding
 * stack of certain codecs. Used for recoding.
 */

#ifndef AVCODEC_CODING_HOOKS_H
#define AVCODEC_CODING_HOOKS_H

struct CABACContext;

typedef struct CABACHooks {
  // Called by h264 decoder before decoding a CABAC-encoded block.
  // Returns a new opaque pointer which will be passed to other CABAC hooks.
  void* (*init_decoder)(void *opaque, struct CABACContext *c, const uint8_t *buf, int size);

  int (*get)(void *opaque, uint8_t *state);
  int (*get_bypass)(void *opaque);
  int (*get_terminate)(void *opaque);
  const uint8_t* (*skip_bytes)(void *opaque, int n);
} CABACHooks;

typedef struct AVCodecCodingHooks {
  // Passed to callbacks.
  void *opaque;

  // Hook into CABAC-encoded block decoding.
  CABACHooks cabac;
} AVCodecCodingHooks;

#endif  // AVCODEC_CODING_HOOKS_H
