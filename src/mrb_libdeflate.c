#include "mrb_libdeflate.h"

static mrb_value
mrb_libdeflate_alloc_compressor(mrb_state *mrb, mrb_value self)
{
  mrb_int compression_level = 6;
  mrb_get_args(mrb, "|i", &compression_level);

  struct libdeflate_compressor *compressor = libdeflate_alloc_compressor((int) compression_level);
  if (unlikely(!compressor)) {
    mrb_sys_fail(mrb, "libdeflate_alloc_compressor");
  }

  mrb_data_init(self, compressor, &mrb_libdeflate_compressor_type);

  return self;
}

static mrb_value
mrb_libdeflate_deflate_compress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_compressor *compressor = DATA_PTR(self);

  char *in;
  mrb_int in_nbytes;
  mrb_get_args(mrb, "s", &in, &in_nbytes);

  mrb_value out = mrb_str_buf_new(mrb, libdeflate_deflate_compress_bound(compressor, in_nbytes));

  size_t out_len = libdeflate_deflate_compress(compressor, in, in_nbytes, RSTRING_PTR(out), RSTRING_CAPA(out));
  return mrb_str_resize(mrb, out, out_len);
}

static mrb_value
mrb_libdeflate_zlib_compress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_compressor *compressor = DATA_PTR(self);

  char *in;
  mrb_int in_nbytes;
  mrb_get_args(mrb, "s", &in, &in_nbytes);

  mrb_value out = mrb_str_buf_new(mrb, libdeflate_zlib_compress_bound(compressor, in_nbytes));

  size_t out_len = libdeflate_zlib_compress(compressor, in, in_nbytes, RSTRING_PTR(out), RSTRING_CAPA(out));
  return mrb_str_resize(mrb, out, out_len);
}

static mrb_value
mrb_libdeflate_gzip_compress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_compressor *compressor = DATA_PTR(self);

  char *in;
  mrb_int in_nbytes;
  mrb_get_args(mrb, "s", &in, &in_nbytes);

  mrb_value out = mrb_str_buf_new(mrb, libdeflate_gzip_compress_bound(compressor, in_nbytes));

  size_t out_len = libdeflate_gzip_compress(compressor, in, in_nbytes, RSTRING_PTR(out), RSTRING_CAPA(out));
  return mrb_str_resize(mrb, out, out_len);
}

static mrb_value
mrb_libdeflate_alloc_decompressor(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_decompressor *decompressor = libdeflate_alloc_decompressor();
  if(unlikely(!decompressor)) {
    mrb_sys_fail(mrb, "libdeflate_alloc_decompressor");
  }

  mrb_data_init(self, decompressor, &mrb_libdeflate_decompressor_type);

  return self;
}

static long
next_power_of_two(long n)
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#if LONG_MAX > UINT32_MAX
    n |= n >> 32;
#endif
    n++;
    return n;
}

static mrb_value
mrb_libdeflate_deflate_decompress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_decompressor *decompressor = DATA_PTR(self);
  char *in;
  mrb_int in_bytes;
  mrb_get_args(mrb, "s", &in, &in_bytes);

  mrb_value out = mrb_str_buf_new(mrb, next_power_of_two(in_bytes) << 4);
  enum libdeflate_result decompress_result;
  size_t actual_out_nbytes_ret;

  for(;;) {
    decompress_result = libdeflate_deflate_decompress(decompressor, in, in_bytes, RSTRING_PTR(out), RSTRING_CAPA(out), &actual_out_nbytes_ret);
    if (decompress_result != LIBDEFLATE_INSUFFICIENT_SPACE) {
      break;
    }

    out = mrb_str_resize(mrb, out, RSTRING_CAPA(out) << 1);
  }

  if (likely(decompress_result == LIBDEFLATE_SUCCESS)) {
    return mrb_str_resize(mrb, out, actual_out_nbytes_ret);
  } else {
    mrb_raise(mrb, E_STANDARD_ERROR, "decompress failure");
  }
}

