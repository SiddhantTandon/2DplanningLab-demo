import pygame

class MapTiling(pygame.sprite.Sprite):
    def __init__(self, x, y, scale_x, scale_y, obj_str, map_group_tile, object_group_tile):
        super().__init__()

        # now for each different type of object we need
        # to add different groups and then add those
        # groups to the main tile group
        # look up why that is

        if obj_str == "w":
            image = pygame.image.load("../resources/assets/wall_10x10.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)
        
        if obj_str == "j":
            image = pygame.image.load("../resources/assets/toilet_16x31.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)

        if obj_str == "o":
            image = pygame.image.load("../resources/assets/coffee-table_24x19.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)

        if obj_str == "q":
            image = pygame.image.load("../resources/assets/sofa-chair_31x32.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)
        
        if obj_str == "u":
            image = pygame.image.load("../resources/assets/sink_21x31.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)
        
        if obj_str == "b":
            image = pygame.image.load("../resources/assets/bed_62x89.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)

        if obj_str == "n":
            image = pygame.image.load("../resources/assets/bed2_63x89.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            self.image = self.orient_image(self.image, "right")
            object_group_tile.add(self)

        if obj_str == "p":
            image = pygame.image.load("../resources/assets/car_40x64.png").convert_alpha()
            self.image = pygame.transform.scale(image, (scale_x,scale_y))
            object_group_tile.add(self)
        
        map_group_tile.add(self)
        
        self.rect = self.image.get_rect()
        self.rect.topleft = (x,y)
    
    def orient_image(self, image, orientation):
        if orientation == "left":
            return pygame.transform.rotate(image, 90)
        elif orientation == "right":
            return pygame.transform.rotate(image, -90)
        elif orientation == "up":
            return pygame.transform.rotate(image, 180)
        else:
            return image

