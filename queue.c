#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include "tile_game.h"

/**
 * Enqueue: Add to tail
 * Dequeue: Remove from head
 */

void enqueue(struct queue *q, struct game_state state) {
    insert_at_tail(&(q->data), serialize(state));
}

struct game_state dequeue(struct queue *q) { 
    size_t serialized_state = remove_from_head(&(q->data));
    return deserialize(serialized_state);
}

bool in(size_t* arr, size_t* max_idx, size_t val) {
    // is val in arr? search until max_idx
    for(int i=0; i<*max_idx; i++) {
        if (arr[i] == val) {
            return true;
        }
    }
    return false;
}

void get_children(struct queue* q, struct game_state curr, size_t* visited, size_t* max_idx) {
    /**
     * enqueues up, down, left, right children of curr to q
     */
    // make four copies of curr
    struct game_state* currU = malloc(sizeof(struct game_state));
    struct game_state* currD = malloc(sizeof(struct game_state));
    struct game_state* currL = malloc(sizeof(struct game_state));
    struct game_state* currR = malloc(sizeof(struct game_state));
    *currU = curr;
    *currD = curr;
    *currL = curr;
    *currR = curr;

    size_t serialized;
    
    // only try valid moves.
    if (curr.empty_row < 3) { 
        move_up(currU);
        // perform the reverse operation of the bit shift that adds the depth of transversal.
        serialized = serialize(*currU) >> 15; 
        if (!in(visited, max_idx, serialized)) { // if we haven't seen this node before.
            enqueue(q, *currU);
            visited[*max_idx] = serialized;
            (*max_idx)++;
        }
    }
    
    if (curr.empty_row > 0) { 
        move_down(currD);
        serialized = serialize(*currD)  >> 15; 
        if (!in(visited, max_idx, serialized)) { // if we haven't seen this node before.
            enqueue(q, *currD);
            visited[*max_idx] = serialized;
            (*max_idx)++;
        }
    }
    
    if (curr.empty_col < 3) { 
        move_left(currL);
        serialized = serialize(*currL) >> 15; 
        if (!in(visited, max_idx, serialized)) { // if we haven't seen this node before.
            enqueue(q, *currL);
            visited[*max_idx] = serialized;
            (*max_idx)++;
        }
    }
    
    if (curr.empty_col > 0) { 
        move_right(currR);
        serialized = serialize(*currR) >> 15; 
        if (!in(visited, max_idx, serialized)) { // if we haven't seen this node before.
            enqueue(q, *currR);
            visited[*max_idx] = serialized;
            (*max_idx)++;
        }
    }
    
    // free the pointers, since we enqueued the literal values
    free(currU);
    free(currD);
    free(currL);
    free(currR);
}

bool goal_test(struct game_state state) {
    // true if goal, false if no. 
    // instaed of checking just empty cell, see if the 
    // numbers match up.
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 3 && j == 3) {
                if (state.tiles[3][3] != 0) {
                    return false;
                }
            } 
            else if (state.tiles[i][j] != (i * 4 + j + 1)) {
                return false;
            }
        }
    }
    return true;
}

int number_of_moves(struct game_state start) { 
    struct queue* q = malloc(sizeof(struct queue));
    // Initialize the queue
    q->data.head = NULL;
    enqueue(q, start);

    struct game_state curr;

    // i mean there are 15! states but lets pretend there are less
    size_t* visited = calloc(1000000, sizeof(size_t));
    size_t iter = 0; // how far along in visited have we got? 

    while(q->data.head != NULL) {
        printf("%ld\n", iter);
        curr = dequeue(q);
        if (goal_test(curr)) {
            // Free the queue before returning
            free_list(q->data);
            free(visited);
            free(q);
            return curr.num_steps;
        }
        // add UDLR to queue
        get_children(q, curr, visited, &iter);
    }

    // Free the queue before returning
    free_list(q->data);
    free(q);
    // free visited
    free(visited);
    return 0;
}
