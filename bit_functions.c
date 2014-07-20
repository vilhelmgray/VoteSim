/*======================================================================
 *                           bit_functions.c
 *======================================================================
 * Functions for bit operations.
 *----------------------------------------------------------------------
 * Author:      William Breathitt Gray
 * Date:        October 18, 2012
 * Version:     1.0
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
 * Functions for bit operations.
 * 
 * @author	William Breathitt Gray
 * @date	October 18, 2012
 * @version	1.0
 * @copyright	Simplified BSD License
 */
#include "bit_functions.h"
#include "hamming_table.h"

unsigned char bit_count(unsigned num){
    unsigned char bit_count = 0;
    while(num){
        num >>= 1;
        bit_count++;
    }
    return bit_count;
}

unsigned char hamming_weight(unsigned num){
    // MAX_LOOKUP_SIZE is the byte size of the maximum lookup available
    // in the hamming weight table. Here, we ensure there is no overflow
    // later on in the code.
    if(MAX_LOOKUP_SIZE >= sizeof(unsigned)){
        MAX_LOOKUP_SIZE = sizeof(unsigned) - 1;
    }

    // calculate slice size in bits
    unsigned slice_size = MAX_LOOKUP_SIZE * 8;
    
    // create mask for lower bits to calculate hamming weight in slices
    unsigned mask = ~(((unsigned)(-1)) << slice_size);

    // sum hamming weights of each slice
    unsigned char weight = 0;
    do{
        weight += HAMMING[num & mask];
    }while(num >>= slice_size);
    
    return weight;
}
