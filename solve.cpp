#include <bits/stdc++.h>
using namespace std;
#define N 2

int cost(vector<vector<int>> initial,vector<vector<int>> target){
    int count = 0;
    for(int i=0;i<initial.size();i++){
        for(int j=0;j<initial[0].size();j++){
            if(initial[i][j] != target[i][j] && initial[i][j]!= 0){
                count++;
            }
        }
    }
    return count;
}

pair<int,int> spacePosition(vector<vector<int>> initial){
    int r=-1,c=-1;
    for(int i=0;i<initial.size();i++){
        for(int j=0;j<initial[0].size();j++){
            if(initial[i][j] == 0){
                r = i;
                c = j;
                break;
            }
        }
    }
    return {r,c};
}

map<vector<vector<int>>,vector<vector<int>>> mp;
int nextState(vector<vector<int>> initial,vector<vector<int>> target){

    int dist = cost(initial,target);
    vector<pair<int,int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};

    // mapping parent to child for retrace
    //map<vector<vector<int>>,vector<vector<vector<int>>>> mp;

    // min heap contains minimum cost, state
    priority_queue<pair<int,vector<vector<int>>>,vector<pair<int,vector<vector<int>>>>,greater<pair<int,vector<vector<int>>>>> pq;
    pq.push({dist,initial});

    // push state to set to check uniqueness
    set<vector<vector<int>>> st;
    st.insert(initial);
    int height = 0;
    while(!pq.empty()){
        int size = pq.size();
        for(int i=0;i<size;i++){
            auto top = pq.top();
            pq.pop();
            int current = top.first;
            vector<vector<int>> state = top.second;
            pair<int,int> pos = spacePosition(state);
            int x = pos.first;
            int y = pos.second;
            for(auto dir : dirs){
                if(x - dir.first >= 0 && y - dir.second >= 0 && x - dir.first < state.size() && y - dir.second < state[0].size()){
                    vector<vector<int>> temp = state;
                    temp[x][y] = temp[x - dir.first][y - dir.second];
                    temp[x - dir.first][y - dir.second] =  0;
                    int dist = cost(temp,target);
                    if(st.find(temp) == st.end()){
                        mp[temp]= state;
                        pq.push({dist,temp});
                        st.insert(temp);
                    }
                    if(temp == target) return height;
                }
            }
        }
        height++;
    }
    //cout<<height;
    return 0;
}

vector<vector<vector<int>>> leaf_to_root_path(map<vector<vector<int>>,vector<vector<int>>> mp,vector<vector<int>> leaf){
    vector<vector<vector<int>>> path;
    vector<vector<int>> current = leaf;

    while(mp.find(current) != mp.end()){
        path.push_back(current);
        current = mp[current];
    }
    path.push_back(current);

    return path;
}

vector<vector<int>> take_int(string str) {
    vector<vector<int>> arr(2,vector<int>(2,0));
    int k=1;
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            arr[i][j] = str[k]-'0';
            k += 2;
        }
    }
    return arr;
}

int main(int argc, char* argv[]){

      if (argc != 2) {
        cerr << "Usage: " << argv[0] << " \"1,2,3,0\"" << endl;
        return 1;
    }

    // Parse the input array from the command-line argument
    string inputArg = argv[1];

    vector<vector<int>> initial = take_int(inputArg);
    int ind=0;

    vector<vector<int>> target = {{1,2},{3,0}};

    nextState(initial,target);

    vector<vector<vector<int>>> path = leaf_to_root_path(mp,target);

    reverse(path.begin(),path.end());
    vector<vector<int>> nextMove = path[1];
    cout<<"[";
    for(int i=0;i<2;i++)
      for(int j=0;j<2;j++)
        {  
            cout<<nextMove[i][j];
            if(i==1 && j==1){}
            else cout<<",";
        }
    cout<<"]";
    return 0;
}