import requests
import json

"""
How this will work?
1. In terminal one, you should run -> ollama run llama3.2
2. In terminal two, run this script

We are already ALWAYS running ollama on 11434, so you know
dont need to `run ollama serve` cause it will error
"""


prompt = "Suggest a robot goal for picking up objects in a messy room."

res = requests.post(
    "http://localhost:11434/api/generate",
    json={
        "model": "llama3.2",
        "prompt": prompt
    }
)

response_text = ""

for line in res.iter_lines():
    if line:
        try:
            data = line.decode('utf-8')
            chunk = json.loads(data)
            response_text += chunk.get("response", "")
        except Exception as e:
            print(f"Error decoding line: {e}")

print("Full Response:\n", response_text)