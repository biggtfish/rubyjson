require 'mkmf'

dir_config('json', 'include', 'lib')

have_header('stdio.h')
have_header('json.h')
have_library('json')

create_makefile('json')