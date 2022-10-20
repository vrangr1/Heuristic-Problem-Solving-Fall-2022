from enum import Enum
from http.client import PRECONDITION_FAILED
from data_structures import *
from debug_print import *

class io_parser:
    @staticmethod
    def parse_input(input_list : list) -> dict:
        data = dict()
        # print_func(input_list)
        # return dict()
        input_list = [int(x) for x in input_list]
        for data_item in DATA:
            if data_item == DATA.WALL_DATA:
                break
            data[data_item] = input_list[data_item.value]
        data[DATA.HUNTER_DATA] = coord(input_list[8], input_list[9], input_list[10], input_list[11])
        data[DATA.PREY_LOC] = coord(input_list[12], input_list[13])
        input_list = input_list[DATA.WALL_DATA.value:]
        assert len(input_list) == 4*data[DATA.WALL_COUNT], "Shouldn't have happened"
        data[DATA.WALL_DATA] = list()
        for i in range(0, data[DATA.WALL_COUNT]):
            data[DATA.WALL_DATA].append(
                wall(
                    input_list[0 + i * 4], input_list[1 + i * 4], int(input_list[2 + i * 4]), int(input_list[3 + i * 4]), i
                    ))
        return data
    
    @staticmethod
    def parse_output(player_type : player, output_dict : dict) -> str:
        """
            output_dict should have the following key, value pairs:
            DATA.GAME_NUM (enum) : int (read from parsed data dict received)
            DATA.TICK_NUM (enum) : int (read from parsed data dict received)

            For hunter:
            DATA.ADD_WALL (enum) : wall_type (enum)
            DATA.DELETE_WALL (enum) : list (of indices of walls to delete)

            For prey:
            DATA.PREY_MOVEMENT : coord (data structure; this should be relative movement.
                                        As such, -1 <= coord.x, coord.y <= 1; coord.x : int, coord.y: int)
            
        """
        output = []

        output.append(str(output_dict[DATA.GAME_NUM]))
        output.append(str(output_dict[DATA.TICK_NUM]))

        if player_type == player.HUNTER:
            walltype : wall_type = output_dict[DATA.ADD_WALL]
            walltype = 0 if walltype == wall_type.NOWALL else walltype.value + 1
            output.append(str(walltype))

            for i in output_dict[DATA.DELETE_WALL]:
                output.append(str(i))
        else:
            move : coord = output_dict[DATA.PREY_MOVEMENT]
            output.append(str(move.x))
            output.append(str(move.y))

        return " ".join(output)