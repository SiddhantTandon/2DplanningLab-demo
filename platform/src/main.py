from argparse import ArgumentParser
from src.agent_service import ShareStateService
import threading
import time
import logging
import tracemalloc

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.StreamHandler()
    ]
)

logger = logging.getLogger(__name__)

tracemalloc.start()
start_time = time.time()

def on_exit():
    end_time = time.time()
    total_time = end_time - start_time

    current, peak = tracemalloc.get_traced_memory()

    logger.info(f"Program run time: {total_time:.2f} seconds")
    logger.info(f"Current memory: {current / 1024**2:.2f} MB; Peak memory: {peak / 1024**2:.2f} MB")

    # Flush all log handlers
    for handler in logger.handlers:
        handler.flush()

    print("Shutdown hook executed. Metrics logged.")

def main():
    parser = ArgumentParser()
    parser.add_argument("--map", required=True, help="Give a maps file, the path should be relative to platform directory -> ../maps/map_5x5_simple.txt")
    parser.add_argument("--just_map", help="This is a boolean flag, if used program only renders the map", action='store_true')
    parser.add_argument("--warm_start", help="Give the warm start period for robot to get data from map")

    args = parser.parse_args()

    service = ShareStateService(workers=10, port=50051, args=args)
    if not args.just_map:
        # Start gRPC server in a background daemon thread -> this will ensure that the port closes after pygame window is closed
        grpc_thread = threading.Thread(target=service.run_server, daemon=True)
        grpc_thread.start()

    try:
        # Start simulation pygame in main thread
        service.start_sim()
        print("Simulation ended. Main thread exiting.")
    finally:
        # Ensure cleanup happens no matter how the program exits
        if not args.just_map:
            service.stop_server()
            time.sleep(0.5)
        on_exit()

if __name__ == "__main__":
    main()

'''
For this file - we can send map message protos, with some type that is 
unique for loading. Once the loading (setting up maps, potential fields etc)
is finished then we will commence the main sim loop for protobuf sharing .
'''