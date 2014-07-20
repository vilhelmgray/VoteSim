/*======================================================================
 *                               VoteSim
 *======================================================================
 * Simulates various types of elections for statistical analysis.
 *----------------------------------------------------------------------
 * Author:      William Breathitt Gray
 * Date:        October 19, 2012
 * Version:     1.0
 * Copyright:   Simplified BSD License
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
 * Simulates various types of elections for statistical analysis.
 * 
 * @author      William Breathitt Gray
 * @date        October 18, 2012
 * @version     1.0
 * @copyright   Simplified BSD License
 */
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bit_functions.h"
#include "confuzz.h"
#include "election.h"

/**
 * Compares Candidate structures by greatest number of votes; for use in
 * the standard C library qsort function.
 *
 * @param a Candidate structure for comparison
 * @param b Candidate structure for comparison
 *
 * @return the relative sorting of the two Candidate structures
 *
 * @retval -1 a comes before b
 * @retval 0 a and b match in ranking
 * @retval 1 a comes after b
 *
 * @sa qsort
 */
int sort_candidates(const void *a, const void *b){
    if(((Candidate*)a)->votes > ((Candidate*)b)->votes){
        return -1;
    }else if(((Candidate*)a)->votes < ((Candidate*)b)->votes){
        return 1;
    }
    return 0;
}

/**
 * Standard C main function.
 *
 * @note To print election data to a file, additional code must be added
 *       at line 362 in the votesim.c file.
 */
