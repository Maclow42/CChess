#include <limits.h>
#include "alpha_beta.h"

int CHESS_MALUS = 10e6;
int MATE_MALUS = INT_MAX;

movement_coords* moveCoords(coords start, coords end, int score){
    movement_coords* result = malloc(sizeof(movement_coords));
    result->start_pos = (coords) {start.posx, start.posy};
    result->end_pos = (coords) {end.posx, end.posy};
    result->score = score;
    return result;
}

list_t* getAllPossibleMove(game_board* board, enum color color){
    list_t* possible_move = list_new();
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
                score += (current->color==WHITE?1:-1) * current->value;
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

    // if no move is possible, return the current board evaluation
    if(possible_move->len == 0){
        list_destroy(possible_move);
        ((movement_coords*)resultTree->data)->score = evaluateBoard(board);
        return ((movement_coords*)resultTree->data)->score;
    }

    int best_score = 0;
    movement_coords* best_move = NULL;
    
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
            new_child->data = node->val;
            new_child->next = resultTree->child;
            resultTree->child = new_child;

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, BLACK, new_child, depth-1, alpha, beta);
            printf("move %c%c -> %c%c has score %i\n", start_pos.posx + 'A', start_pos.posy + '1', end_pos.posx + 'A', end_pos.posy + '1', child_score);
            ((movement_coords*)new_child->data)->score = evaluateBoard(board);
            
            if(child_score > max_eval){
                max_eval = child_score;
                best_move = node->val;
            }

            // undo the indicated movement
            movePiece(board, end_pos, start_pos);
            board->board[end_pos.posx][end_pos.posy] = tmp;

            // alpha beta pruning
            alpha = alpha > max_eval ? alpha : max_eval;
            if(beta <= alpha)
                break;
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
            new_child->data = node->val;
            new_child->next = resultTree->child;
            resultTree->child = new_child;

            // recursivly launch to the new_child and set result_Tree's score accordingly
            int child_score = minmax(board, WHITE, new_child, depth-1, alpha, beta);
            printf("move %c%c -> %c%c has score %i\n", start_pos.posx + 'A', start_pos.posy + '1', end_pos.posx + 'A', end_pos.posy + '1', child_score);
            ((movement_coords*)new_child->data)->score = evaluateBoard(board);
            
            if(child_score < min_eval){
                min_eval = child_score;
                best_move = node->val;
            }

            // undo the indicated movement
            movePiece(board, end_pos, start_pos);
            board->board[end_pos.posx][end_pos.posy] = tmp;

            // alpha beta pruning
            beta = beta < min_eval ? beta : min_eval;
            if(beta <= alpha)
                break;
        }
        list_iterator_destroy(it);
        best_score = min_eval;
    }

    // free the list of possible move (it do not free the movement_coords* inside)
    list_destroy(possible_move);

    printf("depth %i : best move is %c%c -> %c%c with score %i\n", depth, best_move->start_pos.posx + 'A', best_move->start_pos.posy + '1', best_move->end_pos.posx + 'A', best_move->end_pos.posy + '1', best_score);

    return best_score;
}

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth){
    tree_t* eval_tree = newTree();
    eval_tree->data = malloc(sizeof(movement_coords));

    int best_score = minmax(board, board->to_play, eval_tree, evaluation_depth, INT_MIN, INT_MAX);

    //get the best move from the tree
    movement_coords* best_move;
    tree_t* current_node = eval_tree->child;
    while(current_node != NULL){
        if(((movement_coords*)current_node->data)->score == best_score){
            best_move = current_node->data;
            break;
        }
        current_node = current_node->next;
    }

    printf("final best move is %c%c -> %c%c\n", best_move->start_pos.posx + 'A', best_move->start_pos.posy + '1', best_move->end_pos.posx + 'A', best_move->end_pos.posy + '1');

    return best_move;
}
