class Zlib
  DEFAULT_COMPRESSION = -1
  MAX_WBITS = 15
  DEF_MEM_LEVEL = 8
  DEFAULT_STRATEGY = 0

  class Deflate
    @@compressor = ::Deflate::Compressor.new

    def self.deflate(data, _ = nil, _ = nil, _ = nil, _ = nil)
      @@compressor.deflate(data)
    end
  end
end
