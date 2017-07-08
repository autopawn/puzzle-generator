#ifndef PG_EXEC_H
#define PG_EXEC_H

#include "state.h"

#define MAX_CHOICES 32
// ^ Maximum number of choices that can derivate from a single state.
#define HASH_SLOTS 1299721
// ^ Number of linked lists on the hash table (prime number).
#define QUEUE_SIZE 8388608
// ^ Size of the queue of states to proliferate.

typedef union{
    pgstate step;
    pgstate choices[MAX_CHOICES];
} pgnext;

typedef enum{
    WIN, STEP, CHOICES
} pgconclusion;

typedef struct{
    pgconclusion conclusion;
    int n_choices;
    pgnext next;
} pgresult;

typedef struct _pgexecnode pgexecnode;

struct _pgexecnode{
    int deepness;
    pgstate state;
    pgconclusion conclusion;
    pgexecnode *link; // For use on a linked list on a hash slot.
    pgexecnode *nexts[MAX_CHOICES]; // For use on a tree graph.
    int n_nexts;
};

typedef struct _pgexectree pgexectree;

typedef pgresult (*pgrule)(const pgstate *);
typedef pgresult (*pglevelrule)(const pglevel*, const pgstate *);

pgexectree *compute_exectree(const pglevel *level, pgstate initial,
    	pglevelrule rule, int max_deepness);

void free_exectree(pgexectree *tree);

#endif