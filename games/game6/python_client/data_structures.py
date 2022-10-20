from enum import Enum

class coord:
    def __init__(self, x, y, vx = None, vy = None):
        self.x = int(x)
        self.y = int(y)

        self.vy = vy if vy == None else int(vy)
        self.vx = vx if vx == None else int(vx)

        self.direction = [self.vx, self.vy]

    def equals(self, coord):
        return self.x == coord.x and self.y == coord.y

class wall_type(Enum):
    HORIZONTAL = 0
    VERTICAL = 1

class wall:
    def __init__(self, wallType : int, coord : coord, start : int, end : int):
        # 0 for horizontal, 1 for vertical
        self.type = wall_type(wallType)
        self.coord = coord
        self.start = start
        self.end = end

    def occupies(self, coord : coord):
        if self.type == wall_type.HORIZONTAL:
            return coord.y == self.coord and self.start <= coord.x and self.end >= coord.x
        elif self.type == wall_type.VERTICAL:
            return coord.x == self.coord and self.start <= coord.y and self.end >= coord.y

        # We should not be here technically, just in case
        return 0