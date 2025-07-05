import pygame
from utils.potential_field import PotentialField, Node
from utils.helpers import get_grid_ids
# Apparently you always need to define self.image? Maybe how I use self within group?
class RobotSprite(pygame.sprite.Sprite):
    def __init__(self, x, y, main_group_tile):
        super().__init__()
        robot = pygame.image.load("../resources/assets/robot_32x32.png").convert_alpha()
        self.image = pygame.transform.scale(robot, (32,32))
        self.rect = self.image.get_rect()
        self.rect.topleft = (x,y)
        main_group_tile.add(self)
    
    def updatePosition(self, dx, dy):
        print(f"method called: dx = {dx} and dy = {dy}")
        self.rect.x = dx
        self.rect.y = dy

class CatSprite(pygame.sprite.Sprite):
    def __init__(self, x, y, main_group_tile):
        super().__init__()
        cat = pygame.image.load("../resources/assets/cat_24x22.png").convert_alpha()
        self.image = pygame.transform.scale(cat, (32,32))
        self.rect = self.image.get_rect()
        self.rect.topleft = (x,y)
        main_group_tile.add(self)
    
    def updatePosition(self, dx, dy): # should use each position from below list of paths to update map real time
        self.rect.x = dx
        self.rect.y = dy

    def potentialGoal(self, grid, empty_space, goal_x, goal_y): # should generate a list of paths real time
        potential = PotentialField(grid, empty_space, goal_x, goal_y)
        current_x, current_y = get_grid_ids(self.rect.x, self.rect.y)
        current_node = Node(current_x, current_y)
        current_neighbors = potential.get_neighbors(current_node)
        current_neighbors.append(current_node)
        cost = 1000000
        next_pos = current_node
        for n in current_neighbors:
            n_cost = potential.field[(n)]
            if n_cost < cost:
                next_pos = n
                cost = n_cost
