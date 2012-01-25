/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011 Sebastian Pancratz

******************************************************************************/

#include "fmpz_mat.h"
#include "padic_mat.h"

void _padic_mat_mul(padic_mat_t C, const padic_mat_t A, const padic_mat_t B, 
                                   const padic_ctx_t ctx)
{
    if (padic_mat_is_empty(C))
        return;

    fmpz_mat_mul(padic_mat(C), padic_mat(A), padic_mat(B));

    padic_mat_val(C) = padic_mat_val(A) + padic_mat_val(B);

    _padic_mat_canonicalise(C, ctx);
}

void padic_mat_mul(padic_mat_t C, const padic_mat_t A, const padic_mat_t B, 
                                  const padic_ctx_t ctx)
{
    _padic_mat_mul(C, A, B, ctx);
    _padic_mat_reduce(C, ctx);
}

