import pygame
from src.map_tiling import MapTiling
from src.agents import RobotSprite
import utils.helpers as utils
from dataclasses import dataclass
from google.protobuf.timestamp_pb2 import Timestamp
from network import state_message_pb2

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


class SimulationPlatformViz:
    def __init__(self, args):
        self._args = args
        self._agent_message = ""
        self._map_message = ""

    def getAgentResponseMessage(self, agent_messages):
        self._message = agent_messages
    
    def sendMapResponseMessage(self):
        # return self._map_message #FIXME: use the code below to fix the next fix me
        msg = state_message_pb2.MapMessage()

        # Set timestamp
        timestamp = Timestamp()
        timestamp.GetCurrentTime()
        msg.timestamp.CopyFrom(timestamp)

        # Set one valid MapState with CellState
        cell_state = state_message_pb2.CellState()
        cell_state.position.row = 5
        cell_state.position.col = 5
        cell_state.value = "debug"

        map_state = state_message_pb2.MapState()
        map_state.cell_state.CopyFrom(cell_state) 

        msg.map_states.append(map_state)

        return msg

    def clearMessages(self):
        self._map_message.Clear()

    def run(self):

        # load map
        map_grid = utils.load_map_file(str(self._args.map).strip())
        

        #initialize pygame
        pygame.init()

        #display
        display_surface = pygame.display.set_mode((32*32,32*16))

        #fps and clock
        FPS = 15
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

        # agent coord
        pos_x = 325
        pos_y = 325
        running = True

        robot = RobotSprite(pos_x, pos_y, map_tile_group)

        if not self._args.just_map:
            while running:
                for event in pygame.event.get(): #TODO: fix this in case it won't work when user hovers over sim or clicks on it
                    if event.type == pygame.QUIT:
                        running = False
                if self._agent_message:
                    self.clearMessages()
                    print(f"received message: {self._agent_message.timestamp}")
                    display_surface.fill(color=(255,255,255))
                    map_tile_group.draw(display_surface)
                    pygame.draw.rect(display_surface, (255, 0, 0), robot.rect, 2) # bounding box
                    pygame.display.update()
                    #FIXME: make sure this loop runs with map message and cell state protobuf messages
                    self._map_message = state_message_pb2.MapMessage()
                    timestamp = Timestamp()
                    timestamp.GetCurrentTime()
                    map_state = state_message_pb2.MapState()
                    map_state.cell_state.position.row = 0
                    map_state.cell_state.position.col = 0
                    map_state.cell_state.value = "debug_tile"
                    self._map_message.map_states.append(map_state)
                    print(f"Map message: {self._map_message}")
                    self._map_message.timestamp.CopyFrom(timestamp)
                    self.sendMapResponseMessage(self._map_message)
                    self.clearMessages()
                    clock.tick(FPS)

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
