from numpy import Inf
from debug_print import print_func
from data_structures import *
from io_parser import io_parser
import random

class hunter:
    INT_MAX = 2147483647

    def __init__(self):
        self.__setup_done = False
    
    def setup(self, parsed_data : dict) -> None:
        if self.__setup_done: return
        self.__setup_done = True
        self.max_walls : int = parsed_data[DATA.MAX_WALLS]
        self.delay : int = parsed_data[DATA.WALL_DELAY]
        self.board_dims : coord = coord(parsed_data[DATA.BOARD_SIZE_X], parsed_data[DATA.BOARD_SIZE_Y])
    
    def update(self, parsed_data : dict) -> None:
        self.walls : list() = parsed_data[DATA.WALL_DATA]
        self.location : coord = parsed_data[DATA.HUNTER_DATA]
        self.prey_location : coord = parsed_data[DATA.PREY_LOC]
        self.wall_timer : int = parsed_data[DATA.WALL_TIMER]
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
    
    def is_prey_ahead(self):
        diff = [self.prey_location.x - self.location.x, self.prey_location.y - self.location.y]
        return diff[0] * self.location.vx > 0 or diff[1] * self.location.vy > 0

    def does_wall_interpose(self, wall : wall):
        loc, prey_loc = (self.location.y, self.prey_location.y) if wall.type == wall_type.HORIZONTAL \
                else (self.location.x, self.prey_location.x)
        return loc < wall.coord < prey_loc or prey_loc < wall.coord < loc

    def check_interposing_walls(self):
        present = False
        for wall in self.walls:
            if present: return present
            present = self.does_wall_interpose(wall)
        return present

    def is_wall_there_or_out_of_range(self, coord):
        if coord.x < 0 or coord.x >= self.board_dims.x or coord.y < 0 or coord.y >= self.board_dims.y:
            return True
        
        cur_wall : wall
        for cur_wall in self.walls:
            if (cur_wall.occupies(coord)):
                return True

        return False
    
    def prey_area_remaining(self, walls : list) -> int:
        left, right, top, down = 0, self.board_dims.x - 1, self.board_dims.y - 1, 0
        cur_wall : wall
        for cur_wall in walls:
            if cur_wall.type == wall_type.HORIZONTAL:
                dist = cur_wall.coord - self.prey_location.y
                met = cur_wall.start <= self.prey_location.x <= cur_wall.end
                
                if met:
                    if dist < 0 and abs(dist) < abs(down - self.prey_location.y):
                        down = cur_wall.coord
                    elif dist > 0 and dist <  abs(top - self.prey_location.y):
                        top = cur_wall.coord
            elif cur_wall.type == wall_type.VERTICAL:
                dist = cur_wall.coord - self.prey_location.x
                met = cur_wall.start <= self.prey_location.y <= cur_wall.end

                if met:
                    if dist < 0 and abs(dist) < abs(left - self.prey_location.x):
                        left = cur_wall.coord
                    elif dist > 0 and dist < abs(right - self.prey_location.x):
                        right = cur_wall.coord

        return abs(top - down) * abs(left - right)
    
    def new_vertical_wall(self):
        greater : coord = coord(self.location.x, self.location.y)
        lesser : coord = coord(self.location.x, self.location.y)

        while not self.is_wall_there_or_out_of_range(greater):
            if greater.equals(self.prey_location):
                return False
            greater.y += 1

        while not self.is_wall_there_or_out_of_range(lesser):
            if lesser.equals(self.prey_location):
                return False
            lesser.y -= 1

        return wall(wall_type.VERTICAL, self.location.x, lesser.y, greater.y)

    def new_horizontal_wall(self):
        greater : coord = coord(self.location.x, self.location.y)
        lesser : coord = coord(self.location.x, self.location.y)

        while not self.is_wall_there_or_out_of_range(greater):
            if greater.equals(self.prey_location):
                return False
            greater.x += 1

        while not self.is_wall_there_or_out_of_range(lesser):
            if lesser.equals(self.prey_location):
                return False
            lesser.x -= 1

        return wall(wall_type.HORIZONTAL, self.location.y, lesser.x, greater.x)

    def clear_and_build(self):
        clear_walls = list()
        move = dict()
        for i in range(len(self.walls)):
            cur_wall : wall = self.walls[i]
            diff = abs(cur_wall.coord - self.location.y) if cur_wall.type == wall_type.HORIZONTAL else abs(cur_wall.coord - self.location.x)
            
            if self.does_wall_interpose(cur_wall) and diff <= 2: clear_walls.append(i)

        move[DATA.DELETE_WALL] = list()
        move[DATA.ADD_WALL] = wall_type.NOWALL
        if len(clear_walls) == 0:
            return move

        move[DATA.DELETE_WALL] = clear_walls

        j = 0
        for i in clear_walls:
            self.walls.pop(i - j)
            j += 1

        vertical_wall = self.new_vertical_wall()
        horizontal_wall = self.new_horizontal_wall()
        vertical_area = self.INT_MAX
        horizontal_area = self.INT_MAX
        if vertical_wall != False:
            self.walls.append(vertical_wall)
            vertical_area = self.prey_area_remaining(self.walls)
            self.walls.pop()
        if horizontal_wall != False:
            self.walls.append(horizontal_wall)
            horizontal_area = self.prey_area_remaining(self.walls)
            self.walls.pop()
        
        if vertical_area == self.INT_MAX and horizontal_area == self.INT_MAX:
            move[DATA.ADD_WALL] = wall_type.NOWALL
        else:
            if vertical_area < horizontal_area:
                move[DATA.ADD_WALL] = wall_type.VERTICAL
                self.walls.append(vertical_wall)
            else:
                move[DATA.ADD_WALL] = wall_type.HORIZONTAL
                self.walls.append(horizontal_wall)

        return move
    
    def is_cooldown_active(self):
        return int(self.wall_timer) > 0
    
    def is_this_good_time_for_wall(self) -> dict:
        move = dict()
        if self.is_cooldown_active():
            move[DATA.ADD_WALL] = wall_type.NOWALL
            return move
        
        vertical_wall = self.new_vertical_wall()
        horizontal_wall = self.new_horizontal_wall()
        vertical_area = self.INT_MAX
        horizontal_area = self.INT_MAX
        current_area = self.prey_area_remaining(self.walls)
        if vertical_wall != False:
            self.walls.append(vertical_wall)
            vertical_area = self.prey_area_remaining(self.walls)
            self.walls.pop()
        if horizontal_wall != False:
            self.walls.append(horizontal_wall)
            horizontal_area = self.prey_area_remaining(self.walls)
            self.walls.pop()
        
        new_minimum_area = min(vertical_area, horizontal_area)

        if new_minimum_area > current_area:
            move[DATA.ADD_WALL] = wall_type.NOWALL
            return move
        elif new_minimum_area == horizontal_area:
            self.walls.append(horizontal_wall)
            move[DATA.ADD_WALL] = wall_type.HORIZONTAL
        else:
            self.walls.append(vertical_wall)
            move[DATA.ADD_WALL] = wall_type.VERTICAL
        return move
    
    def remove_walls(self):
        current_area = self.prey_area_remaining(self.walls)

        walls_remove = list()

        for i in range(len(self.walls)):
            area = self.prey_area_remaining(self.walls[:i] + self.walls[i + 1: ])
            if area == current_area:
                walls_remove.append(i)

        return walls_remove
    
    def move_ahead(self, parsed_data : dict) -> dict:
        flag  = False
        move = dict()
        if self.check_interposing_walls():
            move = self.clear_and_build()
            flag = True
        else:
            move = self.is_this_good_time_for_wall()

        if len(self.walls) >= self.max_walls and not flag:
            move[DATA.DELETE_WALL] = self.remove_walls()

        return move
    
    def move_behind(self, parsed_data) -> dict:
        move = self.is_this_good_time_for_wall()

        if len(self.walls) >= self.max_walls:
            move[DATA.DELETE_WALL] = self.remove_walls()

        return move

    def my_move(self, parsed_data : dict) -> dict:
        move = dict()
        if self.is_prey_ahead():
            move = self.move_ahead(parsed_data)
        else:
            move = self.move_behind(parsed_data)

        move[DATA.GAME_NUM] = parsed_data[DATA.GAME_NUM]
        move[DATA.TICK_NUM] = parsed_data[DATA.TICK_NUM]

        if DATA.DELETE_WALL not in move.keys():
            move[DATA.DELETE_WALL] = list()
        return move

    def get_move(self, parsed_data : dict) -> str:
        self.setup(parsed_data)
        self.update(parsed_data)
        # return io_parser.parse_output(player.HUNTER, self.random_move(parsed_data))
        return io_parser.parse_output(player.HUNTER, self.my_move(parsed_data))