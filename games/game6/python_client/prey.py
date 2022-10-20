from debug_print import print_func
from data_structures import *
from io_parser import io_parser
import random
import math

class prey:
    def __init__(self):
        self.turn_number = 0
        self.__setup_done = False
    
    def setup(self, parsed_data : dict) -> None:
        if self.__setup_done: 
            return
        self.__setup_done = True
        self.max_walls : int = parsed_data[DATA.MAX_WALLS]
        self.delay : int = parsed_data[DATA.WALL_DELAY]
        self.board_dims : coord = coord(parsed_data[DATA.BOARD_SIZE_X], parsed_data[DATA.BOARD_SIZE_Y])
    
    def update(self, parsed_data : dict) -> None:
        self.walls : list() = parsed_data[DATA.WALL_DATA]
        self.hunter_location : coord = parsed_data[DATA.HUNTER_DATA]
        self.prey_location : coord = parsed_data[DATA.PREY_LOC]
        self.wall_timer : int = parsed_data[DATA.WALL_TIMER]

    def random_move(self, parsed_data : dict) -> dict:
        move = dict()
        x = random.randint(-1,1)
        y = random.randint(-1,1)
        move[DATA.PREY_MOVEMENT] = coord(x, y)
        
        
        move[DATA.GAME_NUM] = parsed_data[DATA.GAME_NUM]
        move[DATA.TICK_NUM] = parsed_data[DATA.TICK_NUM]
        return move
    
    def my_move(self, parsed_data : dict) -> dict:
        move = dict()
        dx, dy = 0, 0

        if math.hypot(self.prey_location.x - self.hunter_location.x, self.prey_location.y - self.hunter_location.y) < 12:
            if self.hunter_location.vx == 1 and self.hunter_location.vy == -1:
                if self.prey_location.x > self.hunter_location.x and self.prey_location.y < self.hunter_location.y:
                    dx = -1 if abs(self.prey_location.x - self.hunter_location.x) < abs(self.prey_location.y - self.hunter_location.y) else 1
                    dy = -1 if abs(self.prey_location.x - self.hunter_location.x) < abs(self.prey_location.y - self.hunter_location.y) else 1
                else:
                    dx, dy = -1, 1
            elif self.hunter_location.vx == 1 and self.hunter_location.vy == 1:
                if self.prey_location.x > self.hunter_location.x and self.prey_location.y > self.hunter_location.y:
                    dx = -1 if abs(self.prey_location.x - self.hunter_location.x) < abs(self.prey_location.y - self.hunter_location.y) else 1
                    dy = 1 if abs(self.prey_location.x - self.hunter_location.x) < abs(self.prey_location.y - self.hunter_location.y) else -1
                else:
                    dx, dy = -1, -1
            elif self.hunter_location.vx == -1 and self.hunter_location.vy == 1:
                if self.prey_location.x < self.hunter_location.x and self.prey_location.y > self.hunter_location.y:
                    dx = -1 if abs(self.prey_location.x - self.hunter_location.x) > abs(self.prey_location.y - self.hunter_location.y) else 1
                    dy = -1 if abs(self.prey_location.x - self.hunter_location.x) > abs(self.prey_location.y - self.hunter_location.y) else 1
                else:
                    dx, dy = 1, -1
            elif self.hunter_location.vx == -1 and self.hunter_location.vy == -1:
                if self.prey_location.x < self.hunter_location.x and self.prey_location.y < self.hunter_location.y:
                    dx = -1 if abs(self.prey_location.x - self.hunter_location.x) > abs(self.prey_location.y - self.hunter_location.y) else 1
                    dy = 1 if abs(self.prey_location.x - self.hunter_location.x) > abs(self.prey_location.y - self.hunter_location.y) else -1
                else:
                    dx, dy = 1, 1
        else:
            dx = -1 if self.prey_location.x > self.hunter_location.x else 1
            dy = -1 if self.prey_location.y > self.hunter_location.y else 1
        
        move[DATA.PREY_MOVEMENT] = coord(dx, dy)
        
        
        move[DATA.GAME_NUM] = parsed_data[DATA.GAME_NUM]
        move[DATA.TICK_NUM] = parsed_data[DATA.TICK_NUM]
        return move

    def get_move(self, parsed_data : dict) -> str:
        self.setup(parsed_data)
        self.update(parsed_data)
        ## Currently sending back random move
        return io_parser.parse_output(player.PREY, self.my_move(parsed_data))