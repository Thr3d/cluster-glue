
/*							
 * Copyright (C) 2005 Guochun Shi <gshi@ncsa.uiuc.edu>
 * Copyright (C) 2005 International Business Machines Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdlib.h>
unsigned int	cl_randseed(void);
/* Is currently rand() based.
 * Does srand(cl_randseed()) once internally.
 * Assumes mainloop is setup. */
int		get_next_random(void);

/* generate some random number in the range [a;b];
 * typically used to randomly delay messages. */
#define HAVE_CL_RAND_FROM_INTERVAL 1
static inline
int		cl_rand_from_interval(const int a, const int b)
{
	/*
	 * Be careful here, you don't know RAND_MAX at coding time,
	 * only at compile time. If you think
	 *   (int)(a + (rand()*(b-a)+(RAND_MAX/2))/RAND_MAX);
	 * was correct, think again with RAND_MAX = INT_MAX,
	 * which is the case for many rand() implementations nowadays.
	 *
	 * Don't do modulo, either, as that will skew the distribution, and
	 * still has possible wraparounds, or an insufficient input set for too
	 * small RAND_MAX.
	 *
	 * Rather do the whole calculation in 64 bit, which should be correct
	 * as long as r, a, b, and RAND_MAX are all int.
	 * Of course, if you prefer, you can do it with floating point as well.
	 */
#if 1		/* use long long */
	long long r = get_next_random();
	r = a + (r * (b-a) + RAND_MAX/2)/RAND_MAX;
#else		/* use floating point */
	int r = get_next_random();
	r = a + (int)(1.0 / RAND_MAX * r * (b-a) + 0.5);
#endif
	return r;
}
