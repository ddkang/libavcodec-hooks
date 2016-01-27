/**
 * Table of callbacks used to hook deeply into the decoding stack of certain
 * codecs. Used for recoding.
 */
struct CABACContext;

typedef struct AVCodecCodingHooks {
  // Passed to callbacks.
  void *opaque;

  // Called by h264 decoder before decoding a CABAC-encoded block.
  void (*init_cabac_decoder)(void *opaque, struct CABACContext *c, const uint8_t *buf, int size);
} AVCodecCodingHooks;
