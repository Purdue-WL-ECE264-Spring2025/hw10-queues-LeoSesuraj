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

    while(q.data.head!=NULL)
    {
        struct game_state current = dequeue(&q);

        //check goal state
        if(is_solved(current))
        {
            return current.num_steps;
        }

        struct game_state next_state;

        //check all ways

        //up
        next_state = current;
        move_up(&next_state);
        next_state.num_steps = current.num_steps + 1;
        enqueue(&q, next_state);

        //down
        next_state = current;
        move_down(&next_state);
        next_state.num_steps = current.num_steps + 1;
        enqueue(&q, next_state);

        //left
        next_state = current;
        move_left(&next_state);
        next_state.num_steps = current.num_steps + 1;
        enqueue(&q, next_state);

        //right
        next_state = current;
        move_right(&next_state);
        next_state.num_steps = current.num_steps + 1;
        enqueue(&q, next_state);
    }

    return -1;
}
