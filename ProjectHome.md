Parses and Generates JSON from and to Ruby Objects. Ruby 1.9 compatible.

Ruby extension written in C. (using json-c library by Michael Clark at http://oss.metaparadigm.com/json-c/)

Usage is simple:

```
JSON::generate(object)    # Generates a JSON string from Object
JSON::parse(json_string)  # Parses a JSON string into a Ruby Object
```

See test.rb for examples. Get Sources from SVN repository.

To build:
```
ruby extconf.rb (Make sure json-c is in your library path)
make (nmake in msvc)
ruby test.rb (test it)
```

Notes for win32 users:

You might need to apply the win32.patch file to json-c library in order to compile it.