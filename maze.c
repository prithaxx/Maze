//-----------------------------------------
// NAME: Pritha Das 
// STUDENT NUMBER: 7924180
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Dr. Mehdi Niknam
// ASSIGNMENT: assignment #2, QUESTION: question #1
// 
// REMARKS: To find an escape route for the mouse inside a maze
// using backtracking algorithm
//------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES

const int MAX_LENGTH = 50;
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 50

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE Cell_Node;
struct CELL_NODE
{
  Cell     cell;
  Cell_Node *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
int k=0;
Cell goal_cell;
Cell start_cell;
Cell current_cell;
//-------------------------------------------------------------------------------------

Cell_Node *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int maze_rows;
int maze_cols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equal_cells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell make_cell(const int row, const int col);
// returns true if the cell is within our maze
Boolean valid_cell(const Cell the_cell);
char char_at(char str[], int index);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean no_more_cells();
// returns the next cell to try for a path out of the maze
Cell next_Cell();
// introduces a new cell to try
void add_cell(const Cell cell);

void print_maze();
void load_maze();

// returns true if there's a solution to the maze
Boolean solve_maze();

// our invariant checker
void check_state(const int row, const int col);

//-------------------------------------------------------------------------------------

// ----------------------------------------------------
// main
//
// PURPOSE: Tests all the other methods with test cases.
// RETURN TYPE: int				
// returns 0
//-----------------------------------------------------
int main( int argc, char *argv[] )
{
    load_maze();
	
    if ( solve_maze() )
      printf( "The mouse is free!!!!\n" );
    else
      printf( "The mouse is trapped!!!!\n" );
    
    printf( "\nEnd of processing\n" );
    
	return EXIT_SUCCESS;
}

//-------------------------------------------------------------------------------------
// FUNCTIONS

//////////////////////////////////////////////
// Cell routines

// --------------------------------------------------
// equal_cells
//
// PURPOSE: returns true if the cells are at the same position in our maze
// INPUT PARAMETERS: @param cell1	: a constant that accepts the first cell
//					 @param cell2	: a constant that accepts the second cell
// OUTPUT PARAMETERS: @param result	: a boolean variable that returns true if the two cells are equal
// RETURN TYPE: Boolean
// --------------------------------------------------
Boolean equal_cells(const Cell cell1, const Cell cell2)
{
	Boolean result = false;
	
	check_state(cell1.row, cell1.column);
	check_state(cell2.row, cell2.column);
	
	if (cell1.row >= 0 && cell1.column >= 0 && cell2.row >= 0 && cell2.column >= 0 && cell1.row <= MAX_DIMENSION && cell1.column <= MAX_DIMENSION && cell2.row <= MAX_DIMENSION && cell2.column <= MAX_DIMENSION)
	{
		if((cell1.row == cell2.row) && (cell1.column == cell2.column))
			result = true;
		else
			result = false;
	}
	return result;
}

// --------------------------------------------------
// make_cell
//
// PURPOSE: returns a new cell object
// INPUT PARAMETERS: @param row	: a constant that accepts the number of rows
//					 @param col	: a constant that accepts the number of columns
// OUTPUT PARAMETERS: @param cell	: with the help of CELL struct makes and returns a cell
// RETURN TYPE: Cell
// --------------------------------------------------
Cell make_cell(const int row, const int col)
{	
	check_state(row, col);
	
	Cell cell;
	cell.row = row;
	cell.column = col;

	return cell;
}

// --------------------------------------------------
// valid_cell
//
// PURPOSE: returns true if the cell is within our maze
// INPUT PARAMETERS: @param the_cell	: a constant that accepts a cell
// RETURN TYPE: false
// --------------------------------------------------
Boolean valid_cell(const Cell the_cell)
{
	check_state(the_cell.row, the_cell.column);
	
	Cell temp_cell;
	for(int i=0; i<maze_rows; i++)
	{
		for(int j=0; j<maze_cols; j++)
		{
			temp_cell = make_cell(i,j);
			if(equal_cells(temp_cell, the_cell) == 1)
				return true;
		}
	}
	return false;
}
//////////////////////////////////////////////


//////////////////////////////////////////////
// List routines

// --------------------------------------------------
// no_more_cell
//
// PURPOSE: returns true if there are no more cells to try
// RETURN TYPE: Boolean
// --------------------------------------------------
Boolean no_more_cells()
{
	return top == NULL;
}

// --------------------------------------------------
// next_cell
//
// PURPOSE: returns the next cell to try for a path out of the maze
// RETURN TYPE: Cell
// -------------------------------------------------- 
Cell next_Cell()
{
	return top->cell;
}

// --------------------------------------------------
// add_cell
//
// PURPOSE: introduces a new cell to try
// INPUT PARAMETERS: @param cell	: a constant that accepts a cell
// ----------------------------------------------------
void add_cell(const Cell cell)
{	
	Cell_Node *node = malloc(sizeof(Cell_Node));
	node->cell = cell;
	node->next = top;
	top = node;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
// Maze routines


// --------------------------------------------------
// load_maze
//
// PURPOSE: reads a maze from standard input
// --------------------------------------------------
void load_maze()
{
	scanf("%d %d\n", &maze_rows, &maze_cols);
	
	check_state(maze_rows, maze_cols);
	
	if(maze_rows >= 0 && maze_cols >= 0 && maze_rows <= MAX_DIMENSION && maze_cols <= MAX_DIMENSION)
		maze[maze_rows][maze_cols] = 0;
	
	char line[MAX_LENGTH];
	
	for(int i=0; i<maze_rows; i++)
    {
        fgets(line, MAX_LENGTH, stdin);
        for(int j=0; j<maze_cols; j++)
        {
            maze[i][j] = char_at(line, k);
            k=k+2;
        }
        k=0;
    }
	
	// setting the initial position of goal_cell and start_cell
	for(int i=0; i<maze_rows; i++)
	{
		for(int j=0; j<maze_cols; j++)
		{
			if(maze[i][j] == MOUSE)
			{
				mouse.row = i;
				mouse.column = j;
			}
			else if(maze[i][j] == EXIT)
			{
				escape.row = i;
				escape.column = j;
			}
		}
	}
	goal_cell = escape;
	start_cell = mouse;
	current_cell = start_cell;
}


// --------------------------------------------------
// print_maze
//
// PURPOSE: prints the maze
// --------------------------------------------------
void print_maze()
{
	check_state(maze_rows, maze_cols);
	
	if(maze_rows >= 0 && maze_cols >= 0 && maze_rows <= MAX_DIMENSION && maze_cols <= MAX_DIMENSION){
		for(int i=0; i<maze_rows; i++)
		{
			for(int j=0; j<maze_cols; j++)
			{
				printf("%c ",maze[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
}


// --------------------------------------------------
// char_at
//
// PURPOSE: returns a characer from a specific index of a char array
// INPUT PARAMETERS: @param str[]	: accepts a char array
//					 @param index	: accepts a position
// RETURN TYPE: char
// --------------------------------------------------
char char_at(char str[], int index)
{
	assert (str != NULL);
	assert (str[index] != ' ');
	assert (index >= 0 && index < MAX_LENGTH);
	return str[index];
}

// --------------------------------------------------
// solve_maze
//
// PURPOSE: returns true if there is a solution to the maze
// INPUT PARAMETERS: @param str[]	: accepts a char array
//					 @param index	: accepts a position
// RETURN TYPE: Boolean
// --------------------------------------------------
Boolean solve_maze()
{	
	Cell cell;
	check_state(maze_rows, maze_cols);

	while(equal_cells(goal_cell, current_cell) == 0)
	{
		maze[current_cell.row][current_cell.column] = VISITED;
		print_maze();
		
		if((maze[current_cell.row+1][current_cell.column] == SPACE) ||
		maze[current_cell.row+1][current_cell.column] == EXIT)
		{
			cell = make_cell(current_cell.row+1, current_cell.column);
			if(valid_cell(cell))
				add_cell(cell);
		}
		
		if((maze[current_cell.row-1][current_cell.column] == SPACE) ||
		maze[current_cell.row-1][current_cell.column] == EXIT)
		{
			cell = make_cell(current_cell.row-1, current_cell.column);
			if(valid_cell(cell))
				add_cell(cell);
		}
		
		if((maze[current_cell.row][current_cell.column+1] == SPACE) ||
		maze[current_cell.row][current_cell.column+1] == EXIT)
		{
			cell = make_cell(current_cell.row, current_cell.column+1);
			if(valid_cell(cell))
				add_cell(cell);
		}
		
		if((maze[current_cell.row][current_cell.column-1] == SPACE) ||
		maze[current_cell.row][current_cell.column-1] == EXIT)
		{
			cell = make_cell(current_cell.row, current_cell.column-1);
			if(valid_cell(cell))
				add_cell(cell);
		}
		
		if(no_more_cells())
		{
			return false;
		}
		else
		{
			current_cell = next_Cell();
			top = top->next;
		}
	}
	return true;
}
//////////////////////////////////////////////

void check_state(const int row, const int col)
{
	assert(row >= 0 && col >= 0);
	assert(row <= MAX_DIMENSION && col <= MAX_DIMENSION);
}