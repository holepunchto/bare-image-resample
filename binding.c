#include <assert.h>
#include <bare.h>
#include <js.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC

#include <stb_image_resize2.h>

static js_value_t *
bare_image_resample_resize(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 5);

  uint8_t *source;
  err = js_get_typedarray_info(env, argv[0], NULL, (void **) &source, NULL, NULL, NULL);
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

  js_value_t *result;

  uint8_t *target;
  err = js_create_unsafe_arraybuffer(env, target_width * target_height * 4, (void **) &target, &result);
  assert(err == 0);

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
