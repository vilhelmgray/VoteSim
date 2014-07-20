/*======================================================================
 *                              election.c
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
#include <stdio.h>

#include "bit_functions.h"
#include "confuzz.h"
#include "election.h"

/*
 * The method for determining a candidate's hitler doesn't account for
 * platforms which produce the same disapproval value; thus hitler id is
 * biased toward the front of the candidates array.
 */
void compute_candidate_statistics(unsigned num_issues,
                                  size_t num_candidates,
                                  Candidate *candidate,
                                  Candidate *candidates){
    candidate->sum_disapproval = 0;
    candidate->hitler = candidate->id;
    candidate->pro = 0;
    candidate->contra = 0;
    candidate->medius = 0;
    
    unsigned max_disapproval = 0;
    
    // algorithm: hamming weight of XOR to determine number of disagreements with candidate
    //            multiply by number of voters with those specific disagreements
    size_t h;
    for(h = 0; h < num_candidates; h++){
        unsigned disapproval = hamming_weight((candidate->id)^(candidates[h].id));

        // determine if this group of voters leans toward
        // this particular candidate's platform
        if((double)disapproval > (double)num_issues/2.0){
            candidate->contra += candidates[h].votes;
        }else if((double)disapproval < (double)num_issues/2.0){
            candidate->pro += candidates[h].votes;
        }else{
            candidate->medius += candidates[h].votes;
        }

        // determine each group of voters' "hitler" by
        // finding candidate with most different platform
        if(disapproval > max_disapproval){
            candidate->hitler = candidates[h].id;
            max_disapproval = disapproval;
        }

        // accumulate disapprovals
        candidate->sum_disapproval += disapproval * candidates[h].votes;
    }
}

void compute_election_statistics(unsigned num_issues,
                                 size_t num_candidates,
                                 unsigned *stance_poll,
                                 unsigned *hitler_table,
                                 Candidate *candidates){
    // compute statistics for each candidate
    size_t i;
    for(i = 0; i < num_candidates; i++){        
        // compute candidate disapproval statistics
        compute_candidate_statistics(num_issues,
                                     num_candidates,
                                     candidates+i,
                                     candidates);
        // record "hitler" votes
        hitler_table[candidates[i].hitler] += candidates[i].votes;

        // tally up number of voters with stance=1 for respective issues
        unsigned w;
        for(w = 0; w < num_issues; w++){
            unsigned stance = ((candidates[i].id)>>w) & 0x1;
            stance_poll[w] += stance * candidates[i].votes;
        }
    }
}

/*
 * for most cases:
 * voters_left = population_size
 */
void perform_election(unsigned voters_left,
                      size_t pool_size,
                      size_t *num_candidates,
                      Candidate *candidates){
    *num_candidates = 0;
    
    // Fisher-Yates shuffle
    unsigned i;
    for(i = 0; voters_left && i < pool_size-1; i++){
        // select candidate index
        size_t grab = rand_ceiling(pool_size-i) + i;

        // swap out candidate
        size_t toss = candidates[*num_candidates].id;
        candidates[*num_candidates].id = candidates[grab].id;
        candidates[grab].id = toss;

        // allocate voters
        unsigned votes = rand_ceiling(voters_left+1);
        // check if candidate got any votes
        if(votes){
            // allocate votes to candidate
            voters_left -= votes;
            candidates[*num_candidates].votes = votes;
            // record new candidate
            (*num_candidates)++;
        }
    }
    // take care of any trailing voters
    if(voters_left){
        candidates[*num_candidates].id = candidates[pool_size-1].id;
        candidates[*num_candidates].votes = voters_left;
        // record new candidate
        (*num_candidates)++;
    }
}

