#pragma once

#include <iostream>
#include <map>

const int VNum = 5;
std::vector<int> path;

struct Edge
{
    int m_src, m_dest, m_weight;
};

struct Graph
{
    int m_vertNum = VNum, m_edgeNum;

    Edge* m_edge;
};

Graph* CreateGraph(int e)
{
    Graph* graph = new Graph();
    graph->m_edgeNum = e;
    graph->m_edge = new Edge[e];

    return graph;
}

void Print(int dist[], int n)
{
    L_TRACE("Vertex \t\t Distance from Source");
    for (size_t i = 0; i < n; ++i)
        L_TRACE("{0} \t\t {1}", static_cast<char>(i + 65), dist[i]);

    L_TRACE("=======================================");
}

void BellmenFord(Graph* graph, int src)
{
    int vertNum = graph->m_vertNum;
    int edgeNum = graph->m_edgeNum;

    int dist[VNum];

    std::map<int, int> predecessor;

    for (size_t i = 0; i < vertNum; i++)
        dist[i] = INT_MAX;

    dist[src] = 0;

    // Relax
    for (size_t i = 1; i <= vertNum - 1; i++) 
    {
        for (size_t j = 0; j < edgeNum; j++) 
        {
            int u = graph->m_edge[j].m_src;
            int v = graph->m_edge[j].m_dest;
            int weight = graph->m_edge[j].m_weight;

            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) 
            {  
                dist[v] = dist[u] + weight;
                predecessor[v] = u;
                //L_TRACE("Src: {0} \t\t Dest: {1}", u, v);
                //Print(dist, vertNum);
            }
        }
    }
    for (size_t i = 0; i < edgeNum; i++) 
    {
        int u = graph->m_edge[i].m_src;
        int v = graph->m_edge[i].m_dest;
        int weight = graph->m_edge[i].m_weight;

        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) 
        {
            L_TRACE("Graph contains negative weight cycle");
            return;
        }
    }

    Print(dist, vertNum);

    std::vector<int> paths;

    for (size_t i = 0; i < vertNum - 1; i++)
    {
        int current = i + 1;
        paths.clear();

        while (current != src)
        {
            paths.insert(paths.begin(), current);
            current = predecessor[current];
        }

        paths.insert(paths.begin(), current);

        std::cout << "Paths: ";
        for (auto& node : paths)
            std::cout << static_cast<char>(node + 65) << " ";
        std::cout << std::endl;
    }
    

    return;
}
