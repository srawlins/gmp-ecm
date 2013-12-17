require "rspec/core/rake_task"
require "gmp"

# Dependencies base directory. I have to figure out how to... not hard code this?
DEPENDENCIES_DIR = '/usr/local'

task :clean do
  sh "cd ext && make clean; echo"
end

task :extconf => [:clean] do
  sh "cd ext && ruby extconf.rb #{ecm_opt}"
end

task :make => [:extconf] do
  sh "cd ext && make"
end

task :console do
  require 'irb'
  require File.join(File.dirname(__FILE__), 'lib', 'ecm')
  ARGV.clear
  IRB.start
end

desc "Run all specs"
RSpec::Core::RakeTask.new(:spec) do |t|
end

task :default => [:ci]
task :ci => [:make, :spec]

namespace :dependencies do
  task :list do
    suffix = (DEPENDENCIES_DIR.size+1)..-1

    puts "GMP version linked in gmp gem: #{GMP::GMP_VERSION}"
    puts ""

    puts "GMP-ECM packages installed #{DEPENDENCIES_DIR}:"
    Dir.glob(File.join(DEPENDENCIES_DIR, 'ecm') + '*').each do |dir|
      puts "    #{dir[suffix]}"
    end
  end
end

def ecm_opt
  version = ENV['ECM'] || '6.4.4'
  return '--with-ecm-dir='+File.join(DEPENDENCIES_DIR, "ecm-#{version}")
end


