import math

class Coordinate:
    def __init__(self, x : int, y : int):
        self.x = int(x)
        self.y = int(y)

    def get_x(self):
        return self.x
    def get_y(self):
        return self.y
    def set_x(self, x):
        self.x = x
    def set_y(self, y):
        self.y = y

    def get_distance(self, loc):
        return math.sqrt((self.x - loc.x) * (self.x - loc.x) + (self.y - loc.y) * (self.y - loc.y))
