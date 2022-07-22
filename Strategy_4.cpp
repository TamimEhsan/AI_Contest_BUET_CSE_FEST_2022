#include<bits/stdc++.h>
using namespace std;
#define WALL '#'
#define UNKNOWN '?'
#define EMPTY '.'
//#define BOMB 'B'
//#define COIN 'C'

#define COIN 2
#define UNCERTAIN 1
#define TAKEN 0

int dx[] = {1,-1,0,0};
int dy[] = {0,0,1,-1};
struct Coord{
    int x;
    int y;

    Coord(int xx=0,int yy=0){
        x = xx;
        y = yy;
    }
    bool operator == (const Coord & crd) const {
        return ((x == crd.x) and (y == crd.y));
    }
    bool operator != (const Coord & crd) const {
        return (x != crd.x) or (y != crd.y);
    }
    bool operator < (const Coord & crd) const {
        return (x < crd.x) or (y < crd.y);
    }

    int Manhattan(const Coord & crd) {
        return abs(x-crd.x)+abs(y-crd.y);
    }
};
Coord my_flag_base,enemy_flag_base;
struct Power{
    string name;
    int price;
    int damage;
};
Power fire,freeze,mine;
struct Bot{
    int hp;
    Coord curr_pos;
    Coord target_pos = {0,0};
    int timeout;
    bool isVisible;
};
vector<int> bots_alive; // 0th bot is the seeker and 1st is the keeper
struct Player{
    int score;
    Coord flag_base;
    Coord curr_flag_pos;
    int flag_carrier; // opponent will cary flag
    int bots_cnt;
    Bot bots[10];


};
Player hero,enemy;
// when 0 or 1 dies, swap it will last bot and pop back
int HEIGHT,WIDTH;
const int MAX_HEIGHT = 50;
string grid[MAX_HEIGHT];
int coins[MAX_HEIGHT][MAX_HEIGHT];
int hero_flag_distance[50][50];
int enemy_flag_distance[50][50];

vector<Coord> emptyCells;
int emptyCounter = 0;
int randomfunc(int j){
    return rand() % j;
}
void generateRandomPos(){
    random_shuffle(emptyCells.begin(), emptyCells.end(), randomfunc);
}
/*
int getDistance(Coord st,Coord en){
    cerr<<"start at "<<st.x<<" "<<st.y<<" end at "<<en.x<<" "<<en.y<<endl;
    int isVisited[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++) isVisited[i][j] = 0;
    }
    queue<Coord> qq;
    qq.push(st);
    isVisited[st.x][st.y] = 1;
    while( !qq.empty() ){
        auto u = qq.front();
        //cerr<<u.x<<" "<<u.y<<" "<<isVisited[u.x][u.y]<<endl;
        if( u == en ) return isVisited[en.x][en.y];
        qq.pop();
        for(int i=0;i<4;i++){
            int vx = u.x+dx[i];
            int vy = u.y+dy[i];
            if( vx>=0 and vx<HEIGHT and vy>=0 and vy<WIDTH and grid[vx][vy] != WALL and isVisited[vx][vy] == 0 ){
                qq.push({vx,vy});
                isVisited[vx][vy] = isVisited[u.x][u.y]+1;
            }
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++) cerr<<isVisited[i][j]<<" ";
        cerr<<endl;
    }
    return isVisited[en.x][en.y];
}
*/

