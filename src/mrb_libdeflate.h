#include <mruby.h>
#include <errno.h>
#include <mruby/error.h>
#include <mruby/data.h>
#include <libdeflate.h>
#include <mruby/string.h>
#include <mruby/class.h>
#include <mruby/presym.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define NELEMS(argv) (sizeof(argv) / sizeof(argv[0]))

static void
mrb_libdeflate_free_compressor_gc(mrb_state *mrb, void *p)
{
  libdeflate_free_compressor(p);
}

static const struct mrb_data_type mrb_libdeflate_compressor_type = {
  "$i_mrb_libdeflate_compressor_type", mrb_libdeflate_free_compressor_gc
};

static void
mrb_libdeflate_free_decompressor_gc(mrb_state *mrb, void *p)
{
  libdeflate_free_decompressor(p);
}

static const struct mrb_data_type mrb_libdeflate_decompressor_type = {
  "$i_mrb_libdeflate_decompressor_type", mrb_libdeflate_free_decompressor_gc
};