from debug_print import print_func
from data_structures import *
from io_parser import io_parser
import random

class hunter:
    def __init__(self):

        pass

    def random_move(self, parsed_data : dict) -> dict:
        wall = wall_type(random.randint(0,2))
        wall_delete = list()
        if random.randint(0,30) == 0:
            wall_delete += [0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
        move = dict()
        move[DATA.GAME_NUM] = parsed_data[DATA.GAME_NUM]
        move[DATA.TICK_NUM] = parsed_data[DATA.TICK_NUM]
        move[DATA.ADD_WALL] = wall
        move[DATA.DELETE_WALL] = wall_delete
        return move

    def get_move(self, parsed_data : dict) -> str:

        return io_parser.parse_output(player.HUNTER, self.random_move(parsed_data))