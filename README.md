GMP-ECM
=======

GMP-ECM is a C implementation of the [Elliptic Curve Method](http://en.wikipedia.org/wiki/Elliptic_curve_method) for factorizing integers. This library, gmp_ecm, provides Ruby bindings to this library.

The only prerequisite to using gmp_ecm is the [gmp](https://github.com/srawlins/gmp) gem, which provides Ruby bindings to GMP, a prerequisite to using the GMP-ECM library. The gmp gem is required because most numbers (input integer, resultant factors, and various parameters) are passed around as `GMP::Z` numbers.

Usage
=====

The function of the gmp_ecm library is contained almost entirely in the `GMP::Z#ecm_factor` method (binding to GMP-ECM's `ecm_params` C function). So in order to start factorizing an integer, one must first initialize a `GMP::Z` number, and then call `#ecm_factor` against it. Just as in the C library, this method requires the `B1` parameter to be passed in. Optionally, the user can pass in a second parameter, a Hash, or an ECMParams object. This will be passed to `ecm_factor` as an `ecm_params` struct.

Just now, not all of the fields in `ecm_params` is available. The current list of supported parameters is in the docs.

License
=======

gmp_ecm is licensed under the Apache License, Version 2.0
