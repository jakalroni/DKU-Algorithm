INF = 9999
def choose_vertex(dist, found):
    min = INF
    minpos = -1
    for i in range(len(dist)):
        if dist[i]<min and found[i]==False:
            min = dist[i]
            minpos = i
    return minpos

# vtx == graph[0]
# adj == graph[1]

def dijkstra_ShortestPath_print(graph):
    start = 0
    vsize = len(graph[0])           # 정점 개수
    dist = list(graph[1][start])    # 시작 정점으로부터 최단 경로 거리를 저장
    path = [start] * vsize          # 바로 이전 정점을 저장, 이전 정점을 따라 시작 정점까지의 경로가 최단 경로가 됨
    found = [False] * vsize         # 방문한 정점 표시를 위해 사용
    found[start] = True
    dist[start] = 0

    for i in range(vsize):
        u = choose_vertex(dist, found)      # 매 단계에서 최소 distance인 정점을 선택
        found[u] = True                     # 선택 후 방문 처리

        for w in range(vsize):
            if not found[w]:
                if dist[u] + graph[1][u][w] < dist[w]:
                    dist[w] = dist[u] + graph[1][u][w]
                    path[w] = u

    # 최종 경로를 출력하기 위한 코드
    order_of_path = []
    print("--------------------------------------------------------------------------------")
    print("Src->Dst\tDist\tPath")
    for end in range(len(graph[0])):
        if end != start:
            print(" %c -> %c\t\t%2d\t" %(graph[0][start],graph[0][end],dist[end]), end="")
            order_of_path.append(end)
            while path[end] != start:
                order_of_path.append(path[end])
                end = path[end]
            order_of_path.append(path[end])
            order_of_path.reverse()
            for index in order_of_path:
                print(graph[0][index], end=" ")
            print()
            order_of_path = []
    print("--------------------------------------------------------------------------------")
    return

vertex1 =   ['A', 'B', 'C', 'D', 'E']
weight1 = [ [0, 2, 5, 1, 12],
           [3, 0, 3, 1, 8],
           [5, 3, 0, 2, INF],
           [1, 1, 2, 0, 7],
           [12, 8, INF, 7, 0] ]   

graph1 = (vertex1, weight1)
dijkstra_ShortestPath_print(graph1)

vertex2 =   ['A', 'B', 'C', 'D', 'E', 'F']
weight2 = [ [0, 5, INF, 2, 4, INF],
           [5, 0, 3, 3, INF, INF],
           [INF, 3, 0, 3, 4, 2],
           [2, 3, 3, 0, 1, INF],
           [4, INF, 4, 1, 0, 2],
           [INF, INF, 2, INF, 2, 0] ]   

graph2 = (vertex2, weight2)
dijkstra_ShortestPath_print(graph2)

vertex3 =   ['A', 'B', 'C', 'D', 'E', 'F']
weight3 = [ [0, 2, 5, 1, INF, INF],
           [2, 0, 3, 2, INF, INF],
           [5, 3, 0, 3, 1, 5],
           [1, 2, 3, 0, 1, INF],
           [INF, INF, 1, 1, 0, 2],
           [INF, INF, 5, INF, 2, 0] ]   

graph3 = (vertex3, weight3)
dijkstra_ShortestPath_print(graph3)