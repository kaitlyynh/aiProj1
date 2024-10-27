import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import sys
import math

VERTICAL_MOVES = [0, 2, 4, 6]
DIAGONAL_MOVES = [1, 3, 5, 7]
def plot_maze(file_path, file_type):
    try:
        # Load the maze from the .txt file
        with open(file_path, "r") as file:
            if (file_type == "-i"):
                skip_rows = 1
            elif (file_type == "-o"):
                skip_rows = 4
            maze = np.loadtxt(file_path, delimiter=" ", usecols=range(50), skiprows = skip_rows)

        # Initialize the plot
        fig, ax = plt.subplots(figsize=(12, 7))
        nrows, ncols = maze.shape

        # Define colors for each tile type
        colors = {0: "white", 1: "black", 2: "red", 3: "white", 4: "yellow", 5: "green"}

        # Plot each tile
        for row in range(nrows):
            for col in range(ncols):
                tile_color = colors.get(
                    maze[row, col], "white"
                )  # Default to white if unknown
                rect = Rectangle(
                    (col, row), 1, 1, edgecolor="black", facecolor=tile_color
                )
                ax.add_patch(rect)
        plt.get_current_fig_manager().set_window_title(file_path)
        plt.xlim(0, ncols)
        plt.ylim(0, nrows)
        plt.gca().invert_yaxis()  # Invert y-axis to match array layout
        plt.axis("off")  # Turn off the axis
        plt.show()
        

    except Exception as e:
        print(f"Failed to read the file: {e}")






def main():
    file_path = sys.argv[1]
    if (len(sys.argv) - 1 < 2): 
        print("Usage: python vis.py filename -i|-o")
        return -1
    file_type = sys.argv[2]
    with open(file_path, "r") as file:
            if (file_type == "-i"):
                skip_rows = 1
            elif (file_type == "-o"):
                skip_rows = 4


def calculate_utility(k, angle, direction):
    distance_cost = (1 if direction in VERTICAL_MOVES else math.sqrt(2))
    angle_cost = k * (angle / 180)
    return distance_cost + angle_cost



if __name__ == "__main__":
    main()