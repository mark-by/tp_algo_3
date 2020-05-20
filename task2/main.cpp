#include <vector>
#include <iostream>
#include <tuple>
#include <queue>
#include <functional>

class ListGraph {
public:
    explicit ListGraph(int verticesCount) : _vertices(verticesCount){}

    void addEdge(int from, int to);

    int verticesCount() const;

    std::vector<int> getNextVertices(int vertex) const;

    int countPath(int from, int to) const;

private:

    std::vector<std::vector<int>> _vertices;
};

std::tuple<ListGraph, int, int> input();

int main() {
    auto[graph, from, to] = input();
    std::cout << graph.countPath(from, to);
}

std::tuple<ListGraph, int, int> input() {
    int verticesCount, edgesCount, from, to;
    std::cin >> verticesCount;
    std::cin >> edgesCount;
    ListGraph graph(verticesCount);
    for (int edgesCounter = 0; edgesCounter < edgesCount; edgesCounter++) {
        std::cin >> from >> to;
        graph.addEdge(from, to);
    }
    std::cin >> from >> to;
    return {graph, from, to};
}

void ListGraph::addEdge(int from, int to) {
    _vertices[from].push_back(to);
    _vertices[to].push_back(from);
}

int ListGraph::verticesCount() const {
    return _vertices.size();
}

std::vector<int> ListGraph::getNextVertices(int vertex) const {
    std::vector<int> result;

    result.resize(_vertices[vertex].size());
    std::copy(_vertices[vertex].cbegin(), _vertices[vertex].cend(), result.begin());

    return result;
}

int ListGraph::countPath(int from, int to) const {
    std::vector<std::pair<int, int>> description(verticesCount()); //[depth, pathCount]
    std::queue<int> verticesQueue;

    verticesQueue.push(from);
    description[from] = {0, 1};
    while (!verticesQueue.empty()) {
        int currVertex = verticesQueue.front();
        verticesQueue.pop();
        std::vector<int> nextVertices = getNextVertices(currVertex);

        for (int child : nextVertices) {
            if (!description[child].second) { //если не посещали
                verticesQueue.push(child);
                //глубина увеличивается на 1 по сравнению с родителем, количество путей передается
                description[child] = {description[currVertex].first + 1, description[currVertex].second};
            } else if (description[child].first == description[currVertex].first + 1) {
                //отсекаем элементы, лежащие на одной глубине относительно стартового узла и более глубокие
                description[child].second += description[currVertex].second; //увеличиваем количество путей
            }
        }
    }

    return description[to].second;
}

