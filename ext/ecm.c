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
//void r_ecm_params_free(void *ptr) { ecm_clear (ptr); free (ptr); }
void r_ecm_params_free(void *ptr) { ecm_clear (ptr); }

VALUE r_ecmsg_new(int argc, VALUE *argv, VALUE klass)
{
  ECM_PARAMS *res_val;
  VALUE res;
  (void)klass;

  if (argc > 1)
    rb_raise (rb_eArgError, "wrong # of arguments (%d for 0 or 1)", argc);

//  ecm_params_make_struct (res, res_val);
  ecm_params_make_struct_init (res, res_val);
  rb_obj_call_init (res, argc, argv);

  return res;
}

VALUE r_ecm_factor (int argc, VALUE *argv, VALUE self_value) {
  MP_INT *self, *res;
  VALUE res_value;
  int found;
  (void)self;

  mpz_get_struct (self_value, self);
  mpz_make_struct (res_value, res);
  mpz_init (res);

  found = ecm_factor (res, self, 11e7, NULL);

  return res_value;
//  return INT2NUM(found);
}

void Init_ecm() {
  cECM_PARAMS = rb_define_class ("ECMParams", rb_cObject);

  // Initialization Functions and Assignment Functions
  rb_define_singleton_method (cECM_PARAMS, "new", r_ecmsg_new, -1);
  //rb_define_method (cECM_PARAMS, "initialize", r_ecm_initialize, -1);

  rb_define_method(cGMP_Z, "ecm_factor", r_ecm_factor, -1);
}
