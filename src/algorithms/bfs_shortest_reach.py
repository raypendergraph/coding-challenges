import collections

import io
fake_input = io.StringIO(""" """)


class Node:
    def __init__(self, index):
        self.index = index
        self.relationship_indexes = set()
        self.distance = -1
        self.previous_node = None

    def __str__(self):
        return "Node {}<previous={} distance={} relationships = [{}]>".format(
            str(self.index),
            str(self.previous_node.index if self.previous_node else "None"),
            str(self.distance),
            " ".join([str(_) for _ in self.relationship_indexes]))

def lazily_read_lines(n):
    for _ in range(n):
        #yield tuple(int(_.strip()) for _ in input().split())
        yield tuple(int(_.strip()) for _ in fake_input.readline().split())


def traverse(start_node, graph):
    index_queue = collections.deque()
    index_queue.appendleft(start_node.index)
    current_node = graph[0]
    while index_queue:
        tmp_node = graph[index_queue.pop()]
        tmp_node.previous_node = current_node
        current_node = tmp_node
        #print("dequeing "+str(current_node))
        for edge_index in current_node.relationship_indexes:
            edge_node = graph[edge_index]

            if not edge_node.previous_node:
                edge_node.previous_node = current_node
                edge_node.distance = max(0, current_node.distance) + 6
                index_queue.appendleft(edge_index)
                #print("enque "+str(graph[edge_index]))

# T - Test cases
# M - edges
# N - nodes

#T = input()
T = int(fake_input.readline().strip())
for test_case in range(T):
    N, M = [int(_.strip()) for _ in fake_input.readline().split()]
    #N, M = [int(_.strip()) for _ in input().split()]
    graph = [Node(i) for i in range(N+1)]
    for node_a, node_b in lazily_read_lines(M):
        #print("Joining",node_a, node_b)
        graph[node_a].relationship_indexes.add(node_b)
        graph[node_b].relationship_indexes.add(node_a)

    #S = input()
    S = int(fake_input.readline().strip())
    graph[0].distance = 0
    start_node = graph[S]
    traverse(start_node, graph)
    #print("\n".join(str(_) for _ in graph))
    print(" ".join(str(_.distance) for _ in graph[1:] if _ is not start_node))

#6 6 6 6 12 6 12 6 12 12 6 6 6 6 6 12 12 6 6 6 6 12 6 12 6 12 6 12 12 12 12 6 12 12 6 12 12 6 12 6 12 6 12 12 6 6 12 6 6 6 6 12 12 12 12 6 6 6 12 6 6 12 12 12 12 12 12 6 6