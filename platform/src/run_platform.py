import pygame
from src.agent_service import AgentService
from src.map_tiling import MapTiling
from src.agents import RobotSprite
import utils.helpers as utils
from argparse import ArgumentParser

def main():
    parser = ArgumentParser()
    parser.add_argument("--map", required=True, help="Give a maps file, the path should be relative to platform directory -> ../maps/map_5x5_simple.txt")
    parser.add_argument("--just_map", help="This is a boolean flag, if used program only renders the map", action='store_true')
    
    args = parser.parse_args()

    # load map
    map_grid = utils.load_map_file(str(args.map).strip())
    

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
    dresser_tile_group = pygame.sprite.Group()

    # tiling
    for j in range(len(map_grid[0])):
        for i in range(len(map_grid)):
            if map_grid[i][j] == 'w':
                MapTiling(j, i, 32, 32, "w", map_tile_group, wall_tile_group)
            elif map_grid[i][j] == 'c':
                pass
                # create a DFS helper function here to find all objects


    #main loop
    running = True

    # setting up channel
    agent_service = AgentService(enable=True, duration=1000)
    agent_response = agent_service.runAgentService()

    # agent coord
    pos_x = 325
    pos_y = 325
    running = True

    robot = RobotSprite(pos_x, pos_y, map_tile_group)

    if not args.just_map:
        for response in agent_response:
            print(f"received: {response.position.x , response.position.y}")
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            display_surface.fill(color=(255,255,255))

            pos_x += response.position.x
            pos_y += response.position.y
            robot.update(pos_x, pos_y)
            map_tile_group.draw(display_surface)
            # dot = pygame.draw.circle(surface=display_surface, color=(0,0,0), radius=5, center=(pos_x,pos_y))
            
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