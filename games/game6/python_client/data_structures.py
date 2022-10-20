from enum import Enum

class DATA(Enum):
    __order__ = "TIME_LEFT GAME_NUM TICK_NUM MAX_WALLS WALL_DELAY BOARD_SIZE_X BOARD_SIZE_Y WALL_TIMER WALL_COUNT WALL_DATA HUNTER_DATA PREY_LOC ADD_WALL DELETE_WALL PREY_MOVEMENT"
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
    ADD_WALL = 18
    DELETE_WALL = 19
    PREY_MOVEMENT = 20

class player(Enum):
    HUNTER = 21
    PREY = 22

class wall_type(Enum):
    HORIZONTAL = 0
    VERTICAL = 1
    NOWALL = 2

class coord:
    def __init__(self, x, y, vx = None, vy = None):
        self.x = int(x)
        self.y = int(y)

        self.vy = vy if vy == None else int(vy)
        self.vx = vx if vx == None else int(vx)

        self.direction = [self.vx, self.vy]

    def equals(self, coord):
        return self.x == coord.x and self.y == coord.y


class wall:
    def __init__(self, wallType : int, coord : int, start : int, end : int, index : int):
        # 0 for horizontal, 1 for vertical
        self.type = wall_type(wallType)
        self.coord = coord
        self.start = start
        self.end = end
        self.index = index

    def occupies(self, coord : coord):
        if self.type == wall_type.HORIZONTAL:
            return coord.y == self.coord and self.start <= coord.x and self.end >= coord.x
        elif self.type == wall_type.VERTICAL:
            return coord.x == self.coord and self.start <= coord.y and self.end >= coord.y

        # We should not be here technically, just in case
        return 0