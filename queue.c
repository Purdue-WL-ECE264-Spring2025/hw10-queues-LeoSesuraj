#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) {
    uint16_t serial = serialize(state);

    insert_at_tail(&q->data,serial);
}

struct game_state dequeue(struct queue *q) {
    if(q->data.head == NULL)
    {
        return (struct game_state){0}; 
    }
    
    uint64_t serial = remove_from_head(&q->data);

    struct game_state state = deserialize(serial);
    
    return state;
}

bool is_solved(struct game_state state) {
    const uint8_t solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state.tiles[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    enqueue(&q, start);

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);

        if (is_solved(current)) {
            free_list(q.data);
            return current.num_steps;
        }

        if (current.empty_row > 0) {
            struct game_state next = current;
            next.tiles[next.empty_row][next.empty_col] = next.tiles[next.empty_row-1][next.empty_col];
            next.tiles[next.empty_row-1][next.empty_col] = 0;
            next.empty_row--;
            next.num_steps++;
            enqueue(&q, next);
        }

        if (current.empty_row < 3) {
            struct game_state next = current;
            next.tiles[next.empty_row][next.empty_col] = next.tiles[next.empty_row+1][next.empty_col];
            next.tiles[next.empty_row+1][next.empty_col] = 0;
            next.empty_row++;
            next.num_steps++;
            enqueue(&q, next);
        }

        if (current.empty_col > 0) {
            struct game_state next = current;
            next.tiles[next.empty_row][next.empty_col] = next.tiles[next.empty_row][next.empty_col-1];
            next.tiles[next.empty_row][next.empty_col-1] = 0;
            next.empty_col--;
            next.num_steps++;
            enqueue(&q, next);
        }

        if (current.empty_col < 3) {
            struct game_state next = current;
            next.tiles[next.empty_row][next.empty_col] = next.tiles[next.empty_row][next.empty_col+1];
            next.tiles[next.empty_row][next.empty_col+1] = 0;
            next.empty_col++;
            next.num_steps++;
            enqueue(&q, next);
        }
    }

    free_list(q.data);
    return -1;
}