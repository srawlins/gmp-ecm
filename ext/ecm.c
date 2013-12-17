/*
 * ecm.c
 *
 * This file contains everything you would expect from a C extension.
 */

#include <ruby_ecm.h>
#include <ruby/io.h>

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
ID ecmp_k_id;
ID ecmp_S_id;
ID ecmp_repr_id;
ID ecmp_nobase2step2_id;
ID ecmp_verbose_id;
ID ecmp_os_id;
ID ecmp_es_id;
ID ecmp_chkfilename_id;
ID ecmp_TreeFilename_id;
ID ecmp_maxmem_id;
ID ecmp_stage1time_id;
//void r_ecm_params_free(void *ptr) { ecm_clear (ptr); free (ptr); }
void r_ecm_params_free(void *ptr) { ecm_clear (ptr); }

int add_item_to_ecm_params (VALUE key, VALUE value, VALUE params_value) {
  ECM_PARAMS *params;
  MP_INT *params_x, *params_sigma, *params_go, *params_B2min, *params_B2;
  FILE *fd;
  ecm_params_get_struct (params_value, params);

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
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "sigma_is_A must be a Fixnum.");
    }
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
  } else if (rb_to_id (key) == ecmp_k_id) {
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "k must be a Fixnum.");
    }
    params->k = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_S_id) {
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "S must be a Fixnum.");
    }
    params->S = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_repr_id) {
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "repr must be a Fixnum.");
    }
    params->repr = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_nobase2step2_id) {
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "nobase2step2 must be a Fixnum.");
    }
    params->nobase2step2 = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_verbose_id) {
    if (! FIXNUM_P (value)) {
      rb_raise (rb_eTypeError, "verbose must be a Fixnum.");
    }
    params->verbose = NUM2INT(value);
  } else if (rb_to_id (key) == ecmp_os_id) {
    if (TYPE (value) != T_FILE) {
      rb_raise (rb_eTypeError, "os must be an IO.");
    }
    fd = rb_io_stdio_file(RFILE(value)->fptr);
    params->os = fd;
  } else if (rb_to_id (key) == ecmp_es_id) {
    if (TYPE (value) != T_FILE) {
      rb_raise (rb_eTypeError, "es must be an IO.");
    }
    fd = rb_io_stdio_file(RFILE(value)->fptr);
    params->es = fd;
  } else if (rb_to_id (key) == ecmp_chkfilename_id) {
    if (TYPE (value) != T_STRING) {
      rb_raise (rb_eTypeError, "chkfilename must be a String.");
    }
    params->chkfilename = malloc (sizeof(char) * strlen (StringValuePtr (value)));
    strcpy (params->chkfilename, StringValuePtr (value));
  } else if (rb_to_id (key) == ecmp_maxmem_id) {
    if (FIXNUM_P (value)) {
      params->maxmem = (double) NUM2INT (value);
    } else if (FLOAT_P (value)) {
      params->maxmem = NUM2DBL (value);
    }
  } else if (rb_to_id (key) == ecmp_stage1time_id) {
    if (FIXNUM_P (value)) {
      params->stage1time = (double) NUM2INT (value);
    } else if (FLOAT_P (value)) {
      params->stage1time = NUM2DBL (value);
    }
  }
  return ST_CONTINUE;
}

/*
 * params should already be initialized
 */
VALUE build_ecm_params_from_hash (VALUE params_hash) {
  VALUE params_value;
  ECM_PARAMS *params;
  ecm_params_make_struct_init (params_value, params);
  params->chkfilename = params->TreeFilename = NULL;
  rb_hash_foreach (params_hash, add_item_to_ecm_params, params_value);
  return params_value;
}