const int MOVE_PER_PLAN = 20;
/*
vector<Coord> getPathPlanner(Coord start){
    int gain[HEIGHT][WIDTH];
    int dis[HEIGHT][WIDTH];
    Coord parent[HEIGHT][WIDTH];
    int inf = 1000;
    for(int i=0;i<HEIGHT;i++)
        for(int j=0;j<WIDTH;j++){
            dis[i][j] = 0;
            gain[i][j] = inf;
        }
    gain[start.x][start.y] = 0;
    parent[start.x][start.y] = start;
    set<pair<int,Coord>> q;
    q.insert({0,start});
    int moves = MOVE_PER_PLAN;
    while(!q.empty()){
        auto v = q.begin()->second;
        q.erase(q.begin());
        if( dis[v.x][v.y]>=MOVE_PER_PLAN ) continue;
        for(int i=0;i<4;i++){
            int tox = v.x+dx[i];
            int toy = v.y+dy[i];
            if( tox<0 or tox>=HEIGHT or toy<0 or toy>=WIDTH or grid[tox][toy] == WALL ) continue;
            if( gain[v.x][v.y]+coins[tox][toy]<dis[tox][toy] ){
                q.erase({gain[v.x][v.y],{tox,toy}});
                gain[tox][toy] = gain[v.x][v.y]+coins[tox][toy];
                q.insert({gain[tox][toy],{tox,toy}});
            }
        }
    }
    int mn = 1000;
    Coord target;
    for(int i=0;i<HEIGHT;i++)
        for(int j=0;j<WIDTH;j++){
            if( gain[i][j]<mn ){
                mn = gain[i][j];
                target = {i,j};
            }
        }
    vector<Coord> path;
    while(target!=parent[target.x][target.y])
}

vector<Coord> getPathPlanner(Coord start){
    int dp[HEIGHT][WIDTH][MOVE_PER_PLAN];
    int dir[HEIGHT][WIDTH][MOVE_PER_PLAN];

    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            for(int k=0;k<MOVE_PER_PLAN;k++){
                dp[i][j][k] = -1;
                dir[i][j][k] = -1;
            }
        }
    }
    dp[start.x][start.y][0] = 0;
    for(int k=0;k<MOVE_PER_PLAN;k++){
        for(int i=0;i<HEIGHT;i++){
            for(int j=0;j<WIDTH;j++){

            }
        }
    }
}
*/

void clearCoins(int id){
    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;
    int k=0;
    while(grid[xx][yy+k]!=WALL){
        coins[xx][yy+k] = 0;
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        coins[xx][yy-k] = 0;
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
       coins[xx+k][yy] = 0;
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        coins[xx-k][yy] = 0;
        k++;
    }
}
Coord getPathPlanner(Coord st,int moves = 20){
    int isVisited[50][50];
    Coord parent[50][50];
    int gain[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++) {
            isVisited[i][j] = 0;
            gain[i][j] = 0;

        }
    }
    queue<Coord> qq;
    qq.push(st);
    isVisited[st.x][st.y] = 1;
    while( !qq.empty() ){
        auto u = qq.front();
        qq.pop();
        for(int i=0;i<4;i++){
            int vx = u.x+dx[i];
            int vy = u.y+dy[i];
            if( vx>=0 and vx<HEIGHT and vy>=0 and vy<WIDTH and grid[vx][vy] != WALL and isVisited[vx][vy] == 0 and isVisited[u.x][u.y] <moves  ){
                qq.push({vx,vy});
                isVisited[vx][vy] = isVisited[u.x][u.y]+1;
                gain[vx][vy] = gain[u.x][u.y]+coins[vx][vy];
                parent[vx][vy] = u;
            }
        }
    }
    int mn = -1;
    Coord target;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++) {
            if( gain[i][j]>mn ){
                mn = gain[i][j];
                target = {i,j};
            }
        }
    }
    cerr<<"path planner ended "<<mn<<" "<<moves<<endl;
    if( mn == 0 ){
        target = getPathPlanner(st,moves*2);
    }else{
        Coord par = target;
        cerr<<"parent planner started "<<endl;
        while(par!=st){
            coins[par.x][par.y] = 0;
            par = parent[par.x][par.y];
        }
        cerr<<"parent planner ended "<<endl;
    }

    return target;
}
void calculateDistanceFromFlag(Coord flagPos,int dis[50][50]){
    int isVisited[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++) isVisited[i][j] = 0;
    }
    queue<Coord> qq;
    qq.push(flagPos);
    dis[flagPos.x][flagPos.y] = 1;
    while( !qq.empty() ){
        auto u = qq.front();
        //cerr<<u.x<<" "<<u.y<<" "<<isVisited[u.x][u.y]<<endl;
        qq.pop();
        for(int i=0;i<4;i++){
            int vx = u.x+dx[i];
            int vy = u.y+dy[i];
            if( vx>=0 and vx<HEIGHT and vy>=0 and vy<WIDTH and grid[vx][vy] != WALL and dis[vx][vy] == 0 ){
                qq.push({vx,vy});
                dis[vx][vy] = dis[u.x][u.y]+1;
            }
        }
    }
}


