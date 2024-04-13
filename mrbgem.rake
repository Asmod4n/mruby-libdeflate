MRuby::Gem::Specification.new('mruby-libdeflate') do |spec|
  spec.license = 'Apache-2.0'
  spec.author  = 'Hendrik Beskow'
  spec.summary = 'libdeflate for mruby'
  spec.add_dependency 'mruby-errno'

  unless spec.search_package('libdeflate')
    raise "mruby-libdeflate: cannot find libdeflate development headers and libraries, please install it."
  end
end
