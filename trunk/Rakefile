#
# Ruby JSON Extension
#

require 'rake/gempackagetask'
require 'rake/testtask'

spec = Gem::Specification.new do |s|
	s.name    = "ruby-jsonc"
	s.version = "0.9" 
	s.summary = "Ruby Jsonc extension"
	s.description = "Ruby JSON-C bindings."
	s.files = 	[ "Rakefile", "README"] +
				Dir.glob("lib/**/*")

	s.require_path = 'lib'

	s.author = "Giancarlo Bellido"
	s.email  = "giancarlo.bellido@gmail.com"
	s.homepage= "cpb.coaxialhost.com"
end

# If you are using nmake run rake MAKE=nmake
ENV['MAKE'] = 'make' unless ENV['MAKE']

desc "Makes Extension"
task :make do
	
	Dir.chdir('ext/jsonc') do
		ruby 'extconf.rb'
		system("#{ENV['MAKE']}")
		FileUtils.cp('jsonc.so', '../../lib')
	end

end

desc "Runs Benchmarks"
task :bm do
	ruby '-Ilib benchmark/bm.rb'
end

desc "Runs make taks."
task :default => :make

Rake::GemPackageTask.new(spec) do |p|
	p.gem_spec = spec
	p.need_tar = true
end

desc 'Runs unit tests.'
Rake::TestTask.new do |t|
    t.libs << "test"
    t.test_files = FileList['test/*.rb']
    t.verbose = true
end
