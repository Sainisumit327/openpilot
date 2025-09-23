#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6002059645141418686) {
   out_6002059645141418686[0] = delta_x[0] + nom_x[0];
   out_6002059645141418686[1] = delta_x[1] + nom_x[1];
   out_6002059645141418686[2] = delta_x[2] + nom_x[2];
   out_6002059645141418686[3] = delta_x[3] + nom_x[3];
   out_6002059645141418686[4] = delta_x[4] + nom_x[4];
   out_6002059645141418686[5] = delta_x[5] + nom_x[5];
   out_6002059645141418686[6] = delta_x[6] + nom_x[6];
   out_6002059645141418686[7] = delta_x[7] + nom_x[7];
   out_6002059645141418686[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7694979851000339522) {
   out_7694979851000339522[0] = -nom_x[0] + true_x[0];
   out_7694979851000339522[1] = -nom_x[1] + true_x[1];
   out_7694979851000339522[2] = -nom_x[2] + true_x[2];
   out_7694979851000339522[3] = -nom_x[3] + true_x[3];
   out_7694979851000339522[4] = -nom_x[4] + true_x[4];
   out_7694979851000339522[5] = -nom_x[5] + true_x[5];
   out_7694979851000339522[6] = -nom_x[6] + true_x[6];
   out_7694979851000339522[7] = -nom_x[7] + true_x[7];
   out_7694979851000339522[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_846483902778139487) {
   out_846483902778139487[0] = 1.0;
   out_846483902778139487[1] = 0.0;
   out_846483902778139487[2] = 0.0;
   out_846483902778139487[3] = 0.0;
   out_846483902778139487[4] = 0.0;
   out_846483902778139487[5] = 0.0;
   out_846483902778139487[6] = 0.0;
   out_846483902778139487[7] = 0.0;
   out_846483902778139487[8] = 0.0;
   out_846483902778139487[9] = 0.0;
   out_846483902778139487[10] = 1.0;
   out_846483902778139487[11] = 0.0;
   out_846483902778139487[12] = 0.0;
   out_846483902778139487[13] = 0.0;
   out_846483902778139487[14] = 0.0;
   out_846483902778139487[15] = 0.0;
   out_846483902778139487[16] = 0.0;
   out_846483902778139487[17] = 0.0;
   out_846483902778139487[18] = 0.0;
   out_846483902778139487[19] = 0.0;
   out_846483902778139487[20] = 1.0;
   out_846483902778139487[21] = 0.0;
   out_846483902778139487[22] = 0.0;
   out_846483902778139487[23] = 0.0;
   out_846483902778139487[24] = 0.0;
   out_846483902778139487[25] = 0.0;
   out_846483902778139487[26] = 0.0;
   out_846483902778139487[27] = 0.0;
   out_846483902778139487[28] = 0.0;
   out_846483902778139487[29] = 0.0;
   out_846483902778139487[30] = 1.0;
   out_846483902778139487[31] = 0.0;
   out_846483902778139487[32] = 0.0;
   out_846483902778139487[33] = 0.0;
   out_846483902778139487[34] = 0.0;
   out_846483902778139487[35] = 0.0;
   out_846483902778139487[36] = 0.0;
   out_846483902778139487[37] = 0.0;
   out_846483902778139487[38] = 0.0;
   out_846483902778139487[39] = 0.0;
   out_846483902778139487[40] = 1.0;
   out_846483902778139487[41] = 0.0;
   out_846483902778139487[42] = 0.0;
   out_846483902778139487[43] = 0.0;
   out_846483902778139487[44] = 0.0;
   out_846483902778139487[45] = 0.0;
   out_846483902778139487[46] = 0.0;
   out_846483902778139487[47] = 0.0;
   out_846483902778139487[48] = 0.0;
   out_846483902778139487[49] = 0.0;
   out_846483902778139487[50] = 1.0;
   out_846483902778139487[51] = 0.0;
   out_846483902778139487[52] = 0.0;
   out_846483902778139487[53] = 0.0;
   out_846483902778139487[54] = 0.0;
   out_846483902778139487[55] = 0.0;
   out_846483902778139487[56] = 0.0;
   out_846483902778139487[57] = 0.0;
   out_846483902778139487[58] = 0.0;
   out_846483902778139487[59] = 0.0;
   out_846483902778139487[60] = 1.0;
   out_846483902778139487[61] = 0.0;
   out_846483902778139487[62] = 0.0;
   out_846483902778139487[63] = 0.0;
   out_846483902778139487[64] = 0.0;
   out_846483902778139487[65] = 0.0;
   out_846483902778139487[66] = 0.0;
   out_846483902778139487[67] = 0.0;
   out_846483902778139487[68] = 0.0;
   out_846483902778139487[69] = 0.0;
   out_846483902778139487[70] = 1.0;
   out_846483902778139487[71] = 0.0;
   out_846483902778139487[72] = 0.0;
   out_846483902778139487[73] = 0.0;
   out_846483902778139487[74] = 0.0;
   out_846483902778139487[75] = 0.0;
   out_846483902778139487[76] = 0.0;
   out_846483902778139487[77] = 0.0;
   out_846483902778139487[78] = 0.0;
   out_846483902778139487[79] = 0.0;
   out_846483902778139487[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_9075956917239856538) {
   out_9075956917239856538[0] = state[0];
   out_9075956917239856538[1] = state[1];
   out_9075956917239856538[2] = state[2];
   out_9075956917239856538[3] = state[3];
   out_9075956917239856538[4] = state[4];
   out_9075956917239856538[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_9075956917239856538[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_9075956917239856538[7] = state[7];
   out_9075956917239856538[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2828857401133093709) {
   out_2828857401133093709[0] = 1;
   out_2828857401133093709[1] = 0;
   out_2828857401133093709[2] = 0;
   out_2828857401133093709[3] = 0;
   out_2828857401133093709[4] = 0;
   out_2828857401133093709[5] = 0;
   out_2828857401133093709[6] = 0;
   out_2828857401133093709[7] = 0;
   out_2828857401133093709[8] = 0;
   out_2828857401133093709[9] = 0;
   out_2828857401133093709[10] = 1;
   out_2828857401133093709[11] = 0;
   out_2828857401133093709[12] = 0;
   out_2828857401133093709[13] = 0;
   out_2828857401133093709[14] = 0;
   out_2828857401133093709[15] = 0;
   out_2828857401133093709[16] = 0;
   out_2828857401133093709[17] = 0;
   out_2828857401133093709[18] = 0;
   out_2828857401133093709[19] = 0;
   out_2828857401133093709[20] = 1;
   out_2828857401133093709[21] = 0;
   out_2828857401133093709[22] = 0;
   out_2828857401133093709[23] = 0;
   out_2828857401133093709[24] = 0;
   out_2828857401133093709[25] = 0;
   out_2828857401133093709[26] = 0;
   out_2828857401133093709[27] = 0;
   out_2828857401133093709[28] = 0;
   out_2828857401133093709[29] = 0;
   out_2828857401133093709[30] = 1;
   out_2828857401133093709[31] = 0;
   out_2828857401133093709[32] = 0;
   out_2828857401133093709[33] = 0;
   out_2828857401133093709[34] = 0;
   out_2828857401133093709[35] = 0;
   out_2828857401133093709[36] = 0;
   out_2828857401133093709[37] = 0;
   out_2828857401133093709[38] = 0;
   out_2828857401133093709[39] = 0;
   out_2828857401133093709[40] = 1;
   out_2828857401133093709[41] = 0;
   out_2828857401133093709[42] = 0;
   out_2828857401133093709[43] = 0;
   out_2828857401133093709[44] = 0;
   out_2828857401133093709[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2828857401133093709[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2828857401133093709[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2828857401133093709[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2828857401133093709[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2828857401133093709[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2828857401133093709[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2828857401133093709[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2828857401133093709[53] = -9.8100000000000005*dt;
   out_2828857401133093709[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2828857401133093709[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2828857401133093709[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2828857401133093709[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2828857401133093709[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2828857401133093709[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2828857401133093709[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2828857401133093709[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2828857401133093709[62] = 0;
   out_2828857401133093709[63] = 0;
   out_2828857401133093709[64] = 0;
   out_2828857401133093709[65] = 0;
   out_2828857401133093709[66] = 0;
   out_2828857401133093709[67] = 0;
   out_2828857401133093709[68] = 0;
   out_2828857401133093709[69] = 0;
   out_2828857401133093709[70] = 1;
   out_2828857401133093709[71] = 0;
   out_2828857401133093709[72] = 0;
   out_2828857401133093709[73] = 0;
   out_2828857401133093709[74] = 0;
   out_2828857401133093709[75] = 0;
   out_2828857401133093709[76] = 0;
   out_2828857401133093709[77] = 0;
   out_2828857401133093709[78] = 0;
   out_2828857401133093709[79] = 0;
   out_2828857401133093709[80] = 1;
}
void h_25(double *state, double *unused, double *out_458663665945936724) {
   out_458663665945936724[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7343506862668169438) {
   out_7343506862668169438[0] = 0;
   out_7343506862668169438[1] = 0;
   out_7343506862668169438[2] = 0;
   out_7343506862668169438[3] = 0;
   out_7343506862668169438[4] = 0;
   out_7343506862668169438[5] = 0;
   out_7343506862668169438[6] = 1;
   out_7343506862668169438[7] = 0;
   out_7343506862668169438[8] = 0;
}
void h_24(double *state, double *unused, double *out_7235414455283319899) {
   out_7235414455283319899[0] = state[4];
   out_7235414455283319899[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6234422346013675326) {
   out_6234422346013675326[0] = 0;
   out_6234422346013675326[1] = 0;
   out_6234422346013675326[2] = 0;
   out_6234422346013675326[3] = 0;
   out_6234422346013675326[4] = 1;
   out_6234422346013675326[5] = 0;
   out_6234422346013675326[6] = 0;
   out_6234422346013675326[7] = 0;
   out_6234422346013675326[8] = 0;
   out_6234422346013675326[9] = 0;
   out_6234422346013675326[10] = 0;
   out_6234422346013675326[11] = 0;
   out_6234422346013675326[12] = 0;
   out_6234422346013675326[13] = 0;
   out_6234422346013675326[14] = 1;
   out_6234422346013675326[15] = 0;
   out_6234422346013675326[16] = 0;
   out_6234422346013675326[17] = 0;
}
void h_30(double *state, double *unused, double *out_6372396692616685097) {
   out_6372396692616685097[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6575540880913773980) {
   out_6575540880913773980[0] = 0;
   out_6575540880913773980[1] = 0;
   out_6575540880913773980[2] = 0;
   out_6575540880913773980[3] = 0;
   out_6575540880913773980[4] = 1;
   out_6575540880913773980[5] = 0;
   out_6575540880913773980[6] = 0;
   out_6575540880913773980[7] = 0;
   out_6575540880913773980[8] = 0;
}
void h_26(double *state, double *unused, double *out_630366011176380682) {
   out_630366011176380682[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7361733892167325954) {
   out_7361733892167325954[0] = 0;
   out_7361733892167325954[1] = 0;
   out_7361733892167325954[2] = 0;
   out_7361733892167325954[3] = 0;
   out_7361733892167325954[4] = 0;
   out_7361733892167325954[5] = 0;
   out_7361733892167325954[6] = 0;
   out_7361733892167325954[7] = 1;
   out_7361733892167325954[8] = 0;
}
void h_27(double *state, double *unused, double *out_4543009601714908921) {
   out_4543009601714908921[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4400777569113349069) {
   out_4400777569113349069[0] = 0;
   out_4400777569113349069[1] = 0;
   out_4400777569113349069[2] = 0;
   out_4400777569113349069[3] = 1;
   out_4400777569113349069[4] = 0;
   out_4400777569113349069[5] = 0;
   out_4400777569113349069[6] = 0;
   out_4400777569113349069[7] = 0;
   out_4400777569113349069[8] = 0;
}
void h_29(double *state, double *unused, double *out_3092378209086025754) {
   out_3092378209086025754[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7085772225228166164) {
   out_7085772225228166164[0] = 0;
   out_7085772225228166164[1] = 1;
   out_7085772225228166164[2] = 0;
   out_7085772225228166164[3] = 0;
   out_7085772225228166164[4] = 0;
   out_7085772225228166164[5] = 0;
   out_7085772225228166164[6] = 0;
   out_7085772225228166164[7] = 0;
   out_7085772225228166164[8] = 0;
}
void h_28(double *state, double *unused, double *out_1856031915698940225) {
   out_1856031915698940225[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2003373208158635590) {
   out_2003373208158635590[0] = 1;
   out_2003373208158635590[1] = 0;
   out_2003373208158635590[2] = 0;
   out_2003373208158635590[3] = 0;
   out_2003373208158635590[4] = 0;
   out_2003373208158635590[5] = 0;
   out_2003373208158635590[6] = 0;
   out_2003373208158635590[7] = 0;
   out_2003373208158635590[8] = 0;
}
void h_31(double *state, double *unused, double *out_4320347142374438376) {
   out_4320347142374438376[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6735525789933974478) {
   out_6735525789933974478[0] = 0;
   out_6735525789933974478[1] = 0;
   out_6735525789933974478[2] = 0;
   out_6735525789933974478[3] = 0;
   out_6735525789933974478[4] = 0;
   out_6735525789933974478[5] = 0;
   out_6735525789933974478[6] = 0;
   out_6735525789933974478[7] = 0;
   out_6735525789933974478[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_6002059645141418686) {
  err_fun(nom_x, delta_x, out_6002059645141418686);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7694979851000339522) {
  inv_err_fun(nom_x, true_x, out_7694979851000339522);
}
void car_H_mod_fun(double *state, double *out_846483902778139487) {
  H_mod_fun(state, out_846483902778139487);
}
void car_f_fun(double *state, double dt, double *out_9075956917239856538) {
  f_fun(state,  dt, out_9075956917239856538);
}
void car_F_fun(double *state, double dt, double *out_2828857401133093709) {
  F_fun(state,  dt, out_2828857401133093709);
}
void car_h_25(double *state, double *unused, double *out_458663665945936724) {
  h_25(state, unused, out_458663665945936724);
}
void car_H_25(double *state, double *unused, double *out_7343506862668169438) {
  H_25(state, unused, out_7343506862668169438);
}
void car_h_24(double *state, double *unused, double *out_7235414455283319899) {
  h_24(state, unused, out_7235414455283319899);
}
void car_H_24(double *state, double *unused, double *out_6234422346013675326) {
  H_24(state, unused, out_6234422346013675326);
}
void car_h_30(double *state, double *unused, double *out_6372396692616685097) {
  h_30(state, unused, out_6372396692616685097);
}
void car_H_30(double *state, double *unused, double *out_6575540880913773980) {
  H_30(state, unused, out_6575540880913773980);
}
void car_h_26(double *state, double *unused, double *out_630366011176380682) {
  h_26(state, unused, out_630366011176380682);
}
void car_H_26(double *state, double *unused, double *out_7361733892167325954) {
  H_26(state, unused, out_7361733892167325954);
}
void car_h_27(double *state, double *unused, double *out_4543009601714908921) {
  h_27(state, unused, out_4543009601714908921);
}
void car_H_27(double *state, double *unused, double *out_4400777569113349069) {
  H_27(state, unused, out_4400777569113349069);
}
void car_h_29(double *state, double *unused, double *out_3092378209086025754) {
  h_29(state, unused, out_3092378209086025754);
}
void car_H_29(double *state, double *unused, double *out_7085772225228166164) {
  H_29(state, unused, out_7085772225228166164);
}
void car_h_28(double *state, double *unused, double *out_1856031915698940225) {
  h_28(state, unused, out_1856031915698940225);
}
void car_H_28(double *state, double *unused, double *out_2003373208158635590) {
  H_28(state, unused, out_2003373208158635590);
}
void car_h_31(double *state, double *unused, double *out_4320347142374438376) {
  h_31(state, unused, out_4320347142374438376);
}
void car_H_31(double *state, double *unused, double *out_6735525789933974478) {
  H_31(state, unused, out_6735525789933974478);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
