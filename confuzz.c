/*======================================================================
 *                              confuzz.c
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
#include <limits.h>
#include <stdlib.h>
#include <time.h>

/* http://www.azillionmonkeys.com/qed/random.html
 * If the value of the rand function is modded by a ceiling value that
 * does not divide evenly into (RAND_MAX + 1), then the
 * distribution will no longer be uniform, since rand is powered
 * by a linear congruential generator. The following function tries
 * to account for this issue.
 *
 * NOTE: ceiling must be less than (RAND_MAX + 1)
 */
unsigned rand_ceiling(unsigned ceiling){
    unsigned rand_num = 0;
    const unsigned reject_multiplier = (RAND_MAX + 1U)/ceiling;
    const unsigned reject = ceiling*reject_multiplier;

    do{
        rand_num = rand();
    }while(rand_num >= reject);

    return rand_num/reject_multiplier;
}

/* http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
 * By hashing the time_t value via type punning, a portable method of
 * generating a seed from the system time for the standard C library
 * srand function is achieved.
 */
unsigned int time_seed(){
    time_t now = time(NULL);
    unsigned char *p = (unsigned char*)(&now);
    unsigned seed = 0;

    size_t i;
    for (i = 0; i < sizeof(now); i++){
        /* (UCHAR_MAX + 2U) is a relatively
         * portable way to get a Mersenne prime
         */
        seed = seed*(UCHAR_MAX + 2U) + p[i];
    }

    return seed;
}
