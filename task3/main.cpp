#include <set>
#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm>

class ListGraph {
    struct EndEdge {
        int vertex;
        int weight;

        EndEdge() = default;
        EndEdge(int to, int weight) : vertex(to), weight(weight) {}

        bool operator<(const EndEdge & other) const {
            return weight < other.weight;
        }
    };

public:
    explicit ListGraph(int verticesCount) : _vertices(verticesCount) {}
    void addEdge(int from, int to, int weight);
    int verticesCount() const;

    std::vector<EndEdge> getNextVertices(int vertex) const;

    int distanceOfEasyWay(int from, int to) const;
private:
    std::vector<std::vector<EndEdge>> _vertices;
};

std::tuple<ListGraph, int, int> input();

int main() {
    auto [graph, from, to] = input();
    std::cout << graph.distanceOfEasyWay(from, to);
}

std::tuple<ListGraph, int, int>  input() {
   int verticesCount, edgesCount, from, to, weight;
   std::cin >> verticesCount >> edgesCount;
   ListGraph graph(verticesCount);
   for (int edge = 0; edge < edgesCount; edge++) {
       std::cin >> from >> to >> weight;
       graph.addEdge(from, to, weight);
   }
   std::cin >> from >> to;
   return {graph, from, to};
}

void ListGraph::addEdge(int from, int to, int weight) {
    _vertices[from].emplace_back(to, weight);
    _vertices[to].emplace_back(from, weight);
}

int ListGraph::verticesCount() const {
    return _vertices.size();
}

std::vector<ListGraph::EndEdge> ListGraph::getNextVertices(int vertex) const {
    std::vector<EndEdge> result;

    result.resize(_vertices[vertex].size());
    std::copy(_vertices[vertex].cbegin(), _vertices[vertex].cend(), result.begin());

    return result;
}

int ListGraph::distanceOfEasyWay(int from, int to) const {
    std::vector<int> distance(verticesCount(), INT32_MAX);
    distance[from] = 0;
    std::set<std::pair<int, int>> queue;
    queue.emplace(0, from);
    while (!queue.empty()) {
        int currVertex = queue.begin()->second;
        queue.erase(queue.begin());

        std::vector<EndEdge> nextVertices = getNextVertices(currVertex);
        for (auto & child : nextVertices) {
            if (distance[child.vertex] > distance[currVertex] + child.weight) {
               if (distance[child.vertex] != INT32_MAX) {
                   queue.erase({distance[child.vertex], child.vertex});
               }

                distance[child.vertex] = distance[currVertex] + child.weight;
                queue.emplace(distance[child.vertex], child.vertex);
            }
        }
    }
    return distance[to];
}


