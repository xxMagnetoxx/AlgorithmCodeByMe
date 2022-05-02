/*
@authors BHARGAV KALARIA
@date    2022-04-26 19:04:39
*/

#include <bits/stdc++.h>
using namespace std; 
#define MAX 100

#define debug(x)            cerr << #x << " = " << x << endl;
#define debug2(x, y)        {cerr <<#x<<" = " <<x<<", "<<#y <<" = " <<y <<"\n";}
#define debug3(x, y, z)     {cerr <<#x<<" = " <<x<<", "<<#y <<" = " <<y <<", "<<#z<<" = "<<z<<"\n";}
#define debug4(x, y, z, w)  {cerr <<#x<<" = " <<x<<", "<<#y <<" = " <<y <<", "<<#z<<" = "<<z<<", "<<#w << " = " <<w <<"\n";}

template<class T, class S> std::ostream& operator<<(std::ostream &os, const std::pair<T, S> &t) {
  os<<"("<<t.first<<",   "<<t.second<<")";
  return os;
}
template<typename T> ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "["; size_t last = v.size() - 1; for(size_t i = 0; i < v.size(); ++i) {
    out << v[i]; if (i != last) out << ", "; } out << "]"; return out;
}

// vector<int>g[MAX];
const int INF = 1e8;


struct Fedge{
    int v, u;
    int capacity, flow;
};

vector<Fedge>edges;
vector< int > g[MAX];
vector<int>level(MAX, 0), nextPtr(MAX, 0);


// here bfs is used to get the level of all node according to its remainging flow 
bool bfs(int s, int t){
    queue<int>q;
    q.push(s);  

    while(!q.empty()){
        int node = q.front();
        q.pop();
        
        // iterating no edges that is connected with node
        for(auto child : g[node]){
            
            // if it is backward edge than we need to skip that edges
            if(edges[child].capacity - edges[child].flow <= 0){
                continue;
            }
            // if it forward but it is visited then we have to skip that edges
            if(level[edges[child].u] != -1){
                continue;
            }
            
            // assign the child node level to parent + 1
            level[edges[child].u] = level[node] + 1;
            q.push(edges[child].u);
            
        }
    }
    return level[t] != -1;
}

int dfs(int node, int posFlow, int t){

// dead end is reached
if(posFlow == 0){
    return 0;
}
// reach the end
if(node == t){
return posFlow;
}


// here nextPtr is pointer to edge from which we have to start from that node
// basically, it descrease the time by continue edge that reached to it's max capacity

/* here we go over through all adjacent edges to the node and find if that's node capacity is greater than 0 then go 
over through that edge.
*/
for(int& edgePointer = nextPtr[node]; edgePointer < g[node].size(); edgePointer++){
    int edgeindex = g[node][edgePointer];
    int child = edges[edgeindex].u;
    if(level[node] + 1 != level[child] || edges[edgeindex].capacity - edges[edgeindex].flow <= 0) continue;

    int canTranfer = dfs(child, min(posFlow, edges[edgeindex].capacity - edges[edgeindex].flow), t);

    // if we remaining capacity of the node is zero.
    if(canTranfer == 0) continue;
    
    // adding flow to forward pass
    edges[edgeindex].flow += canTranfer;
    // subtracting to backward pass and here next edge will we equal to backward edge (v, u)
    edges[edgeindex ^ 1].flow -= canTranfer;
    return canTranfer;
}
return 0;


}

int DinicMaxFlow(int n, int e, int s, int t){
    int totalFlow = 0;

    while(true){
        
        fill(level.begin(), level.end(), -1);
        level[s] = 0;

        // not other possible combination of  hypothetical level graph
        bool pas = bfs(s, t);
        // cout << level << endl;
        if(!pas){
            break;
        }
        

        fill(nextPtr.begin(), nextPtr.end(), 0);

        // run dfs until we find blocking of flow      
        // here in each dfs the it will find the one path from s to t and flow of that path.
        int fl = dfs(s, INF, t);
        while(fl){
            totalFlow += fl;
            // calling dfs until blocking of flow is not happend
            fl = dfs(s, INF, t);
        }
    }
return totalFlow;
}

int main(){

// freopen("input.txt", "r+", stdin);

int n, e;
cin >> n >> e;
level.resize(n, 0);
nextPtr.resize(n, 0);
int a, b, cap;

// here edgenumber is for numbering the edge.
int edgeNumber = 0;


for(int i=0; i<e; i++){
cin >> a >> b >> cap;
struct Fedge fr, sc;
 fr.v = a;
 fr.u = b;
 fr.capacity = cap;
 fr.flow = 0;
 sc.v = b;
 sc.u = a;
 sc.capacity = 0;
 sc.flow = 0;
g[a].push_back(edgeNumber++);
g[b].push_back(edgeNumber++);
edges.push_back(fr);
edges.push_back(sc);
}

// here i have take and source and end as 0 and n-1
int s = 0, t = n-1;
cout << DinicMaxFlow(n, e, s, t) << endl;

}

