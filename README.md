# mruby-libdeflate
mruby wrapper for libdeflate

Installation
============

Add this
```ruby
conf.gem mgem: 'mruby-libdeflate'
```
to your build_config.rb

Examples
========
Compression
-----------

```ruby
compressor = Deflate::Compressor.new #default level is 6, can be 0-12, 0 means no compression, 12 means slowest compression
deflate = compressor.deflate "hallo"
zlib = compressor.zlib "hallo"
gzip = compressor.gzip "hallo"
```

Decompression
-------------
```ruby
decompressor = Deflate::Decompressor.new
puts decompressor.deflate(deflate)
puts decompressor.zlib(zlib)
puts decompressor.gzip(gzip)
```
Outputs hallo 3 times

## Acknowledgments

This project bundles [libdeflate](https://github.com/ebiggers/libdeflate),  
a fast compression/decompression library by Eric Biggers.

libdeflate is distributed under the MIT license.  
The original source and license text are included in `deps/libdeflate/`.

