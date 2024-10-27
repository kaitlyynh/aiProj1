import heapq
import numpy as np
import sys



class Node:
    def __init__(self, position, utility_score = None, angle = 0, neighbors = [], ):
        self.position = position
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


def main():
    try:
        file_path = sys.argv[1]
    
        
        with open(file_path, "r") as file:
            frontier = []
            first_line = file.readline().strip().split()
            start_pos = first_line[0], first_line[1]
            end_pos = first_line[2], first_line[3]

            maze = np.loadtxt(file_path, delimiter=" ", usecols=range(50), skiprows = 1)

            start_node = Node(start_pos, 1) 
            test_node = Node((1,1), -1)
            heapq.heappush(frontier, start_node)
            # print(heapq.heappop(frontier))
            heapq.heappush(frontier, test_node)
            
            # print(heapq.heappop(frontier))
            print(heapq.nsmallest(1, frontier)[0])
            
    except Exception as e:
        print(f"Failed to read the file: {e}")


if __name__ == "__main__":
    main()