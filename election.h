/*======================================================================
 *                              election.h
 *======================================================================
 * Functions for computing and printing election statistics.
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
 * Functions for computing and printing election statistics.
 * 
 * @author	William Breathitt Gray
 * @date	October 18, 2012
 * @version	1.0
 * @copyright	Simplified BSD License
 */
#ifndef ELECTION_H
#define ELECTION_H

/**
 * Represents a candidate in an election.
 */
typedef struct{
    unsigned contra; /**< number of voters who agree with less than half
                          of this candidate's platform */
    unsigned medius; /**< number of voters who agree with exactly half
                          of this candidate's platform */
    unsigned pro; /**< number of voters who agree with more than half of
                       this candidate's platform */
    unsigned sum_disapproval; /**< sum of all voters' disagreements with
                                   this candidate */
    unsigned votes; /**< number of votes cast for this candidate */
    size_t hitler; /**< id of an actual candidate whose platform most
                        differs from this candidate */
    size_t id; /**< id (base 10 representation of platform) of this
                    candidate */
} Candidate;

/**
 * Populates a Candidate structure.
 *
 * @param num_issues number of issues in the election
 * @param num_candidates number of candidates
 * @param candidate pointer to a candidate
 * @param candidates array of candidates
 *
 * @return Candidate structure pointed by candidate is populated
 *
 * @sa Candidate
 */
void compute_candidate_statistics(unsigned num_issues,
                                  size_t num_candidates,
                                  Candidate *candidate,
                                  Candidate *candidates);

/**
 * Computes election statistics.
 *
 * @param num_issues number of issues in the election
 * @param num_candidates number of candidates
 * @param stance_poll array containing average voter stance on election
 *                    issues
 * @param hitler_table array of votes in hitler election method
 * @param candidates array of candidates
 *
 * @return stance_poll array, hitler_table array, and candidates in
 *         candidates array are populated
 */
void compute_election_statistics(unsigned num_issues,
                                 size_t num_candidates,
                                 unsigned *stance_poll,
                                 unsigned *hitler_table,
                                 Candidate *candidates);

/**
 * Perform traditional election.
 *
 * @param voters_left number of voters who have not yet voted
 * @param pool_size number of possible candidates
 * @param num_candidates pointer to the variable holding the number of
 *                       actual candidates
 * @param candidates array to hold actual candidates
 *
 * @return candidates array is populated and the number of actual
 *         candidates is stored in the variable pointed by
 *         num_candidates
 */
void perform_election(unsigned voters_left,
                      size_t pool_size,
                      size_t *num_candidates,
                      Candidate *candidates);

/**
 * Print candidate statistics to stdout.
 *
 * @param population_size number of voters in the election
 * @param num_issues number of issues in the election
 * @param prefix string prefix for output line
 * @param hitler_table array of votes in hitler election method
 * @param candidate pointer to a candidate
 *
 * @sa Candidate
 */
void print_candidate_statistics(unsigned population_size,
                                unsigned num_issues,
                                char *prefix,
                                unsigned *hitler_table,
                                Candidate *candidate);

/**
 * Print election statistics to stdout.
 *
 * @param population_size number of voters in the election
 * @param num_issues number of issues in the election
 * @param num_a_winners number of approval winners
 * @param num_candidates number of candidates
 * @param num_h_winners number of hitler election winners
 * @param num_winners number of traditional election winners
 * @param hitler_table array of votes in hitler election method
 * @param candidates array of candidates
 * @param washington pointer to washington candidate
 * @param status_quo array holding traditional election winner and
 *                   runner-up
 * @param elected array of pointers to traditional election winners
 * @param elected_a array of pointers to approval winners
 * @param elected_h array of pointers to hitler election winners
 */
void print_election_statistics(unsigned population_size,
                               unsigned num_issues,
                               size_t num_a_winners,
                               size_t num_candidates,
                               size_t num_h_winners,
                               size_t num_winners,
                               unsigned *hitler_table,
                               Candidate *candidates,
                               Candidate *washington,
                               Candidate *status_quo,
                               Candidate **elected,
                               Candidate **elected_a,
                               Candidate **elected_h);

#endif
