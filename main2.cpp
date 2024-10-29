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
    Node(){}
    Node(pair<int,int> pos, int direction, float utility_score) : position(pos),
                                                                direction(direction),
                                                                utility_score(utility_score){}
    friend bool operator<( const Node& lhs,  const Node& rhs)
    {
        return lhs.utility_score < rhs.utility_score;
    }


    friend bool operator==(const Node& lhs,  const Node& rhs)
    {
        return lhs.utility_score == rhs.utility_score;
    }
 
    pair<int, int> position;
    int direction;
    float path_cost = 0;
    //vector<Node> neighbors;
    float utility_score;
};

//Global constants

const int NUM_ROWS = 7, 
          NUM_COLS = 7;

const int BLANK = 0;
const int WALL = 1;
const int PATH = 4; //indicates path taken

const float K = 1.0f;  //angle cost weight

//Global variables
vector<vector<int>> g_maze;
vector<int> g_solution_path; // stores sequence of actions taken to reach goal node
vector<float> g_utility_values; //stores utility values of nodes in the solution path
priority_queue<Node> g_frontier;
bool g_reached[NUM_COLS][NUM_ROWS];

int g_total_nodes_generated = 1; //total nodes generated including the root node
int g_depth = 0;
int g_start_x, g_start_y,  g_end_x, g_end_y;

//Prototypes  

void parse_file(const string& filepath); /*reads input file. Parses it to produce 2d matrix of the maze 
                                            and loads into g_maze*/

void transpose(vector<vector<int>>& matrix); // transposes a 2d matrix
void print_matrix(const vector<vector<int>>& matrix); //prints 2d matrix. for debugging purposes

float cost_angle(const int curr_dir, const int action);//calculate angle cost function
float cost_distance(const int action); //calculate distance cost function
float cost_function(const int s_curr, const int action); //entire utility function
float heuristic(int curr_x, int curr_y); //Euclidean distance of current node to goal node
float utility_function(const Node& curr, Node& neighbor);// f(n)
Node generate_child(const Node& curr, const int action,
                    const int child_x, const int child_y); //creates child based on given action

int coord_offset_to_action(const int x_off, const int y_off); // matches coordinate offset to an action
void expand(const  Node& node); //expands node, adds children to frontier and updates reached states
void best_first_search(); // best first search algorithm

void initialize(); //intialize root node


int main(int argc, char* argv[]){
    if (argc != 2){cerr << "Requires input file\n"; return -1;}
    string filepath = argv[1];
    parse_file(filepath);
    // cout << g_maze[6][15] << endl;
    // cout << "rows: " << g_maze.size() << endl;
    // cout << "cols: " << g_maze[0].size() << endl;
    //print_matrix(g_maze);
    //print_matrix(g_maze);
    initialize();
    best_first_search();
    print_matrix(g_maze);
    cout << "Finished" << endl;
}

void initialize() {
    Node root(pair<int,int>{g_start_x, g_start_y}, 
              0,
              heuristic(g_start_x, g_start_y) //g(n) = 0 at root
    );
    g_frontier.push(root);
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

int coord_offset_to_action(const int x_off, const int y_off){ //not sure if this is correct
    if (x_off == 1 && y_off == 0){return 0;} //RIGHT
    else if (x_off == 0 && y_off == -1){return 2;} //UP
    else if (x_off == -1 && y_off == 0){return 4;} //LEFT
    else if (x_off == 0 && y_off == 1){return 6;}//DOWN
    //Diagonals
    else if (x_off == 1 && y_off == -1){return 1;} // Right + Up
    else if (x_off == 1 && y_off == 1){return 3;} // Right + Down
    else if (x_off == -1 && y_off == -1){return 5;} // Left + Up
    else if (x_off == -1 && y_off == 1){return 7;} // Left + Down
    
}


float cost_angle(const int curr_dir, const int action){ 
    if (g_depth == 0){return 0;} //initial angle cost is 0
    int diff = abs((curr_dir - action) * 45);
    if (diff > 180){diff = 360 - diff;} 
    return diff;
}


float cost_distance(const int action){
    return (action % 2 ? 1 : sqrt(2));
}


float cost_function(const int s_curr, const int action){
    return K * cost_angle(s_curr, action) / 180 + cost_distance(action);
}

float heuristic(int curr_x, int curr_y){
    return sqrt(pow(curr_x - g_end_x, 2)  + pow(curr_y - g_end_y, 2));
}

float utility_function(const Node& curr, Node& neighbor) {        
    float h = heuristic(curr.position.first, curr.position.second);         
    neighbor.path_cost = curr.path_cost + cost_function(curr.direction, neighbor.direction);
    return h + neighbor.path_cost; 
}

Node generate_child(const Node& curr, const int action, const int child_x, const int child_y){
    //calculate g(n)
    ++g_total_nodes_generated;
    Node child;
    child.position = pair<int, int>{child_x, child_y};
    child.path_cost = cost_function(curr.direction, action);
    child.direction = action;
    child.utility_score = child.path_cost + heuristic(child.position.first, child.position.second);
    return child;
}


void best_first_search(){
    Node curr;
    while (!g_frontier.empty()){
        print_matrix(g_maze);
        curr = g_frontier.top();
        g_frontier.pop();
        g_solution_path.push_back(curr.direction);
        g_utility_values.push_back(curr.utility_score);
        //return if curr is goal state
        if (curr.position.first == g_end_x && curr.position.first == g_end_y) {
            cout << "Goal node found: " << curr.position.first  <<  " " << curr.position.second << endl;
            return;
        }
        g_maze[curr.position.first][curr.position.second] = PATH;
        expand(curr);
        ++g_depth;        
    }
}


void expand(const struct Node& node){
    int n_x, n_y; //neighbor's x and y position
    int action;
    for (int x = -1; x <= 1; ++x){
        for (int y = -1; y <= 1; ++y){
            if (!(x == 0 && y == 0)){
                //convert coordinate offset to action
                action = coord_offset_to_action(x,y);
                //get coordinate of neighbor
                n_x = node.position.first  + x; 
                n_y = node.position.second + y;
                //make sure it is a valid coordinate and not visited
                if (0 <= n_x < NUM_COLS && 0 <= n_y < NUM_ROWS 
                    && !g_reached[n_x][n_y] && g_maze[n_x][n_y] != WALL)
                {
                    g_reached[n_x][n_y] = true;
                    g_frontier.push(generate_child(node, action, n_x, n_y)); 
                }
            }
        }
    }
}


