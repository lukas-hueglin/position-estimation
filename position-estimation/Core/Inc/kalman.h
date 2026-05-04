/*
 * kalman.h
 *
 *  Created on: 04.05.2026
 *      Author: gutal
 */

#ifndef INC_KALMAN_H_
#define INC_KALMAN_H_

typedef arm_matrix_instance_f32 mat_t;

mat_t* f(mat_t* x, mat_t* u);
mat_t* h(mat_t* x, mat_t* u);

mat_t* F(mat_t* x, mat_t* u);
mat_t* H(mat_t* x, mat_t* u);

mat_t* Q(mat_t* x, mat_t* u);
mat_t* R;

void predict(mat_t* x, mat_t* x_priori, mat_t* u,  mat_t* p, mat_t* p_priori);
void update(mat_t* x, mat_t* x_priori, mat_t* p,  mat_t* p_priori, mat_t* z);

void normalize(mat_t* x);

#endif /* INC_KALMAN_H_ */

