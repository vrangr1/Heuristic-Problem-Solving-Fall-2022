import random
import socket
import time
from enum import Enum
from io_parser import *

class player(Enum):
    HUNTER = 1
    PREY = 2

class game_player:
    HOST = "localhost"
    TEAM_NAME = "Kitkat Addicts"
    def __init__(self, port):
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.HOST, self.port))
        self.player = player.HUNTER
    
    def run(self):
        stream = ""
        while True:
            while True:
                stream = stream + self.sock.recv(4096)
                lines = stream.split("\n")
                if len(lines) > 1:
                    line = lines[-2]
                    stream = lines[-1]
                    break
                else:
                    continue

            print("received: " + line)

            val = .01
            time.sleep(val)

            to_send = None

            if line == "done":
                break
            elif line == "hunter":
                # hunter = True
                self.player = player.HUNTER
            elif line == "prey":
                # hunter = False
                self.player = player.PREY
            elif line == "sendname":
                # to_send = "random_player_" + str(port) #Put team name here
                to_send = self.TEAM_NAME
            else:
                data = line.split(" ")
                parsed_data = io_parser.parse_input(data)
                if self.player == player.HUNTER:
                    """
                    TODO - Place Hunter code here

                    The line read from server contains:
                        playerTimeLeft
                        gameNum - 0/1
                        tickNum
                        maxWalls
                        WallPlacementDelay
                        Board Size x
                        Board size y
                        currentWallTimer
                        hunter x pos
                        hunter y pos
                        hunter x vel
                        hunter y vel
                        prey x pos
                        prey y pos
                        numWalls
                        WallInfo1, WallInfo2 ...

                    To return:
                        gameNum
                        tickNum
                        WallsToAdd - 0,1,2,3,4
                        Indices of walls to be deleted, based on the game state sent by server
                    """
                    wall = "0"
                    to_send = data[1] + " " + data[2] + " " + wall
                else:
                    """
                    TODO - Place Prey code here

                    The line read from server contains:
                        playerTimeLeft
                        gameNum - 0/1
                        tickNum
                        maxWalls
                        WallPlacementDelay
                        Board Size x
                        Board size y
                        currentWallTimer
                        hunter x pos
                        hunter y pos
                        hunter x vel
                        hunter y vel
                        prey x pos
                        prey y pos
                        numWalls
                        WallInfo1, WallInfo2 ...


                    To return:
                        gameNum
                        tickNum
                        prey x movement
                        prey y movement
                    """
                    x = random.randint(-1,1)
                    y = random.randint(-1,1)
                    to_send = data[1] + " " + data[2] + " " + str(x) + " " + str(y)

            if to_send is not None:
                print("sending: " + to_send)
                self.sock.sendall(to_send + "\n")
        pass
