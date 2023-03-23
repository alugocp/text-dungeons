"""
Procedural dungeon generation module.
"""
import random
import copy
import networkx
from matplotlib import pyplot
ROOMS = 8 # Max rooms in dungeon
MAX_DOORS = 3 # Max doorways per room
LINEARITY = 0.15 # Forced linearity rate

def draw(filepath, title, edges):
    """
    Generates a visualization of the given graph.
    """
    graph = networkx.DiGraph()
    for parent, children in edges.items():
        for child in children:
            graph.add_edge(parent, child)
    networkx.draw_networkx(graph)
    pyplot.title(title)
    pyplot.savefig(filepath)
    pyplot.show()

def has_path(edges, src, dst):
    """
    Returns True if the network contains a path from src to dst.
    """
    explore = [src]
    while len(explore) > 0:
        if dst in explore:
            return True
        node = explore.pop(0)
        explore += edges[node]
    return False

def get_parent(edges, child):
    """
    Grabs the parent node for a given node.
    """
    for parent, children in edges.items():
        if child in children:
            return parent
    return None

def procgen():
    """
    Runs the procedural generation algorithm.
    """

    # Generate layout tree
    boss_room = None
    layout = {'Room 1': []}
    while len(layout.keys()) < ROOMS:
        parent = random.choice(list(filter(lambda k: len(layout[k]) < MAX_DOORS, layout.keys())))
        new_room = f'Room {len(layout.keys()) + 1}'
        layout[parent].append(new_room)
        layout[new_room] = []
    boss_room = random.choice(list(filter(lambda k: len(layout[k]) == 0, layout.keys())))
    draw('procgen/layout.jpg', 'Dungeon room layout', layout)

    # Generate traversal order
    traversal = {'Room 1': []}
    unexplored = copy.deepcopy(layout['Room 1'])
    while len(unexplored) > 0:
        child = unexplored.pop(0)
        unexplored += layout[child]
        if random.random() < LINEARITY:
            parent = get_parent(layout, child)
        else:
            parent = random.choice(list(filter(
                    lambda k: k != boss_room and len(traversal[k]) < 2,
                    traversal.keys())))
        traversal[parent].append(child)
        traversal[child] = []
        if not has_path(traversal, get_parent(layout, child), child):
            traversal[parent].append(child)
    for parent, children in traversal.items():
        if len(children) == 0 and parent != boss_room:
            child = random.choice(list(filter(
                lambda k: not has_path(traversal, k, parent),
                traversal.keys())))
            children.append(child)
    draw('procgen/traversal.jpg', 'Dungeon room traversal', traversal)

if __name__ == '__main__':
    procgen()
