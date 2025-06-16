from argparse import ArgumentParser
from src.agent_service import ShareStateService
import threading

def main():
    parser = ArgumentParser()
    parser.add_argument("--map", required=True, help="Give a maps file, the path should be relative to platform directory -> ../maps/map_5x5_simple.txt")
    parser.add_argument("--just_map", help="This is a boolean flag, if used program only renders the map", action='store_true')
    
    args = parser.parse_args()

    service = ShareStateService(workers=10, port=50051, args=args)

    # Start gRPC server in a background daemon thread -> this will ensure that the port closes after pygame window is closed
    grpc_thread = threading.Thread(target=service.run_server, daemon=True)
    grpc_thread.start()

    # Start simulation (Pygam) as the main thread
    service.start_sim()

    print("Simulation ended. Main thread exiting.")


if __name__ == "__main__":
    main()