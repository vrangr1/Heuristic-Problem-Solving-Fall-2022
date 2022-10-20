import sys
from game_player import *

if __name__ == '__main__':
    port = int(sys.argv[1])
    print_func("","","../python_client/debug.out") # for debug print
    game_runner = game_player(port)
    game_runner.run()