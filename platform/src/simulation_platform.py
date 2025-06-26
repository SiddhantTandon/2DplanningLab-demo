import pygame
from src.map_tiling import MapTiling
from src.agents import RobotSprite
import utils.helpers as utils
import utils.map_message_helpers as map_msg_utils
from dataclasses import dataclass
import time
# from google.protobuf.timestamp_pb2 import Timestamp
# from network import state_message_pb2

@dataclass
class Tile:
    height: int = 32
    width: int = 32

OBJECT = {
    "BED_HEIGHT": 3,
    "BED_WIDTH": 3,
    "CAR_HEIGHT": 3,
    "CAR_WIDTH": 2
}


class SimulationPlatformViz:
    def __init__(self, args):
        self._args = args
        self._agent_message = None
        self._map_message = None

    def getAgentResponseMessage(self, agent_messages):
        self._agent_message = agent_messages
    
    def sendMapResponseMessage(self):
        return self._map_message 

    def clearMapMessage(self):
        self._map_message.Clear()
        self._map_message = None

    def clearAgentMessage(self):
        self._agent_message.Clear()
        self._agent_message = None

    def run(self):

        # load map
        map_grid = utils.load_map_file(str(self._args.map).strip())
        

        #initialize pygame
        pygame.init()

        #display
        display_surface = pygame.display.set_mode((32*32,32*16))

        #fps and clock
        FPS = 1
        clock = pygame.time.Clock()

        # sprite groups
        map_tile_group = pygame.sprite.Group()
        wall_tile_group = pygame.sprite.Group()
        object_tile_group = pygame.sprite.Group()
        #FIXME: Add my name to the bottom left corner of the window

        # tiling
        #TODO: add remaining assets to the map
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

        # agent coord
        pos_x = 320
        pos_y = 320
        running = True

        robot = RobotSprite(pos_x, pos_y, map_tile_group)

        #TODO: add warm start to arguments
        if not self._args.warm_start:
            warm_start = 0

        step = 0 # for grid based movement
        no_response_counter = 0

        if not self._args.just_map:
            while running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                display_surface.fill(color=(255,255,255))
                map_tile_group.draw(display_surface)
                pygame.draw.rect(display_surface, (255, 0, 0), robot.rect, 2) # bounding box
                pygame.display.update()
                if (step > warm_start and self._agent_message is not None
                ): #FIXME: condition cannot be on message has to be on step
                    #print(f"received message: {self._agent_message}")

                    rec_agents = self._agent_message.agents
                    for agents in rec_agents:
                        if agents.agent_id == "1":
                            rec_position = agents.position
                            print(f"the positions are {rec_position.row} and {rec_position.col}")
                            robot.updatePosition(rec_position.col * 32, rec_position.row*32)

                    pygame.display.update()

                    if map_grid[rec_position.row][rec_position.col] is not "x":
                        print(f"Collision occured map value: {map_grid[rec_position.row][rec_position.col]}!")

                    self.clearMapMessage()
                    msg = map_msg_utils.generate_map_message()

                    # Set timestamp
                    timestamp = map_msg_utils.set_timestamp()
                    msg.timestamp.CopyFrom(timestamp)

                    # Set one valid MapState with CellState
                    cell_state = map_msg_utils.set_cell_state(5, 5, "debug")

                    map_state = map_msg_utils.generate_map_state()
                    map_state.cell_state.CopyFrom(cell_state) 

                    msg.map_states.append(map_state)
                    self._map_message = msg
                    self.sendMapResponseMessage()
                    self.clearAgentMessage()
                    no_response_counter = 0
                else:

                    if no_response_counter == 10:
                        running = False

                    msg = map_msg_utils.generate_map_message()

                    # Set timestamp
                    timestamp = map_msg_utils.set_timestamp()
                    msg.timestamp.CopyFrom(timestamp)

                    # Set one valid MapState with CellState
                    cell_state = map_msg_utils.set_cell_state(5, 5, "debug")

                    map_state = map_msg_utils.generate_map_state()
                    map_state.cell_state.CopyFrom(cell_state) 

                    msg.map_states.append(map_state)
                    self._map_message = msg
                    self.sendMapResponseMessage()
                    no_response_counter += 1
                clock.tick(FPS)
                step += 1
                time.sleep(1)
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
