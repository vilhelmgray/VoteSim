/*======================================================================
 *                              confuzz.h
 *======================================================================
 * Functions for the generation of random numbers.
 *----------------------------------------------------------------------
 * Author:      William Breathitt Gray
 * Date:        October 18, 2012
 * Version:	1.0
 * License:     Simplified BSD License
 * 
 * Language:    C
 * Standard:    C99
 * Doc System:  Doxygen
 *----------------------------------------------------------------------
 * Copyright (c) 2012, William Breathitt Gray
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *======================================================================
 *======================================================================
 */
/**
 * @file
 * Functions for the generation of random numbers.
 * 
 * @author	William Breathitt Gray
 * @date	October 18, 2012
 * @version	1.0
 * @copyright	Simplified BSD License
 */
#ifndef CONFUZZ_H
#define CONFUZZ_H

/**
 * Returns a pseudo-random integral number in the range [0, ceiling).
 * 
 * If the value of the standard C library rand function is modded by a
 * ceiling value that does not divide evenly into (RAND_MAX + 1), then
 * the distribution will no longer be uniform, since rand is powered by
 * a linear congruential generator; the rand_ceiling fucntion tries to
 * account for this caveat.
 *
 * @param ceiling a range ceiling less than (RAND_MAX + 1)
 *
 * @return a pseudo-random integral number in the range [0, ceiling)
 *
 * @sa rand
 * @sa http://www.azillionmonkeys.com/qed/random.html
 */
unsigned rand_ceiling(unsigned ceiling);

/**
 * Generates a portable seed from the system time for the standard C
 * library srand function.
 *
 * A common method to seed the standard C library srand function is to
 * simply pass in a value returned by the standard C library time
 * function. However, while the time function returns a value of type
 * time_t, the srand function expects an argument of type unsigned int.
 * This results in a portability issue since a cast from an
 * implementation-defined type (time_t) to a native C type (unsigned
 * int) is not guranteed to result in a distinct meaningful value; for
 * example, an implementation might define such casts to consistently
 * result in a value of 0, thereby eliminating the uniqueness expected
 * when supplying the system time as a seed. By hashing the time_t value
 * via type punning, a portable method of generating a seed from the
 * system time for the standard C library srand function is achieved.
 *
 * @return a seed generated from the system time for the standard C
 *         library srand function
 *
 * @sa srand, time
 * @sa http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
 */
unsigned int time_seed();

#endif
