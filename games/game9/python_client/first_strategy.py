import math
import coordinateList
from grid import Grid
from coordinate import Coordinate

class STONE:
    __MIN_DIST_SQUARED = 66 * 66

    def __init__(self, x_coord : int, y_coord : int):
        self.x = x_coord
        self.y = y_coord

    def check_viability(self, stone) -> bool:
        if (stone.x - self.x) * (stone.x - self.x) + (stone.y - self.y) * (stone.y - self.y) >= self.__MIN_DIST_SQUARED:
            return True
        return False

    def get_distance(self, x : int, y : int) -> float:
        return math.sqrt((self.x - x) * (self.x - x) + (self.y - y) * (self.y - y))
    
    def get_squared_distance(self, x : int, y : int) -> int:
        return (self.x - x) * (self.x - x) + (self.y - y) * (self.y - y)

class FIRST_STRATEGY:
    def __init__(self, stride, nStones):
        self.JUMP = 6

        if nStones < 6:
            self.JUMP = 3

        if nStones > 15:
            self.JUMP = nStones - 8
        self.stride = stride
        self.grid = Grid(stride, self.JUMP)
        self.coordList = coordinateList.get_coordinate_list()

    def getAllTiles(self):
        locs = []
        for i in range(0, self.grid.WIDTH, self.stride):
            for j in range(0, self.grid.HEIGHT, self.stride):
                locs.append(Coordinate(i, j))

        return locs

    def getStoneTiles(self, stones):
        locs = []

        for k in range(0, 2):
            for stone in stones[k]:
                locs.append(self.getTile(stone.x, stone.y))

        return locs

    def getTile(self, x, y):
        return Coordinate(int(math.floor(x / self.stride) * self.stride), int(math.floor(y / self.stride) * self.stride))

    def getOpponentStones(self, stones):
        return stones[1]

    def getOpponentLastStone(self, stones):
        return stones[1][-1]

    def move(self, stones : list):
        num = 0
        i = 0
        j = 0

        if (len(stones[0]) == 0 and len(stones[1]) == 0):
            return self.grid.center()

        # Get last stone of opponent
        opponentLastStone : STONE
        opponentLastStone = stones[1][-1]
        if opponentLastStone.x + 66 < self.grid.WIDTH:
            finalCoord = Coordinate(opponentLastStone.x + 66, opponentLastStone.y)
        else:
            finalCoord = Coordinate(opponentLastStone.x - 66, opponentLastStone.y)


        for i in range(0, len(self.coordList), self.JUMP):
            coord = self.coordList[i]

            x = opponentLastStone.x + coord.x
            y = opponentLastStone.y + coord.y

            if x < 0 or y < 0 or x > self.grid.WIDTH or y > self.grid.HEIGHT:
                continue

            currStone = STONE(x, y)
            flag = 0

            if not opponentLastStone.check_viability(currStone):
                continue

            for l in range(0, 2):
                stone : STONE
                for stone in stones[l]:
                    if not stone.check_viability(currStone):
                        flag = 1
                        break
                if flag == 1:
                    break

            if flag == 1:
                continue


            stones[0].append(currStone)

            self.grid.set_color(stones)

            ret = self.grid.getColorDist()

            stones[0].pop()

            if (ret[0] > num):
                num = ret[0]
                finalCoord = Coordinate(currStone.x, currStone.y)

        return finalCoord

    def update_pull(self, stones, player):
        self.grid.update_pull(stones[player][-1], player)