#include <SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
/*
Author:HaydenD100 
GitHub: https://github.com/HaydenD100
A* search algorithm implementation v1.0.0

A* is a path finding algorithm that finds the shortest path to a destination using a cost function to move in the right direction. For more information on A*: https://en.wikipedia.org/wiki/A*_search_algorithm#:~:text=A*%20is%20an%20informed%20search,shortest%20time%2C%20etc

Controls:
Using the left mouse button you can make a tile solid.
Press any button on the keyboard to start finding the path

*/

//Window size
const int WIDTH = 800, HEIGHT = 600;

//Grid Structure this holds all the information on each point
struct GridPoint {
	//Cordinates 
	int x;
	int y;
	//Bool for if the point is solid etc a wall 0 for false 1 for true, solid points are shown in gray
	int solid;
	//Bool for if the algorithm has picked this point as the best point this is shown in red
	int pickedPoint;
	//Bool for if this point has already been calculated, this is used so there arent repated calculated points in the calculated vectors, tiles that have been calculated but not picked are shown in blue
	int calculated;
	//Bool to check if this tile has been picked as the shortest path this is shown as dark red
	int truePath = 0;
	
	//H cost is calculated by how far this point is from the end point
	int hCost;
	//G cost is calculated by how far this point is from the start point
	int gCost;
	//F = gCost + hCost
	int fCost;
	//the last node that was chosen as the best node
	GridPoint* parent;


};

//Starting and ending point
const int startingX = 20; 
const int startingY = 20;
const int endX = 60; 
const int endY = 50;

//2D Grid of all the points
struct GridPoint grid[80][60];
//This vector stores all Calculated points
std::vector<GridPoint*> CalculatedPoints;

//This function draws all the points in grid and assigns each tile to a certain colour depending on if it is a picked point, solid point, calculated point or end/starting point
void Draw(SDL_Renderer* renderer) {
	//sets the background colour and then clears the window 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			SDL_Rect rect = { x * 10,y * 10,9,9 };

			
			if(x == startingX && y == startingY)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else if (x == endX && y == endY) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			}
			else if (grid[x][y].truePath == 1) {
				SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
			}
			else if (grid[x][y].pickedPoint == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (grid[x][y].solid == 1) {
				SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
			}
			else if (grid[x][y].calculated == 1 && grid[x][y].pickedPoint == 0) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	//updates the screen with the new drawings in the renderer 
	SDL_RenderPresent(renderer);

}

//This function calculates the H, G and F cost of each poiunt
void CalculateCost(struct GridPoint *point,int x,int y) {
	if (point->solid == 1)
		return;
	
	point->gCost = std::sqrt((startingX - point->x) * (startingX - point->x) + (startingY - point->y) * (startingY - point->y)) * 10;
	point->hCost = std::sqrt((endX - point->x) * (endX - point->x) + (endY - point->y) * (endY - point->y)) * 10;
	point->fCost = point->hCost + point->gCost;
	//This changes the parent point for any calculated node to its most recently picked neighbour.
	if (point->pickedPoint == 0) {
		point->parent = &grid[x][y];
	}
	//If the point is newly calculated add the point to the CalculatedPoints vector
	if (point->pickedPoint == 0 && point->calculated == 0)
	{
		point->calculated = 1;
		CalculatedPoints.push_back(point);
	}
}

//This function runs after the end point has been found and finds the shortest path between the start and end by going through all the parentpoints starting from the end point
//This function can be modified to store the parent points in a vector giving a vector of all points someone would have to follow to go from the start to the end
void FindPath() {
	GridPoint* currentTIle = &grid[endX][endY];

	while (currentTIle->x != startingX || currentTIle->y != startingY) {
		currentTIle->truePath = 1;
		currentTIle = currentTIle->parent;
	}

}



//This function calculates all neighbouring points of the inputed point (x,y) and then selects the next point by checking the CalculatedPoints vector and choosing the point with the lowest F value, if there are multiple lowest F values then it picks the one with the Lowest H value
struct GridPoint* NextPoint(int x, int y) {

	//Calculate all the points next to current point
	if (x > 1) {
		CalculateCost(&grid[x - 1][y],x,y);
		
	}
	if (x < 79) {
		CalculateCost(&grid[x + 1][y], x, y);
	}
	if (x > 1 && y > 1) {
		CalculateCost(&grid[x - 1][y - 1], x, y);
	}
	if (x > 1 && y > 59) {
		CalculateCost(&grid[x - 1][y + 1], x, y);
	}
	if (x < 79 && y > 1) {
		CalculateCost(&grid[x + 1][y - 1], x, y);
	}
	if (x < 79 && y < 59) {
		CalculateCost(&grid[x + 1][y + 1], x, y);
	}
	if (y > 1) {
		CalculateCost(&grid[x][y - 1], x, y);
	}
	if (y < 79) {
		CalculateCost(&grid[x][y + 1], x, y);
	}

	GridPoint* lowestCostPoint = CalculatedPoints[0];
	for (int point = 0; point < CalculatedPoints.size(); point++) {

		if (CalculatedPoints[point]->fCost < lowestCostPoint->fCost) {
			lowestCostPoint = CalculatedPoints[point];
		}
		else if (CalculatedPoints[point]->x == endX && CalculatedPoints[point]->y == endY) {
			lowestCostPoint = CalculatedPoints[point];
			break;
		}
		else if (CalculatedPoints[point]->fCost == lowestCostPoint->fCost && CalculatedPoints[point]->hCost < lowestCostPoint->hCost) {
			lowestCostPoint = CalculatedPoints[point];
		}
	}

	CalculatedPoints.erase(std::find(CalculatedPoints.begin(),CalculatedPoints.end(),lowestCostPoint));
	lowestCostPoint->pickedPoint = 1;

	

	return lowestCostPoint;
}




int main(int argc, char* args[]) {

	int CurrentX = startingX;
	int CurrentY = startingY;

	//Mouse Pos
	int mouseX;
	int mouseY;

	int solve = 0;

	//adding structs to grid
	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			struct GridPoint newPoint;
			newPoint.x = x;
			newPoint.y = y;
			newPoint.calculated = 0;
			newPoint.solid = 0;
			newPoint.pickedPoint = 0;
			grid[x][y] = newPoint;
		}
	}

	//SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("A*", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Event event;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	while (true) {
		if (SDL_PollEvent(&event)) {
			if (SDL_QUIT == event.type) break;
			if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_LMASK) {
				if (mouseX >= 0 && mouseX <= WIDTH && mouseY >= 0 && mouseY <= HEIGHT)
				{
					grid[mouseX / 10][mouseY / 10].solid = 1;
				}
					
			}
			if (event.type == SDL_KEYDOWN)
				solve = 1;
		}

		Draw(renderer);

		if (solve == 1) {
			struct GridPoint* lastGrid = NextPoint(CurrentX, CurrentY);
			CurrentX = lastGrid->x;
			CurrentY = lastGrid->y;
			if (CurrentX == endX && CurrentY == endY)
			{
				solve = 0;
				std::cout << "solved";
				FindPath();
			}
				
		}
		
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 1;
}
