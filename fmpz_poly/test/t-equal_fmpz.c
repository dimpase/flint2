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

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("equal_fmpz....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b;
        fmpz_t c;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_init(c);

        fmpz_randtest(c, state, 1 + n_randint(state, 200));
        fmpz_poly_set_fmpz(b, c);

        fmpz_poly_randtest(a, state, n_randint(state, 6),
            1 + n_randint(state, 200));
        if (n_randint(state, 2))
            fmpz_poly_set_coeff_fmpz(a, 0, c);

        result = fmpz_poly_equal(a, b) == fmpz_poly_equal_fmpz(a, c);
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_print(a), printf("\n\n");
            fmpz_poly_print(b), printf("\n\n");
            fmpz_print(c), printf("\n\n");
            abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_clear(c);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