static mrb_value
mrb_libdeflate_zlib_decompress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_decompressor *decompressor = DATA_PTR(self);
  char *in;
  mrb_int in_bytes;
  mrb_get_args(mrb, "s", &in, &in_bytes);

  mrb_value out = mrb_str_buf_new(mrb, next_power_of_two(in_bytes) << 4);
  enum libdeflate_result decompress_result;
  size_t actual_out_nbytes_ret;

  for(;;) {
    decompress_result = libdeflate_zlib_decompress(decompressor, in, in_bytes, RSTRING_PTR(out), RSTRING_CAPA(out), &actual_out_nbytes_ret);
    if (decompress_result != LIBDEFLATE_INSUFFICIENT_SPACE) {
      break;
    }

    out = mrb_str_resize(mrb, out, RSTRING_CAPA(out) << 1);
  }

  if (likely(decompress_result == LIBDEFLATE_SUCCESS)) {
    return mrb_str_resize(mrb, out, actual_out_nbytes_ret);
  } else {
    mrb_raise(mrb, E_STANDARD_ERROR, "decompress failure");
  }
}

static mrb_value
mrb_libdeflate_gzip_decompress(mrb_state *mrb, mrb_value self)
{
  struct libdeflate_decompressor *decompressor = DATA_PTR(self);
  char *in;
  mrb_int in_bytes;
  mrb_get_args(mrb, "s", &in, &in_bytes);

  mrb_value out = mrb_str_buf_new(mrb, next_power_of_two(in_bytes) << 4);
  enum libdeflate_result decompress_result;
  size_t actual_out_nbytes_ret;

  for(;;) {
    decompress_result = libdeflate_gzip_decompress(decompressor, in, in_bytes, RSTRING_PTR(out), RSTRING_CAPA(out), &actual_out_nbytes_ret);
    if (decompress_result != LIBDEFLATE_INSUFFICIENT_SPACE) {
      break;
    }

    out = mrb_str_resize(mrb, out, RSTRING_CAPA(out) << 1);
  }

  if (likely(decompress_result == LIBDEFLATE_SUCCESS)) {
    return mrb_str_resize(mrb, out, actual_out_nbytes_ret);
  } else {
    mrb_raise(mrb, E_STANDARD_ERROR, "decompress failure");
  }
}

void
mrb_mruby_libdeflate_gem_init(mrb_state* mrb)
{
  struct RClass *deflate_class, *deflate_compressor_class, *deflate_decompressor_class;

  deflate_class = mrb_define_class_id(mrb, MRB_SYM(Deflate), mrb->object_class);

  deflate_compressor_class = mrb_define_class_under_id(mrb, deflate_class, MRB_SYM(Compressor), mrb->object_class);
  MRB_SET_INSTANCE_TT(deflate_compressor_class, MRB_TT_CDATA);

  mrb_define_method_id(mrb, deflate_compressor_class, MRB_SYM(initialize),
                       mrb_libdeflate_alloc_compressor, MRB_ARGS_OPT(1));
  mrb_define_method_id(mrb, deflate_compressor_class, MRB_SYM(deflate),
                       mrb_libdeflate_deflate_compress, MRB_ARGS_REQ(1));
  mrb_define_method_id(mrb, deflate_compressor_class, MRB_SYM(zlib),
                       mrb_libdeflate_zlib_compress, MRB_ARGS_REQ(1));
  mrb_define_method_id(mrb, deflate_compressor_class, MRB_SYM(gzip),
                       mrb_libdeflate_gzip_compress, MRB_ARGS_REQ(1));

  deflate_decompressor_class = mrb_define_class_under_id(mrb, deflate_class, MRB_SYM(Decompressor), mrb->object_class);
  MRB_SET_INSTANCE_TT(deflate_decompressor_class, MRB_TT_CDATA);

  mrb_define_method_id(mrb, deflate_decompressor_class, MRB_SYM(initialize),
                       mrb_libdeflate_alloc_decompressor, MRB_ARGS_NONE());
  mrb_define_method_id(mrb, deflate_decompressor_class, MRB_SYM(deflate),
                       mrb_libdeflate_deflate_decompress, MRB_ARGS_REQ(1));
  mrb_define_method_id(mrb, deflate_decompressor_class, MRB_SYM(zlib),
                       mrb_libdeflate_zlib_decompress, MRB_ARGS_REQ(1));
  mrb_define_method_id(mrb, deflate_decompressor_class, MRB_SYM(gzip),
                       mrb_libdeflate_gzip_decompress, MRB_ARGS_REQ(1));
}


void mrb_mruby_libdeflate_gem_final(mrb_state* mrb) {}
