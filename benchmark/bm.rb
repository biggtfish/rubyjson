#
# Ruby-JSONC Benchmark
#

require 'jsonc'
require 'json'
require 'benchmark'

random_int_array = []
string_array = []
mixed_array = []

1000000.times { |i| random_int_array[i] = rand }
1000000.times { |i| string_array[i] = "hello world #{i}#{i+1}#{i+2}" }
1000000.times { |i| mixed_array[i] = (i % 2 == 0) ? i : "Hello #{i}" }

json_ia = JSONC::generate(random_int_array)
json_sa = JSONC::generate(string_array)
json_ma = JSONC::generate(mixed_array)

puts "Generating"

Benchmark.bm do |b|

	b.report('jsonc random') { JSONC::generate(random_int_array) }
	b.report('json2 random') { JSON::fast_generate(random_int_array) }
	b.report('json random ') { random_int_array.to_json }
	b.report('jsonc string') { JSONC::generate(string_array) }
	b.report('json2 string') { JSON::fast_generate(string_array) }
	b.report('json string ') { string_array.to_json }
	b.report('jsonc mixed ') { JSONC::generate(mixed_array) }
	b.report('json2 mixed ') { JSON::fast_generate(mixed_array) }
	b.report('json mixed  ') { mixed_array.to_json }

end

puts "Parsing"

Benchmark.bm do |b|

	b.report('jsonc random') { JSONC::parse(json_ia) }
	b.report('json random ') { JSON::parse(json_ia) }
	b.report('jsonc string') { JSONC::parse(json_sa) }
	b.report('json string ') { JSON::parse(json_sa) }
	b.report('jsonc mixed ') { JSONC::parse(json_ma) }
	b.report('json mixed  ') { JSON::parse(json_ma) }

end