void populateEmptyCells(Coord flagPos){
    int isVisited[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++) isVisited[i][j] = 0;
    }
    queue<Coord> qq;
    qq.push(flagPos);
    isVisited[flagPos.x][flagPos.y] = 1;
    while( !qq.empty() ){
        auto u = qq.front();
        //cerr<<u.x<<" "<<u.y<<" "<<isVisited[u.x][u.y]<<endl;
        qq.pop();
        emptyCells.push_back(u);
        for(int i=0;i<4;i++){
            int vx = u.x+dx[i];
            int vy = u.y+dy[i];
            if( vx>=0 and vx<HEIGHT and vy>=0 and vy<WIDTH and grid[vx][vy] != WALL and isVisited[vx][vy] == 0 ){
                qq.push({vx,vy});
                isVisited[vx][vy] = isVisited[u.x][u.y]+1;
            }
        }
    }
}

int fireInTheHole(int id){
    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;

    int enemyCount = 0,heroCount = 0;
    bool deepFreeze = false,hellFire = false;
    bool useFreeze = false;
    int k=0;
    cerr<<"fire in hole "<<id<<endl;
    while(grid[xx][yy+k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy+k};
            if( hero.bots[i].curr_pos == boo and i!=id ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i ) {
                    if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( enemy.bots[ enemy.flag_carrier ].timeout == 0 and hero.score>=freeze.price+fire.price*3 ) {
                        useFreeze = true;
                    }else {
                        hellFire = true;
                    }
                }else if( enemy.bots[i].timeout ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy-k};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i ) {
                    if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( enemy.bots[ enemy.flag_carrier ].timeout == 0 and hero.score>=freeze.price+fire.price*3 ) {
                        useFreeze = true;
                   }else {
                        hellFire = true;
                    }
                }else if( enemy.bots[i].timeout ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx+k,yy};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i ) {
                    if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( enemy.bots[ enemy.flag_carrier ].timeout == 0 and hero.score>=freeze.price+fire.price*3 ) {
                        useFreeze = true;
                   }else {
                        hellFire = true;
                    }
                }else if( enemy.bots[i].timeout ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx-k,yy};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i ) {
                    if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( enemy.bots[ enemy.flag_carrier ].timeout == 0 and hero.score>=freeze.price+fire.price*3 ) {
                        useFreeze = true;
                   }else {
                        hellFire = true;
                    }
                }else if( enemy.bots[i].timeout ) hellFire = true;
            }
        }
        k++;
    }
    if( hellFire ) cerr<<"Found enemy flag carrier by "<<id<<endl; cerr<<endl;

    if( deepFreeze ) return 0;
    if(useFreeze) return 2;
    if( hellFire ) return 1;
    return 0;
    //return enemyCount>heroCount;
}

bool secondaryFireOption(int id){
    // will only be used for seeker
    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;

    int enemyCount = 0,heroCount = 0;
    bool deepFreeze = false,hellFire = false;
    bool useFreeze = false;
    int k=0;

    while(grid[xx][yy+k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy+k};
            if( hero.bots[i].curr_pos == boo and i!=id ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy-k};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx+k,yy};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx-k,yy};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++;
            }
        }
        k++;
    }



    if( deepFreeze ) return 0;
    return enemyCount>=heroCount and enemyCount;
}

