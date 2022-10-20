from debug_print import print_func
from data_structures import *
from io_parser import io_parser
import random

class prey:
    def __init__(self):

        pass

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
        x, y = 0, 0
        # Figure out x and y here in range(-1, 1) both inclusive
        
        
        
        move[DATA.PREY_MOVEMENT] = coord(x, y)
        
        
        move[DATA.GAME_NUM] = parsed_data[DATA.GAME_NUM]
        move[DATA.TICK_NUM] = parsed_data[DATA.TICK_NUM]
        return move

    def get_move(self, parsed_data : dict) -> str:
        ## Currently sending back random move
        return io_parser.parse_output(player.PREY, self.random_move(parsed_data))