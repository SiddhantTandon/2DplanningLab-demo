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
