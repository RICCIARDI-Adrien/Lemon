/** @file Game_Of_Life.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Vertical size of the world. */
#define WORLD_ROWS_COUNT SCREEN_ROWS_COUNT
/** Horizontal size of the world. */
#define WORLD_COLUMNS_COUNT SCREEN_COLUMNS_COUNT

/** The cell is dead. */
#define CELL_STATE_DEAD 0
/** The cell is alive. */
#define CELL_STATE_ALIVE 1

/** The foreground and background colors used to display an alive cell. */
#define CELL_DISPLAYING_COLOR SCREEN_MAKE_COLOR(SCREEN_COLOR_GREEN, SCREEN_COLOR_BLACK)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static char Previous_World[WORLD_ROWS_COUNT][WORLD_COLUMNS_COUNT], Current_World[WORLD_ROWS_COUNT][WORLD_COLUMNS_COUNT], Next_World[WORLD_ROWS_COUNT][WORLD_COLUMNS_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Create a new random world. */
static void CreateNewWorld(void)
{
	int Row, Column;
	char Cell_State;

	for (Row = 0; Row < WORLD_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < WORLD_COLUMNS_COUNT; Column++)
		{
			if (RandomGenerateNumber() % 7 == 2) Cell_State = CELL_STATE_ALIVE;
			else Cell_State = CELL_STATE_DEAD;

			Current_World[Row][Column] = Cell_State;
		}
	}
	
	// Reset previous world to avoid erroneous comparisons
	MemorySetAreaValue(Previous_World, sizeof(Previous_World), 0);
}

/** Display the current world. */
static void DisplayWorld(void)
{
	unsigned char Video_Buffer[SCREEN_ROWS_COUNT][SCREEN_COLUMNS_COUNT * 2];
	char Character;
	int Row, Column, Video_Column;

	// Create the raw video buffer representing the world state (yes, this is not optimized)
	for (Row = 0; Row < SCREEN_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < SCREEN_COLUMNS_COUNT; Column++)
		{
			// Choose the right character to display according to cell state
			if (Current_World[Row][Column] == CELL_STATE_ALIVE) Character = 'O';
			else Character = ' ';

			// "Display" character
			Video_Column = Column << 1;
			Video_Buffer[Row][Video_Column] = Character;

			// Set character color
			Video_Buffer[Row][Video_Column + 1] = CELL_DISPLAYING_COLOR;
		}
	}

	ScreenDisplayBuffer((unsigned char *) Video_Buffer);
}

/** Retrieve the number of neighbor cells a specified cell has.
 * @param Cell_Row The cell to count neighbors row coordinate.
 * @param Cell_Column The cell to count neighbors column coordinate.
 * @return The number of cell's neighbors.
 */
static int GetAliveCellNeighborsCount(int Cell_Row, int Cell_Column)
{
	int Neighbors_Count = 0;

	// Nord cells
	if (Cell_Row > 0)
	{
		// Nord west cell
		if (Cell_Column > 0) Neighbors_Count += Current_World[Cell_Row - 1][Cell_Column - 1];
		// Nord cell
		Neighbors_Count += Current_World[Cell_Row - 1][Cell_Column];
		// Nord east cell
		if (Cell_Column < WORLD_COLUMNS_COUNT - 1) Neighbors_Count += Current_World[Cell_Row - 1][Cell_Column + 1];
	}

	// Same row cells
	// West cell
	if (Cell_Column > 0) Neighbors_Count += Current_World[Cell_Row][Cell_Column - 1];
	// East cell
	if (Cell_Column < WORLD_COLUMNS_COUNT - 1) Neighbors_Count += Current_World[Cell_Row][Cell_Column + 1];

	// South cells
	if (Cell_Row < WORLD_ROWS_COUNT - 1)
	{
		// South west cell
		if (Cell_Column > 0) Neighbors_Count += Current_World[Cell_Row + 1][Cell_Column - 1];
		// South cell
		Neighbors_Count += Current_World[Cell_Row + 1][Cell_Column];
		// South east cell
		if (Cell_Column < WORLD_COLUMNS_COUNT - 1) Neighbors_Count += Current_World[Cell_Row + 1][Cell_Column + 1];
	}

	return Neighbors_Count;
}

/** Compute the next world cells generation. */
static void ComputeNextWorldGeneration(void)
{
	int Row, Column, Alive_Cell_Neighbors_Count, Is_New_Generation_Different = 0, Is_Old_Generation_Different = 0;
	char Cell_State;
	
	for (Row = 0; Row < WORLD_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < WORLD_COLUMNS_COUNT; Column++)
		{
			// Count cell neighbors
			Alive_Cell_Neighbors_Count = GetAliveCellNeighborsCount(Row, Column);

			// Decide whether the cell will be dead or alive
			if (Alive_Cell_Neighbors_Count == 3) Cell_State = CELL_STATE_ALIVE;
			else if ((Current_World[Row][Column] == CELL_STATE_ALIVE) && (Alive_Cell_Neighbors_Count == 2)) Cell_State = CELL_STATE_ALIVE;
			else Cell_State = CELL_STATE_DEAD;
			
			// Check if the cell state has changed or not (if none of the cell states changed, the world is stable)
			if (Previous_World[Row][Column] != Cell_State) Is_Old_Generation_Different = 1;
			if (Next_World[Row][Column] != Cell_State) Is_New_Generation_Different = 1;

			// Update the cells
			Previous_World[Row][Column] = Current_World[Row][Column]; // Keep N-1 generation
			Next_World[Row][Column] = Cell_State; // Create N+1 generation
		}
	}
	
	// Generate a new world if the current one is stable
	if ((!Is_Old_Generation_Different) || (!Is_New_Generation_Different)) CreateNewWorld();
	else MemoryCopyArea(Next_World, Current_World, sizeof(Current_World)); // Update current world
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void GameOfLife(void)
{
	int Wait_Time = 2; // 100 ms

	RandomInitialize();
	CreateNewWorld();
	DisplayWorld();

	while (1)
	{
		ComputeNextWorldGeneration();
		DisplayWorld();

		// Did the user hit a key
		if (KeyboardIsKeyAvailable())
		{
			switch (KeyboardReadCharacter())
			{
				// Exit program
				case KEYBOARD_KEY_CODE_ESCAPE:
					return;
					
				// Display the world faster
				case 'f':
				case 'F':
					if (Wait_Time > 0) Wait_Time--;
					break;
					
				// Display the world slower
				case 's':
				case 'S':
					if (Wait_Time < 60) Wait_Time++; // Maximum displaying time is 3 seconds
					break;

				// Generate a new world
				case 'n':
				case 'N':
					CreateNewWorld();
					break;
			}
		}

		// Wait for 50ms steps
		if (Wait_Time > 0) SystemWait(Wait_Time * 50); // Avoid a system call if there is no need to wait
	}
}
