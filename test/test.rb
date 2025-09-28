##
# mruby-libdeflate tests
#

assert('Deflate::Compressor#deflate / Deflate::Decompressor#deflate roundtrip') do
  input = "hello world" * 10
  comp  = Deflate::Compressor.new
  decomp = Deflate::Decompressor.new

  compressed = comp.deflate(input)
  assert_true compressed.bytesize < input.bytesize

  output = decomp.deflate(compressed)
  assert_equal input, output
end

assert('Deflate::Compressor#zlib / Deflate::Decompressor#zlib roundtrip') do
  input = "zlib test data" * 20
  comp  = Deflate::Compressor.new(9)
  decomp = Deflate::Decompressor.new

  compressed = comp.zlib(input)
  assert_true compressed.bytesize < input.bytesize

  output = decomp.zlib(compressed)
  assert_equal input, output
end

assert('Deflate::Compressor#gzip / Deflate::Decompressor#gzip roundtrip') do
  input = "gzip test data" * 30
  comp  = Deflate::Compressor.new
  decomp = Deflate::Decompressor.new

  compressed = comp.gzip(input)
  assert_true compressed.bytesize < input.bytesize

  output = decomp.gzip(compressed)
  assert_equal input, output
end

assert('Deflate::Compressor accepts compression level') do
  c1 = Deflate::Compressor.new(1)
  c9 = Deflate::Compressor.new(9)
  data = "x" * 1000
  out1 = c1.deflate(data)
  out9 = c9.deflate(data)
  # level 9 should not be larger than level 1
  assert_true out9.bytesize <= out1.bytesize
end

assert('Deflate::Decompressor raises on invalid input') do
  decomp = Deflate::Decompressor.new
  begin
    decomp.deflate("not a deflate stream")
    false
  rescue => e
    assert_kind_of StandardError, e
    true
  end
end
