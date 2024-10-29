#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

using namespace std; 

struct Node {
public:
    pair<int, int> position;
    int angle;
    //vector<Node> neighbors;
    float utility_score;
};

//Global constants
const int NUM_ROWS = 30, 
          NUM_COLS = 50;

//Global variables
vector<vector<int>> g_maze;
vector<vector<bool>> visited;
priority_queue<Node> g_frontier;
int g_start_x, g_start_y,  g_end_x, g_end_y;


//Prototypes

void parse_file(const string& filepath); /*reads input file. Parses it to produce 2d matrix of the maze 
                                            and loads into g_maze*/

void transpose(vector<vector<int>>& matrix); // transposes a 2d matrix
void print_matrix(const vector<vector<int>>& matrix); //prints 2d matrix. for debugging purposes

float cost_angle(const int curr_dir, const int action);//calculate angle cost function
float cost_distance(const int action); //calculate distance cost function
float utility_function(const int s_curr, const int action, const int k); //entire utility function

int coord_offset_to_action(const pair<int, int>& coord_offset); // matches coordinate offset to an action
vector<Node> expand(const struct Node& node); //expands node 
void best_first_search(const struct Node& node); // best first search algorithm





int main(int argc, char* argv[]){
   
    if (argc != 2){cerr << "Requires input file\n"; return -1;}
    string filepath = argv[1];
    parse_file(filepath);
    // cout << g_maze[6][15] << endl;
    // cout << "rows: " << g_maze.size() << endl;
    // cout << "cols: " << g_maze[0].size() << endl;
    //print_matrix(g_maze);
    
}


void parse_file(const string& filepath){
   
    ifstream input_file(filepath);
    string line;
    if (getline(input_file, line)){ //read the first line to get starting point and goal 
        istringstream firstline(line);
        firstline >> g_start_x >> g_start_y >> g_end_x >> g_end_y;
    }
    
    while (getline(input_file, line)) {
        vector<int> row;
        istringstream ss(line);
        int number;

        // Read integers in the line and add them to the row vector
        while (ss >> number && row.size() < NUM_COLS) {
            row.push_back(number);
        }
        
        // Add the row to the 2D vector
        if (g_maze.size() < NUM_ROWS){g_maze.push_back(row);}
    }
    //transform matrix to achieve [x][y] indexing, will undo before creating output file
    reverse(g_maze.begin(), g_maze.end());
    transpose(g_maze);
    input_file.close();
}




void transpose(vector<vector<int>>& matrix){
    if (matrix.empty() || matrix[0].empty()) {
        return ;  // Return empty if input is empty
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    // Create a transposed matrix with dimensions `cols x rows`
    vector<vector<int>> transposed(cols, vector<int>(rows));

    // Fill the transposed matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    matrix =  transposed;
}


void print_matrix(const vector<vector<int>>& matrix){
    for (int r = 0; r < matrix.size(); ++r){
        for (int c = 0; c < matrix[0].size(); ++c){
            cout << matrix[r][c] << " ";
        }
        cout << "\n\n";
    }
}

int coord_offset_to_action(const pair<int, int>& coord_offset){ //not sure if this is correct
    if (coord_offset.first == 1 && coord_offset.second == 0){return 0;} //RIGHT
    else if (coord_offset.first == 0 && coord_offset.second == -1){return 2;} //UP
    else if (coord_offset.first == -1 && coord_offset.second == 0){return 4;} //LEFT
    else if (coord_offset.first == 0 && coord_offset.second == 1){return 6;}//DOWN
    //Diagonals
    else if (coord_offset.first == 1 && coord_offset.second == -1){return 1;} // Right + Up
    else if (coord_offset.first == 1 && coord_offset.second == 1){return 3;} // Right + Down
    else if (coord_offset.first == -1 && coord_offset.second == -1){return 5;} // Left + Up
    else if (coord_offset.first == -1 && coord_offset.second == 1){return 7;} // Left + Down
    
}


float cost_angle(const int curr_dir, const int action){ 
    int diff = abs((curr_dir - action) * 45);
    if (diff > 180){diff = 360 - diff;} 
    return diff;
}


float cost_distance(const int action){
    return (action % 2 ? 1 : sqrt(2));
}


float utility_function(const int s_curr, const int action, const int k){
    return k * cost_angle(s_curr, action) / 180 + cost_distance(action);
}


void best_first_search(){
    Node curr;
    while (!g_frontier.empty()){
        curr = g_frontier.top();
        g_frontier.pop();
        for (Node n : expand(curr)){
            //frontier.push(n, n.utility_score);
        }


    }
}

vector<Node> expand(const struct Node& node){

}