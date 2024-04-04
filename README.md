# What is A *
A* is a path finding algorithm invented by Peter Hart. A* finds the shortest path to a destination using a cost function F = G + H H is how far the tile is from the end destination and G is how far a tile is from the starting destination. The algorithm moves by picking the lowest F cost and then calculating all the costs of the tiles surrounding it adding it to a list of all the calculate tiles (this is shown in blue in my implementation) and then picking the lowest cost from those tiles. You can read more about the A star Algorithm at: https://en.wikipedia.org/wiki/A*_search_algorithm#:~:text=A*%20is%20an%20informed%20search,shortest%20time%2C%20etc


Screenshot of my implementation of the A* algorithm 
![screenshot](/docs/assets/screenshot3.png)


# Features and Controls

-Using the left mouse button you can add solid tiles making it more complex for the algorithm to find the destination.

-Pressing any button on the keyboard will start the solving algorithm 



# Screenshots
![screenshot](/docs/assets/screenshot4.png)
Solid tiles are shown as grey

# Implementation
Coded in c++ with SDL2 for rendering.




