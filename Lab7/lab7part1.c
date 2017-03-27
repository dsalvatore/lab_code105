//
//  main.c
//  Lab7.1
//
//  Created by YITONG on 2017-03-20.
//  Copyright © 2017 yitong. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);
bool positionInBounds(char board[][26], int n, char row, char col);
void printBoard(char board[][26], int n);
void flipTile(char board[][26], int deltaRow, int deltaCol, char colour, char userRow, char userCol);
void printMoves(char board[][26], int n, char colour);
void comMoves(char board[][26], int dimension, char comPlays, char* cRow, char* cCol);
bool availableMoves(char board[][26], int n, char colour);
int highestScore(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);
/*------------------------------------------------------------------------------------------------------*/

int main(void)
{
    int dimension = 0;
    char board[26][26];
    char userRow;
    char userCol;
    char comRow;
    char comCol;
    char comPlays;
    char userPlays = 'B';
    bool comFirst = false;
    
    printf("Enter the board dimension: ");
    scanf("%d", &dimension);
    printf("Computer plays (B/W) : ");
    scanf(" %c", &comPlays);
    
    //initiate the board
    for(int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {
            board[i][j] = 'U';
        }
    }
    //initiate the tiles
    board[dimension/2 - 1][dimension/2 - 1] = 'W';
    board[dimension/2 - 1][dimension/2] = 'B';
    board[dimension/2][dimension/2 - 1]= 'B';
    board[dimension/2][dimension/2] = 'W';
    //print the board
    printBoard(board, dimension);
    
    if (comPlays == 'B')
        comFirst = true;
    
    do
    {
        if((availableMoves(board, dimension, userPlays) == false) && (availableMoves(board, dimension, comPlays) == false))
        {
            int comTile = 0;
            int userTile = 0;
            
            for (int row = 0; row < dimension; row ++)
            {
                for(int col = 0; col < dimension; col ++)
                {
                    if(board[row][col] == comPlays)
                        comTile++;
                    else if (board[row][col] == userPlays)
                        userTile++;
                }
            }
            
            if (comTile > userTile)
                printf("%c player wins.\n", comPlays);
            else if (comTile < userTile)
                printf("%c player wins.\n", userPlays);
            else
                printf("Draw!\n");
            
            return 0;
        }
        
        if (comFirst)
        {
            userPlays = 'W';
            comFirst = false;
            comMoves(board, dimension, comPlays, &comRow, &comCol);
            printf("Computer places %c at %c%c.\n", comPlays, comRow, comCol);
            board[comRow - 'a'][comCol - 'a'] = comPlays;
            for (int deltaRow = -1; deltaRow < 2; ++deltaRow)
            {
                for (int deltaCol = -1; deltaCol < 2; ++deltaCol)
                {
                    if ((deltaRow != 0) || (deltaCol != 0))
                    {
                        if(checkLegalInDirection(board, dimension, comRow, comCol, comPlays, deltaRow, deltaCol))
                        {
                            flipTile(board, deltaRow, deltaCol, comPlays, comRow, comCol);
                        }
                    }
                }
            }
            printBoard(board, dimension);
        }
        
        if(availableMoves(board, dimension, userPlays))
        {
            printf("Enter move for colour %c (RowCol): ", userPlays);
            scanf(" %c%c", &userRow, &userCol);
            bool inBoundAndValid = false;
            if(positionInBounds(board, dimension, userRow, userCol))
            {
                //check eight directions around the userMove
                for (int deltaRow = -1; deltaRow < 2; ++deltaRow)
                {
                    for (int deltaCol = -1; deltaCol < 2; ++deltaCol)
                    {
                        if ((deltaRow != 0) || (deltaCol != 0))
                        {
                            //check if the userMove is valid
                            //if is, put the tile on the board
                            //then, flip a tile
                            if(checkLegalInDirection(board, dimension, userRow, userCol, userPlays, deltaRow, deltaCol))
                            {
                                board[userRow - 'a'][userCol - 'a'] = userPlays;
                                inBoundAndValid = true;
                                flipTile(board, deltaRow, deltaCol, userPlays, userRow, userCol);
                            }
                        }
                    }
                }
                
                printBoard(board, dimension);
                
                if(inBoundAndValid == false)
                {
                    printf("Invalid move.\n");
                    printf("%c player wins.\n", comPlays);
                    return 0;
                }
            }
            else
            {
                printf("Invalid move.\n");
                printf("%c player wins.\n", comPlays);
                return 0;
            }
        }
        else
            printf("%c player has no valid move.\n", userPlays);
        
        
        
        if (!comFirst)
        {
            if(availableMoves(board, dimension, comPlays))
            {
                comMoves(board, dimension, comPlays, &comRow, &comCol);
                printf("Computer places %c at %c%c.\n", comPlays, comRow, comCol);
                board[comRow - 'a'][comCol - 'a'] = comPlays;
                for (int deltaRow = -1; deltaRow < 2; ++deltaRow)
                {
                    for (int deltaCol = -1; deltaCol < 2; ++deltaCol)
                    {
                        if ((deltaRow != 0) || (deltaCol != 0))
                        {
                            if(checkLegalInDirection(board, dimension, comRow, comCol, comPlays, deltaRow, deltaCol))
                            {
                                flipTile(board, deltaRow, deltaCol, comPlays, comRow, comCol);
                            }
                        }
                    }
                }
                printBoard(board, dimension);
            }
            else if(availableMoves(board, dimension, userPlays))
                printf("%c player has no valid move.\n", comPlays);
        }
        
    }while(true);
    
    
    return 0;
}



