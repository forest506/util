
#ifndef GRAPH_BY_ARRAY_H
#define GRAPH_BY_ARRAY_H

#define MAX_VERTEX_NUM 20

//点
typedef struct Vertex{
    int index;
    bool visited;
}Vertex;

//弧
typedef struct InfoType
{
}InfoType;

typedef struct ArcCell
{
    int adj;       //顶点关系类型；对无权图用0或1表示是否相邻；对带权图，则为权值
    InfoType info; //该弧相关其他信息
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

//图
typedef struct Graph{
    Vertex vexs[MAX_VERTEX_NUM]; //顶点数组
    AdjMatrix arcs; //邻接矩阵
    int vexnum, arcnum;//当前顶点书和弧数    
}MGraph;


bool CreateGraph(MGraph& graph);
void DepthFirstSearch( MGraph& graph);
void BreadthFirstSearch( MGraph& graph);

void VisiteVertex(Vertex& v, MGraph& graph);

void BreadthVisiteVertex(Vertex& v, MGraph& graph);

#endif 
