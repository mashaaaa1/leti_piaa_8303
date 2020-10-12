#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>

class Edge {
public:
    char start;//исток
    char end;//сток
    int weight;//вес
    
};

class Graph {
public:
    bool isEdge(char u, char v) {//существует ли ребро
        return this->data[u].count(v);
    }

    void addEdge(char u, char v, int w) {//добавляем ребро
        this->data[u][v] = w;
    }

    std::vector<Edge> getEdges(char u) {//получить ребра выходящие из вершины
        std::vector<Edge> edgesFromU;
        for (auto nextEdge : this->data[u]) {
            edgesFromU.push_back(Edge({ u, nextEdge.first, nextEdge.second }));
        }
        return edgesFromU;
    }


    int getValue(char u, char v) {//вес ребра
        return this->data[u][v];
    }

    void addValue(char u, char v, int value) {//добавить вес
        this->data[u][v] += value;
    }

    void print() {//выводит ребра графа в консоль
        for (auto& in : data) {
            for (auto& to : in.second) {
                std::cout << in.first << ' ' << to.first << ' ' << to.second << '\n';
            }
        }
    }

    bool cmp(Edge a, Edge b) {
        if (a.weight < b.weight) return true;
        int count1 = this->getEdges(a.start).size();
        int count2 = this->getEdges(b.start).size();
        if (a.weight == b.weight && count1 < count2) return true;
        return false;
    }
private:
    std::map<char, std::map<char, int>> data;//список смежности для графа
};




void read_graph(Graph* graph, std::set<std::pair<char, char>>& edges, int n) {//считываем граф
    char u, v;
    int w;
    for (int i = 0; i < n; i++) {
        std::cin >> u >> v >> w;
        edges.insert({ u,v });
        graph->addEdge(u, v, w); // прямое ребро
        if (!graph->isEdge(v, u)) {
            graph->addEdge(v, u, 0); // обратное
        }
    }
}

bool bfs(Graph* graph, int start, int finish, std::map<char, char>& parents) {//поиск в ширину
    std::queue<char> q;//очередь вершин
    q.push(start);
    parents[start] = start;//путь
    char v;
    auto cmp{
        [&graph](Edge a, Edge b) {
        return graph->cmp(a,b); }
    };
    while (!q.empty()) {//пока очередь не пуста
        v = q.front();//берем первую вершину
        q.pop();//убираем из очереди
        std::vector<Edge> vec = graph->getEdges(v);
        std::sort(vec.begin(), vec.end(), cmp);
        for (Edge edge : graph->getEdges(v)) {//проходим по всем ребрам вершины
            char to = edge.end;// вершина обрабатываемая
            int w = edge.weight;//вес
            if (parents.count(to) == 0 && w > 0) {//если в пути еще нет этой вершины и вес больше 0
                q.push(to);//помещаем в очередь
                parents[to] = v;//увеличиваем путь
                if (to == finish) break;//если дошли до стока
            }
        }
        if (parents.count(finish)) break;//если дошли до стока
    }
    return !q.empty();
}

int minWeightOnCurrentPath(Graph* graph, std::map<char, char>& parents, char finish) {//ищем мин пропускную способность в увеличивающем пути
    char prevVert = finish;//начинаем со стока
    char curVert = parents[prevVert];//предок стока
    int weight = graph->getValue(curVert, prevVert);//вес ребра
    std::vector<char> path;//путь
    path.push_back(finish);
    while (prevVert != curVert) {//пока не дошли до истока
        path.push_back(curVert);
        weight = std::min(weight, graph->getValue(curVert, prevVert));//выбираем минимум
        curVert = parents[curVert];//переходим к след вершине
        prevVert = parents[prevVert];
    }
    std::cout << "Found new available path\n";
    for (auto i = path.rbegin(); i != path.rend(); i++) {//увеличивающий путь
        std::cout << (*i) << ' ';
    }
    std::cout << '\n' << "minimum weight on the path is " << weight << "\n";//мин элемент
    return weight;
}

void changeWeights(Graph* graph, std::map<char, char>& parents, char finish,
    int flow, std::map<std::pair<char, char>, int>& answer) {//изменеие весов в графе
    char prevVert = finish;//начианем со стока
    char curVert = parents[prevVert];
    while (prevVert != curVert) {//пока не дошли до итсока
        graph->addValue(curVert, prevVert, -flow);//отнимаем поток в сторону стока
        answer[{curVert, prevVert}] += flow;//прибавляем к  ответу
        graph->addValue(prevVert, curVert, flow);//прибавляем поток в сторону истока
        curVert = parents[curVert];//след вершина
        prevVert = parents[prevVert];
    }
    graph->print();//печать графа
    std::cout << '\n';
}

void fordFulkerson(Graph* graph, int& maxFlow, char start, char finish, std::map<std::pair<char, char>, int>& answer) {//запуск алгоритма
    bool isWayExist = true;//есть ли путь
    while (isWayExist) {//пока есть путь
        std::map<char, char> parents;//путь
        isWayExist = bfs(graph, start, finish, parents);//ищем путь
        if (isWayExist) {//если нашли
            int flow = minWeightOnCurrentPath(graph, parents, finish);//ищем мин поток
            maxFlow += flow;//прибавляем к макс потоку
            changeWeights(graph, parents, finish, flow, answer);//пересчет пропускных способностей
        }
    }
}

void writeAnswer(std::set<std::pair<char, char>>& edges, std::map<std::pair<char, char>, int> answer, int maxFlow) {//вывод ответа
    std::cout << maxFlow << '\n';
    for (auto edge : edges) {
        char u = edge.first;
        char v = edge.second;
        std::cout << u << ' ' << v << ' ';
        if (edges.count({ v,u })) {
            std::cout << answer[{u, v}] << '\n';
        }
        else {
            std::cout << answer[{u, v}] - answer[{v, u}] << '\n';
        }
    }
}

int main() {
    // ввод данных
    int n;
    std::cin >> n;//количество вершин
    char start, finish;//исток и сток
    std::cin >> start >> finish;

    auto* graph = new Graph;
    std::set<std::pair<char, char>> edges;//ребра графа
    read_graph(graph, edges, n);//считывание графа

    int maxFlow = 0;
    std::map<std::pair<char, char>, int> answer;//ответ
    fordFulkerson(graph, maxFlow, start, finish, answer);//запуск алгоритма

    writeAnswer(edges, answer, maxFlow);//вывод ответа

    delete graph;
}