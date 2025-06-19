from google.protobuf.timestamp_pb2 import Timestamp
from network import state_message_pb2

def generate_map_message() -> state_message_pb2:
    return state_message_pb2.MapMessage()

def generate_agent_message() -> state_message_pb2:
    return state_message_pb2.AgentMessage()

def set_timestamp() -> Timestamp:
    time = Timestamp()
    time.GetCurrentTime()
    return time

def set_cell_state(row: int, col: int, value: str) -> state_message_pb2:
    cell_state = state_message_pb2.CellState()
    cell_state.position.row = row
    cell_state.position.col = col
    cell_state.value = value
    return cell_state

def set_agent_state(agent_id: str, agent_type: str) -> state_message_pb2:
    agent_update = state_message_pb2.AgentUpdate()
    agent_update.agent_id = agent_id
    agent_update.type = agent_type
    return agent_update

def generate_map_state() -> state_message_pb2:
    return state_message_pb2.MapState()