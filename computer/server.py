import asyncio
import websockets

async def echo(websocket, path):
    message = await websocket.recv()
    print(f"Received message: {message}")
    
    await websocket.send(f"Server says: {message}")

async def main():
    server = await websockets.serve(echo, "localhost", 8080)
    
    # Run the server until it is stopped
    print("WebSocket server is running on ws://localhost:8080")
    await server.wait_closed()

# Start the server loop
if __name__ == "__main__":
    asyncio.run(main())