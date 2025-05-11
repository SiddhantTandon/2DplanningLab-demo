import pygame
from utils.agent_service import AgentService
def main():
    #initialize pygame
    pygame.init()

    #display
    display_surface = pygame.display.set_mode((600,600))

    #fps and clock
    FPS = 30
    clock = pygame.time.Clock()

    #main loop
    running = True

    # setting up channel

    # channel = grpc.insecure_channel('localhost:50051')
    agent_service = AgentService(enable=True, duration=1000)
    agent_response = agent_service.runAgentService()

    # service stub
    # stub = position_pb2_grpc.PositionServiceStub(channel)

    # agent coord
    pos_x = 125
    pos_y = 125
    # request = position_pb2.PositionRequest(enable= True, duration = 1000)
    running = True


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
    pygame.quit()

if __name__ == "__main__":
    main()