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

#include "amr_forestclaw.H"
#include "amr_utils.H"
#include "clawpack_fort.H"

static
void cb_amrout(fclaw2d_domain_t *domain,
               fclaw2d_patch_t *this_patch,
               int this_block_idx,
               int this_patch_idx,
               void *user)
{
    int iframe = *((int *) user);
    fclaw2d_block_t *this_block = &domain->blocks[this_block_idx];
    int num = this_block->num_patches_before + this_patch_idx + 1;
    int matlab_level = this_patch->level + 1;  // Matlab wants levels to start at 1.

    fclaw2d_domain_data_t* ddata = get_domain_data(domain);
    (ddata->f_patch_output)(domain,this_patch,this_block_idx,
                            this_patch_idx,iframe,num,matlab_level);
}


void amrout(fclaw2d_domain_t *domain, int iframe)
{
    // Get total number of patches
    int ngrids = 0;
    for(int i = 0; i < domain->num_blocks; i++)
    {
        fclaw2d_block_t *block = &domain->blocks[i];
        ngrids += block->num_patches;
    }

    fclaw2d_domain_iterate_patches(domain, cb_amrout, (void *) &iframe);
}
