import heapq
import numpy as np
import sys
import utility_functions
NUM_ROWS = 30
NUM_COLS = 50
WALL = 1 

# (row, col)
COORD_OFFSET_MAP = {(-1, -1): 3, (-1, 0): 2, (-1, 1): 1,
                    (1, -1): 5, (1, 0): 6, (1, 1): 7,
                    (0, -1): 4, (0 , 1): 0             
                   }

COORD_OFFSET_MAP = {(1, 0): 0, (0, -1): 2, (-1, 0): 4, (0, 1): 6
                    (1)
                   }

class Node:
    def __init__(self, position, utility_score = None, angle = 0, neighbors = [], ):
        self.position = (int(position[0]), int(position[1]))
        self.angle = angle
        self.neighbors = neighbors
        self.utility_score = utility_score

    def __str__(self):
        return f"Node ({self.position}) {self.utility_score}"

    def __lt__(self, other):
        return self.utility_score < other.utility_score

    def __eq__(self, other):
        return self.utility_score == other.utility_score

    def __gt__(self, other):
        return self.utility_score > other.utility_score



def is_valid_coordinate(pos): # is within bounds
    r_pos, c_pos = pos[0], pos[1]
    return 0 <= r_pos < NUM_ROWS and 0 <= c_pos < NUM_COLS 
    
"""
x x x   
x s x 
x x g
"""
def expand(node):
    print("node pos:", node.position)
    
                   
                            


def best_first_search(start_node):
    for r in range(-1, 2): 
        for c in range(-1, 2):
            coord = (start_node.position[0] + r, start_node.position[1] + c - 1) # 
            if coord == start_node.position: continue
            if is_valid_coordinate(coord):
                print("is valid coord", coord)
                if (g_maze[coord[0], coord[1]] != WALL):
                    total = utility_functions.utility_func()

            else:
                print("is not a valid coord", coord)
    pass

g_visited = set()
global g_maze 
def main():

   # try:
        file_path = sys.argv[1]
    
        
        with open(file_path, "r") as file:
            global g_maze, g_visited
            # frontier = []
            first_line = file.readline().strip().split()
            start_pos = int(first_line[0]), int(first_line[1])
            end_pos = first_line[2], first_line[3]
            g_maze = np.loadtxt(file_path, delimiter=" ", usecols=range(50), skiprows = 1)
            # modifying array to match indexing
            g_maze = np.flipud(g_maze) 
            g_maze = g_maze.transpose() 
            print(g_maze[6, 15])
            start_node = Node(start_pos, 1) 
            expand(start_node)



            # # test_node = Node((1,1), -1)
            # heapq.heappush(frontier, start_node)
            # # print(heapq.heappop(frontier))
            # # heapq.heappush(frontier, test_node)
            # while f
            # # print(heapq.heappop(frontier))
            # print(heapq.nsmallest(1, frontier)[0])

            
    #except Exception as e:
     #   print( f"{e}")


if __name__ == "__main__":
    main()