import pygame

class MapTiling(pygame.sprite.Sprite):
    def __init__(self, x, y, obj_str, map_group_tile, object_group_tile):
        super().__init__()

        # now for each different type of object we need
        # to add different groups and then add those
        # groups to the main tile group
        # look up why that is

        if obj_str == "w":
            image = pygame.image.load("../resources/assets/wall_10x10.png").convert_alpha()
            self.image = pygame.transform.scale(image, (32,32))
            object_group_tile.add(self)
        
        map_group_tile.add(self)
        
        self.rect = self.image.get_rect()
        self.rect.topleft = (x,y)

