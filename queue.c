#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

void enqueue(struct queue *q, struct game_state state) {
    if (q == NULL) return;
    uint64_t serial = serialize(state);
    insert_at_tail(&q->data, serial);
}

struct game_state dequeue(struct queue *q) {
    if (q == NULL || q->data.head == NULL) {
        return (struct game_state){0};
    }
    uint64_t serial = remove_from_head(&q->data);
    return deserialize(serial);
}

bool is_solved(const struct game_state *state) {
    const uint8_t solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state->tiles[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool is_valid_state(const struct game_state *state) {
    bool found_zero = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state->tiles[i][j] == 0) {
                if (found_zero) return false;
                found_zero = true;
                if (i != state->empty_row || j != state->empty_col) return false;
            }
        }
    }
    return found_zero;
}

int number_of_moves(struct game_state start) {
    if (!is_valid_state(&start)) {
        return -1;
    }

    struct queue q = {0};
    struct linked_list visited = {0}; //list to track visited states
    
    enqueue(&q, start);
    insert_at_head(&visited, serialize(start));

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);

        if (is_solved(&current)) {
            free_list(q.data);
            free_list(visited);
            return current.num_steps;
        }

        const int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        for (int i = 0; i < 4; i++) {
            int new_row = current.empty_row + moves[i][0];
            int new_col = current.empty_col + moves[i][1];
            
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
                struct game_state next = current;
                
                next.tiles[current.empty_row][current.empty_col] = next.tiles[new_row][new_col];
                next.tiles[new_row][new_col] = 0;
                next.empty_row = new_row;
                next.empty_col = new_col;
                next.num_steps++;
                
                
                uint64_t serial = serialize(next);
                bool already_visited = false;
                for (struct list_node *n = visited.head; n; n = n->next) {
                    if (n->value == serial) {
                        already_visited = true;
                        break;
                    }
                }
                
                if (!already_visited) {
                    insert_at_head(&visited, serial); 
                    enqueue(&q, next);
                }
            }
        }
    }

    free_list(q.data);
    free_list(visited);
    return -1;
}