#ifndef _RUBY_ECM_H_
#define _RUBY_ECM_H_

#include <stdio.h>
#include <ruby.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <ruby_gmp.h>
#include <ecm.h>

#include <stdlib.h>

/*
 * (a comment borrowed from the Ruby GMP bindings:)
 * MP_???* is used because they don't have side-effects of single-element arrays ecm_params
 */
typedef __ecm_param_struct ECM_PARAMS;

#define ecm_params_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, ECM_PARAMS, c_var); }
#define ecm_params_make_struct(ruby_var,c_var) {                                      \
  ruby_var = Data_Make_Struct(cECM_PARAMS, ECM_PARAMS, 0, r_ecm_params_free, c_var);  \
}
#define ecm_params_make_struct_init(ruby_var,c_var) {  \
  ecm_params_make_struct(ruby_var,c_var);              \
  ecm_init (c_var);                                    \
}
#define ECM_PARAMS_P(value) (rb_obj_is_instance_of(value,cECM_PARAMS) == Qtrue)

extern VALUE cECM_PARAMS;

extern void ecm_params_set_value(ECM_PARAMS *target, VALUE source);

extern void r_ecm_params_free(void *ptr);

#endif /* _RUBY_ECM_H_ */