int main(void){
    srand(time_seed());

    // sqrt prevents overflow when calculating disapproval rating of candidates
    unsigned MAX_POPULATION = sqrt(RAND_MAX);
    unsigned MAX_ISSUES = hamming_weight(MAX_POPULATION);
    // Check if RAND_MAX is not a Mersenne prime
    if((MAX_POPULATION | (MAX_POPULATION>>1)) != MAX_POPULATION){
        MAX_ISSUES = bit_count(MAX_POPULATION>>1);
    }

    /****************
     * CONFIGURATION 
     ****************/
    char buffer[256];
    char *check;
    
    unsigned verbose = 1;
    printf("Print election statistics to the screen (0 to exit) [Y/n]: ");
    fgets(buffer, sizeof(buffer), stdin);
    if(buffer[0] == '0'){
        exit(0);
    }else if(buffer[0] == 'N' || buffer[0] == 'n'){
        verbose = 0;
    }
    
    FILE *fout = NULL;
    printf("Print election data to a file (0 to exit) [y/N]: ");
    fgets(buffer, sizeof(buffer), stdin);
    if(buffer[0] == '0'){
        exit(0);
    }else if(buffer[0] == 'Y' || buffer[0] == 'y'){
        do{
            printf("Input filepath (0 to cancel): ");
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] == '0' && isspace(buffer[1])){
                break;
            }
            
            // trim whitespace
            size_t t;
            for(t = 0; t < sizeof(buffer); t++){
                // find end of string
                if(buffer[t] == 0){
                    // going to search from end now
                    t--;
                    break;
                }
            }
            do{
                // find end of filepath
                if(!isspace(buffer[t])){
                    // set end of string
                    buffer[++t] = 0;
                    break;
                }
                // go to next character from end
                t--;
            }while(t != 0);
            
            fout = fopen(buffer, "w");
            if(fout){
                break;
            }
        }while(fprintf(stderr, "***** UNABLE TO CREATE FILE *****\n"
                               ">>>>> %s\n\n", strerror(errno)));
    }

    unsigned long num_issues;
    size_t pool_size = 0;
    unsigned *stance_poll = NULL;
    unsigned *hitler_table = NULL;
    Candidate *candidates = NULL;
    Candidate **elected = NULL, **elected_h = NULL, **elected_a = NULL;
    do{
        printf("Number of issues (0 to exit) [0-%u]: ", MAX_ISSUES);
        fgets(buffer, sizeof(buffer), stdin);
        num_issues = strtoul(buffer, &check, 0);

        if(*buffer != *check){
            if(!num_issues){
                exit(0);
            }else if(num_issues <= MAX_ISSUES){
                pool_size = 1<<num_issues;
                hitler_table = malloc(sizeof(*hitler_table) * pool_size);
                stance_poll = malloc(sizeof(*stance_poll) * num_issues);
                candidates = malloc(sizeof(*candidates) * pool_size);
                elected = malloc(sizeof(*elected) * pool_size);
                elected_h = malloc(sizeof(*elected_h) * pool_size);
                elected_a = malloc(sizeof(*elected_a) * pool_size);
                // check if all memory was allocated
                if(hitler_table && stance_poll && candidates &&
                   elected && elected_h && elected_a){
                    break;
                }else{
                    // if failed at *elected_a then frees memory allocated,
                    // else rest are NULL anyway so no worries
                    free(hitler_table);
                    free(stance_poll);
                    free(candidates);
                    free(elected);
                    free(elected_h);
                    fprintf(stderr,"\n** ERROR: Not enough memory to populate election tables\n"
                                   ">>>>>>>>> Select a smaller number of issues.\n\n");
                }
            }
        }
    }while(fprintf(stderr, "***** INVALID INPUT *****\n\n"));

    unsigned long population_size;
    do{
        printf("Population Size (0 to exit) [0-%u]: ", MAX_POPULATION);
        fgets(buffer, sizeof(buffer), stdin);
        population_size = strtoul(buffer, &check, 0);

        if(*buffer != *check){
            if(!population_size){
                exit(0);
            }else if(population_size <= MAX_POPULATION){
                break;
            }
        }
    }while(fprintf(stderr, "***** INVALID INPUT *****\n\n"));

    unsigned long num_elections, MAX_ELECTIONS = (-1);
    do{
        printf("Number of Elections (0 to exit) [0-%lu]: ", MAX_ELECTIONS);
        fgets(buffer, sizeof(buffer), stdin);
        num_elections = strtoul(buffer, &check, 0);

        if(*buffer != *check){
            if(!num_elections){
                exit(0);
            }else if(num_elections <= MAX_ELECTIONS){
                break;
            }
        }
    }while(fprintf(stderr, "***** INVALID INPUT *****\n\n"));

    /*************
     * SIMULATION 
     *************/
    unsigned long j;
    for(j = 0; j < num_elections; j++){
        size_t i;
        // initialize candidate pool
        for(i = 0; i < pool_size; i++){
            candidates[i].id = i;
        }

        // perform traditional election
        size_t num_candidates = 0;
        perform_election(population_size,
                         pool_size,
                         &num_candidates,
                         candidates);

        // initialize hitler table
        for(i = 0; i < num_candidates; i++){
                hitler_table[candidates[i].id] = 0;
        }
        // initialize washington platform
        for(i = 0; i < num_issues; i++){
            stance_poll[i] = 0;
        }

        // compute election statistics
        compute_election_statistics(num_issues,
                                    num_candidates,
                                    stance_poll,
                                    hitler_table,
                                    candidates);
        // sort candidates by lowest disapproval rating
        qsort(candidates, num_candidates, sizeof(*candidates), sort_candidates);
        
        // find election winners
        size_t num_winners = 1, num_a_winners = 1, num_h_winners = 1;
        elected[0] = candidates;
        elected_a[0] = candidates;
        elected_h[0] = candidates;
        for(i = 1; i < num_candidates; i++){
            // traditional election winners
            if(candidates[i].votes == elected[0]->votes){
                elected[num_winners++] = candidates+i;
            }else{
                break;
            }
            // approval winners
            if(candidates[i].sum_disapproval < elected_a[0]->sum_disapproval){
                elected_a[0] = candidates+i;
                num_a_winners = 1;
            }else if(candidates[i].sum_disapproval == elected_a[0]->sum_disapproval){
                elected_a[num_a_winners++] = candidates+i;
            }
            // hitler election winners
            if(hitler_table[candidates[i].id] < hitler_table[elected_h[0]->id]){
                elected_h[0] = candidates+i;
                num_h_winners = 1;
            }else if(hitler_table[candidates[i].id] == hitler_table[elected_h[0]->id]){
                elected_h[num_h_winners++] = candidates+i;
            }
        }
        // continue finding approval winners and hitler election winners
        for(; i < num_candidates; i++){
            // approval winners
            if(candidates[i].sum_disapproval < elected_a[0]->sum_disapproval){
                elected_a[0] = candidates+i;
                num_a_winners = 1;
            }else if(candidates[i].sum_disapproval == elected_a[0]->sum_disapproval){
                elected_a[num_a_winners++] = candidates+i;
            }
            // hitler election winners
            if(hitler_table[candidates[i].id] < hitler_table[elected_h[0]->id]){
                elected_h[0] = candidates+i;
                num_h_winners = 1;
            }else if(hitler_table[candidates[i].id] == hitler_table[elected_h[0]->id]){
                elected_h[num_h_winners++] = candidates+i;
            }
        }
        
        // find washington candidate
        Candidate washington;
        washington.id = 0;
        for(i = 0; i < num_issues; i++){
            size_t stance_0 = population_size - stance_poll[i];
            // NOTE: slight bias toward stance=1 when population size is even
            if(stance_poll[i] >= stance_0){
                washington.id += 1<<i;
            }
        }
        
        // compute statistics for washington candidate
        compute_candidate_statistics(num_issues,
                                     num_candidates,
                                     &washington,
                                     candidates);

        // perform two-party system election
        Candidate status_quo[2] = {candidates[0], candidates[1]};
        status_quo[0].votes = 0;
        status_quo[1].votes = 0;
        // record votes
        for(i = 0; i < num_candidates; i++){
            unsigned disapproval_0 = hamming_weight((status_quo[0].id)^(candidates[i].id));
            unsigned disapproval_1 = hamming_weight((status_quo[1].id)^(candidates[i].id));

            // if disapproval values are equal, then votes aren't counted
            if(disapproval_0 < disapproval_1){
                status_quo[0].votes += candidates[i].votes;
            }else if(disapproval_0 > disapproval_1){
                status_quo[1].votes += candidates[i].votes;
            }
        }
        // sort two-party system candidates
        qsort(status_quo, 2, sizeof(*status_quo), sort_candidates);
        
        /*****************
         * REPORT RESULTS 
         *****************/
        // print to screen
        if(verbose){
            printf("\n========== ELECTION #%lu ==========\n", j+1);
            print_election_statistics(population_size, num_issues,
                                      num_a_winners, num_candidates, num_h_winners, num_winners,
                                      hitler_table, candidates, &washington, status_quo,
                                      elected, elected_a, elected_h);
        }
        
        // print to file
        if(fout){
            /********************************************
             * Add code here to produce custom data file
             ********************************************/
        }
    }

    return 0;
}
