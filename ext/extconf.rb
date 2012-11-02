#!/usr/bin/env ruby

require 'mkmf'
dir_config('ecm')
ok = true

unless have_header('gmp.h')
  $stderr.puts "can't find gmp.h, try --with-gmp-include=<path>"
  ok = false
end

unless have_library('gmp', '__gmpz_init')
  $stderr.puts "can't find -lgmp, try --with-gmp-lib=<path>"
  ok = false
end

unless have_header('ecm.h')
  $stderr.puts "can't find ecm.h, try --with-ecm-include=<path>"
  ok = false
end

unless have_library('ecm', 'ecm_init')
  $stderr.puts "can't find -lecm, try --with-ecm-lib=<path>"
  ok = false
end

$CFLAGS += ' -Wall -W -O6 -g'

if not ok
  raise "Unable to build, correct above errors and rerun"
end

create_makefile('ecm')
