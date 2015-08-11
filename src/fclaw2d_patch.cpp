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

#include <fclaw2d_forestclaw.h>
#include <p4est_base.h>

#include <fclaw2d_patch.hpp>
#include <ClawPatch.hpp>

struct fclaw2d_patch_data
{
    ClawPatch *cp;
};


fclaw2d_patch_data_t*
fclaw2d_patch_get_user_data(fclaw2d_patch_t* patch)
{
    return (fclaw2d_patch_data_t *) patch->user;
}

ClawPatch*
fclaw2d_patch_get_cp(fclaw2d_patch_t* this_patch)

{
    fclaw2d_patch_data_t *pdata = fclaw2d_patch_get_user_data(this_patch);
    return pdata->cp;
}


/* -------------------------------------------------------------
   This should be the only place where patch user data gets
   allocated or deleted.
   ------------------------------------------------------------- */

void fclaw2d_patch_data_new(fclaw2d_domain_t* domain,
                                 fclaw2d_patch_t* this_patch)
{
    fclaw2d_domain_data_t *ddata = fclaw2d_domain_get_data(domain);

    /* Initialize user data */
    fclaw2d_patch_data_t *pdata = FCLAW2D_ALLOC(fclaw2d_patch_data_t, 1);
    this_patch->user = (void *) pdata;

    /* create new ClawPatch */
    ClawPatch *cp = new ClawPatch();
    pdata->cp = cp;
    ++ddata->count_set_clawpatch;
}

void fclaw2d_patch_data_delete(fclaw2d_domain_t* domain,
                                    fclaw2d_patch_t *this_patch)
{
    fclaw2d_patch_data_t *pdata = (fclaw2d_patch_data_t*) this_patch->user;

    if (pdata != NULL)
    {
        fclaw2d_domain_data_t *ddata = fclaw2d_domain_get_data(domain);
        delete pdata->cp;
        ++ddata->count_delete_clawpatch;

        FCLAW2D_FREE(pdata);
        this_patch->user = NULL;
    }
}

int
fclaw2d_patch_on_parallel_boundary (const fclaw2d_patch_t * patch)
{
    return patch->flags & FCLAW2D_PATCH_ON_PARALLEL_BOUNDARY ? 1 : 0;
}


void
fclaw2d_domain_iterate_level_mthread (fclaw2d_domain_t * domain, int level,
                                      fclaw2d_patch_callback_t pcb, void *user)
{
    int i, j;
    fclaw2d_block_t *block;
    fclaw2d_patch_t *patch;

    for (i = 0; i < domain->num_blocks; i++)
    {
        block = domain->blocks + i;

#pragma omp parallel for private(patch,j)
        for (j = 0; j < block->num_patches; j++)
        {
            patch = block->patches + j;
            if (patch->level == level)
            {
                pcb (domain, patch, i, j, user);
            }
        }
    }
}
