/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Name: Andy Stubblefield																			   //
//Date: 5/27/2011																					   //
//Project: Knight's Tour - IGN																		   //
//Description: This is a program that solves the Knight's Tour problem. The program is based off of    //
//Warnsdorff's algorithm, which looks at the possible moves available from the current location, and   //
//whichever move provides the least amount of possible next moves is the move the algorithm takes.     //
//(i.e. The vertex with the smallest degree is the correct move.) It generates a random number between //
//0 and the total number of spaces on  the board minus one to be the starting location of the knight.  //
//The 'board' is numbered from 0 - 63 (assuming a standard 8x8 board), starting with 0 at (0,0) and 63 //
//at (7,7). The board size is variable, but closed tours are not possible with certain row and column  //
//size conditions. The output of the program is the layout of the board, numbered 1 - 64 indicating    //
//the move number. The starting position is marked as '1', the second spot the knight moved to is      //
//labled '2', etc.																					   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include<time.h>
#include<iomanip>

using namespace std;

//These constant integers define the size of the board. The total number of spaces must be even for
//the alorithm to work properly.
const int NUM_ROWS = 8;
const int NUM_COLUMNS = 8;

//This struct holds a coordiante pair of a location.
struct node;
struct node
{
	int x, y;
};

//Function declarations
int getStartPos(int numSpaces);
node getNextSpot(int pos, int final[]);
bool checkSpot(node temp, int final[]);
node getSpot(int posX, int posY, int x, int y);
bool checkIfVisited(node spot, int final[]);
bool checkIfValid(node spot);
int findNumMoves(node currSpot, int moves[][2], int final[]);
void output(int final[]);

//The main function declares and initializes several variables. It also contains the for loop to itterate 
//through, which 'moves' the knight around the board.
void main()
{
	const int numSpaces = NUM_ROWS * NUM_COLUMNS;
	int currPos = getStartPos(numSpaces);
	int final[numSpaces];
	node nextSpot;

	for(int i = 0; i < numSpaces; i++)
		final[i] = 0;

	final[currPos] = 1;

	//Inside this for loop, the next location to move to is found, then 'currPos' is updated to the integer
	//value of the new location and that value is marked in 'final' with the current move number.
	for(int i = 2; i < numSpaces+1; i++)
	{
		nextSpot = getNextSpot(currPos, final);
		currPos = (nextSpot.y * NUM_COLUMNS) + nextSpot.x;
		final[currPos] = i;
	}

	output(final);

	system("pause");
}

//This function generates and returns a random number between 0 and the total number of spaces minus one.
int getStartPos(int numSpaces)
{
	srand(time(NULL));

	return rand() % numSpaces;
}

//The getNextSpot function goes to the 8 possible move locations, and updates 'finalMoves' if the location
//that it is at has less moves than the current location with the least amount of moves. The node 
//coordinates which contain the smallest number of available next moves is then returned by the function.
//Warnsdorff's algorithm states the vertex with the least degree is the correct move, which is why those 
//coordinates are returned as the next move.
node getNextSpot(int pos, int final[])
{
	int posX, posY, tempMoves;

	//This multi-dimensional array stores the 8 possible moves a knight can make. (Positive 'x' values 
	//indicate moves to the right, while positive 'y' values indicate moves downward. Orientation is 
	//looking at the board top-down, with your pieces at the bottom of the board.)
	int moves[8][2] = {{1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}};
	int finalMoves = 9, i = 0;
	node temp, smallest;

	//These two lines convert the integer label for the location on the board to proper (x,y) coordiantes.
	posX = pos % NUM_COLUMNS;
	posY = pos / NUM_COLUMNS;

	//This for loop goes to each possible move from the current location, then checks to see if the move is
	//valid. A location is valid if it is within the bounds of the board, and that location has not already
	//been visited. If the location is valid, it then determines how many valid moves are available from 
	//that location, and if there are less valid moves available than the location that is being stored in
	//'temp', temp is updated with the new coordinates, and 'finalMoves' is updated with the total number
	//of valid moves.
	for(int i = 0; i < 8; i++)
	{
		temp = getSpot(posX, posY, moves[i][0], moves[i][1]);
		if(checkSpot(temp, final))
		{
			tempMoves = findNumMoves(temp, moves, final);
			if(tempMoves < finalMoves)
			{
				finalMoves = tempMoves;
				smallest = temp;
			}
		}
	}

	//This if statement is triggered if there are no more possible valid moves from the current location, 
	//and the tour is incomplete.
	if(finalMoves == 9)
	{
		cout << "Invalid board size.\n";
		output(final);
		cout << endl;
		system("pause");
		exit(1);
	}

	return smallest;
}

//The checkSpot function validates the 'currSpot' location by returning true if the location is both within
//the bounds of the board, and has not been visited.
bool checkSpot(node currSpot, int final[])
{
	if(checkIfValid(currSpot))
		if(checkIfVisited(currSpot, final))
			return true;

	return false;
}

//This function takes a coordinate pair as two integers, and puts them in a node structure, as well as
//making the 'move'. The integer parameters 'x' and 'y' are passed in from 'moves' multi-dimensional
//aaray. The updated temp node is returned.
node getSpot(int posX, int posY, int x, int y)
{
	node temp;

	temp.x = posX + x;
	temp.y = posY + y;

	return temp;
}

//This function takes a location as a node, converts it into an integer, and checks if that integer has 
//already been visited by looking in the 'final' array. If there is a value other than the initialized 
//'0' value, the location has been visited and the function will return false.
bool checkIfVisited(node currSpot, int final[])
{
	int value = (currSpot.y * NUM_COLUMNS) + currSpot.x;

	if(final[value] != 0)
		return false;

	else 
		return true;
}

//This function takes a location and verifies that it is within the bounds of the board. If not, the
//function returns false.
bool checkIfValid(node currSpot)
{
	if((currSpot.x < 0)||(currSpot.x > NUM_COLUMNS-1)||(currSpot.y < 0)||(currSpot.y > NUM_ROWS-1))
		return false;

	else
		return true;
}

//This function takes a location, and counts how many valid moves are available from that location. 
//The total number of valid moves is returned by the function.
int findNumMoves(node currSpot, int moves[][2], int final[])
{
	int finalCount = 0;
	node temp;

	for(int i = 0; i < 8; i++)
	{
		temp = getSpot(currSpot.x, currSpot.y, moves[i][0], moves[i][1]);
		if(checkSpot(temp, final))
			finalCount++;
	}

	return finalCount;
}

//This is the function which handles the proper output of the 'final' array, arranging the values in 
//a manner that resembles a chess board.
void output(int final[])
{
	for(int i =0; i < NUM_ROWS; i++)
	{
		for(int j = 0; j < NUM_COLUMNS; j++)
			cout << setw(5) << final[(i*NUM_COLUMNS + j)] << "  ";
		cout << endl;
	}
}