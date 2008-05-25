require './json'
require 'test/unit'

class JSONTest < Test::Unit::TestCase
  
  def setup
    @json_hash = '{"hello": {"world" : 10}, "world": 10.1, "null": null}'
    @json_array = '["hello", "world", null, 10, 10.1, [1,2,3]]'
        
    @hash = {"hello" => {"world" => 10}, "world"=> 10.1, "null"=> nil}
    @array = ["hello", "world", nil, 10, 10.1, [1,2,3]]    
  end
  
  def test_parsing()
    assert(JSON::parse(@json_array) == @array)
    assert(JSON::parse(@json_hash) == @hash)
  end
  
  def test_constants()
    assert(JSON::VERSION, 0.1)
  end
  
  def test_json_generation()
    assert (JSON::parse(JSON::generate(10)) == 10) 
    assert (JSON::parse(JSON::generate(10.1)) == 10.1)
    assert (JSON::parse(JSON::generate("Hello")) == "Hello")
    assert (JSON::parse(JSON::generate(@hash)) == @hash)
    assert (JSON::parse(JSON::generate(@array)) == @array)
  end
    
end

