GMP-ECM
=======

GMP-ECM is a C implementation of the [Elliptic Curve Method](http://en.wikipedia.org/wiki/Elliptic_curve_method) for factorizing integers. This library, gmp_ecm, provides Ruby bindings to this library.

The only prerequisite to using gmp_ecm is the [gmp](https://github.com/srawlins/gmp) gem, which provides Ruby bindings to GMP, a prerequisite to using the GMP-ECM library. The gmp gem is required because most numbers (input integer, resultant factors, and various parameters) are passed around as `GMP::Z` numbers.

Usage
=====

The function of the gmp_ecm library is contained almost entirely in the `GMP::Z#ecm_factor` method (binding to GMP-ECM's `ecm_params` C function). So in order to start factorizing an integer, one must first initialize a `GMP::Z` number, and then call `#ecm_factor` against it. Just as in the C library, this method requires the `B1` parameter to be passed in. Optionally, the user can pass in a second parameter, a Hash, or an ECMParams object. This will be passed to `ecm_factor` as an `ecm_params` struct.

Just now, not all of the fields in `ecm_params` is available. The current list of supported parameters is in the docs.

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

`GMP::Z#ecm_factor` returns a pair. The first element tells whether or not a factor was found:

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
