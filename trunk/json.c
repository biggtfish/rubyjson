#include <ruby.h>
#include <json.h>
#include <string.h>

#ifdef RJ_DEBUG_ENABLED
#	define RJ_DEBUG(x) puts(x)
#	define RJ_DEBUGF(x, val) printf(x, val)
#else
#	define RJ_DEBUG(x)
#	define RJ_DEBUGF(x, val)
#endif

VALUE rubyjson_module;
VALUE rubyjson_version;

/**
 * Recursively builds a Ruby VALUE using json_object routines.
 * 
 */
VALUE rubyjson_parse_json_object(struct json_object* object)
{
	VALUE result;
	
	switch (json_object_get_type(object)) {
	case json_type_object:
		RJ_DEBUG("Adding Object.");
		result = rubyjson_parse_object(object);
		break;
	case json_type_array:
		RJ_DEBUG("Adding Array.");
		result = rubyjson_parse_array(object);
		break;
	case json_type_string:
		result = rb_str_new2(json_object_get_string(object));
		break;
	case json_type_int:
		result = INT2NUM(json_object_get_int(object));
		break;
	case json_type_double:
		result = DOUBLE2NUM(json_object_get_double(object));
		break;
	case json_type_boolean:
		result = (json_object_get_boolean ? Qtrue : Qfalse);
		break;
	case json_type_null:
		RJ_DEBUG("Adding Null.");
		result = Qnil;
		break;
	default:
		rb_raise(rb_eArgError, "JSON type not implemented yet.");
		result = Qnil;
	}
	
	return result;
}

/**
 * Parses a JSON object into a Hash
 * 
 * @return Hash
 */
VALUE rubyjson_parse_object(struct json_object* obj)
{
	VALUE hash = rb_hash_new();
	
	json_object_object_foreach (obj, key, value)
	{
		RJ_DEBUGF("Adding Key %s to object.", key);
		rb_hash_aset(hash, rb_str_new2(key), rubyjson_parse_json_object(value));
		RJ_DEBUG("[OK]");
	}
	
	return hash;
}

/**
 * Parses a JSON Array into a Ruby Array
 * 
 * @return Array
 */
VALUE rubyjson_parse_array(struct json_object* obj)
{
	VALUE array = rb_ary_new();
	int i;
		
	for(i = 0; i<json_object_array_length(obj); i++)
		rb_ary_push(array, rubyjson_parse_json_object(json_object_array_get_idx(obj, i)));
	
	return array;
}

/**
 * Parses a JSON string into a Ruby Object
 */
VALUE rubyjson_parse(VALUE module, VALUE str)
{
	struct json_object *object;
	const char *s; 
	VALUE result = Qnil;
	
	/* Mandatory Type Checking */
	if (rb_type(str) != T_STRING)
		rb_raise(rb_eArgError, "Argument should be a string.");
	
	/* Convert Ruby string to char* */
	s = RSTRING_PTR(str);
	
	object = json_tokener_parse(s);
	
	result = rubyjson_parse_json_object(object);
	/*result = rb_str_new2(json_object_to_json_string(object));*/
	
	json_object_put(object);
	 
	return result;
}

struct json_object* rubyjson_generate_array(VALUE);
struct json_object* rubyjson_generate_hash(VALUE);

/**
 * Generates a JSON string from a Ruby Object
 */
struct json_object* rubyjson_generate_json_object(VALUE object)
{
	struct json_object* json;
		
	switch (rb_type(object)) {
	case T_BIGNUM:
	case T_FIXNUM:
		json = json_object_new_int(NUM2INT(object));
		break;
	case T_FLOAT:
		json = json_object_new_double(NUM2DBL(object));
		break;
	case T_HASH:
		json = rubyjson_generate_hash(object);
		break;
	case T_ARRAY:
		json = rubyjson_generate_array(object);
		break;
	case T_STRING:
		json = json_object_new_string(RSTRING_PTR(object));
		break;
	case T_NIL:
		json = NULL;
		break;
	default:
		rb_raise(rb_eArgError, "Ruby type not implemented yet.");	
	}	
	
	return json;
}

/**
 * Iterator for Ruby Hash pair
 */
int rubyjson_hash_i(VALUE key, VALUE value, struct json_object* json)
{
	json_object_object_add(json, StringValuePtr(key), rubyjson_generate_json_object(value));
	return ST_CONTINUE;
}

/**
 * Iterates through a Ruby Hash and generates a json_object*
 */
struct json_object* rubyjson_generate_hash(VALUE object)
{
	struct json_object *json = json_object_new_object();
	
	/*RETURN_ENUMERATOR(object, 0, 0);*/
	rb_hash_foreach(object, rubyjson_hash_i, (VALUE)json);
	
	return json;
}

/**
 * Iterates through a Ruby Array and returns a json_object
 */
struct json_object* rubyjson_generate_array(VALUE object)
{
	struct json_object *json;
	int i;
	
	json  =	json_object_new_array();
	
	for (i=0; i<RARRAY_LEN(object); i++)
	{
		json_object_array_add(json, rubyjson_generate_json_object( RARRAY(object)->ptr[i] ));
	}
		
	return json;
}

/**
 * Generates a JSON string from a Ruby Object
 */
VALUE rubyjson_generate(VALUE module, VALUE object)
{
	VALUE result;
	
	struct json_object* json = rubyjson_generate_json_object(object);
	
	result = rb_str_new2(json_object_to_json_string(json));
	
	json_object_put(json);
	
	return result;
}

/**
 * Initialises JSON Ruby Extension
 */
void Init_json()
{
	rubyjson_module = rb_define_module("JSON");
	rubyjson_version = rb_float_new(0.1);
	
	rb_define_module_function(rubyjson_module, "parse", &rubyjson_parse, 1);
	rb_define_module_function(rubyjson_module, "generate", &rubyjson_generate, 1);
	
	rb_define_const(rubyjson_module, "VERSION", rubyjson_version);
}