VALUE r_ecmsg_new (int argc, VALUE *argv, VALUE klass)
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
   *   Search for a factor of `z`, using GMP-ECM's `ecm_factor()` method.
   *   @param [Fixnum, GMP::Z] b1 the stage 1 bound
   *   @param [Hash] ecm_params auxiliary parameters; must be a Hash for now.
   *   @option ecm_params [Symbol] :method the factorization method (`:ecm_ecm` for ECM, `:ecm_pm1` for P-1, `:ecm_pp1` for P+1). Default is ECM_ECM.
   *   @option ecm_params [Fixnum, GMP::Z] :x (if non zero) is the starting point (ECM, P-1, P+1). For ECM, we take as starting point $(x\_0 : y\_0)$ where $x\_0=x$, $y\_0=1$; for P-1, we take $x\_0$; for P+1, we take $x\_0$ as starting point of the Lucas sequence. When `ecm_factor()` returns, p->x is the point obtained after stage 1.
   *   @option ecm_params [Fixnum, GMP::Z] :sigma (ECM only) is the "sigma" parameter. The elliptic curve chosen is $b \* y^2 = x^3 + a\*x^2 + x$ where $a = (v-u)^3 \* (3\*u+v) / (4\*u^3\*v) -2$, $u = sigma^2-5$, $v = 4\*sigma$ (Suyama's parametrization). The initial point (if p->x is zero) is taken as $x\_0=u^3/v^3$, $y0=1$ (thus $b$ is taken as $x\_0^3 + a\*x\_0^2 + x\_0$).
   *   @option ecm_params [-1, 0, 1] :sigma_is_A (ECM only) indicates that `:sigma` is the `a` parameter from the elliptic curve.
   *   @option ecm_params [Fixnum, GMP::Z] :go the initial group order to preload (default is 1).
   *   @option ecm_params [Float] :B1done tells that step 1 was already done up to this value. This means that all prime powers <= this value were dealt with. If for example `:B1done => 100` and `:B1 => 200`, prime 2 was dealt with up to power 6, thus it remains to "multiply" once by 2 to go up to power 7. Of course, all primes $p$ such that $B1done < p <= B1$ will be considered with power 1.
   *   @option ecm_params [Fixnum, GMP::Z] :B2min the lower bound for stage 2, which will treat all primes $p$ such that $B2min <= p <= B2$. If negative, `:B2min` will be set to `:B1`.
   *   @option ecm_params [Fixnum, GMP::Z] :B2 the upper bound for stage 2 (default is automatically computed from `:B1`, to optimize the efficiency of the method).
   *   @option ecm_params [Fixnum] :k the number of blocks used in stage 2 (default is `ECM_DEFAULT_K`).
   *   @option ecm_params [Fixnum] :S defines the polynomial used for Brent-Suyama's extension in stage 2. If positive, the polynomial used is $x^S$; if negative, it is Dickson's polynomial of degree $S$ with parameter $a=-1$, where $D\_\\{1,a\}(x) = x, D\_\\{2,a\}(x) = x^2-2\*a$, and $D\_\\{k+2,a\}(x) = x\*D\_\\{k+1,a\}(x) - a\*D\_\\{k,a\}(x)$, or equivalently $D\_\\{k,a\}(2\*sqrt(a)\*cos(t)) = 2\*a^\\{k/2\}\*cos(k\*t)$. If zero, choice is automatic (and should be close to optimal). Default is `ECM_DEFAULT_S`.
   *   @option ecm_params [Fixnum] :repr defines the representation used for modular arithmetic: 1 means the 'mpz' class from GMP, 2 means 'modmuln' (Montgomery's multiplication, quadratic implementation), 3 means 'redc' (Montgomery's multiplication, subquadratic implementation), -1 indicates not to use a special base-2 representation (when the input number is a factor of $2^n +/- 1$). Other values (including 0) mean the representation will be chosen automatically (hopefully in some optimal way).
   *   @option ecm_params [Fixnum] :verbose the verbosity level: 0 for no output, 1 for normal output (like default for GMP-ECM), 2 for diagnostic output without intermediate residues (like `-v` in GMP-ECM), 3 for diagnostic output with residues (like `-v -v`), 4 for high diagnostic output (`-v -v -v`), and 5 for trace output (`-v -v -v -v`).
   *   @option ecm_params [IO] :os the output stream used for verbose output. Default is stdout.
   *   @option ecm_params [IO] :es the output stream used for errors. Default is stderr.
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

  return rb_assoc_new (INT2FIX (found), res_value);
}

void Init_ecm() {
  ecmp_method_id       = rb_intern("method");
  ecmp_ecm_ecm_id      = rb_intern("ecm_ecm");
  ecmp_ecm_pm1_id      = rb_intern("ecm_pm1");
  ecmp_ecm_pp1_id      = rb_intern("ecm_pp1");
  ecmp_x_id            = rb_intern("x");
  ecmp_sigma_id        = rb_intern("sigma");
  ecmp_sigma_is_A_id   = rb_intern("sigma_is_A");
  ecmp_go_id           = rb_intern("go");
  ecmp_B1done_id       = rb_intern("B1done");
  ecmp_B2min_id        = rb_intern("B2min");
  ecmp_B2_id           = rb_intern("B2");
  ecmp_k_id            = rb_intern("k");
  ecmp_S_id            = rb_intern("S");
  ecmp_repr_id         = rb_intern("repr");
  ecmp_nobase2step2_id = rb_intern("nobase2step2");
  ecmp_verbose_id      = rb_intern("verbose");
  ecmp_os_id           = rb_intern("os");
  ecmp_es_id           = rb_intern("es");
  ecmp_chkfilename_id  = rb_intern("chkfilename");
  ecmp_TreeFilename_id = rb_intern("TreeFilename");
  ecmp_maxmem_id       = rb_intern("maxmem");
  ecmp_stage1time_id   = rb_intern("stage1time");
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
