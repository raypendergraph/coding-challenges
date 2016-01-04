__author__ = 'ray.pendergraph'

from collections import namedtuple

Point = namedtuple('Point', ['x','y'])

def quick_hull(points):
    sorted_points = sorted(points, key=lambda p: p.x)
