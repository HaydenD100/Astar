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

const int WIDTH = 800, HEIGHT = 600;

struct GridPoint {
	int x;
	int y;
	int solid;
	int pickedPoint;
	int calculated;
	int hCost;
	int gCost;
	int fCost;

};

const int startingX = 20; 
const int startingY = 20;
const int endX = 60; 
const int endY = 50;

struct GridPoint grid[80][60];
std::vector<GridPoint*> CalculatedPoints;


void Draw(SDL_Renderer* renderer) {
	//sets the background colour and then clears the window 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			SDL_Rect rect = { x * 10,y * 10,9,9 };

			if (grid[x][y].pickedPoint == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if(x == startingX && y == startingY)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

			}
			else if (grid[x][y].solid == 1) {
				SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

			}
			else if (grid[x][y].calculated == 1 && grid[x][y].pickedPoint == 0) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}
			else if(x == endX && y == endY){
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
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

void CalculateCost(struct GridPoint *point) {
	
	point->gCost = std::sqrt((startingX - point->x) * (startingX - point->x) + (startingY - point->y) * (startingY - point->y));
	point->hCost = std::sqrt((endX - point->x) * (endX - point->x) + (endY - point->y) * (endY - point->y));
	point->fCost = point->hCost + point->gCost;
	point->calculated = 1;
	if (point->solid == 0 && point->pickedPoint == 0)
	{
		CalculatedPoints.push_back(point);
	}
		
	
}

struct GridPoint* NextPoint(int x, int y) {

	//Calculate all the points next to current point
	if (x > 1) {
		CalculateCost(&grid[x - 1][y]);
	}
	if (x < 79) {
		CalculateCost(&grid[x + 1][y]);
	}
	if (x > 1 && y > 1) {
		CalculateCost(&grid[x - 1][y - 1]);
	}
	if (x > 1 && y > 59) {
		CalculateCost(&grid[x - 1][y + 1]);
	}
	if (x < 79 && y > 1) {
		CalculateCost(&grid[x + 1][y - 1]);
	}
	if (x < 79 && y < 59) {
		CalculateCost(&grid[x + 1][y + 1]);
	}
	if (y > 1) {
		CalculateCost(&grid[x][y - 1]);
	}
	if (y < 79) {
		CalculateCost(&grid[x][y + 1]);
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


	while (1) {
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
			}
				
		}
		
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 1;
}