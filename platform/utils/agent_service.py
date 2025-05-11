from network import position_pb2
from network import position_pb2_grpc
import grpc


class AgentService:
    def __init__(self, enable: bool, duration: int):
        self._channel = grpc.insecure_channel("localhost:50051")
        self._stub = position_pb2_grpc.PositionServiceStub(self._channel)
        self._enable = enable
        self._duration = duration
    
    def makeRequest(self) -> position_pb2.PositionRequest:
        request = position_pb2.PositionRequest(enable=self._enable, duration=self._duration)
        return request
    
    def getResponse(self, request) -> position_pb2_grpc:
        response = self._stub.StreamPosition(request)
        return response

    def runAgentService(self):
        request = self.makeRequest()
        response = self.getResponse(request)
        return response