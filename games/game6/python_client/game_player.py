import socket
import time
from io_parser import *
from hunter import hunter
from prey import prey

class game_player:
    HOST = "localhost"
    TEAM_NAME = "Kitkat Addicts"
    def __init__(self, port):
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.HOST, self.port))
        self.player_type = player.HUNTER
        self.player = None
    
    def run(self):
        stream = ""
        while True:
            while True:
                recv = self.sock.recv(4096)
                stream = stream + recv.decode()
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
                self.player_type = player.HUNTER
                self.player = hunter()
            elif line == "prey":
                self.player_type = player.PREY
                self.player = prey()
            elif line == "sendname":
                to_send = self.TEAM_NAME
            else:
                data = line.split(" ")
                parsed_data = io_parser.parse_input(data)
                to_send = self.player.get_move(parsed_data)

            if to_send is not None:
                print("sending: " + to_send)
                self.sock.sendall((to_send + "\n").encode())
