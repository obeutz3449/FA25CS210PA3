//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        if (r == -1 || c == -1) {
            cout << "Error at: ("<<path[path.size() - 1].first << ", "<<path[path.size() - 1].second << ")";
            break;
        }
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
vector<pair<int, int>> neighbors(const vector<vector<int>>& maze, const vector<vector<bool>>& visited, const pair<int, int> &pos) {
    vector<pair<int, int>> neighbors;
    for (int i = 0; i < 4; i++) {
        int r = pos.first + dr[i];
        int c = pos.second + dc[i];
        if (r >= 0 && c >= 0 && r < maze.size() && c < maze[0].size() && maze[r][c] == 0 && !visited[r][c]) {
            neighbors.emplace_back(r, c);
        }
    }
    return neighbors;
}

bool dfs(const vector<vector<int>>& maze, vector<vector<bool>>& visited, const pair<int,int> &entrance, const pair<int,int> &exit, vector<vector<int>>& parent_r, vector<vector<int>>& parent_c) {
    // Your code here
    cout<<"("<<entrance.first<<", "<<entrance.second<<")\n";
    visited[entrance.first][entrance.second] = true;
    const auto next_pos = neighbors(maze, visited, entrance);
    for (const auto s : next_pos) {
        parent_r[s.first][s.second] = entrance.first;
        parent_c[s.first][s.second] = entrance.second;
        visited[s.first][s.second] = true;
        //cout<<"Set ("<<s.first<<", "<<s.second<<")'s parent to ("<<entrance.first<<", "<<entrance.second<<")\n";
    }
    bool found = false;
    for (auto s : next_pos) {
        if (s.first == exit.first && s.second == exit.second) found = true;
        else found = found || dfs(maze, visited, s, exit, parent_r, parent_c);
    }
    return found;
}


// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    const pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);
    cout<<"Entrance: ("<<entrance.first<<", "<<entrance.second<<")\n";
    cout<<"Exit: ("<<exitcell.first<<", "<<exitcell.second<<")\n";
    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    // Display the maze
    printMaze(maze, entrance.first, entrance.second, exitcell.first, exitcell.second);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
    const bool found = dfs(maze, visited, entrance, exitcell, parent_r, parent_c);
    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
    if (found) printPath(exitcell, parent_r, parent_c, entrance.second, entrance.first);
    else cout << "\nNo path exists.\n";

    return 0;
}