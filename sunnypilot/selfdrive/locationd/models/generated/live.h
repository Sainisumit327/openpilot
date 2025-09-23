#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_6302962061809102962);
void live_err_fun(double *nom_x, double *delta_x, double *out_7521927350906963905);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_7082631028093210540);
void live_H_mod_fun(double *state, double *out_6355181012613804815);
void live_f_fun(double *state, double dt, double *out_1423317223410286188);
void live_F_fun(double *state, double dt, double *out_6777982878212362341);
void live_h_4(double *state, double *unused, double *out_3945996595638754706);
void live_H_4(double *state, double *unused, double *out_5304784645150146049);
void live_h_9(double *state, double *unused, double *out_314189559151727011);
void live_H_9(double *state, double *unused, double *out_5854740493294958097);
void live_h_10(double *state, double *unused, double *out_6256161826784860507);
void live_H_10(double *state, double *unused, double *out_4475427823520874786);
void live_h_12(double *state, double *unused, double *out_3093308230759717292);
void live_H_12(double *state, double *unused, double *out_8122503020527443772);
void live_h_35(double *state, double *unused, double *out_5941759621288917390);
void live_H_35(double *state, double *unused, double *out_8671446702522753425);
void live_h_32(double *state, double *unused, double *out_7612649491767367995);
void live_H_32(double *state, double *unused, double *out_8802611487463493894);
void live_h_13(double *state, double *unused, double *out_1514171038728814283);
void live_H_13(double *state, double *unused, double *out_3175201262285195984);
void live_h_14(double *state, double *unused, double *out_314189559151727011);
void live_H_14(double *state, double *unused, double *out_5854740493294958097);
void live_h_33(double *state, double *unused, double *out_6624475422695679023);
void live_H_33(double *state, double *unused, double *out_6624740366547940587);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}