import numpy as np
from itertools import islice, chain
import sys


def index_of_node(node_id):
    return node_id - 1


def id_of_index(node_idx):
    return node_idx + 1


def find_distances(relationships, start_idx):
    node_count = len(relationships)
    distances = [-1] * node_count
    not_visited = [np.array(range(node_count))]

    def recur(idx, distance_of_idx):
        distances[idx] = distance_of_idx
        related = np.array(relationships[idx])
        complement = np.setdiff1d(not_visited[0], related)
        not_visited[0] = np.setdiff1d(not_visited[0], complement)
        for complement_idx in complement:
            relationship_distance = distance_of_idx + 1
            distances[complement_idx] = relationship_distance
            recur(complement_idx, relationship_distance)

    recur(start_idx, 0)
    return distances

# T - Test cases
# M - edges
# N - nodes
def main():
    T = int(input().strip())
    for test_case in range(T):
        N, M = [int(_) for _ in input().split()]
        relationships = [set() for _ in range(N)]
        for node_a, node_b in [[int(x) for x in input().split()] for i in range(M)]:
            relationships[index_of_node(node_a)].add(index_of_node(node_b))
            relationships[index_of_node(node_b)].add(index_of_node(node_a))
        S = int(input())

        distances = find_distances(relationships, index_of_node(S))
        pre = islice((_ for _ in distances), 0, index_of_node(S))
        post = islice((_ for _ in distances), S, len(distances))
        print(*chain (pre, post), sep=" ", end="\n", file=sys.stdout)
main()