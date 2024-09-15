#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define WHITE 8
#define BLACK 16

#define WHITE_PAWN (WHITE | PAWN)
#define WHITE_KNIGHT (WHITE | KNIGHT)
#define WHITE_BISHOP (WHITE | BISHOP)
#define WHITE_ROOK (WHITE | ROOK)
#define WHITE_QUEEN (WHITE | QUEEN)
#define WHITE_KING (WHITE | KING)

#define BLACK_PAWN (BLACK | PAWN)
#define BLACK_KNIGHT (BLACK | KNIGHT)
#define BLACK_BISHOP (BLACK | BISHOP)
#define BLACK_ROOK (BLACK | ROOK)
#define BLACK_QUEEN (BLACK | QUEEN)
#define BLACK_KING (BLACK | KING)

int board[8][8] = {
    {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
    {BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN},
    {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}
};

// Function to print the chessboard
void print_board() {
    printf("  0 1 2 3 4 5 6 7\n");
    printf("  ---------------\n");
    for (int r = 0; r < 8; r++) {
        printf("%d |",  r);
        for (int c = 0; c < 8; c++) {
            int piece = board[r][c];
            switch (piece) {
              case EMPTY: printf(". "); break;
                case WHITE_PAWN: printf("♟ "); break;
                case WHITE_KNIGHT: printf("♞ "); break;
                case WHITE_BISHOP: printf("♝ "); break;
                case WHITE_ROOK: printf("♜ "); break;
                case WHITE_QUEEN: printf("♛ "); break;
                case WHITE_KING: printf("♚ "); break;
                case BLACK_PAWN: printf("♙ "); break;
                case BLACK_KNIGHT: printf("♘ "); break;
                case BLACK_BISHOP: printf("♗ "); break;
                case BLACK_ROOK: printf("♖ "); break;
                case BLACK_QUEEN: printf("♕ "); break;
                case BLACK_KING: printf("♔ "); break;
                default: printf("? "); break;
            }
        }
        printf("\n");
    }
}
               
// Function to check if a square is a valid position on the board
int is_valid_square(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

// Function to check if a square is empty
int is_square_empty(int r, int c) {
    return is_valid_square(r, c) && board[r][c] == EMPTY;
}

// Function to check if a square contains an opponent's piece
int is_opponent_piece(int r, int c, int color) {
    return is_valid_square(r, c) && (board[r][c] & color) == 0 && board[r][c] != EMPTY;
}

// Function to check if a pawn move is legal
int is_legal_pawn_move(int sr, int sc, int dr, int dc) {
    int piece = board[sr][sc];
    int direction = (piece & BLACK) ? 1 : -1;
    int start_row = (piece & BLACK) ? 1 : 6;
    int opponent_color = (piece & BLACK) ? WHITE : BLACK;

    if (dc == sc && is_square_empty(dr, dc)) {
        if (dr == sr + direction) return 1;  // Single move
        if (sr == start_row && dr == sr + 2 * direction && is_square_empty(sr + direction, sc)) return 1;  // Double move
    } else if ((dc == sc + 1 || dc == sc - 1) && dr == sr + direction && is_opponent_piece(dr, dc, opponent_color)) {
        return 1;  // Capture
    }
    return 0;
}

// Function to check if a knight move is legal
int is_legal_knight_move(int sr, int sc, int dr, int dc, int color) {
    int row_diff = abs(dr - sr);
    int col_diff = abs(dc - sc);

    if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) {
        if (is_square_empty(dr, dc) || is_opponent_piece(dr, dc, color)) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a bishop move is legal
int is_legal_bishop_move(int sr, int sc, int dr, int dc, int color) {
    if (abs(dr - sr) == abs(dc - sc)) {
        int row_step = (dr - sr) / abs(dr - sr);
        int col_step = (dc - sc) / abs(dc - sc);
        int r = sr + row_step;
        int c = sc + col_step;

        while (r != dr && c != dc) {
            if (!is_square_empty(r, c)) return 0;
            r += row_step;
            c += col_step;
        }

        if (is_square_empty(dr, dc) || is_opponent_piece(dr, dc, color)) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a rook move is legal
int is_legal_rook_move(int sr, int sc, int dr, int dc, int color) {
    if (sr == dr || sc == dc) {
        int row_step = (dr - sr) ? (dr - sr) / abs(dr - sr) : 0;
        int col_step = (dc - sc) ? (dc - sc) / abs(dc - sc) : 0;
        int r = sr + row_step;
        int c = sc + col_step;

        while (r != dr || c != dc) {
            if (!is_square_empty(r, c)) return 0;
            r+= row_step;
            c += col_step;
        }

        if (is_square_empty(dr, dc) || is_opponent_piece(dr, dc, color)) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a queen move is legal
int is_legal_queen_move(int sr, int sc, int dr, int dc, int color) {
    return is_legal_bishop_move(sr, sc, dr, dc, color) || is_legal_rook_move(sr, sc, dr, dc, color);
}

// Function to check if a king move is legal
int is_legal_king_move(int sr, int sc, int dr, int dc, int color) {
    int row_diff = abs(dr - sr);
    int col_diff = abs(dc - sc);

    if ((row_diff <= 1 && col_diff <= 1) && (is_square_empty(dr, dc) || is_opponent_piece(dr, dc, color))) {
        return 1;
    }
    return 0;
}

// Function to generate possible knight moves
void generate_knight_moves(int r, int c) {
    int color = board[r][c] & WHITE ? WHITE : BLACK;
    int dr[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    int dc[8] = {1, -1, 1, -1, 2, -2, 2, -2};

    for (int i = 0; i < 8; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (is_valid_square(nr, nc) && (is_square_empty(nr, nc) || is_opponent_piece(nr, nc, color))) {
            printf("Knight move from %d,%d to %d,%d\n", r, c, nr, nc);
            board[nr][nc] = board[r][c];  // Move the knight
            board[r][c] = EMPTY;          // Empty the source square
            return;
        }
    }
}

// Function to generate possible pawn moves
void generate_pawn_moves(int r, int c) {
    int color = board[r][c] & WHITE ? WHITE : BLACK;
    int direction = (color == WHITE) ? -1 : 1; // Direction of movement for pawns
    int start_row = (color == WHITE) ? 6 : 1; // Starting row for pawns
    int promote_row = (color == WHITE) ? 0 : 7; // Promotion row for pawns

    // Forward move
    int nr = r + direction;
    if (is_valid_square(nr, c) && is_square_empty(nr, c)) {
        printf("Pawn move from %d,%d to %d,%d\n", r, c, nr, c);
        board[nr][c] = board[r][c];
        board[r][c] = EMPTY;

        // Double move from starting row
        if (r == start_row) {
            nr = r + 2 * direction;
            if (is_valid_square(nr, c) && is_square_empty(nr, c)) {
                printf("Pawn move from %d,%d to %d,%d\n", r, c, nr, c);
                board[nr][c] = board[r][c];
                board[r][c] = EMPTY;
            }
        }

        // Promotion
        if (nr == promote_row) {
            int choice;
            printf("Enter promotion choice for pawn at %d,%d (1 - Knight, 2 - Bishop, 3 - Rook, 4 - Queen): ", nr, c);
            scanf("%d", &choice);
            switch(choice) {
                case 1:
                    board[nr][c] = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
                    break;
                case 2:
                    board[nr][c] = (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
                    break;
                case 3:
                    board[nr][c] = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
                    break;
                case 4:
                    board[nr][c] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                    break;
                default:
                    printf("Invalid choice. Defaulting to Queen promotion.\n");
                    board[nr][c] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
            }
            printf("Pawn promoted at %d,%d\n", nr, c);
        }
    }

    // Captures
    int capture_cols[2] = {c - 1, c + 1};
    for (int i = 0; i < 2; i++) {
        int nc = capture_cols[i];
        if (is_valid_square(nr, nc) && is_opponent_piece(nr, nc, color)) {
            printf("Pawn capture from %d,%d to %d,%d\n", r, c, nr, nc);
            board[nr][nc] = board[r][c];
            board[r][c] = EMPTY;

            // Promotion on capture
            if (nr == promote_row) {
                int choice;
                printf("Enter promotion choice for pawn at %d,%d (1 - Knight, 2 - Bishop, 3 - Rook, 4 - Queen): ", nr, nc);
                scanf("%d", &choice);
                switch(choice) {
                    case 1:
                        board[nr][nc] = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
                        break;
                    case 2:
                        board[nr][nc] = (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
                        break;
                    case 3:
                        board[nr][nc] = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
                        break;
                    case 4:
                        board[nr][nc] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                        break;
                    default:
                        printf("Invalid choice. Defaulting to Queen promotion.\n");
                        board[nr][nc] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                }
                printf("Pawn promoted at %d,%d\n", nr, nc);
            }
        }
    }
}

// Function for the user's move
// Function for the user's move
void make_user_move() {
    int sr, sc, dr, dc;
    printf("Enter your move (source_row source_col dest_row dest_col): ");
    scanf("%d %d %d %d", &sr, &sc, &dr, &dc);

    if (is_valid_square(sr, sc) && is_valid_square(dr, dc)) {
        int piece = board[sr][sc];
        int color = piece & WHITE ? WHITE : BLACK;

        int is_legal = 0;
        switch (piece & 0x7) { // Mask to get piece type
            case PAWN:
                is_legal = is_legal_pawn_move(sr, sc, dr, dc);
                break;
            case KNIGHT:
                is_legal = is_legal_knight_move(sr, sc, dr, dc, color);
                break;
            case BISHOP:
                is_legal = is_legal_bishop_move(sr, sc, dr, dc, color);
                break;
            case ROOK:
                is_legal = is_legal_rook_move(sr, sc, dr, dc, color);
                break;
            case QUEEN:
                is_legal = is_legal_queen_move(sr, sc, dr, dc, color);
                break;
            case KING:
                is_legal = is_legal_king_move(sr, sc, dr, dc, color);
                break;
            default:
                printf("Invalid piece.\n");
                return;
        }

        if (is_legal) {
            board[dr][dc] = board[sr][sc];
            board[sr][sc] = EMPTY;

            // Pawn promotion
            if ((piece == WHITE_PAWN && dr == 0) || (piece == BLACK_PAWN && dr == 7)) {
                int choice;
                printf("Enter promotion choice for pawn at %d,%d (1 - Knight, 2 - Bishop, 3 - Rook, 4 - Queen): ", dr, dc);
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        board[dr][dc] = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
                        break;
                    case 2:
                        board[dr][dc] = (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
                        break;
                    case 3:
                        board[dr][dc] = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
                        break;
                    case 4:
                        board[dr][dc] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                        break;
                    default:
                        printf("Invalid choice. Defaulting to Queen promotion.\n");
                        board[dr][dc] = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                }
                printf("Pawn promoted at %d,%d\n", dr, dc);
            }

            printf("Move applied.\n");
        } else {
            printf("Illegal move.\n");
        }
    } else {
        printf("Invalid square.\n");
    }
}
// Function for the AI's move
void make_ai_move() {
    // Placeholder implementation, just generates knight moves for the first knight found.
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((board[r][c] & 0x7) == KNIGHT && (board[r][c] & BLACK)) {
                generate_knight_moves(r, c);
                return;
            }
        }
    }
}

// Function to check if the specified king is in check
int is_in_check(int king_row, int king_col, int color) {
    // Iterate over the board to find opponent's pieces
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((board[r][c] & color) == 0 && board[r][c] != EMPTY) { // Opponent's piece
                switch (board[r][c] & 0x7) {
                    case PAWN:
                        if (is_legal_pawn_move(r, c, king_row, king_col)) {
                            return 1; // King is in check
                        }
                        break;
                    case KNIGHT:
                        if (is_legal_knight_move(r, c, king_row, king_col, color)) {
                            return 1; // King is in check
                        }
                        break;
                    case BISHOP:
                        if (is_legal_bishop_move(r, c, king_row, king_col, color)) {
                            return 1; // King is in check
                        }
                        break;
                    case ROOK:
                        if (is_legal_rook_move(r, c, king_row, king_col, color)) {
                            return 1; // King is in check
                        }
                        break;
                    case QUEEN:
                        if (is_legal_queen_move(r, c, king_row, king_col, color)) {
                            return 1; // King is in check
                        }
                        break;
                    case KING:
                        if (is_legal_king_move(r, c, king_row, king_col, color)) {
                            return 1; // King is in check
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return 0; // King is not in check
}

// Function to check if the specified color is in checkmate
int is_checkmate(int color) {
    // Iterate over the board to find all pieces of the specified color
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((board[r][c] & color) != 0) { // Piece of the specified color
                int piece = board[r][c] & 0x7; // Get the piece type
                // Generate all possible moves for the piece
                switch (piece) {
                    case PAWN:
                        // Iterate over all possible pawn moves
                        for (int dr = -1; dr <= 1; dr++) {
                            for (int dc = -1; dc <= 1; dc++) {
                                if (is_legal_pawn_move(r, c, r + dr, c + dc)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in checkmate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case KNIGHT:
                        // Iterate over all possible knight moves
                        for (int dr = -2; dr <= 2; dr++) {
                            for (int dc = -2; dc <= 2; dc++) {
                                if (abs(dr) + abs(dc) == 3 && is_legal_knight_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in checkmate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case BISHOP:
                        // Iterate over all possible bishop moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if (abs(dr) == abs(dc) && is_legal_bishop_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that theboard[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case ROOK:
                        // Iterate over all possible rook moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if ((abs(dr) == 0 || abs(dc) == 0) && is_legal_rook_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in checkmate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case QUEEN:
                        // Iterate over all possible queen moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if ((abs(dr) == 0 || abs(dc) == 0 || abs(dr) == abs(dc)) &&
                                    is_legal_queen_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in checkmate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case KING:
                        // Iterate over all possible king moves
                        for (int dr = -1; dr <= 1; dr++) {
                            for (int dc = -1; dc <= 1; dc++) {
                                if ((dr != 0 || dc != 0) && is_legal_king_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in checkmate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // King is not in checkmate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return 1; // King is in checkmate
}

// Function to check if the specified color is in stalemate
int is_stalemate(int color) {
    // Iterate over the board to find all pieces of the specified color
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((board[r][c] & color) != 0) { // Piece of the specified color
                int piece = board[r][c] & 0x7; // Get the piece type
                // Generate all possible moves for the piece
                switch (piece) {
                    case PAWN:
                        // Iterate over all possible pawn moves
                        for (int dr = -1; dr <= 1; dr++) {
                            for (int dc = -1; dc <= 1; dc++) {
                                if (is_legal_pawn_move(r, c, r + dr, c + dc)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case KNIGHT:
                        // Iterate over all possible knight moves
                        for (int dr = -2; dr <= 2; dr++) {
                            for (int dc = -2; dc <= 2; dc++) {
                                if (abs(dr) + abs(dc) == 3 && is_legal_knight_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case BISHOP:
                        // Iterate over all possible bishop moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if (abs(dr) == abs(dc) && is_legal_bishop_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case ROOK:
                        // Iterate over all possible rook moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if ((abs(dr) == 0 || abs(dc) == 0) && is_legal_rook_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case QUEEN:
                        // Iterate over all possible queen moves
                        for (int dr = -7; dr <= 7; dr++) {
                            for (int dc = -7; dc <= 7; dc++) {
                                if ((abs(dr) == 0 || abs(dc) == 0 || abs(dr) == abs(dc)) &&
                                    is_legal_queen_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    case KING:
                        // Iterate over all possible king moves
                        for (int dr = -1; dr <= 1; dr++) {
                            for (int dc = -1; dc <= 1; dc++) {
                                if ((dr != 0 || dc != 0) && is_legal_king_move(r, c, r + dr, c + dc, color)) {
                                    // Simulate the move
                                    int temp = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = board[r][c];
                                    board[r][c] = EMPTY;
                                    // Check if the king is still in check after the move
                                    if (!is_in_check(r, c, color)) {
                                        // Undo the move and return that the king is not in stalemate
                                        board[r][c] = board[r + dr][c + dc];
                                        board[r + dr][c + dc] = temp;
                                        return 0; // Not in stalemate
                                    }
                                    // Undo the move
                                    board[r][c] = board[r + dr][c + dc];
                                    board[r + dr][c + dc] = temp;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return 1; // Stalemate
}

// Function to check if the specified color is in draw by insufficient material
int is_draw_by_insufficient_material(int color) {
    // Count the number of pieces remaining for the specified color
    int num_pieces = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((board[r][c] & color) != 0) { // Piece of the specified color
                num_pieces++;
            }
        }
    }

    // Draw by insufficient material occurs when:
    // - Both players have only a king left
    // - Both players have only a king and a knight left
    // - Both players have only a king and a bishop left (of the same color)
    // - Both players have only kings and bishops left (of the same color)
    if (num_pieces == 1 || (num_pieces == 2 && ((color == WHITE && num_pieces == 2 && is_draw_by_insufficient_material(BLACK)) ||
        (color == BLACK && num_pieces == 2 && is_draw_by_insufficient_material(WHITE))))) {
        return 1; // Draw by insufficient material
    }

    return 0; // Not a draw by insufficient material
}
// Main function
int main() {
    while (1) {
        print_board();
        printf("Your move (White):\n");
        make_user_move();
        print_board();
        printf("AI's move(Black):\n");
        make_ai_move();
    }
    return 0;
}