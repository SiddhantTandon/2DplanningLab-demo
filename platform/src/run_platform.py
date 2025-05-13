import pygame
from src.agent_service import AgentService
from src.map_tiling import MapTiling
import utils.helpers as utils

def main():

    # load map
    map_grid = utils.load_map_file("../maps/map_16x32_indoor.txt")
    

    #initialize pygame
    pygame.init()

    #display
    display_surface = pygame.display.set_mode((32*32,32*16))

    #fps and clock
    FPS = 30
    clock = pygame.time.Clock()

    # sprite groups
    map_tile_group = pygame.sprite.Group()
    wall_tile_group = pygame.sprite.Group()

    # tiling
    for i in range(len(map_grid[0])):
        for j in range(len(map_grid)):
            if map_grid[j][i] == 'w':
                MapTiling(i*32, j*32, "w", map_tile_group, wall_tile_group)

    #main loop
    running = True

    # setting up channel
    # agent_service = AgentService(enable=True, duration=1000)
    # agent_response = agent_service.runAgentService()

    # agent coord
    pos_x = 125
    pos_y = 125
    running = True
    mock = True

    if not mock:
        for response in agent_response:
            print(f"received: {response.position.x , response.position.y}")
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            display_surface.fill(color=(255,255,255))

            pos_x += response.position.x
            pos_y += response.position.y
            dot = pygame.draw.circle(surface=display_surface, color=(0,0,0), radius=5, center=(pos_x,pos_y))
            
            pygame.display.update()

            if not running:
                break
    else:
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            
            display_surface.fill(color=(255,255,255))

            map_tile_group.draw(display_surface)

            pygame.display.update()

    pygame.quit()

if __name__ == "__main__":
    main()