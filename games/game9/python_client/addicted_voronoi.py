import math
from first_strategy import STONE, FIRST_STRATEGY

class VORONOI_STUFF:
    def __init__(self, num_players : int, num_stone: int, player_num : int):
        self.grid_size = 1000
        self.min_dist = 66
        self.num_players, self.num_stone, self.player_number = num_players, num_stone, player_num
        self.grid = [[0] * self.grid_size for i in range(self.grid_size)]
        self.moves = []
        self.stones = [[],[]]
        self.strategy = FIRST_STRATEGY(60, self.num_stone)
    
    def log_move(self, move):
        move_x, move_y, player_num = move
        self.stones[player_num].append(STONE(move_x, move_y))
        self.strategy.update_pull(self.stones, player_num)
    
    def get_move(self):
        return self.strategy.move(self.stones)