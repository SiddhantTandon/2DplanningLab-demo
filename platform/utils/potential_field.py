import math
from dataclasses import dataclass
import queue
import logging


logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.StreamHandler()
    ]
)

logger = logging.getLogger(__name__)

@dataclass
class Node:
    row: int
    col: int
    value: str
    cost: float


class PotentialField:
    def __init__(self, grid, empty_space, x, y):
        self.grid = grid
        self.empty_space = empty_space
        self.x = x
        self.y = y
        self.field = dict()  # map for storing field values
        self._compute_field()
    
    def get_neighbors(self, current_n: Node):
        # use current node to find positions
        # of its neighbors in 4 directions
        # using the grid

        neighbor_list = []

        if (current_n.row - 1 >= 0):
            neighbor_left = Node(current_n.row - 1, current_n.col)
            neighbor_list.append(neighbor_left)

        if (current_n.row + 1 < len(self.grid)):
            neighbor_right = Node(current_n.row + 1, current_n.col)
            neighbor_list.append(neighbor_right)

        if (current_n.col - 1 >= 0):
            neighbor_top = Node(current_n.row, current_n.col - 1)
            neighbor_list.append(neighbor_top)

        if (current_n.col - 1 < len(self.grid[0])):
            neighbor_bottom = Node(current_n.row, current_n.col - 1)
            neighbor_list.append(neighbor_bottom)
        
        return neighbor_list


    def _compute_field(self):
        # BFS for starting from

        spawn = Node(self.x, self.y)
        spawn.cost = 0

        expanded = queue.Queue()
        expanded.put(spawn)
        self.field.update({(spawn.row, spawn.col): spawn.cost})

        while not expanded.empty():
            current_n = expanded.get()

            neighbor_list = self.get_neighbors(current_n)

            for n in neighbor_list:
                if (n.row, n.col) in self.empty_space:
                    n.cost = current_n.cost + 1 # update cost
                    expanded.put(n)
                    self.field.update({(n.row, n.col): n.cost})