void print_candidate_statistics(unsigned population_size,
                                unsigned num_issues,
                                char *prefix,
                                unsigned *hitler_table,
                                Candidate *candidate){
    char buffer[6];
    // check pro-contra ratio for division by zero
    if(candidate->pro > candidate->contra){
        sprintf(buffer, "P%.2lf", (double)(candidate->pro - candidate->contra)/population_size);
    }else if(candidate->contra > candidate->pro){
        sprintf(buffer, "C%.2lf", (double)(candidate->contra - candidate->pro)/population_size);
    }else{
        sprintf(buffer, "M0.00");
    }
    // calculate percentages
    double MAX_SUM_DISAPPROVAL = population_size*num_issues;
    double approval = (1.0-(candidate->sum_disapproval/MAX_SUM_DISAPPROVAL))*100;
    double medius_percentage = (double)(candidate->medius)/population_size;

    // print candidate statistics
    printf("%s%zu: %2.2lf%% (%s %.2lf) [%zu] %u %u\n", prefix, candidate->id,
                                                    approval, buffer, medius_percentage,
                                                    candidate->hitler, candidate->votes,
                                                    hitler_table[candidate->id]);
}

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
                               Candidate **elected_h){
    char buffer[16];
    size_t i;
    // report traditional election statistics
    for(i = 0; i < num_candidates; i++){
        print_candidate_statistics(population_size, num_issues, "", hitler_table, candidates+i);
    }
    
    printf("\n----------------------------------------------------------------------\n");

    // report traditional election winners
    printf("\nTraditional Election Winners:\n");
    for(i = 0; i < num_winners; i++){
        sprintf(buffer, "  %zu) ", i+1);
        print_candidate_statistics(population_size, num_issues, buffer, hitler_table, elected[i]);
    }
    
    // report approval winners
    printf("\nApproval Winners: \n");
    for(i = 0; i < num_a_winners; i++){
        sprintf(buffer, "  %zu) ", i+1);
        print_candidate_statistics(population_size, num_issues, buffer, hitler_table, elected_a[i]);
    }

    // report hitler election winners
    printf("\nHitler Election Winners:\n");
    for(i = 0; i < num_h_winners; i++){
        sprintf(buffer, "  %zu) ", i+1);
        print_candidate_statistics(population_size, num_issues, buffer, hitler_table, elected_h[i]);
    }

    // report washington candidate statistics
    printf("\nWashington Candidate: \n");
    // check pro-contra ratio for division by zero
    if(washington->pro > washington->contra){
        sprintf(buffer, "P%.2lf", (double)(washington->pro - washington->contra)/population_size);
    }else if(washington->contra > washington->pro){
        sprintf(buffer, "C%.2lf", (double)(washington->contra - washington->pro)/population_size);
    }else{
        sprintf(buffer, "M0.00");
    }
    // true_whitler is the inversion of washington platform;
    // num_issues is less than the total number of bits in an int so
    // we AND the inverse of washington with a mask to grab just the lower bits
    size_t true_whitler = ~(washington->id) & ~(((unsigned)(-1))<<num_issues);
    // calculate percentages
    double MAX_SUM_DISAPPROVAL = population_size*num_issues;
    double approval = (1.0-(washington->sum_disapproval/MAX_SUM_DISAPPROVAL))*100;
    double medius_percentage = (double)(washington->medius)/population_size;
    // print washington statistics
    printf("   > %zu: %2.2lf%% (%s %.2lf) [%zu]{%zu}\n", washington->id,
                                                         approval, buffer, medius_percentage,
                                                         washington->hitler, true_whitler);
    
    // report two-party system statistics
    printf("\nTwo-Party System Election: \n");
    for(i = 0; i < 2; i++){
        // check pro-contra ratio for division by zero
        if(status_quo[i].pro > status_quo[i].contra){
            sprintf(buffer, "P%.2lf", (double)(status_quo[i].pro - status_quo[i].contra)/population_size);
        }else if(status_quo[i].contra > status_quo[i].pro){
            sprintf(buffer, "C%.2lf", (double)(status_quo[i].contra - status_quo[i].pro)/population_size);
        }else{
            sprintf(buffer, "M0.00");
        }
        // calculate percentages
        approval = (1.0-(status_quo[i].sum_disapproval/MAX_SUM_DISAPPROVAL))*100;
        medius_percentage = (double)(status_quo[i].medius)/population_size;
        double vote_ratio = (double)(status_quo[i].votes)/population_size;
        // print status quo candidate statistics
        printf("  %zu) %zu: %2.2lf%% (%s %.2lf) %u %.2lf\n", i+1, status_quo[i].id,
                                                              approval, buffer, medius_percentage,
                                                              status_quo[i].votes, vote_ratio);
    }

    printf("\n----------------------------------------------------------------------\n");
}
