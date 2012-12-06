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

    Copyright (C) 2007 David Howden
    Copyright (C) 2007, 2008, 2009, 2010 William Hart
    Copyright (C) 2008 Richard Howell-Peak
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2012 Lina Kulakova

******************************************************************************/

#include <stdlib.h>
#include "nmod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int iter;
    flint_rand_t state;
    flint_randinit(state);

    printf("factor_distinct_deg....");
    fflush(stdout);

    for (iter = 0; iter < 200; iter++)
    {
        nmod_poly_t poly1, poly, q, r, product;
        nmod_poly_factor_t res;
        mp_limb_t modulus, lead;
        long i, length, num;
        long *degs;

        modulus = n_randtest_prime(state, 0);

        nmod_poly_init(poly1, modulus);
        nmod_poly_init(poly, modulus);
        nmod_poly_init(q, modulus);
        nmod_poly_init(r, modulus);

        nmod_poly_zero(poly1);
        nmod_poly_set_coeff_ui(poly1, 0, 1);

        length = n_randint(state, 7) + 2;
        do
        {
            nmod_poly_randtest(poly, state, length);
            if (poly->length)
                nmod_poly_make_monic(poly, poly);
        }
        while ((poly->length < 2) || (!nmod_poly_is_irreducible(poly)));

        nmod_poly_mul(poly1, poly1, poly);

        num = n_randint(state, 5) + 1;

        for (i = 1; i < num; i++)
        {
            do 
            {
                length = n_randint(state, 7) + 2;
                nmod_poly_randtest(poly, state, length);
                if (poly->length)
                {
                    nmod_poly_make_monic(poly, poly);
                    nmod_poly_divrem(q, r, poly1, poly);
                }
            }
            while ((poly->length < 2) || (!nmod_poly_is_irreducible(poly)) ||
                (r->length == 0));

            nmod_poly_mul(poly1, poly1, poly);
        }

        if (!(degs = flint_malloc((poly1->length - 1) * sizeof(long))))
        {
            printf("Fatal error: not enough memory.");
            abort();
        }
        nmod_poly_factor_init(res);
        nmod_poly_factor_distinct_deg(res, poly1, &degs);

        nmod_poly_init_preinv(product, poly1->mod.n, poly1->mod.ninv);
        nmod_poly_set_coeff_ui(product, 0, 1);
        for (i = 0; i < res->num; i++)
            nmod_poly_mul(product, product, res->p + i);

        lead = poly1->coeffs[poly1->length - 1];
        nmod_poly_scalar_mul_nmod(product, product, lead);

        if (!nmod_poly_equal(poly1, product))
        {
            printf("Error: product of factors does not equal to the original polynomial\n");
            printf("poly:\n"); nmod_poly_print(poly1); printf("\n");
            printf("product:\n"); nmod_poly_print(product); printf("\n");
            abort();
        }

        flint_free(degs);
        nmod_poly_clear(product);
        nmod_poly_clear(q);
        nmod_poly_clear(r);
        nmod_poly_clear(poly1);
        nmod_poly_clear(poly);
        nmod_poly_factor_clear(res);
    }

    flint_randclear(state);

    printf("PASS\n");
    return 0;
}
