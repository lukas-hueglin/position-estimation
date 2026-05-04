/*
 * kalman.c
 *
 *  Created on: 04.05.2026
 *      Author: gutal
 */

#include "kalman.h"
#include "lsm6dsl.h"

mat_t g[3] = {0,0,EARTH_GRAVITY};


mat_t* f(mat_t* x, mat_t* u);
mat_t* h(mat_t* x, mat_t* u);

mat_t* F(mat_t* x, mat_t* u);
mat_t* H(mat_t* x, mat_t* u);

mat_t* Q(mat_t* x, mat_t* u);
mat_t* R;

mat_t* I;

void predict(mat_t* x, mat_t* x_priori, mat_t* u, mat_t* p, mat_t* p_priori){
	x_priori = f(x,u);

	mat__t* F_T;
	mat_t* F = F(x,u);
	arm_mat_trans_f32(F, F_T);

	arm_mat_mul_f32(F, p, p_priori);
	arm_mat_mul_f32(p_priori, F_T, p_priori);
	arm_mat_add_f32(p_priori, Q(x, u), p_priori);
};

void update(mat_t* x, mat_t* x_priori, mat_t* p,  mat_t* p_priori, mat_t* z){
	mat_t* V;
	mat_t* S;
	mat_t* K;
	mat_t* H_T;
	mat_t* H = H(x_priori, u);

	arm_mat_trans_f32(H, H_T);

	arm_mat_sub_f32(z, h(x_priori, u), V);

	arm_mat_mul_f32(H, p_priori, S);
	arm_mat_mul_f32(S, H_T, S);
	arm_mat_add_f32(S, R(x_priori,u), S);

	arm_mat_mul_f32(p_priori, H_T, K);
	mat_t* inv_S;
	arm_mat_mul_inverse_f32(S, inv_S);
	arm_mat_mul_f32(K, inv_S, K);

	mat* KV;
	arm_mat_mul_f32(K, V, KV);
	arm_mat_add_f32(x_priori, KV, x);

	mat_t* KHP;
	arm_mat_mul_f32(K, H, KHP);
	arm_mat_mul_f32(KHP, p_priori, KHP);
	arm_mat_sub_f32(I, KHP, p);

	normalize(x);
};

void normalize(mat_t* x){
	pData = x->pData;
	float32_t norm;
	for(int i = 0; i<4; ++i){
		norm += pData[i]*pData[i];
	}

	arm_sqrt_f32(norm, &norm);

	for(int i = 0; i<4; ++i){
			pData[i] /= norm;
	}
}
