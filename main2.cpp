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
//Global constants
const int NUM_ROWS = 30, 
          NUM_COLS = 50;

// Custom hash function for pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        // Combine the hashes of the two elements in the pair
        size_t h1 = hash<T1>()(p.first);
        size_t h2 = hash<T2>()(p.second);
        return h1 ^ (h2 << 1); // Simple combination using XOR and bit-shifting
    }
};

unordered_map< pair<int, int>, int, pair_hash> COORD_OFFSET_MAP =  {{{-1, -1}, 3},
                                                        {{-1, 0}, 2},
                                                        {{-1, 1}, 1},
                                                        {{1, -1}, 5},
                                                        {{1, 0}, 6},
                                                        {{1, 1}, 7},
                                                        {{0, -1}, 4},
                                                        {{0, 1}, 0}
                                                    };
//Global variables
vector<vector<int>> g_maze;
vector<vector<bool>> visited;
int g_start_x, g_start_y;


//Prototypes
void parse_file(const string& filepath); /*reads input file. Parses it to produce 2d matrix of the maze 
                                            and loads into g_maze*/
void transpose(vector<vector<int>>& matrix); // transposes a 2d matrix
void print_matrix(const vector<vector<int>>& matrix); //prints 2d matrix. for debugging purposes

float cost_angle(const int curr_dir, const int action);//calculate angle cost function
float cost_distance(const int action); //calculate distance cost function
float utility_function(const int s_curr, const int action, const int k); //entire utility function

int coord_offset_to_action(const pair<int, int>& coord_offset){
    
}







int main(int argc, char* argv[]){
   
    if (argc != 2){cerr << "Requires input file\n"; return -1;}
    string filepath = argv[1];
    parse_file(filepath);
    cout << g_maze[6][15] << endl;


}

void parse_file(const string& filepath){
   
    ifstream input_file(filepath);
    string line;
    while (getline(input_file, line)) {
        vector<int> row;
        istringstream ss(line);
        int number;

        // Read integers in the line and add them to the row vector
        while (ss >> number) {
            row.push_back(number);
        }
        
        // Add the row to the 2D vector
        g_maze.push_back(row);
    }
    //transform matrix to achieve [x][y] indexing
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
        for (int c = 0; c , matrix[0].size(); ++c){
            cout << matrix[r][c] << " ";
        }
        cout << endl;
    }
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