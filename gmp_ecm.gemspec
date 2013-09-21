Gem::Specification.new do |s|
  s.name = "gmp_ecm"
  s.version = "0.3.1"
  s.authors = ["srawlins"]
  s.date = "2012-11-09"
  s.description = "gmp_ecm - Elliptic Curve Method Factorization on GMP::Z integers"
  s.email = ["sam.rawlins@gmail.com"]

  s.extensions = ["ext/extconf.rb"]

  s.has_rdoc = "yard"
  s.homepage = "http://github.com/srawlins/gmp-ecm"
  s.summary = "Provides Ruby bindings to the GMP-ECM library."

  s.required_ruby_version = '>= 1.8.7'
  s.requirements = ["GMP-ECM compiled and working properly."]
  s.add_dependency('gmp')
  s.require_paths = ["lib"]
  s.files  = Dir["ext/*.c"] + Dir["ext/*.h"] + ["ext/extconf.rb"]
  s.files += Dir["lib/ecm.rb"]

  s.files += Dir["spec/*.rb"]
  s.files += ["README.md", "COPYING.md", "Gemfile", "Guardfile"]
end
