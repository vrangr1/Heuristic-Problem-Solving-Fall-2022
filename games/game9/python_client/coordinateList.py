import math
from coordinate import Coordinate
my_coordinate_list = []

def get_coordinate_list():
	global my_coordinate_list
	if(len(my_coordinate_list)!=0):
		return my_coordinate_list
	xPrev = -1
	for y in range(0,67):
		x = math.ceil(math.sqrt(4356 - y ** 2))
		x = int(x)
		if(x != xPrev):
			my_coordinate_list.append(Coordinate(x, y))
		xPrev = x
	for pair in reversed(my_coordinate_list):
		x = pair.x * -1
		y = pair.y
		if(x!=0):
			my_coordinate_list.append(Coordinate(x,y))

	for pair in my_coordinate_list[:]:
		x = pair.x
		y = pair.y * -1
		if(y!=0):
			my_coordinate_list.append(Coordinate(x,y))

	return my_coordinate_list

m = get_coordinate_list()