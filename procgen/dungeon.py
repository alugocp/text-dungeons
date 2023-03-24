"""
Procedural dungeon generation module.
This algorithm generates a dungeon's physical layout as well as the required
traversal of its rooms in order to complete it. Boh of these results are represented
by directed graphs.
"""
import random
import math
import json
import copy
import sys
import re
import os
from typing import Tuple, Dict, List
import networkx
from matplotlib import pyplot
ROOMS = 8 # Max rooms in dungeon
MAX_DOORS = 3 # Max doorways per room
LINEARITY = 0.15 # Forced linearity rate
Edges = Dict[str, List[str]]

def draw(filepath: str, title: str, edges: Edges) -> None:
    """
    Generates a visualization of the given graph.
    """
    graph = networkx.DiGraph()
    for parent, children in edges.items():
        for child in children:
            graph.add_edge(re.sub('^Room ', '', parent), re.sub('^Room ', '', child))
    pos = networkx.spring_layout(graph, k = 5 / math.sqrt(graph.order()))
    networkx.draw_networkx(graph, pos = pos, node_size = 200)
    pyplot.title(title)
    pyplot.savefig(filepath)
    pyplot.show()

def write_dungeon(dungeon: Edges, boss_room: str) -> None:
    """
    Writes dungeon layout information to the file system.
    """
    with open('procgen/dungeon.json', 'w', encoding = 'utf-8') as file:
        file.write(json.dumps({'dungeon': dungeon, 'boss_room': boss_room}))

def read_dungeon() -> Tuple[Edges, str]:
    """
    Reads dungeon layout information from the file system.
    """
    with open('procgen/dungeon.json', 'r', encoding = 'utf-8') as file:
        data = json.loads(file.read())
        return data['dungeon'], data['boss_room']

def has_path(edges: Edges, src: str, dst: str) -> bool:
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

def get_parent(edges: Edges, child: str) -> str:
    """
    Grabs the parent node for a given node.
    """
    for parent, children in edges.items():
        if child in children:
            return parent
    return None

def generate_layout() -> Tuple[Edges, str]:
    """
    Generate a dungeon's layout tree.
    """
    print('Generating dungeon layout...')
    boss_room = None
    layout = {'Room 1': []}
    num_rooms = int(os.environ['PROCGEN_ROOMS']) if os.environ['PROCGEN_ROOMS'] else ROOMS
    while len(layout.keys()) < num_rooms:
        parent = random.choice(list(filter(lambda k: len(layout[k]) < MAX_DOORS, layout.keys())))
        new_room = f'Room {len(layout.keys()) + 1}'
        layout[parent].append(new_room)
        layout[new_room] = []
    boss_room = random.choice(list(filter(lambda k: len(layout[k]) == 0, layout.keys())))
    draw('procgen/layout.jpg', 'Dungeon room layout', layout)
    write_dungeon(layout, boss_room)
    return layout, boss_room

def generate_traversal(layout: Edges, boss_room: str) -> None:
    """
    Generate a dungeon's traversal tree.
    """
    print('Generating dungeon traversal...')
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

def main():
    """
    Driver code for the CLI.
    """
    if len(sys.argv) > 1 and sys.argv[1] == 'traversal':
        edges, boss_room = read_dungeon()
    else:
        edges, boss_room = generate_layout()
    generate_traversal(edges, boss_room)

if __name__ == '__main__':
    main()
