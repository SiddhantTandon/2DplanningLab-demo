import pygame
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

class DogSprite(pygame.sprite.Sprite):
    def __init__(self, x, y, main_group_tile):
        super().__init__()
    
    def updatePosition(self, dx, dy): # should use each position from below list of paths to update map real time
        pass

    def insertDynamicObstacle(self): # should generate a list of paths real time
        pass
 
    def _dynamic_behavior(self, args): # behavior option 1 - maybe use potential fields in discrete setting and keep changing goals?
        pass

    def _dynamic_behavior2(self, args): # behavior option 2
        pass