/*
Copyright (c) 2012-2020 Carsten Burstedde, Donna Calhoun, Erik Chudzik, Christiane Helzel
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

#include "periodic_user.h"

#include <fclaw2d_include_all.h>

/* Two versions of Clawpack */
#include <fc2d_activeflux.h>
#include <fc2d_activeflux_options.h>

#include <activeflux_user_fort.h>

#include <fclaw2d_clawpatch.h>
#include <fclaw2d_clawpatch_fort.h>
#include <fc2d_activeflux_patch_fort.h>

static
void periodic_problem_setup(fclaw2d_global_t* glob)
{
    const user_options_t* user = periodic_get_options(glob);

    if (glob->mpirank == 0)
    {
        FILE *f = fopen("setprob.data","w");
        fprintf(f,    "%20d    %s",user->example,"\% example\n");
        fprintf(f,    "%20d    %s",user->refinement_strategy,"\% refinement strategy\n");
        fprintf(f, "%20.16f    %s",user->ubar,"\% ubar\n");
        fprintf(f, "%20.16f    %s",user->vbar,"\% vbar\n");
        fclose(f);
    }
    fclaw2d_domain_barrier (glob->domain);
    PERIODIC_SETPROB();
}


static
void cb_periodic_output_ascii (fclaw2d_domain_t * domain,
                            fclaw2d_patch_t * this_patch,
                            int this_block_idx, int this_patch_idx,
                            void *user)
{
    int patch_num;
    int level;
    int mx,my,mbc,meqn;
    double xlower,ylower,dx,dy, time;
    double *q, *error, *soln;
    int iframe;

    fclaw2d_global_iterate_t* s = (fclaw2d_global_iterate_t*) user;
    fclaw2d_global_t      *glob = (fclaw2d_global_t*) s->glob;

    //fclaw2d_clawpatch_vtable_t *clawpatch_vt = fclaw2d_clawpatch_vt();
    const fclaw_options_t         *fclaw_opt = fclaw2d_get_options(glob);
    const user_options_t  *user_opt =  periodic_get_options(glob);


    iframe = *((int *) s->user);

    time = glob->curr_time;


    /* Get info not readily available to user */
    fclaw2d_patch_get_info(glob->domain,this_patch,
                           this_block_idx,this_patch_idx,
                           &patch_num,&level);
    
    fclaw2d_clawpatch_grid_data(glob,this_patch,&mx,&my,&mbc,
                                &xlower,&ylower,&dx,&dy);

    fclaw2d_clawpatch_soln_data(glob,this_patch,&q,&meqn);
    error = fclaw2d_clawpatch_get_error(glob,this_patch);
    soln = fclaw2d_clawpatch_get_exactsoln(glob,this_patch);

    char fname[BUFSIZ];
    snprintf (fname, BUFSIZ, "%s.q%04d", fclaw_opt->prefix, iframe);


    /* Here, we pass in q and the error, so need special headers and files */
    if (user_opt->claw_version == 4)
    {
        ACTIVEFLUX_FORT_WRITE_FILE(fname, &mx,&my,&meqn,&mbc,
                                   &xlower,&ylower,
                                   &dx,&dy,
                                   q,error,soln, &time, 
                                   &patch_num,&level,
                                   &this_block_idx,
                                   &glob->mpirank);
    }
}



void periodic_link_solvers(fclaw2d_global_t *glob)
{
    fclaw2d_vtable_t *vt = fclaw2d_vt();


    vt->problem_setup = &periodic_problem_setup;  /* Version-independent */

    const user_options_t* user = periodic_get_options(glob);
    if (user->claw_version == 4)
    {
        fc2d_activeflux_vtable_t *activeflux_vt = fc2d_activeflux_vt();        

        activeflux_vt->fort_qinit     = &ACTIVEFLUX_QINIT;
        activeflux_vt->fort_rpn2      = &ACTIVEFLUX_RPN2;
        activeflux_vt->fort_rpt2      = &ACTIVEFLUX_RPT2;

        /* Include this for conservative fix;   only second order so far */
        // clawpack46_vt->fort_rpn2_cons = &RPN2CONS_UPDATE;

        fclaw2d_clawpatch_vtable_t *clawpatch_vt = fclaw2d_clawpatch_vt();
        clawpatch_vt->fort_tag4coarsening = &TAG4COARSENING;
        clawpatch_vt->fort_tag4refinement = &TAG4REFINEMENT;

        const fclaw_options_t  *fclaw_opt = fclaw2d_get_options(glob);
        if (fclaw_opt->compute_error)
        {
            clawpatch_vt->fort_compute_patch_error = &PERIODIC_COMPUTE_ERROR;
            clawpatch_vt->fort_header_ascii        = &ACTIVEFLUX_FORT_HEADER_ASCII;
            clawpatch_vt->cb_output_ascii          = &cb_periodic_output_ascii;                
        }

        fc2d_activeflux_options_t *aflux_opt = fc2d_activeflux_get_options(glob);
        if (aflux_opt->order[0] == 3)
        {
            clawpatch_vt->fort_interpolate_face   = &ACTIVEFLUX_FORT_INTERPOLATE_FACE;
            clawpatch_vt->fort_interpolate_corner = &ACTIVEFLUX_FORT_INTERPOLATE_CORNER;
            clawpatch_vt->fort_interpolate2fine   = &ACTIVEFLUX_FORT_INTERPOLATE2FINE;
        }
    }
}





