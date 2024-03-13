import time
import websocket
import pygame

pygame.init()

# Initialize the joystick
pygame.joystick.init()

# Check for connected joysticks
joystick_count = pygame.joystick.get_count()
if joystick_count < 1:
    print("No joysticks found.")
    pygame.quit()
    quit()

# Use the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Initialize x and y
x = 0.0
y = 0.0
z = 0.0

def applyDeadband(val):
    if (abs(val) < 0.05):
        val = 0.0
    
    return round(val, 2)



# Set the ESP8266 IP address and port
HOST = '192.168.4.1'  # Replace with the actual IP address of your ESP8266
PORT = 81  # Use the port your ESP8266 WebSocket server is listening on

# Connect to the WebSocket server
ws = websocket.create_connection(f'ws://{HOST}:{PORT}')


while True:
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            if event.axis == 0:
                x = applyDeadband(event.value)
            elif event.axis == 1:
                y = applyDeadband(event.value)
            elif event.axis == 2:
                z = applyDeadband(event.value)

            ws.send(f'X{x}Y{y}Z{z}')
            print(f'X{x}Y{y}Z{z}')
        elif event.type == pygame.JOYBUTTONDOWN:
            ws.close()
        
    time.sleep(0.02)
