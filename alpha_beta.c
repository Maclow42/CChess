#include <limits.h>
#include <time.h>
#include "alpha_beta.h"


#define DB printf("%s:%d\n", __FILE__, __LINE__);
#define getProb() ((float) (rand() % 100) / 100)

int max(int a, int b){
    return a <= b ? b : a;
}

int min(int a, int b){
    return a <= b ? a : b;
}

alpha_beta_predictor* new_Alpha_Beta_Predictor(game_board* board, unsigned int evaluation_depth){
    /*
        * Create a new alpha beta predictor
        * @param board : the game board
        * @param evaluation_depth : the depth of the minmax algorithm
    */
    alpha_beta_predictor* predictor = malloc(sizeof(alpha_beta_predictor));
    predictor->board = board;
    predictor->evaluation_depth = evaluation_depth;
    predictor->treeEvaluation = new_gtree(malloc(sizeof(movement_coords)), 0);

    return predictor;
}

void free_Alpha_Beta_Predictor(alpha_beta_predictor* predictor){
    /*
        * Free an alpha beta predictor
        * @param predictor : the alpha beta predictor
    */
    free_gtree(predictor->treeEvaluation);
    free(predictor);
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
    int MATE_MALUS = 9999;

    int score = 10 * (board->white_castled - board->black_castled); // castle bonus

    // ajust score according to game status
    game_status status = getGameStatus(board);
    switch(status){
        case WHITE_MATE:
            return -MATE_MALUS;
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

int minmax(game_board* board, enum color color_to_play, gtree_t* resultTree, unsigned int depth, int alpha, int beta){
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

    // resize the resultTree to the number of possible move
    gtree_resize_nb_children(resultTree, possible_move->len);

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
            gtree_t* new_child = new_gtree(moveCoords(start_pos, end_pos, 0), 0);
            gtree_add_child(resultTree, new_child);

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
            gtree_t* new_child = new_gtree(moveCoords(start_pos, end_pos, 0), 0);
            gtree_add_child(resultTree, new_child);

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

movement_coords* getBestMove(alpha_beta_predictor *predictor){
    /*
        * Get the best move for the current player
        * @param board : the game board
        * @param evaluation_depth : the depth of the minmax algorithm
    */
    srand(time(NULL));

    predictor->treeEvaluation = new_gtree(malloc(sizeof(movement_coords)), 0);

    int best_score = minmax(predictor->board, predictor->board->to_play, predictor->treeEvaluation, predictor->evaluation_depth, INT_MIN, INT_MAX);

    //get all move with the best score
    list_t* best_moves = list_new();

    //get best moves from the tree
    for(unsigned int i = 0; i < predictor->treeEvaluation->nb_children; i++){
        if(((movement_coords*)predictor->treeEvaluation->children[i]->data)->score == best_score)
            list_rpush(best_moves, list_node_new(predictor->treeEvaluation->children[i]->data));
    }

    //get a random move among all those with the best score
    int random_index = rand() % best_moves->len;
    movement_coords* best_move_pointer = list_at(best_moves, random_index)->val;
    movement_coords* best_move = moveCoords(best_move_pointer->start_pos, best_move_pointer->end_pos, best_move_pointer->score);

    list_destroy(best_moves); // no need to free node->val because all will be freed by freeTree()

    return best_move;
}
