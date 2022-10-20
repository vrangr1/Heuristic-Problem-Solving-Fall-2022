from enum import Enum
# import data_structures
from data_structures import *

class DATA(Enum):
    __order__ = "TIME_LEFT GAME_NUM TICK_NUM MAX_WALLS WALL_DELAY BOARD_SIZE_X BOARD_SIZE_Y WALL_TIMER HUNTER_X HUNTER_Y HUNTER_XV HUNTER_XY PREY_X PREY_Y WALL_COUNT WALL_DATA"
    TIME_LEFT = 0
    GAME_NUM  = 1
    TICK_NUM = 2
    MAX_WALLS = 3
    WALL_DELAY = 4
    BOARD_SIZE_X = 5
    BOARD_SIZE_Y = 6
    WALL_TIMER = 7
    # HUNTER_X = 8
    # HUNTER_Y = 9
    # HUNTER_XV = 10
    # HUNTER_YV = 11
    # PREY_X = 12
    # PREY_Y = 13
    WALL_COUNT = 14
    WALL_DATA = 15
    HUNTER_DATA = 16
    PREY_LOC = 17



class io_parser:
    @staticmethod
    def parse_input(input_list) -> dict:
        data = dict()
        print(input_list)
        return dict()
        input_list = [int(x) for x in input_list]
        for data_item in DATA:
            if data_item == DATA.WALL_DATA:
                break
            data[data_item] = input_list[data_item.value]
        data[DATA.HUNTER_DATA] = coord(input_list[8], input_list[9], input_list[10], input_list[11])
        data[DATA.PREY_LOC] = coord(input_list[12], input_list[13])
        input_list = input_list[DATA.WALL_DATA.value:]
        assert len(input_list) == data[DATA.WALL_COUNT], "The count of walls mentioned has to "
        data[DATA.WALL_DATA] = list()
        for i in range(0, data[DATA.WALL_COUNT]):
            data[DATA.WALL_DATA].append(
                wall(
                    input_list[15 + i * 4], input_list[16 + i * 4], int(input_list[17 + i * 4]), int(input_list[18 + i * 4])
                    ))
        return data