/*------------------------------------------------------------------------------------------------------*/

void printBoard(char board[26][26], int n)
{
    char heading = 'a';
    printf("  ");
    for (int x = 0; x < n; x++)
    {
        printf("%c", heading);
        heading ++;
    }
    printf("\n");
    
    heading = 'a';
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if (j == 0)
            {
                printf("%c ", heading);
                heading++;
            }
            
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}
//the end of the printBoard Function
/*------------------------------------------------------------------------------------------------------*/

bool positionInBounds(char board[][26], int n, char row, char col)
{
    bool validPosition = false;
    
    if((row - 'a') >= 0 && (col - 'a') >= 0 && (row - 'a') < n && (col - 'a') < n)
        validPosition = true;
    
    return validPosition;
}
//the end of the positionInBounds Function
/*------------------------------------------------------------------------------------------------------*/

bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol)
{
    int rowNum = (row - 'a') + deltaRow;
    int colNum = (col - 'a') + deltaCol;
    char searchColour = ' ';
    bool validMove = false;
    bool terminateLoop = false;
    
    if(colour == 'W')
        searchColour = 'B';
    else if(colour == 'B')
        searchColour = 'W';
    
    if(board[rowNum][colNum] == searchColour)
    {
        while(positionInBounds(board, n, 'a' + rowNum, 'a' + colNum) && terminateLoop == false)
        {
            if(board[rowNum][colNum] == colour)
            {
                validMove = true;
                return validMove;
            }
            else if(board[rowNum][colNum] == searchColour)
            {
                rowNum = rowNum + deltaRow;
                colNum = colNum + deltaCol;
            }
            else
                terminateLoop = true;
        }
    }
    return validMove;
}
//the end of the checkLegalInDirection Function
/*------------------------------------------------------------------------------------------------------*/

void flipTile(char board[][26], int deltaRow, int deltaCol, char colour, char userRow, char userCol)
{
    int row = (userRow - 'a') + deltaRow;
    int col = (userCol - 'a') + deltaCol;
    
    while(board[row][col] != colour && board[row][col] != 'U')
    {
        board[row][col] = colour;
        row = row + deltaRow;
        col = col + deltaCol;
    }
}
//the end of the flipTile Function
/*------------------------------------------------------------------------------------------------------*/

void comMoves(char board[][26], int dimension, char comPlays, char *cRow, char *cCol)
{
    int highest = 0;
    int score = 0;
    
    for(int row = 0; row < dimension; row++)
    {
        for(int col = 0; col < dimension; col++)
        {
            if (board[row][col] == 'U')
            {
                for (int deltaRow = -1; deltaRow < 2; ++deltaRow)
                {
                    for (int deltaCol = -1; deltaCol < 2; ++deltaCol)
                    {
                        if ((deltaRow != 0) || (deltaCol != 0))
                        {
                            score = score + highestScore(board, dimension, 'a' + row, 'a' + col, comPlays, deltaRow, deltaCol);
                        }
                    }
                }
            }
            
            if(highest < score)
            {
                highest = score;
                *cRow = ('a' + row);
                *cCol = ('a' + col);
            }
            score = 0;
        }
    }
}
//the end of comMoves
/*------------------------------------------------------------------------------------------------------*/

int highestScore(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol)
{
    int rowNum = (row - 'a') + deltaRow;
    int colNum = (col - 'a') + deltaCol;
    int score = 0;
    char searchColour = ' ';
    bool validMove = false;
    bool terminateLoop = false;
    
    if(colour == 'W')
        searchColour = 'B';
    else if(colour == 'B')
        searchColour = 'W';
    
    if(positionInBounds(board, n, 'a' + rowNum, 'a' + colNum) && board[rowNum][colNum] == searchColour)
    {
        score = score + 1;
        rowNum = rowNum + deltaRow;
        colNum = colNum + deltaCol;
        while(positionInBounds(board, n, 'a' + rowNum, 'a' + colNum) && terminateLoop == false)
        {
            if(board[rowNum][colNum] == colour)
            {
                validMove = true;
                return score;
            }
            else if(board[rowNum][colNum] == searchColour)
            {
                score = score + 1;
                rowNum = rowNum + deltaRow;
                colNum = colNum + deltaCol;
            }
            else
            {
                terminateLoop = true;
                score = 0;
            }
        }
    }
    score = 0;
    return score;
}
//the end of highest score
/*------------------------------------------------------------------------------------------------------*/

bool availableMoves(char board[][26], int n, char colour)
{
    bool available = false;
    for(int row = 0; row < n; row++)
    {
        for(int col = 0; col < n; col++)
        {
            if (board[row][col] == 'U')
            {
                for (int deltaRow = -1; deltaRow < 2; ++deltaRow)
                {
                    for (int deltaCol = -1; deltaCol < 2; ++deltaCol)
                    {
                        if ((deltaRow != 0) || (deltaCol != 0))
                        {
                            if(checkLegalInDirection(board, n, 'a' + row, 'a' + col, colour, deltaRow, deltaCol))
                            {
                                available = true;
                                return available;
                            }
                        }
                    }
                }
            }
        }
    }
    return available;
}
//the end of availableMoves
/*------------------------------------------------------------------------------------------------------*/
