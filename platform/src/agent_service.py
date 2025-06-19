from network import state_message_pb2_grpc
from concurrent import futures
import grpc
from src.simulation_platform import SimulationPlatformViz


class ShareStateService(state_message_pb2_grpc.ShareStateServiceServicer):
    def __init__(self, workers=10, port=50051, args=""):
        self._num_workers = workers
        self._port = port
        self._sim_platform = SimulationPlatformViz(args)
        self._server = None

    def ShareState(self, request_iterator, context):
        for agent_message in request_iterator:
            self._sim_platform.getAgentResponseMessage(agent_message)
            map_message = self._sim_platform.sendMapResponseMessage()
            yield map_message

    def run_server(self):
        self._server = grpc.server(futures.ThreadPoolExecutor(max_workers=self._num_workers))
        state_message_pb2_grpc.add_ShareStateServiceServicer_to_server(self, self._server)
        self._server.add_insecure_port(f'[::]:{self._port}')
        self._server.start()
        print(f"Server started on port {self._port}")
        self._server.wait_for_termination()
    
    def start_sim(self):
        self._sim_platform.run()
    
    def stop_server(self):
        self._server.stop(0)