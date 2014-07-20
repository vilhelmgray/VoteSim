/*======================================================================
 *                           hamming_table.h
 *======================================================================
 * Hamming weight lookup table for the integers 0-65535.
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
 * Hamming weight lookup table for the integers 0-65535.
 * 
 * @author	William Breathitt Gray
 * @date	October 18, 2012
 * @version	1.0
 * @copyright	Simplified BSD License
 */
#ifndef HAMMING_TABLE_H
#define HAMMING_TABLE_H

#include <stddef.h>

/**
 * Hamming weight lookup table for the integers 0-65535.
 */
extern const unsigned char HAMMING[65536];
/**
 * The size in bytes of the largest lookup value in the Hamming weight
 * lookup table.
 */
extern size_t MAX_LOOKUP_SIZE;

#endif
