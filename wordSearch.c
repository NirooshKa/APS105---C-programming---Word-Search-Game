/*
 * File:   Lab4.c
 * Author: kammulaa
 *
 * Created on March 14, 2016, 10:26 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_WORD_LEN 2000
#define MAX_COL 20
#define MAX_ROW 20
#define MAX_FILENAME 100
#define QUIT_KEYWORD "QUIT"

/*
 * This program is a word search game!
 */

bool readPuzzle(char cGrid[][MAX_COL], int *pdPuzzleSize) {
    char puzzleName[MAX_FILENAME + 1];
    int limitRow, limitCol, limit;
    int puzzleSize;
    FILE *puzzle;
    char temp; //This is where each letter is stored.

    printf("Enter the name of the file containing the puzzle: ");

    for (limit = 0; limit < MAX_FILENAME; limit++) {
        scanf("%c", &temp);

        if (temp == '\n' || temp == '\r') {
            break;
        }
        else {
            puzzleName[limit] = temp;
        }

    }
    puzzleName[limit] = '\0';

    puzzle = fopen(puzzleName, "r");

    if (puzzle == NULL) //When the file name is not correctly stated.
    {
        printf("ERROR: File failed to open.\n");

        return false;
    }

    else{
   
    fscanf(puzzle, "%d", &puzzleSize);
    *pdPuzzleSize = puzzleSize;

    if ((*pdPuzzleSize < 4) || (*pdPuzzleSize > MAX_ROW) || (*pdPuzzleSize > MAX_COL)) //When the file name is not correctly stated.
    {
        printf("ERROR: Invalid puzzle file provided.\n");

        return false;
    }
    else {
        for (limitRow = 0; limitRow < puzzleSize; limitRow++) //Filling up the puzzle
        {
            for (limitCol = 0; limitCol < puzzleSize; limitCol++) {

                fscanf(puzzle, " %c", &cGrid[limitRow][limitCol]);

            }
        }

    }

    }
   
    fclose (puzzle);
    return true;
}

int measureWordLength(char searchWord[]) {
    int wordLength = strlen(searchWord);
    return wordLength;
}
/* Prints the puzzle given the 2D array and size of puzzle provided */

char turnToLower(char cGrid[][MAX_COL], int dPuzzleSize) {
    int limitRow, limitCol;

    for (limitRow = 0; limitRow < dPuzzleSize + 1; limitRow++) {
        for (limitCol = 0; limitCol < dPuzzleSize + 1; limitCol++) {

            cGrid[limitRow][limitCol] = tolower(cGrid[limitRow][limitCol]);

        }

    }


}

void printPuzzle(char cGrid[][MAX_COL], int dPuzzleSize) {

    int limitRow, limitCol;

    for (limitRow = 0; limitRow < dPuzzleSize; limitRow++) {
        for (limitCol = 0; limitCol < dPuzzleSize; limitCol++) {

            printf("%c", cGrid[limitRow][limitCol]);
            printf(" ");
        }
        printf("\n");
    }

    printf("\n==============================");

}
/* Queries the user for a word to search, storing it in the string provided */

bool readSearchWord(char searchWord[], int dPuzzleSize) {
    int limit;

    printf("\nEnter word to search for (or 'QUIT' to quit): ");

    for (limit = 0; limit < MAX_WORD_LEN; limit++) {
        char temp;
        scanf("%c", &temp);
        if (temp == '\n' || temp == '\r') {
            break;
        }
        else {
            searchWord[limit] = temp;
        }

    }
    searchWord[limit] = '\0';

    int wordLength = measureWordLength(searchWord);
    //flushSearch();
    if (wordLength > dPuzzleSize) {
        printf("ERROR: Invalid search word.");
        return false;
    }

    for (limit = 0; limit < wordLength; limit++) {
        if (!(isalpha(searchWord[limit]))) {
            printf("ERROR: Invalid search word.");
            return false;

        }
    }

    return true;

}

