#include<bits/stdc++.h>
using namespace std;
#define inf 1e18
#define int long long
#define pii pair<int,int>
struct state{
    int city;
    int bonus;
    int dist;
};
struct comp
{
    bool operator () (state s1, state s2)
    {
        return s1.dist > s2.dist;
    }
};
int main(){
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int>> dist(n, vector<int>((k + 1),inf));
    vector<vector<pair<int,int>>> adjList(n);
    vector<bool> vis(n + 1,false);
    for (int i = 0; i < m;i++){
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        adjList[a].push_back({b, c});
        adjList[b].push_back({a, c});
    }
    priority_queue<state, vector<state>, comp> pq;
    pq.push({0,k,0});
    dist[0][k] = 0;
    {
        while(!pq.empty()){
            state t = pq.top();
            int d = t.dist;
            int node = t.city;
            int offers = t.bonus;
            pq.pop();
            if(d>dist[node][offers])
                continue;
            for(auto child:adjList[node]){
                int city = child.first;
                int temp = child.second;
                if(offers>0){
                    if(dist[city][offers-1]>d){
                        dist[city][offers - 1] = d;
                        pq.push({city, offers - 1, d});
                    }
                }
                if(dist[city][offers]>d+temp){
                    dist[city][offers] = d + temp;
                    pq.push({city, offers, d + temp});
                }

            }
        }
    }
    for (int i = 0; i < n;i++){
        cout << *min_element(dist[i].begin(), dist[i].end()) << " ";
    }
    cout << endl;
    return 0;
}