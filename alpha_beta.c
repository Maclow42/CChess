#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "alpha_beta.h"

#define getProb() ((float) (rand() % 100) / 100)

int max(int a, int b){
    return a <= b ? b : a;
}

int min(int a, int b){
    return a <= b ? a : b;
}

movement_coords* moveCoords(coords start, coords end, int score){
    /*
        * Create a movement_coords struct
        * @param start : the starting position of the piece
        * @param end : the ending position of the piece
    */
    movement_coords* result = malloc(sizeof(movement_coords));
    result->start_pos = (coords) {start.posx, start.posy};
    result->end_pos = (coords) {end.posx, end.posy};
    result->score = score;
    return result;
}

list_t* getAllPossibleMove(game_board* board, enum color color){
    /*
        * Get all possible move for a given color
        * @param board : the game board
        * @param color : the color of the player
    */
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
    /*
        * Giving a score to the current board
        * More the score is high, more the board is good for white
        * More the score is low, more the board is good for black
        * @param board : the game board
    */
    int TAKEPIECE_BONUS = 0;
    int CHESS_MALUS = 0;
    int MATE_MALUS = 9999;

    int score = 10 * (board->white_castled - board->black_castled); // castle bonus

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
            if(current != NULL){
                int center_score = (i >= 2 && i <= 5 && j >= 2 && j <= 5)? 2 : 1;
                score += (current->color==WHITE?1:-1) * (center_score * current->value);
            }
        }
    }

    return score;
}

int minmax(game_board* board, enum color color_to_play, tree_t* resultTree, unsigned int depth, int alpha, int beta){
    /*
        * Minmax algorithm using alpha beta pruning
        * @param board : the game board
        * @param color_to_play : the color of the player to play
        * @param resultTree : the tree to fill with the best move
        * @param depth : the depth of the algorithm
        * @param alpha : the alpha value for alpha beta pruning
        * @param beta : the beta value for alpha beta pruning
    */
    // get all move allowed for the current player
    list_t* possible_move = getAllPossibleMove(board, color_to_play);

    if(depth == 0){
        ((movement_coords*)resultTree->data)->score = evaluateBoard(board) + possible_move->len / 5;
        list_destroy(possible_move);
        return ((movement_coords*)resultTree->data)->score;
    }

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
            backup_state(board, start_pos, end_pos);

            // do the indicated movement and check if a piece is taken
            movePiece(board, start_pos, end_pos);

            //create the node to insert as a son to resultTree using moveCoords()
            tree_t* new_child = addChild(resultTree, moveCoords(start_pos, end_pos, 0));

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, BLACK, new_child, depth-1, alpha, beta);
            
            max_eval = max(child_score, max_eval);

            // undo the indicated movement
            restore_state(board);

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
            backup_state(board, start_pos, end_pos);

            // do the indicated movement and check if a piece is taken
            movePiece(board, start_pos, end_pos);

            //create the node to insert as a son to resultTree using moveCoords()
            tree_t* new_child = addChild(resultTree, moveCoords(start_pos, end_pos, 0));

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, WHITE, new_child, depth-1, alpha, beta);
            
            min_eval = min(child_score, min_eval);

            // undo the indicated movement
            restore_state(board);

            // alpha beta pruning
            beta = min(beta, min_eval);
            if(beta < alpha)
                break;
        }
        list_iterator_destroy(it);
        best_score = min_eval;
    }

    ((movement_coords*)resultTree->data)->score = best_score;

    // free the list of possible move
    list_destroy(possible_move);

    return best_score;
}

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth){
    /*
        * Get the best move for the current player
        * @param board : the game board
        * @param evaluation_depth : the depth of the minmax algorithm
    */
    srand(time(NULL));

    tree_t* eval_tree = newTree();
    eval_tree->data = malloc(sizeof(movement_coords));

    int best_score = minmax(board, board->to_play, eval_tree, evaluation_depth, INT_MIN, INT_MAX);

    //get all move with the best score
    list_t* best_moves = list_new();

    //get best moves from the tree
    tree_t* current_child = eval_tree->child;
    while(current_child != NULL){
        if(((movement_coords*)current_child->data)->score == best_score || (board->nb_piece>5 ? (getProb() > 0.95) : 0))
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
