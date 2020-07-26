
#include <iostream>
#include <queue>

#include "GraphByArray.h"

//创建带权有向图
bool CreateGraph(MGraph &graph)
{

    std::cout << "请输入顶点数：" << std::endl;
    std::cin >> graph.vexnum;

    //初始化顶点
    for (size_t i = 0; i < graph.vexnum; i++)
    {
        graph.vexs[i].index = i;
        graph.vexs[i].visited = false;
    }

    for (size_t i = 0; i < graph.vexnum; i++)
    {
        for (size_t j = 0; j < graph.vexnum; j++)
        {
            graph.arcs[i][j].adj = 0;
        }
    }

    //构造边
    while (true)
    {

        int srcID, destID, data;
        std::cout << "请输入源点、目标点、权值，输入q表示结束：" << std::endl;
        std::cin >> srcID >> destID >> data;

        if (srcID == -1)
        {
            break;
        }

        if (srcID < graph.vexnum && destID < graph.vexnum)
            graph.arcs[srcID][destID].adj = data;
    }

    for (int i = 0; i < graph.vexnum; i++)
    {
        for (int j = 0; j < graph.vexnum; j++)
        {
            std::cout << i << " " << j << " " << graph.arcs[i][j].adj << std::endl;
        }
    }
}

void DepthFirstSearch(MGraph &graph)
{
    for (size_t i = 0; i < graph.vexnum; i++)
    {
        if (!graph.vexs[i].visited)
        {
            VisiteVertex(graph.vexs[i], graph);
        }
    }
}

void VisiteVertex(Vertex &v, MGraph &graph)
{
    std::cout << "visite point:" << v.index << std::endl;

    v.visited = true;

    for (size_t i = 0; i < graph.vexnum; i++)
    {
        if (graph.arcs[v.index][i].adj > 0)
        {
            VisiteVertex(graph.vexs[i], graph);
        }
    }
}

void BreadthFirstSearch(MGraph &graph)
{
    for (size_t i = 0; i < graph.vexnum; i++)
    {        
        graph.vexs[i].visited = false;
    }

    std::queue<int> points;

    for (size_t i = 0; i < graph.vexnum; i++)
    {
        if (!graph.vexs[i].visited)
        {
            points.push(i);
        }

        while (!points.empty())
        {
            auto current_point_index = points.front();
            points.pop();

            for (size_t i = 0; i < graph.vexnum; i++)
            {
                if (graph.arcs[current_point_index][i].adj > 0)
                {
                    std::cout << "visit:" << graph.vexs[i].index << std::endl;
                    graph.vexs[i].visited = true;
                    points.push(i);
                }
            }
        }
    }
}
