/*
Copyright (c) 2012 Carsten Burstedde, Donna Calhoun
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RADIAL_USER_H
#define RADIAL_USER_H
#include <fc2d_cudaclaw.h>

#include <fclaw2d_include_all.h>

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

typedef struct user_options
{
    int example;
    double rho;
    double bulk;
    double cc;
    double zz;

    double alpha;

    int claw_version;
    int cuda;
    int is_registered;

} user_options_t;

void radial_problem_setup(fclaw2d_global_t* glob);

void radial_setprob_cuda(double rho, double bulk);

#define RADIAL_SETPROB FCLAW_F77_FUNC(radial_setprob, RADIAL_SETPROB)
void RADIAL_SETPROB(const double* rho, const double *bulk,
                    double* cc, double*zz);

#define RADIAL_SETAUX_MANIFOLD FCLAW_F77_FUNC(radial_setaux_manifold, \
                                             RADIAL_SETAUX_MANIFOLD)

void RADIAL_SETAUX_MANIFOLD(const int* mbc,
                           const int* mx, const int* my,
                           const double* xlower, const double* ylower,
                           const double* dx, const double* dy,
                           const int* maux, double aux[],
                           double xnormals[], double ynormals[],
                           double edgelengths[],
                           double area[]);

void radial_patch_setup(fclaw2d_global_t *global,
                        fclaw2d_patch_t *this_patch,
                        int this_block_idx,
                        int this_patch_idx);

user_options_t* radial_options_register (fclaw_app_t * app, const char *configfile);

void radial_options_store (fclaw2d_global_t* glob, user_options_t* user);

user_options_t* radial_get_options(fclaw2d_global_t* glob);

void radial_link_solvers(fclaw2d_global_t *glob);

fclaw2d_map_context_t* fclaw2d_map_new_nomap();

fclaw2d_map_context_t* fclaw2d_map_new_pillowdisk5(const double scale[],
                                                   const double shift[],
                                                   const double rotate[],
                                                   const double alpha);
/* --------------------------------------- Cuda ----------------------------------------*/

void radial_assign_rpn2(cudaclaw_cuda_rpn2_t *rpn2);
void radial_assign_rpt2(cudaclaw_cuda_rpt2_t *rpt2);

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif
