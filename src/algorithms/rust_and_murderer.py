__author__ = 'ray.pendergraph'
from sys import maxsize
from math import ceil
from collections import OrderedDict

def index_of_node(node_id):
    return node_id - 1

def id_of_index(node_idx):
    return node_idx + 1

class AdjacencyMatrix(object):
    def __init__(self, rows, columns):
        self._slot_width = maxsize.bit_length()
        self._rows = rows
        self._columns = columns
        row_slots = ceil(rows / self._slot_width)
        column_slots = ceil(columns / self._slot_width)
        self._matrix = [maxsize] * (row_slots * column_slots)

    def __setitem__(self, key, value):
        row, column = key
        index, bit = self._calculate_offsets(row, column)
        if value:
            self._matrix[index] |= (1 << bit)
        else:
            self._matrix[index] &= ~(1 << bit)

    def __getitem__(self, item):
        row, column = item
        return self._is_set(row, column)

    def _is_set(self, row, column):
        index, bit = self._calculate_offsets(row, column)
        print("isset ", row, column, bool(self._matrix[index] & (1 << bit)))
        return bool(self._matrix[index] & (1 << bit))

    def _calculate_offsets(self, row, column):
        index = (row * self._columns) + column
        slot_index, bit = divmod(index, self._slot_width)
        return slot_index, bit

    def get_adjacent_node_indices(self, node_idx):
        return [c for c in range(self._columns) if self._is_set(node_idx, c)]

def find_distances(count, start_idx, matrix):
    index_queue = OrderedDict()
    distances = [-1] * count
    index_queue[start_idx] = None
    last_idx = start_idx
    while index_queue:
        idx, _ = index_queue.popitem(last=False)
        distances[idx] = distances[last_idx] + 1
        for _ in matrix.get_adjacent_node_indices(idx):
            index_queue[_] = None
        last_idx = idx
    return distances

# T - Test cases
# M - edges
# N - nodes
# T = int(input().strip())
# for test_case in range(T):
#     N, M = [int(_) for _ in input().split()]
#     matrix = AdjacencyMatrix(N, N)
#     for node_a, node_b in [[int(x) for x in input().split()] for i in range(M)]:
#         matrix[(index_of_node(node_a), index_of_node(node_b))] = False
#         matrix[(index_of_node(node_b), index_of_node(node_a))] = False
#     S = int(input())
#     print(" ".join(str(distance) for distance in find_distances(N, index_of_node(S), matrix) ))
