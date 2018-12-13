'''
PyROUTING function

'''
import noxim as nm
DIRECTION_NORTH=0
DIRECTION_EAST=1
DIRECTION_SOUTH=2
DIRECTION_WEST=3
DIRECTION_LOCAL=4
DIRECTION_HUB=5

#DIRECTION_WIRELESS=747

def route(route_data):
    current = nm.id2Coord(route_data.current_id)
    destination = nm.id2Coord(route_data.dst_id)

    directions=[]
    if destination.x > current.x:
        directions.append(DIRECTION_EAST)
    elif destination.x < current.x:
        directions.append(DIRECTION_WEST)
    elif destination.y > current.y:
        directions.append(DIRECTION_SOUTH)
    else:
        directions.append(DIRECTION_NORTH)
    
    return directions