bool seekerFireOption(int id){
    // will only be used for seeker
    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;

    int enemyCount = 0,heroCount = 0;
    bool deepFreeze = false,hellFire = false;
    bool useFreeze = false;
    int k=0;

    while(grid[xx][yy+k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy+k};
            if( hero.bots[i].curr_pos == boo and i!=id ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible and enemy.bots[i].curr_pos.Manhattan(enemy.curr_flag_pos)<=5 ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy-k};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible and enemy.bots[i].curr_pos.Manhattan(enemy.curr_flag_pos)<=5 ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx+k,yy};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible and enemy.bots[i].curr_pos.Manhattan(enemy.curr_flag_pos)<=5 ) {
                enemyCount++;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        for(int i=0;i<10;i++){
            Coord boo = {xx-k,yy};
            if( hero.bots[i].curr_pos == boo ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible and enemy.bots[i].curr_pos.Manhattan(enemy.curr_flag_pos)<=5 ) {
                enemyCount++;
            }
        }
        k++;
    }



    if( deepFreeze ) return 0;
    return enemyCount>=heroCount and enemyCount;
}
int main(){

    cin >> HEIGHT >> WIDTH; cin.ignore();
  //  cerr<<WIDTH;

    for (int i = 0; i < HEIGHT; i++) {
        cin >> grid[i]; cin.ignore();
        for(int j=0;j<grid[i].size();j++){
            if( grid[i][j] != WALL ){
                grid[i][j] = UNKNOWN;
               // emptyCells.push_back({i,j});
            }
        }
     //   cerr<<grid[i]<<endl;
    }

    cin >> hero.flag_base.x >> hero.flag_base.y; cin.ignore();
    cin >> enemy.flag_base.x >> enemy.flag_base.y; cin.ignore();
    cin >> fire.name >> fire.price >> fire.damage; cin.ignore();
    cin >> freeze.name >> freeze.price >> freeze.damage; cin.ignore();
    cin >> mine.name >> mine.price >> mine.damage; cin.ignore();
    populateEmptyCells(hero.flag_base);
    // game loop
    bool captured = false;
    int seeker = -1;
    int keeper = -1;
    int emptyCounter = 0;
    generateRandomPos();
    bool firstkeeperReached = false;
    while (1) {

        cin >> hero.score >> enemy.score; cin.ignore();

        cin >> hero.curr_flag_pos.x >> hero.curr_flag_pos.y >> enemy.flag_carrier; cin.ignore();
        cin >> enemy.curr_flag_pos.x >> enemy.curr_flag_pos.y >> hero.flag_carrier; cin.ignore();
        cerr<<enemy.flag_carrier<<endl;
        cin >> hero.bots_cnt; cin.ignore();
        seeker = hero.flag_carrier;
        calculateDistanceFromFlag(hero.curr_flag_pos,hero_flag_distance);
        calculateDistanceFromFlag(enemy.curr_flag_pos,enemy_flag_distance);

        int mn_dis = INT_MAX;
        for(int i=0;i<10;i++) hero.bots[i].hp = 0; // make it dead before resurrection U w U



        for (int i = 0; i < hero.bots_cnt; i++) {
            int id,pos_r,pos_c,health,timeout;
            cin >> id >> pos_r >> pos_c >> health >> timeout; cin.ignore();
            hero.bots[id].hp = health;
            hero.bots[id].curr_pos = {pos_r,pos_c};
            hero.bots[id].timeout = timeout;
            //if( hero.bots[id].curr_pos == enemy.flag_base ) captured = true;
          //  int enemy_flag_dis = getDistance(hero.bots[id].curr_pos,enemy.curr_flag_pos);
            clearCoins(id);
            int dis = enemy_flag_distance[hero.bots[id].curr_pos.x][hero.bots[id].curr_pos.y];

            if( dis<mn_dis and hero.flag_carrier == -1 ){
                mn_dis = dis;
                seeker = id;
            }
          //  cerr<<"distance from bot "<<id<<" is "<<dis<<endl;
        }
        keeper = -1;
        if( enemy.flag_carrier !=-1 ){
            int mn_dis2 = INT_MAX;
            for(int i=0;i<10;i++){
                if( hero.bots[i].hp>0 and seeker!=i ){
                    int dis = hero_flag_distance[hero.bots[i].curr_pos.x][hero.bots[i].curr_pos.y];
                    if( dis<mn_dis2 ){
                        mn_dis2 = dis;
                        keeper = i;
                    }
                }
            }
        }

        cin >> enemy.bots_cnt; cin.ignore();
        for(int i=0;i<10;i++) enemy.bots[i].isVisible = false;
        for (int i = 0; i < enemy.bots_cnt; i++) {
            int id,pos_r,pos_c,health,timeout;
            cin >> id >> pos_r >> pos_c >> health >> timeout; cin.ignore();
            enemy.bots[id].isVisible = true;
            enemy.bots[id].hp = health;
            enemy.bots[id].curr_pos = {pos_r,pos_c};
            enemy.bots[id].timeout = timeout;
        }
        int visible_coin_cnt;
        cin >> visible_coin_cnt; cin.ignore();
        for (int i = 0; i < visible_coin_cnt; i++) {
            int pos_r,pos_c;
            cin >> pos_r >> pos_c; cin.ignore();
            coins[pos_r][pos_c] = COIN;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        string commands = "";

        cerr<<"seeker "<<seeker<<" keeper "<<keeper<<endl;
        if( hero.flag_carrier!=-1 ){
          //  cerr<<"found flag"<<endl;
            for(int i=0;i<10;i++){
                if( i == keeper  ) i = 0;
                else if( i == 0 and keeper!=-1 ) i = keeper;

                string command = "";
                // if we have captured flag
                if(hero.bots[i].hp<=0) continue;
                if(i == seeker and secondaryFireOption(i) and hero.score>=7){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if( i == seeker ){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.flag_base.x)+" "+to_string(hero.flag_base.y);
                }else if( hero.score>=7 and i==keeper and fireInTheHole(i) == 2 ){
                    //command = "FREEZE "+to_string(i);
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if(i == keeper and hero.score>=7 and hero.bots[i].curr_pos == hero.curr_flag_pos and secondaryFireOption(i) ){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if(  hero.score>=7 and fireInTheHole(i) ){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if(i == keeper){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.curr_flag_pos.x)+" "+to_string(hero.curr_flag_pos.y);
                }  else if(  (hero.bots[i].curr_pos == hero.bots[i].target_pos or hero.bots[i].target_pos == Coord(0,0) )  ){
                    hero.bots[i].target_pos = getPathPlanner(hero.bots[i].curr_pos);
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }else{
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }
                if(  commands == "" ) commands = command;
                else commands = commands+" | "+command;
                if( i == keeper ) i = 0;
                else if( i == 0  and keeper!=-1 ) i = keeper;
            }
        }else{
           //  cerr<<"not found flag and seeker is "<<seeker<<endl;
            for(int i=0;i<10;i++){
                if( i == keeper ) i = 0;
                else if( i == 0  and keeper!=-1 ) i = keeper;
                string command = "";
                // if we have captured flag
                if(hero.bots[i].hp<=0) continue;
                if(i == seeker and seekerFireOption(i) and hero.score>=7){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if( i == seeker ){
                    command = "MOVE "+to_string(i)+" "+to_string(enemy.curr_flag_pos.x)+" "+to_string(enemy.curr_flag_pos.y);
                }else if( hero.score>=7 and i==keeper and fireInTheHole(i) == 2 ){
                    // command = "FREEZE "+to_string(i);
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if(i == keeper and hero.bots[i].curr_pos == hero.curr_flag_pos and secondaryFireOption(i) ){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if( hero.score>=7 and fireInTheHole(i) ){
                    command = "FIRE "+to_string(i);
                    hero.score-=7;
                }else if(i == keeper){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.curr_flag_pos.x)+" "+to_string(hero.curr_flag_pos.y);
                }  else if( ((hero.bots[i].curr_pos == hero.bots[i].target_pos) or (hero.bots[i].target_pos == Coord(0,0)) )  ){
                    hero.bots[i].target_pos = getPathPlanner(hero.bots[i].curr_pos);
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                } else{
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }
                if(  commands == "" ) commands = command;
                else commands = commands+" | "+command;
                if( i == keeper ) i = 0;
                else if( i == 0  and keeper!=-1 ) i = keeper;
            }
        }


        cout <<commands<<endl;
    }
}


