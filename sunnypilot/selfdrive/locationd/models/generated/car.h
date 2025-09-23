#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_6002059645141418686);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7694979851000339522);
void car_H_mod_fun(double *state, double *out_846483902778139487);
void car_f_fun(double *state, double dt, double *out_9075956917239856538);
void car_F_fun(double *state, double dt, double *out_2828857401133093709);
void car_h_25(double *state, double *unused, double *out_458663665945936724);
void car_H_25(double *state, double *unused, double *out_7343506862668169438);
void car_h_24(double *state, double *unused, double *out_7235414455283319899);
void car_H_24(double *state, double *unused, double *out_6234422346013675326);
void car_h_30(double *state, double *unused, double *out_6372396692616685097);
void car_H_30(double *state, double *unused, double *out_6575540880913773980);
void car_h_26(double *state, double *unused, double *out_630366011176380682);
void car_H_26(double *state, double *unused, double *out_7361733892167325954);
void car_h_27(double *state, double *unused, double *out_4543009601714908921);
void car_H_27(double *state, double *unused, double *out_4400777569113349069);
void car_h_29(double *state, double *unused, double *out_3092378209086025754);
void car_H_29(double *state, double *unused, double *out_7085772225228166164);
void car_h_28(double *state, double *unused, double *out_1856031915698940225);
void car_H_28(double *state, double *unused, double *out_2003373208158635590);
void car_h_31(double *state, double *unused, double *out_4320347142374438376);
void car_H_31(double *state, double *unused, double *out_6735525789933974478);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}