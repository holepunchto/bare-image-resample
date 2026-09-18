#include <assert.h>
#include <bare.h>
#include <js.h>
#include <stdint.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC

#include <stb_image_resize2.h>

// A pixel is four bytes, one 8-bit channel each for R, G, B and A. It is the
// only layout this binding handles: it is what STBIR_RGBA asks stbir_resize()
// for, and it is how both the source and the target buffers are laid out.
static const int64_t BYTES_PER_PIXEL = 4;

// Cap the resampled pixel count to keep the requested dimensions from reaching
// the allocator. 256 Mpx at 4 bytes per pixel covers up to a 16384x16384 image.
#define BARE_IMAGE_RESAMPLE_MAX_PIXELS (1ull << 28)

static js_value_t *
bare_image_resample_resize(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 5);

  uint8_t *source;
  size_t source_len;
  err = js_get_typedarray_info(env, argv[0], NULL, (void **) &source, &source_len, NULL, NULL);
  assert(err == 0);

  int64_t source_width;
  err = js_get_value_int64(env, argv[1], &source_width);
  assert(err == 0);

  int64_t source_height;
  err = js_get_value_int64(env, argv[2], &source_height);
  assert(err == 0);

  int64_t target_width;
  err = js_get_value_int64(env, argv[3], &target_width);
  assert(err == 0);

  int64_t target_height;
  err = js_get_value_int64(env, argv[4], &target_height);
  assert(err == 0);

  if (
    source_width <= 0 || source_width > INT32_MAX ||
    source_height <= 0 || source_height > INT32_MAX
  ) {
    err = js_throw_error(env, NULL, "Invalid source dimensions");
    assert(err == 0);

    return NULL;
  }

  if ((uint64_t) source_width * (uint64_t) source_height * BYTES_PER_PIXEL > (uint64_t) source_len) {
    err = js_throw_error(env, NULL, "Source buffer too small for its dimensions");
    assert(err == 0);

    return NULL;
  }

  if (
    target_width <= 0 || target_width > INT32_MAX ||
    target_height <= 0 || target_height > INT32_MAX
  ) {
    err = js_throw_error(env, NULL, "Invalid target dimensions");
    assert(err == 0);

    return NULL;
  }

  if (
    (uint64_t) source_width * (uint64_t) source_height > BARE_IMAGE_RESAMPLE_MAX_PIXELS ||
    (uint64_t) target_width * (uint64_t) target_height > BARE_IMAGE_RESAMPLE_MAX_PIXELS
  ) {
    err = js_throw_error(env, NULL, "Image dimensions exceed maximum");
    assert(err == 0);

    return NULL;
  }

  uint64_t target_len = (uint64_t) target_width * (uint64_t) target_height * BYTES_PER_PIXEL;

  if (target_len > SIZE_MAX) {
    err = js_throw_error(env, NULL, "Target image too large");
    assert(err == 0);

    return NULL;
  }

  js_value_t *result;

  uint8_t *target;
  err = js_create_unsafe_arraybuffer(env, (size_t) target_len, (void **) &target, &result);

  if (err < 0) return NULL;

  stbir_resize(
    source,
    source_width,
    source_height,
    0,
    target,
    target_width,
    target_height,
    0,
    STBIR_RGBA,
    STBIR_TYPE_UINT8_SRGB,
    STBIR_EDGE_CLAMP,
    STBIR_FILTER_DEFAULT
  );

  return result;
}

static js_value_t *
bare_image_resample_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("resize", bare_image_resample_resize)
#undef V

  return exports;
}

BARE_MODULE(bare_image_resample, bare_image_resample_exports)
