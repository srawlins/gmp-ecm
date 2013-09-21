GMP-ECM
=======

This library, gmp_ecm, provides Ruby bindings to GMP-ECM, a C implementation of
the [Elliptic Curve Method](http://en.wikipedia.org/wiki/Elliptic_curve_method)
for factorizing integers.

Install
=======

If you're using Bundler, add `gem 'gmp_ecm'` to your Gemfile.

If you just want to install with Rubygems, just `gem install gmp_ecm`.

The only dependency is the [gmp](https://github.com/srawlins/gmp) gem, which
provides Ruby bindings to GMP. The gmp gem is required because gmp_ecm passes
around most numbers (input integer, resultant factors, and various parameters)
as `GMP::Z` numbers.

Usage
=====

gmp_ecm essentially just provides the
`GMP::Z#ecm_factor` method (binding to GMP-ECM's `ecm_params` C function). So
in order to start factorizing an integer, one must first initialize a `GMP::Z`
integer, and then call `#ecm_factor`. Just as in the C library, this
method requires the `B1` parameter as the first parameter. Optionally, you can
pass in a second parameter, the params, as either a Hash or an ECMParams object.

Not all of the fields in `ecm_params` are available yet. The current list of
supported parameters is in the docs.

Examples
========

Here is an example of repeatedly hitting up GMP-ECM for factors of `2**71 - 1`:

```ruby
z = GMP::Z(2)**71 - 1 #=> 2361183241434822606847
z.ecm_factor(1_000_000)  #=> [1, 2361183241434822606847]
z.ecm_factor(1_000_000)  #=> [1, 11091312221959]
z.ecm_factor(1_000_000)  #=> [1, 2361183241434822606847]
z.ecm_factor(1_000_000)  #=> [1, 2361183241434822606847]
z.ecm_factor(1_000_000)  #=> [1, 48639942238007]
```

`GMP::Z#ecm_factor` returns a pair. The first element tells whether or not a
factor was found:

* 1 means a factor was found in step 1.
* 2 means a factor was found in step 2.
* 0 means no factor was found.
* a negative value means an error occurred.

ECMParams
=========

The ECMParams class is not implemented yet. For now, you can supply the `ecm_params` parameter as a hash:

* Any value expected to be a `mpz_t` can be a Fixnum or a `GMP::Z`.
* Any value expected to be an `int`, including `unsigned`, must be a Fixnum.
* Any value expected to be a double must be a ruby Float.
* Any value expected to be a `FILE*` must be a ruby IO object, typically created with `File.open('filename', 'w')`.

License
=======

gmp_ecm is licensed under the Apache License, Version 2.0
