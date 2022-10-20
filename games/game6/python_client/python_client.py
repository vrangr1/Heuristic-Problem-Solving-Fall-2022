import sys
# import game_player
from game_player import game_player

if __name__ == '__main__':
    port = int(sys.argv[1])
    game_runner = game_player(port)
    game_runner.run()