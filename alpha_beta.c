#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "alpha_beta.h"

int CHESS_MALUS = 50;
int MATE_MALUS = 10e6;

#define getProb() ((float) (rand() % 100) / 100)

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) <= (b) ? (b) : (a))

movement_coords* moveCoords(coords start, coords end, int score){
    movement_coords* result = malloc(sizeof(movement_coords));
    result->start_pos = (coords) {start.posx, start.posy};
    result->end_pos = (coords) {end.posx, end.posy};
    result->score = score;
    return result;
}

list_t* getAllPossibleMove(game_board* board, enum color color){
    list_t* possible_move = list_new();
    possible_move->free = free;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            // if the piece is of the right color
            if(board->board[i][j] != NULL && board->board[i][j]->color == color){
                // get all possible move for this piece
                coords current_coords = Coords(i, j);

                for(int i = 0; i < 8; i++){
                    for(int j = 0; j < 8; j++){
                        coords to_ij = Coords(i, j);
                        movement_coords* current_movement = moveCoords(current_coords, to_ij, 0);
                        if(isPosAccessible(board, current_coords, to_ij)){
                            list_rpush(possible_move, list_node_new(current_movement));
                        }
                        else
                            free(current_movement);
                    }
                }
            }
        }
    }

    return possible_move;
}

int evaluateBoard(game_board* board){
    int score = 0;

    // ajust score according to game status
    game_status status = getGameStatus(board);
    switch(status){
        case WHITE_CHESS:
            score = -CHESS_MALUS;
            break;
        case WHITE_MATE:
            return -MATE_MALUS;
        case BLACK_CHESS:
            score = CHESS_MALUS;
            break;
        case BLACK_MATE:
            return MATE_MALUS;
        case PAT:
            return 0;
        default:
            break;
    }

    // ajusting score according pieces values
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            piece* current = board->board[i][j];
            if(current != NULL)
                score += (current->color==WHITE?1:-1) * current->value * 8 / ( 1 + min(abs(3-i), abs(4-i)) + (min(abs(3-j), abs(4-j))));
        }
    }

    return score;
}

int minmax(game_board* board, enum color color_to_play, tree_t* resultTree, unsigned int depth, int alpha, int beta){

    if(depth == 0){
        ((movement_coords*)resultTree->data)->score = evaluateBoard(board);
        return ((movement_coords*)resultTree->data)->score;
    }

    // get all move allowed for the current player
    list_t* possible_move = getAllPossibleMove(board, color_to_play);

    #if 0
    puts("");
    printf("possible move at depth %i:\n", depth);
    list_node_t *node;
    list_iterator_t *it = list_iterator_new(possible_move, LIST_HEAD);
    while((node = list_iterator_next(it))){
        movement_coords* current_move = node->val;
        printf("%c%c -> %c%c\n", current_move->start_pos.posx + 'A', current_move->start_pos.posy + '1', current_move->end_pos.posx + 'A', current_move->end_pos.posy + '1');
    }
    list_iterator_destroy(it);
    puts("");
    #endif

    // if no move is possible, return the current board evaluation
    if(possible_move->len == 0){
        list_destroy(possible_move);
        ((movement_coords*)resultTree->data)->score = evaluateBoard(board) * depth;
        return ((movement_coords*)resultTree->data)->score;
    }

    int best_score = 0;
    
    if(color_to_play == WHITE){
        int max_eval = INT_MIN;
        // for each possible move
        list_node_t *node;
        list_iterator_t *it = list_iterator_new(possible_move, LIST_HEAD);
        while((node = list_iterator_next(it))){
            coords start_pos = ((movement_coords*) node->val)->start_pos;
            coords end_pos = ((movement_coords*) node->val)->end_pos;
    
            //save the piece at end_pos to be able to restore it later
            piece* tmp = board->board[end_pos.posx][end_pos.posy];

            // do the indicated movement
            movePiece(board, start_pos, end_pos);

            //create the node to insert as a son to resultTree using move_eval
            tree_t* new_child = newTree();
            new_child->data = moveCoords(start_pos, end_pos, 0);
            addChild(resultTree, new_child);

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, BLACK, new_child, depth-1, alpha, beta);
            ((movement_coords*)new_child->data)->score = child_score;
            
            max_eval = max(child_score, max_eval);

            // undo the indicated movement
            movePiece(board, end_pos, start_pos);
            board->board[end_pos.posx][end_pos.posy] = tmp;

            // alpha beta pruning
            alpha = max(alpha, max_eval);
            if(beta < alpha){
                break;
            }
        }
        list_iterator_destroy(it);
        best_score = max_eval;
    }
    else{
        int min_eval = INT_MAX;
        // for each possible move
        list_node_t *node;
        list_iterator_t *it = list_iterator_new(possible_move, LIST_HEAD);
        while((node = list_iterator_next(it))){
            coords start_pos = ((movement_coords*) node->val)->start_pos;
            coords end_pos = ((movement_coords*) node->val)->end_pos;
    
            //save the piece at end_pos to be able to restore it later
            piece* tmp = board->board[end_pos.posx][end_pos.posy];

            // do the indicated movement
            movePiece(board, start_pos, end_pos);

            //create the node to insert as a son to resultTree using move_eval
            tree_t* new_child = newTree();
            new_child->data = moveCoords(start_pos, end_pos, 0);
            addChild(resultTree, new_child);

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, WHITE, new_child, depth-1, alpha, beta);
            ((movement_coords*)new_child->data)->score = child_score;
            
            min_eval = min(child_score, min_eval);

            // undo the indicated movement
            movePiece(board, end_pos, start_pos);
            board->board[end_pos.posx][end_pos.posy] = tmp;

            // alpha beta pruning
            beta = min(beta, min_eval);
            if(beta < alpha)
                break;
        }
        list_iterator_destroy(it);
        best_score = min_eval;
    }

    // free the list of possible move
    list_destroy(possible_move);

    return best_score;
}

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth){
    srand(time(NULL));

    tree_t* eval_tree = newTree();
    eval_tree->data = malloc(sizeof(movement_coords));

    int best_score = minmax(board, board->to_play, eval_tree, evaluation_depth, INT_MIN, INT_MAX);


    //get a random move among all those with the best score

    //get all move with the best score
    list_t* best_moves = list_new();

    //get best moves from the tree
    tree_t* current_child = eval_tree->child;
    while(current_child != NULL){
        if(((movement_coords*)current_child->data)->score == best_score || getProb() > 0.9)
            list_rpush(best_moves, list_node_new(current_child->data));
        current_child = current_child->next;
    }

    //get a random move among all those with the best score
    int random_index = rand() % best_moves->len;
    movement_coords* best_move_pointer = list_at(best_moves, random_index)->val;
    movement_coords* best_move = moveCoords(best_move_pointer->start_pos, best_move_pointer->end_pos, best_move_pointer->score);

    list_destroy(best_moves); // no need to free node->val because all will be freed by freeTree()
    freeTree(eval_tree);

    return best_move;
}
