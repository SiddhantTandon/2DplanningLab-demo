import pygame
from src.agent_service import AgentService
from src.map_tiling import MapTiling
from src.agents import RobotSprite
import utils.helpers as utils
from argparse import ArgumentParser
from dataclasses import dataclass

@dataclass
class Tile:
    height = 32
    width = 32

OBJECT = {
    "BED_HEIGHT": 3,
    "BED_WIDTH": 3,
    "CAR_HEIGHT": 3,
    "CAR_WIDTH": 2
}


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
    object_tile_group = pygame.sprite.Group()

    # tiling
    for j in range(len(map_grid[0])):
        for i in range(len(map_grid)):
            if map_grid[i][j] == 'w':
                MapTiling(j*Tile.width, i*Tile.height, Tile.width, Tile.height, "w", map_tile_group, wall_tile_group)
            elif map_grid[i][j] == 'j':
                MapTiling(j*Tile.width, i*Tile.height, Tile.width, Tile.height, "j", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'q':
                MapTiling(j*Tile.width, i*Tile.height, Tile.width, Tile.height, "q", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'o':
                MapTiling(j*Tile.width, i*Tile.height, Tile.width, Tile.height, "o", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'u':
                MapTiling(j*Tile.width, i*Tile.height, Tile.width, Tile.height, "u", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'b':
                if utils.is_this_topleft_point(i, j, map_grid, "b"):
                    MapTiling(j*Tile.width, i*Tile.height, OBJECT["BED_WIDTH"]*Tile.width, OBJECT["BED_HEIGHT"]*Tile.height, "b", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'n':
                if utils.is_this_topleft_point(i, j, map_grid, "n"):
                    MapTiling(j*Tile.width, i*Tile.height, OBJECT["BED_WIDTH"]*Tile.width, OBJECT["BED_HEIGHT"]*Tile.height, "n", map_tile_group, object_tile_group)
            elif map_grid[i][j] == 'p':
                if utils.is_this_topleft_point(i, j, map_grid, "p"):
                    MapTiling(j*Tile.width, i*Tile.height, OBJECT["CAR_WIDTH"]*Tile.width, OBJECT["CAR_HEIGHT"]*Tile.height, "p", map_tile_group, object_tile_group)
            else:
                pass
                

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
            if not running:
                break

    pygame.quit()

if __name__ == "__main__":
    main()