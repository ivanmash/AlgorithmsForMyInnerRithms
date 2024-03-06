#include <algorithm>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <iostream>
#include <climits>
#include<cmath>

using WeightedGraph = std::unordered_map<int,std::unordered_map<int,double>>;

struct PriorityQueue
{
    PriorityQueue(){}

    void push(int node, double val) // O(LogN)
    {
        byNodes.insert({node, val});
        byWeights.insert({val, node});
    }


    void update(int node, double newVal) // O(LogN)
    {
        int oldVal = byNodes[node]; // O(1)
        byWeights.erase({oldVal, node});  // O(LogN)
        byWeights.insert({newVal, node}); // O(LogN)
        byNodes[node] = newVal; // O(1)
    }

    // top() returns the minimum element from the priority queue and deletes it
    int top()
    {
        // Take the identifier of the node that has the minimum value
        int topNode = byWeights.begin()->second;

        byNodes.erase(topNode);
        byWeights.erase(byWeights.begin());

        return topNode;
    }

    bool empty()
    {
        return byNodes.size() == 0;
    }

    // (E,8) ----update(E,3)----> (E,3)

    std::unordered_map<int,int> byNodes; // store <node,weight> (C,1) (D,6) (E,8)

    std::set<std::pair<int,int>> byWeights; // store <weight,node> (1,C) (6,D) (8,E)
};

// Returns MST
std::vector<std::pair<int,double>> prim(WeightedGraph& g)
{
    // let us assume that we pick 0 as the start node
    int start = 0;

    // k[x]: distance of x to the growing tree. The cheapest way to connect node x to our tree.
    std::vector<double> k(g.size(), INT_MAX);

    // parent vector. p[b] = a. Node 'b' is connect to the tree by node 'a'
    std::vector<int> p(g.size(), -1);

    // p[i]: -1  0  1  2  5
    //  i:	  0  1  2  3  4
    //        A  B  C  D  E ..

    std::vector<bool> visited(g.size(), false);

    PriorityQueue pq;
    for (int i = 0; i < g.size(); i++)
    {
        pq.push(i, k[i]);
    }

    // init
    k[start] = 0;
    pq.update(start, 0);


    while (!pq.empty())
    {
        int u = pq.top();
        visited[u] = true;

        // u-----w---->v
        for (auto [v,w] : g[u])
        {
            if (!visited[v])
                if (w < k[v])
                    k[v] = w, p[v] = u, pq.update(v, k[v]);
        }
    }

    // minimum spanning tree
    // (A,B), (B,C), (C,I)
    std::vector<std::pair<int,double>> mst;
    for (int i = 1; i < p.size(); i++)
        mst.push_back({p[i], i});

    return mst;

}

WeightedGraph createAGraph(std::vector<std::pair<double, double>>& coordinates, double RMIN, double RMAX, int N)
{
    WeightedGraph g(coordinates.size());
    for (int i = 0; i < N; ++i)
        g[i] = {};
    double dist;
    for(int i = 0; i < N - 1; ++i)
    {
        for(int j = i + 1; j < N; ++j)
        {
            dist = std::sqrt((coordinates[i].first - coordinates[j].first) *
                   (coordinates[i].first - coordinates[j].first) +
                   (coordinates[i].second - coordinates[j].second) *
                   (coordinates[i].second - coordinates[j].second));
            if (RMIN < dist && dist < RMAX)
            {
                g[i].insert({j, dist});
                g[j].insert({i, dist});
            }
        }
    }
    return g;
}


int main()
{
    int N;
    double RMIN, RMAX;
    std::cin >> N >> RMIN >> RMAX;
    double dummyf, dummyt;
    std::vector<std::pair<double, double>> coordinates;
    for (int i = 0; i < N; ++i)
    {
        std::cin >> dummyf >> dummyt;
        coordinates.push_back({dummyf, dummyt});
    }
    WeightedGraph g;
    g = createAGraph(coordinates, RMIN, RMAX, N);
    std::vector<std::pair<int,double>> mst = prim(g);
    double sum = 0;
    for(auto [u,v] : mst)
        sum += g[u][v];

    std::cout << std::ceil(sum) << std::endl;

    return 0;
}