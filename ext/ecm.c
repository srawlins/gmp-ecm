/*
 * ecm.c
 *
 * This file contains everything you would expect from a C extension.
 */

#include <ruby_ecm.h>

/*********************************************************************
 *    Initialization Functions                                       *
 *********************************************************************/

VALUE cECM_PARAMS;
ID ecmp_method_id;
ID ecmp_ecm_ecm_id;
ID ecmp_ecm_pm1_id;
ID ecmp_ecm_pp1_id;
ID ecmp_x_id;
ID ecmp_sigma_id;
ID ecmp_sigma_is_A_id;
ID ecmp_go_id;
ID ecmp_B1done_id;
ID ecmp_B2min_id;
ID ecmp_B2_id;
//void r_ecm_params_free(void *ptr) { ecm_clear (ptr); free (ptr); }
void r_ecm_params_free(void *ptr) { ecm_clear (ptr); }

/*
 * So far, we accept the following:
 *
 * :method is supposed to be one of these constants: ECM_ECM, ECM_PM1, ECM_PP1.
 * I am accepting :ecm_ecm, :ecm_pm1, :ecm_pp1.
 *
 * :x is supposed to be an mpz_t. I am accepting GMP::Z.
 *
 * :sigma is supposed to be an mpz_t. I am accepting Fixnum, Bignum, GMP::Z.
 *
 * :sigma_is_A is supposed to be 1, 0, or -1. I am accepting Fixnum.
 */
int add_item_to_ecm_params (VALUE key, VALUE value, VALUE params_value) {
  ECM_PARAMS *params;
  ecm_params_get_struct (params_value, params);
  MP_INT *params_x, *params_sigma, *params_go, *params_B2min, *params_B2;
  if (rb_to_id (key) == ecmp_method_id) {
    if      ( rb_to_id (value) == ecmp_ecm_ecm_id) { params->method = ECM_ECM; }
    else if ( rb_to_id (value) == ecmp_ecm_pm1_id) { params->method = ECM_PM1; }
    else if ( rb_to_id (value) == ecmp_ecm_pp1_id) { params->method = ECM_PP1; }
  } else if (rb_to_id (key) == ecmp_x_id) {
    mpz_get_struct (value, params_x);
    mpz_init_set (params->x, params_x);
  } else if (rb_to_id (key) == ecmp_sigma_id) {
    if (FIXNUM_P (value) || BIGNUM_P (value)) {
      mpz_init_set_si (params->sigma, NUM2INT(value));
    } else if (GMPZ_P (value)) {
      mpz_get_struct (value, params_sigma);
      mpz_init_set (params->sigma, params_sigma);
    }
  } else if (rb_to_id (key) == ecmp_sigma_is_A_id) {
    params->sigma_is_A = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_go_id) {
    if (FIXNUM_P (value) || BIGNUM_P (value)) {
      mpz_init_set_si (params->go, NUM2INT(value));
    } else if (GMPZ_P (value)) {
      mpz_get_struct (value, params_go);
      mpz_init_set (params->go, params_go);
    }
  } else if (rb_to_id (key) == ecmp_B1done_id) {
    if (FIXNUM_P (value)) {
      params->B1done = (double) NUM2INT (value);
    } else if (FLOAT_P (value)) {
      params->B1done = NUM2DBL (value);
    }
  } else if (rb_to_id (key) == ecmp_B2min_id) {
    if (FIXNUM_P (value) || BIGNUM_P (value)) {
      mpz_init_set_si (params->B2min, NUM2INT(value));
    } else if (GMPZ_P (value)) {
      mpz_get_struct (value, params_B2min);
      mpz_init_set (params->B2min, params_B2min);
    }
  } else if (rb_to_id (key) == ecmp_B2_id) {
    if (FIXNUM_P (value) || BIGNUM_P (value)) {
      mpz_init_set_si (params->B2, NUM2INT(value));
    } else if (GMPZ_P (value)) {
      mpz_get_struct (value, params_B2);
      mpz_init_set (params->B2, params_B2);
    }
  }
  return 1;
}

/*
 * params should already be initialized
 */
VALUE build_ecm_params_from_hash (VALUE params_hash) {
  VALUE params_value;
  ECM_PARAMS *params;
  ecm_params_make_struct_init (params_value, params);
  rb_hash_foreach (params_hash, add_item_to_ecm_params, params_value);
  return params_value;
}

