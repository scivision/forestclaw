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

#ifndef FCLAW2D_VTABLE_H
#define FCLAW2D_VTABLE_H

#include "forestclaw2d.H"
#include "fclaw_base.h"
#include "stdbool.h"
#include "fclaw2d_defs.H"

#ifdef __cplusplus
extern "C"
{
#if 0
}                               /* need this because indent is dumb */
#endif
#endif


typedef void (*fclaw2d_problem_setup_t)(fclaw2d_domain_t *domain);

typedef void (*fclaw2d_patch_setup_t)(fclaw2d_domain_t *domain,
                                      fclaw2d_patch_t *this_patch,
                                      int this_block_idx,
                                      int this_patch_idx);

typedef void (*fclaw2d_patch_initialize_t)(fclaw2d_domain_t *domain,
                                           fclaw2d_patch_t *this_patch,
                                           int this_block_idx,
                                           int this_patch_idx);

typedef void (*fclaw2d_patch_physical_bc_t)(fclaw2d_domain_t *domain,
                                            fclaw2d_patch_t *this_patch,
                                            int this_block_idx,
                                            int this_patch_idx,
                                            double t,
                                            double dt,
                                            fclaw_bool *intersects_bc,
                                            fclaw_bool time_interp);

typedef double (*fclaw2d_patch_single_step_update_t)(fclaw2d_domain_t *domain,
                                                     fclaw2d_patch_t *this_patch,
                                                     int this_block_idx,
                                                     int this_patch_idx,
                                                     double t,
                                                     double dt);

typedef fclaw_bool (*fclaw2d_patch_tag4refinement_t)(fclaw2d_domain_t *domain,
                                                     fclaw2d_patch_t *this_patch,
                                                     int this_block_idx, int this_patch_idx,
                                                     int initflag);

typedef fclaw_bool (*fclaw2d_patch_tag4coarsening_t)(fclaw2d_domain_t *domain,
                                                     fclaw2d_patch_t *this_patch,
                                                     int this_blockno,
                                                     int this_patchno);

typedef void (*fclaw2d_patch_write_header_t)(fclaw2d_domain_t* domain,
                                             int iframe, int ngrids);

typedef void (*fclaw2d_patch_write_output_t)(fclaw2d_domain_t *domain,
                                             fclaw2d_patch_t *this_patch,
                                             int this_block_idx,
                                             int this_patch_idx,
                                             int iframe,int patch_num,
                                             int level);

typedef struct fclaw2d_vtable
{
    fclaw2d_problem_setup_t            problem_setup;
    fclaw2d_patch_setup_t              patch_setup;
    fclaw2d_patch_initialize_t         patch_initialize;
    fclaw2d_patch_physical_bc_t        patch_physical_bc;
    fclaw2d_patch_single_step_update_t patch_single_step_update;
    fclaw2d_patch_tag4refinement_t     patch_tag4refinement;
    fclaw2d_patch_tag4coarsening_t     patch_tag4coarsening;
    fclaw2d_patch_write_header_t       write_header;
    fclaw2d_patch_write_output_t       patch_write_output;
} fclaw2d_vtable_t;

void fclaw2d_set_vtable(fclaw2d_domain_t* domain, fclaw2d_vtable_t *solver);

fclaw2d_vtable_t fclaw2d_get_vtable(fclaw2d_domain_t *domain);


#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif