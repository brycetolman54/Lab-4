#include "Pathfinder.h"

string Pathfinder::toString() const {
    string maze = "";
    for(int i = 0; i < DEP; i++) {
        for(int j = 0; j < COL; j++) {
            for(int k = 0; k < ROW; k++) {
                maze += to_string(currentMaze[j][k][i]);
                if(k != ROW-1) {
                    maze += " ";
                }
            }
            if(j != COL-1) {
                maze += "\n";
            }
        }
        if(i != DEP - 1) {
            maze += "\n\n";
        }
        else {
            maze += "\n";
        }
    }
    return maze;
}

/*
* createRandomMaze
*
* Generates a random maze and stores it as the current maze.
*
* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
*/
void Pathfinder::createRandomMaze() {
    srand(time(NULL));
    for(int i = 0; i < DEP; i++) {
        for(int j = 0; j < ROW; j++) {
            for(int k = 0; k < COL; k++) {
                if(k == 0 && j == 0 && i == 0 || k == ROW - 1 && j == COL - 1 && i == DEP - 1) {
                        currentMaze[j][k][i] = BACK;
                }
                else {
                    currentMaze[j][k][i] = rand() % 2;
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------------------

//Part 2-----------------------------------------------------------------------------------
/*
* importMaze
*
* Reads in a maze from a file with the given file name and stores it as the current maze.
* Does nothing if the file does not exist or if the file's data does not represent a valid
* maze.
*
* The file's contents must be of the format described above to be considered valid.
*
* Parameter:	file_name
*				The name of the file containing a maze
* Returns:		bool
*				True if the maze is imported correctly; false otherwise
*/
bool Pathfinder::importMaze(string file_name) {
    int copyMaze[COL][ROW][DEP];
    ifstream in;
    string line;
    string line2;
    string number;
    in.open(file_name);
    if(!in.is_open()) {
        return false;
    }
    for(int i = 0; i < DEP; i++) {
        for(int j = 0; j < COL; j++) {
            getline(in, line);
            if(line.empty()) {
                getline(in, line);
            }
            if(in.eof() && i < DEP - 1) {
                in.close();
                return false;
            }
            istringstream ss(line);
            for(int k = 0; k < ROW; k++) {
                ss >> number;
                if(stoi(number) != WALL && stoi(number) != BACK) {
                    in.close();
                    return false;
                }
                if(number.length() > 1) {
                    in.close();
                    return false;
                }
                if(isalpha(stoi(number))) {
                    in.close();
                    return false;
                }
                copyMaze[j][k][i] = stoi(number);
            }
        }
    }
    if(!in.eof()) {
        in.close();
        return false;
    }
    in.close();
    if(copyMaze[0][0][0] != 1 || copyMaze[COL-1][ROW-1][DEP-1] != 1) {
        return false;
    }
    else {
        for(int i = 0; i < DEP; i++) {
            for(int j = 0; j < ROW; j++) {
                for(int k = 0; k < COL; k++) {
                    currentMaze[j][k][i] = copyMaze[j][k][i];
                }
            }
        }
        return true;
    }
}
//-----------------------------------------------------------------------------------------

//Part 3-----------------------------------------------------------------------------------
/*
* solveMaze
*
* Attempts to solve the current maze and returns a solution if one is found.
*
* A solution to a maze is a list of coordinates for the path from the entrance to the exit
* (or an empty vector if no solution is found). This list cannot contain duplicates, and
* any two consecutive coordinates in the list can only differ by 1 for only one
* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
* where x, y, and z are the integer coordinates of a cell.
*
* Understand that most mazes will contain multiple solutions
*
* Returns:		vector<string>
*				A solution to the current maze, or an empty vector if none exists
*/
vector<string> Pathfinder::solveMaze() {
    solution.clear();
    int solveThisMaze[ROW][COL][DEP];
    for(int i = 0; i < DEP; i++) {
        for(int j = 0; j < ROW; j++) {
            for(int k = 0; k < COL; k ++) {
                solveThisMaze[j][k][i] = currentMaze[j][k][i];
            }
        }
    }
    findPath(solveThisMaze, 0, 0, 0);
    switchVector();
    return solution;
}

bool Pathfinder::findPath(int maze[COL][ROW][DEP], int x, int y, int z) {
    if(x < 0 || y < 0 || z < 0 || x >= COL || y >= ROW || z >= DEP) {
        return false;
    }
    else if(maze[y][x][z] != BACK) {
        return false;
    }
    else if(maze[y][x][z] == TEMP || maze[y][x][z] == PATH) {
        return false;
    }
    else if(x == COL - 1 && y == ROW - 1 && z == DEP - 1) {
        maze[y][x][z] = PATH;
        solution.push_back("(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")");
        return true;
    }
    else {
        maze[y][x][z] = PATH;
        if(findPath(maze, x - 1, y, z) ||
           findPath(maze, x + 1, y, z) ||
           findPath(maze, x, y - 1, z) ||
           findPath(maze, x, y + 1, z) ||
           findPath(maze, x, y, z - 1) ||
           findPath(maze, x, y, z + 1)) {
            solution.push_back("(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")");
            return true;
        }
        else {
            maze[y][x][z] = TEMP;
            return false;
        }
    }
}

void Pathfinder::switchVector() {
    for(int i = 0; i < (solution.size() / 2); i++) {
        swap(solution.at(i), solution.at(solution.size() - i - 1));
    }
}
//-----------------------------------------------------------------------------------------
