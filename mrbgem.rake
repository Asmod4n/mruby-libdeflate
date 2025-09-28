MRuby::Gem::Specification.new('mruby-libdeflate') do |spec|
  spec.license = 'Apache-2.0'
  spec.author  = 'Hendrik Beskow'
  spec.summary = 'libdeflate for mruby'
  spec.add_dependency 'mruby-errno'

  libdeflate_src = "#{spec.dir}/deps/libdeflate"

  spec.cc.include_paths << "#{libdeflate_src}"

  sources = Dir.glob("#{libdeflate_src}/lib/**/*.c")

  spec.objs += sources.map { |f|
    f.relative_path_from(dir).pathmap("#{build_dir}/%X#{spec.exts.object}")
  }
end
