=begin
 *
 * Ruby JSON Extension
 * 
 * Copyright (c) 2008, Giancarlo Bellido
 * 
 * This copyrighted material is made available to anyone wishing to use, modify,
 * copy, or redistribute it subject to the terms and conditions of the GNU
 * Lesser General Public License, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this distribution; if not, write to:
 * Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor
 * Boston, MA  02110-1301  USA
 *
=end

require 'jsonc'
require 'test/unit'

class JSONTest < Test::Unit::TestCase
  
  def setup
    @json_hash = '{"hello": {"world" : 10}, "world": 10.1, "null": null}'
    @json_array = '["hello", "world", null, 10, 10.1, [1,2,3]]'
        
    @hash = {"hello" => {"world" => 10}, "world"=> 10.1, "null"=> nil}
    @array = ["hello", "world", nil, 10, 10.1, [1,2,3]]    
  end
  
  def test_parsing()
    assert(JSONC::parse(@json_array) == @array)
    assert(JSONC::parse(@json_hash) == @hash)
  end
  
  def test_json_generation()
    assert (JSONC::parse(JSONC::generate(10)) == 10) 
    assert (JSONC::parse(JSONC::generate(10.1)) == 10.1)
    assert (JSONC::parse(JSONC::generate("Hello")) == "Hello")
    assert (JSONC::parse(JSONC::generate(@hash)) == @hash)
    assert (JSONC::parse(JSONC::generate(@array)) == @array)
  end
    
end