void changeToUpper(char searchWord[]) {
    int limit;
    int wordLength = measureWordLength(searchWord);

    for (limit = 0; limit < wordLength; limit++) {

        if (isupper(searchWord[limit]) == 0) {
            searchWord[limit] -= ' ';
        }

    }

}
/* Finds the search word in the 2D array provided */
void findWordInPuzzle(char cGrid[][MAX_COL], int dPuzzleSize, char searchWord[]) {
    int row, col, col1, deltaY, deltaX, index;
    int wordLength = measureWordLength(searchWord);
    bool isValidWord, wordInGrid;
    wordInGrid = false;
    char testWord[wordLength + 1]; // because null character

    // This finds words in the rightmost direction
    for (row = 0; row < dPuzzleSize; row++) //REQUIRED because 2D Array
    {
        for (col = 0; col < dPuzzleSize; col++) //REQUIRED because 2D Array
        {

            for (deltaY = -1; deltaY <= 1; deltaY++) //Deals with movement in the Y direction
            {
                for (deltaX = -1; deltaX <= 1; deltaX++) //Deals with movement in the X direction
                {

                    if ((deltaX == 0)&&(deltaY == 0)) //Implies that no movement is happening; unacceptable.
                    {
                        continue;
                    }
                    else //Movement is happening
                    {

                        isValidWord = true;
                        for (index = 0; index < wordLength; index++) //Used to move in deltaX*index direction
                        {
                            int rowIndex = row + (deltaY * index);
                            int colIndex = col + (deltaX * index);

                            // OUT OF BOUNDS Movement
                            if ((rowIndex < 0) || (colIndex < 0) || rowIndex >= dPuzzleSize || colIndex >= dPuzzleSize) {
                                isValidWord = false;
                                break;
                            }
                                // IN BOUNDS Movement
                            else {
                                testWord[index] = cGrid[rowIndex][colIndex];

                            }

                        }
                        testWord[index] = '\0';
                        if (isValidWord) { //Used to capitalize a word
                            changeToUpper(testWord);
                            if ((strcmp(testWord, searchWord)) == 0) {
                                if (!wordInGrid) {
                                    printf("The word was found in the puzzle!\n\n");
                                }
                                for (index = 0; index < wordLength; index++) // loop 2
                                {
                                    int rowIndex = row + (deltaY * index);
                                    int colIndex = col + (deltaX * index);


                                    if (isupper(cGrid[rowIndex][colIndex])) {
                                        continue;
                                    }
                                    else {
                                        cGrid[rowIndex][colIndex] -= ' ';
                                    }

                                }

                                wordInGrid = true;

                            }

                        }

                    }

                }

            }

        }

    }

    // After you searched all the words in all directions...
    if (!wordInGrid) {
        printf("The word was not found in the puzzle!\n\n");
    }

}

// ------------------------------------------------WHERE THE PROGRAM STARTS --------------------------------------------------------
int main(int argc, char** argv) {
    /*test();*/
    int i = 0, j = 0;
    char cGrid[MAX_ROW][MAX_COL] = {0};
    char searchWord[MAX_WORD_LEN + 1] = {0};
    bool status = true;
    bool isValidWord;
    int dPuzzleSize = 0;


    status = readPuzzle(cGrid, &dPuzzleSize);

    if (status) {
        // Print the grid that was read in, then query
        // the user for a string and try to find it

        turnToLower(cGrid, dPuzzleSize);
        printf("\n");
        printPuzzle(cGrid, dPuzzleSize);
        isValidWord = readSearchWord(searchWord, dPuzzleSize);
        measureWordLength(searchWord);

        // If the entered word isn't 'QUIT', continue...
        while (strcmp(searchWord, QUIT_KEYWORD)) {
            if (isValidWord) {
                printf("Searching for the word: %s ...\n", searchWord);
                changeToUpper(searchWord);
                findWordInPuzzle(cGrid, dPuzzleSize, searchWord);
                printPuzzle(cGrid, dPuzzleSize);
            }


            // Query user for string, then try to find it
            isValidWord = readSearchWord(searchWord, dPuzzleSize);
            measureWordLength(searchWord);
        }
    }

    printf("Exiting program...\n");

    return (EXIT_SUCCESS);
} //------------------------------------------------------------------------------------------------------------------------------------