VALUE r_ecmsg_new(int argc, VALUE *argv, VALUE klass)
{
  ECM_PARAMS *res;
  VALUE res_value;
  (void)klass;

  if (argc > 1)
    rb_raise (rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);

//  ecm_params_make_struct (res, res_val);
  ecm_params_make_struct_init (res_value, res);
  rb_obj_call_init (res_value, argc, argv);

  return res_value;
}

  /*
   * Document-method: GMP::Z#ecm_method
   *
   * @overload ecm_factor(b1, ecm_params)
   *
   * Search for a factor of `z`, using GMP-ECM's `ecm_factor()` method.
   *
   * @param [Fixnum, GMP::Z] b1 the stage 1 bound
   * @param [Hash] ecm_params auxiliary parameters; must be a Hash for now.
   *
   * * option ecm_params [Symbol] :method the factorization method (ECM_ECM for ECM, ECM_PM1 for P-1, ECM_PP1 for P+1). Default is ECM_ECM.
   * * option ecm_params [Fixnum, GMP::Z] :x (if non zero) is the starting point (ECM, P-1, P+1). For ECM, we take as starting point (x0 : y0) where x0=x, y0=1; for P-1, we take x0; for P+1, we take x0 as starting point of the Lucas sequence. When ecm_factor() returns, p->x is the point obtained after stage 1.
   * * option ecm_params [?] :sigma (ECM only) is the "sigma" parameter. The elliptic curve chosen is b*y^2 = x^3 + a*x^2 + x where a = (v-u)^3*(3*u+v)/(4*u^3*v)-2, u = sigma^2-5, v = 4*sigma (Suyama's parametrization). The initial point (if p->x is zero) is taken as x0=u^3/v^3, y0=1 (thus b is taken as x0^3 + a*x0^2 + x0).
   * * option ecm_params [-1, 0, 1] :sigma_is_A (ECM only) indicates that p->sigma is the 'a' parameter from the elliptic curve.
   * * option ecm_params [?] :go the initial group order to preload (default is 1).
   * * option ecm_params [?] :B1done tells that step 1 was already done up to B1done. This means that all prime powers <= B1done were dealt with. If for example B1done=100 and B1=200, prime 2 was dealt with up to power 6, thus it remains to "multiply" once by 2 to go up to power 7. Of course, all primes p such that B1done < p <= B1 will be considered with power 1.
   * * option ecm_params [Fixnum, GMP::Z] :B2min the lower bound for stage 2, which will treat all primes p such that B2min <= p <= B2. If negative, B2min will be set to B1.
   * * option ecm_params [Fixnum, GMP::Z] :B2 the upper bound for stage 2 (default is automatically computed from B1, to optimize the efficiency of the method).
   * * option ecm_params [?] :k the number of blocks used in stage 2 (default is ECM_DEFAULT_K).
   */
VALUE r_ecm_factor (int argc, VALUE *argv, VALUE self_value) {
  MP_INT *self, *res;
  VALUE res_value, b1_value, params_arg, params_value;
  ECM_PARAMS *params;
  double b1;
  int found;
  (void)self;

  mpz_get_struct (self_value, self);
  mpz_make_struct (res_value, res);
  mpz_init (res);

  rb_scan_args (argc, argv, "02", &b1_value, &params_arg);
  if (params_arg == Qnil) {
    params = NULL;
  } else if (ECM_PARAMS_P (params_arg)) {
    ecm_params_get_struct (params_arg, params);
  } else if (TYPE (params_arg) == T_HASH) {
    params_value = build_ecm_params_from_hash (params_arg);
    ecm_params_get_struct (params_value, params);
  } else {
    rb_raise (rb_eArgError, "Second argument must be an ECMParams");
  }
  if (b1_value == Qnil) {
    b1 = 1000000;
  } else if (FIXNUM_P (b1_value)) {
    b1 = (double) NUM2INT (b1_value);
  } else if (FLOAT_P (b1_value)) {
    b1 = NUM2DBL (b1_value);
  } else {
    rb_raise (rb_eArgError, "b1 argument must be a Fixnum");
  }

  found = ecm_factor (res, self, b1, params);

  return res_value;
}

void Init_ecm() {
  ecmp_method_id     = rb_intern("method");
  ecmp_ecm_ecm_id    = rb_intern("ecm_ecm");
  ecmp_ecm_pm1_id    = rb_intern("ecm_pm1");
  ecmp_ecm_pp1_id    = rb_intern("ecm_pp1");
  ecmp_x_id          = rb_intern("x");
  ecmp_sigma_id      = rb_intern("sigma");
  ecmp_sigma_is_A_id = rb_intern("sigma_is_A");
  ecmp_sigma_is_A_id = rb_intern("go");
  ecmp_B1done_id     = rb_intern("B1done");
  ecmp_B2min_id      = rb_intern("B2min");
  ecmp_B2_id         = rb_intern("B2");
  cECM_PARAMS = rb_define_class ("ECMParams", rb_cObject);

  // Initialization Functions and Assignment Functions

  rb_define_singleton_method (cECM_PARAMS, "new", r_ecmsg_new, -1);
  //rb_define_method (cECM_PARAMS, "initialize", r_ecm_initialize, -1);

  if (0) {
    mGMP = rb_define_module("GMP");
    cGMP_Z = rb_define_class_under(mGMP, "Z", rb_cInteger);
  }
  rb_define_method(cGMP_Z, "ecm_factor", r_ecm_factor, -1);
}
