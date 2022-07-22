#include<bits/stdc++.h>
using namespace std;
#define WALL '#'
#define UNKNOWN '?'
#define EMPTY '.'
#define BOMB 'B'
//#define COIN 'C'
#define INF 1000


const int MAX_HEIGHT = 50;
int dx[] = {1,-1,0,0};
int dy[] = {0,0,1,-1};

int HEIGHT,WIDTH;

string grid[MAX_HEIGHT];
int coins[MAX_HEIGHT][MAX_HEIGHT];
int pairWiseDistance[1500][1500];


struct Coord{
    int x;
    int y;

    Coord(int xx=0,int yy=0){
        x = xx;
        y = yy;
    }
    bool operator == (const Coord & crd) {
        return ((x == crd.x) and (y == crd.y));
    }
    bool operator != (const Coord & crd) {
        return (x != crd.x) or (y != crd.y);
    }
    int convertTo1D(){
        return x*WIDTH+y;
    }
    int getDistance( Coord & crd) {
        return pairWiseDistance[ this->convertTo1D() ][crd.convertTo1D()];
    }
    int getManhattan( Coord & crd) {
        return abs(x-crd.x)+abs(y-crd.y);
    }
    Coord getNextPos(int dir){
        if(dir>=4 or dir<0) dir = 0;
        return Coord(x+dx[dir],y+dy[dir]);
    }
    bool inVisual(const Coord & crd) {
        return x==crd.x or y==crd.y;
    }
};
Coord my_flag_base,enemy_flag_base;

Coord convertTo2D(int pt){
    return {pt/WIDTH,pt%WIDTH};
}

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
    int prev_flag_carrier = -1; // opponent will cary flag
    int bots_cnt;
    Bot bots[10];


};
Player hero,enemy;
// when 0 or 1 dies, swap it will last bot and pop back
 // 2d points will changed to 1d

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



Coord getNextPossiblePosOfEnemySeeker(){
    int id = enemy.flag_carrier;
    int currDistance = enemy.flag_base.getDistance(enemy.bots[id].curr_pos);
    for(int i=0;i<4;i++){
        Coord nxtPos = enemy.bots[id].curr_pos.getNextPos(i);
        int nxtDistance = enemy.flag_base.getDistance( nxtPos );
        if( nxtDistance == currDistance-1 ) return nxtPos;
    }
    return enemy.bots[id].curr_pos;
}
Coord getNextPossiblePosOfHeroSeeker(){
    int id = hero.flag_carrier;
    if( id == -1 ) return {100,100};
    int currDistance = hero.flag_base.getDistance(hero.bots[id].curr_pos);
    for(int i=0;i<4;i++){
        Coord nxtPos = hero.bots[id].curr_pos.getNextPos(i);
        int nxtDistance = hero.flag_base.getDistance( nxtPos );
        if( nxtDistance == currDistance-1 ) return nxtPos;
    }
    return enemy.bots[id].curr_pos;
}

void calculateDistanceFromAPoint(Coord start,int dis[50][50]){
    int isVisited[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++) {
            isVisited[i][j] = 0;
            dis[i][j] = INF;
        }
    }
    if( grid[start.x][start.y] == WALL ) return;
    queue<Coord> qq;
    qq.push(start);
    dis[start.x][start.y] = 1;
    while( !qq.empty() ){
        auto u = qq.front();
        //cerr<<u.x<<" "<<u.y<<" "<<isVisited[u.x][u.y]<<endl;
        qq.pop();
        for(int i=0;i<4;i++){
            int vx = u.x+dx[i];
            int vy = u.y+dy[i];
            if( vx>=0 and vx<HEIGHT and vy>=0 and vy<WIDTH and grid[vx][vy] != WALL and dis[vx][vy] == INF ){
                qq.push({vx,vy});
                dis[vx][vy] = dis[u.x][u.y]+1;
            }
        }
    }
}

void calculatePairWiseDistance(){
    int dis[50][50];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            calculateDistanceFromAPoint({i,j},dis);
            for(int ii=0;ii<HEIGHT;ii++){
                for(int jj=0;jj<WIDTH;jj++){
                    pairWiseDistance[ Coord(i,j).convertTo1D() ][Coord(ii,jj).convertTo1D()] = dis[ii][jj];
                }
            }
        }
    }
}

int getPairWiseDistance(Coord A,Coord B){
    return A.getDistance(B);
}

vector<Coord> emptyCells;
int randomfunc(int j)
{
    return rand() % j;
}
void generateRandomPos(){
    random_shuffle(emptyCells.begin(), emptyCells.end(), randomfunc);
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
Coord getPathPlanner(Coord st,int moves = 1){
    if( moves >1000 ) return hero.curr_flag_pos;
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

int fireInTheHole(int id){

    bool visibleArea[50][50];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++)
            visibleArea[i][j] = false;
    }

    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;

    int enemyCount = 0,heroCount = 0;
    bool deepFreeze = false,hellFire = false;
    bool useFreeze = false;
    int k=0;
    cerr<<"fire in hole "<<id<<endl;
    while(grid[xx][yy+k]!=WALL){
        visibleArea[xx][yy+k] = true;
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy+k};
            if( hero.bots[i].curr_pos == boo and i!=id ) {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i and hero.bots[id].curr_pos.inVisual( getNextPossiblePosOfEnemySeeker() ) ) {


                   // auto pp = getNextPossiblePosOfEnemySeeker(enemy.flag_carrier);
                   // cerr<<"next possible pos of seeker "<<enemy.flag_carrier<<" is "<<pp.x<<" "<<pp.y<<endl;
                   // cerr<<"curr pos of seeker "<<i<<" is "<<enemy.bots[i].curr_pos.x<<" "<<enemy.bots[i].curr_pos.y<<endl;


                    if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( hero.score>=freeze.price+fire.price*2  ) {
                        useFreeze = true;
                        enemy.bots[ enemy.flag_carrier ].timeout = 1;
                    }else{
                        hellFire = true;
                    }
                }else if( enemy.prev_flag_carrier == i and enemy.flag_carrier == -1 ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        visibleArea[xx][yy-k] = true;
        for(int i=0;i<10;i++){
            Coord boo = {xx,yy-k};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i and hero.bots[id].curr_pos.inVisual( getNextPossiblePosOfEnemySeeker() ) ) {
                     if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( hero.score>=freeze.price+fire.price*2 ) {
                        useFreeze = true;
                        enemy.bots[ enemy.flag_carrier ].timeout = 1;
                    }else{
                        hellFire = true;
                    }
                }else if(  enemy.prev_flag_carrier == i and enemy.flag_carrier == -1 ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
        visibleArea[xx+k][yy] = true;
        for(int i=0;i<10;i++){
            Coord boo = {xx+k,yy};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i and hero.bots[id].curr_pos.inVisual( getNextPossiblePosOfEnemySeeker() ) ) {
                     if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( hero.score>=freeze.price+fire.price*2 ) {
                        useFreeze = true;
                        enemy.bots[ enemy.flag_carrier ].timeout = 1;
                    }else{
                        hellFire = true;
                    }
                }else if(  enemy.prev_flag_carrier == i and enemy.flag_carrier == -1 ) hellFire = true;
            }
        }
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        visibleArea[xx-k][yy] = true;
        for(int i=0;i<10;i++){
            Coord boo = {xx-k,yy};
            if( hero.bots[i].curr_pos == boo )  {
                heroCount++;
                if( hero.flag_carrier == i ) deepFreeze = true;
            }
            if( enemy.bots[i].curr_pos == boo and enemy.bots[i].isVisible ) {
                enemyCount++; cerr<<"x";
                if( enemy.flag_carrier == i and hero.bots[id].curr_pos.inVisual( getNextPossiblePosOfEnemySeeker() ) ) {
                     if(enemy.bots[i].hp<=fire.damage*2){
                        hellFire = true;
                    }else if( hero.score>=freeze.price+fire.price*2 ) {
                        useFreeze = true;
                        enemy.bots[ enemy.flag_carrier ].timeout = 1;
                    }else{
                        hellFire = true;
                    }
                }else if(  enemy.prev_flag_carrier == i and enemy.flag_carrier == -1 ) hellFire = true;
            }
        }
        k++;
    }
    bool heroSeekerWillBeVisible = false;
    auto nextPos = getNextPossiblePosOfHeroSeeker();
    if( nextPos != Coord(100,100) and visibleArea[nextPos.x][nextPos.y] == true and id!= hero.flag_carrier ) heroSeekerWillBeVisible = true;
    if( hellFire ) cerr<<"Found enemy flag carrier by "<<id<<endl; cerr<<endl;
    cerr<<"next move of hero seeker "<<getNextPossiblePosOfHeroSeeker().x<<" "<<getNextPossiblePosOfHeroSeeker().y<<endl;
    if(useFreeze) {
        cerr<<"a "<<endl;
        return 1;
    }
    if( hellFire ) {
        cerr<<"b "<<endl;
        return 1;
    }
    //return 0;
    cerr<<"c "<<endl;
    if( enemy.flag_carrier!=-1 and hero.bots[id].curr_pos.inVisual( enemy.bots[enemy.flag_carrier].curr_pos )  and !hero.bots[id].curr_pos.inVisual( getNextPossiblePosOfEnemySeeker() ) ) enemyCount--;
    return enemyCount>heroCount;
}

Coord getFarthestPos(){
    int mx = 0;
    Coord farthest;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            Coord curr = {i,j};
            int temp = hero.flag_base.getDistance(curr);
            if( temp> mx and temp!=INF ){
                mx = temp;
                farthest = curr;
            }
        }
    }
    return farthest;
}

bool sittingDuck(int seeker){
    int duck;
    int id = seeker;
    bool duckFound = false;
    cerr<<"duck searching "<<endl;
    for(int i=0;i<10;i++){
        if( enemy.bots[i].hp<=0 or !enemy.bots[i].isVisible ) continue;
        if(/* enemy.bots[i].curr_pos == enemy.curr_flag_pos or*/ enemy.bots[i].curr_pos.getManhattan(enemy.curr_flag_pos)<=1 ){
            duckFound = true;
            duck = i;
        }
    }
    if( !duckFound ) return false;
    cerr<<"duck found "<<duck<<endl;
    int xx = hero.bots[id].curr_pos.x;
    int yy = hero.bots[id].curr_pos.y;
    int k=0;
    while(grid[xx][yy+k]!=WALL){
        if( Coord(xx,yy+k) == enemy.bots[duck].curr_pos ) return true;
        k++;
    }
    k = 1;
    while(grid[xx][yy-k]!=WALL){
        if( Coord(xx,yy-k) == enemy.bots[duck].curr_pos ) return true;
        k++;
    }
    k = 1;
    while(grid[xx+k][yy]!=WALL){
       if( Coord(xx+k,yy) == enemy.bots[duck].curr_pos ) return true;
        k++;
    }
    k = 1;
    while(grid[xx-k][yy]!=WALL){
        if( Coord(xx-k,yy) == enemy.bots[duck].curr_pos ) return true;
        k++;
    }
    return false;
}

int main(){
    int totalScore = 0;
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
    calculatePairWiseDistance();
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
    int timer = 0;
    bool bombPlanted = false;
    int bomber= -1;
    Coord bombingLocation;
    while (++timer) {

        cin >> hero.score >> enemy.score; cin.ignore();

        cin >> hero.curr_flag_pos.x >> hero.curr_flag_pos.y >> enemy.flag_carrier; cin.ignore(); if(enemy.flag_carrier!=-1) enemy.prev_flag_carrier = enemy.flag_carrier;
        cin >> enemy.curr_flag_pos.x >> enemy.curr_flag_pos.y >> hero.flag_carrier; cin.ignore();
        cerr<<enemy.flag_carrier<<endl;
        cin >> hero.bots_cnt; cin.ignore();
        seeker = hero.flag_carrier;

        int mn_dis = INF;
        for(int i=0;i<10;i++) hero.bots[i].hp = 0; // make it dead before resurrection U w U

        for (int i = 0; i < hero.bots_cnt; i++) {
            int id,pos_r,pos_c,health,timeout;
            cin >> id >> pos_r >> pos_c >> health >> timeout; cin.ignore();
            hero.bots[id].hp = health;
            hero.bots[id].curr_pos = {pos_r,pos_c};
            hero.bots[id].timeout = timeout;
            clearCoins(id);
            //if( hero.bots[id].curr_pos == enemy.flag_base ) captured = true;
          //  int enemy_flag_dis = getDistance(hero.bots[id].curr_pos,enemy.curr_flag_pos);
            int dis = enemy.curr_flag_pos.getDistance(hero.bots[id].curr_pos);
           // cerr<<"distance from flag "<<id<<" "<<dis<<" "<<endl;
            if( dis<mn_dis and hero.flag_carrier == -1 ){
                mn_dis = dis;
                seeker = id;
            }
          //  cerr<<"distance from bot "<<id<<" is "<<dis<<endl;
        }

        keeper = -1; seeker = -1;
        if( enemy.flag_carrier !=-1 or timer>40 or ( enemy.prev_flag_carrier!=-1 and enemy.bots[enemy.prev_flag_carrier].isVisible ) ){
            int mn_dis2 = INF;
            for(int i=0;i<10;i++){
                if( hero.bots[i].hp>0 and seeker!=i ){
                    int dis = hero.curr_flag_pos.getDistance(hero.bots[i].curr_pos);
                    if( dis<mn_dis2 ){
                        mn_dis2 = dis;
                        keeper = i;
                    }
                }
            }
        }
        if( keeper!=-1 ) hero.bots[keeper].target_pos = hero.bots[keeper].curr_pos;
        if( timer == 1 ){
            hero.bots[0].target_pos = getFarthestPos();
            int mn_dis2 = INF;
            for(int i=0;i<10;i++){
                if( hero.bots[i].hp>0 and seeker!=i ){
                    int dis = hero.curr_flag_pos.getDistance(hero.bots[i].curr_pos);
                    if( dis<mn_dis2 ){
                        mn_dis2 = dis;
                        bomber = i;
                    }
                }
            }
            for(int i=0;i<4;i++){
                Coord temp = hero.flag_base.getNextPos(i);
                if( hero.flag_base.getDistance( temp ) == 2 ){
                    bombingLocation = hero.flag_base.getNextPos(i);
                    hero.bots[bomber].target_pos = bombingLocation;
                    cerr<<"aaaaaaaaaa "<<bombingLocation.x<<" "<<bombingLocation.y<<endl;
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
            coins[pos_r][pos_c] = 1;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        string commands = "";

        cerr<<"seeker "<<seeker<<" keeper "<<keeper<<endl;
        seeker = hero.flag_carrier = -1;
        if( hero.flag_carrier!=-1 ){
          //  cerr<<"found flag"<<endl;
            bool usedFreeze = false;
            for(int i=0;i<10;i++){
                string command = "";
                // if we have captured flag
                if(hero.bots[i].hp<=0) continue;
                if(  hero.score>=7 and fireInTheHole(i) == 2 and !usedFreeze ){
                    command = "FREEZE "+to_string(i); hero.score-=freeze.price; totalScore+=freeze.price; usedFreeze = true;
                   // command = "FIRE "+to_string(i); hero.score-=fire.price; totalScore+=fire.price;
                }else if(  hero.score>=7 and fireInTheHole(i) ){
                    command = "FIRE "+to_string(i); totalScore+=fire.price;
                    hero.score-=7;
                }else if( i == seeker ){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.flag_base.x)+" "+to_string(hero.flag_base.y);
                } else if(i == keeper){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.curr_flag_pos.x)+" "+to_string(hero.curr_flag_pos.y);
                }  else if( (hero.bots[i].curr_pos == hero.bots[i].target_pos or hero.bots[i].target_pos == Coord(0,0) )  ){
                    hero.bots[i].target_pos = getPathPlanner(hero.bots[i].curr_pos);
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }else{
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }
                if(  commands == "" ) commands = command;
                else commands = commands+" | "+command;
            }
        }else{
           //  cerr<<"not found flag and seeker is "<<seeker<<endl;
            bool usedFreeze = false;
            for(int i=0;i<10;i++){
                string command = "";
                // if we have captured flag
                if(hero.bots[i].hp<=0) continue;
                if( i == bomber and !bombPlanted and hero.bots[i].curr_pos == hero.bots[i].target_pos ){
                    bombPlanted = true;
                    command = "MINE "+to_string(i)+" "+to_string(hero.flag_base.x)+" "+to_string(hero.flag_base.y);
                    commands = commands+" | "+command;
                    continue;

                }
                if( i ==1 ){

                }
                if( hero.score>=7 and fireInTheHole(i) == 2 and !usedFreeze ){
                    command = "FREEZE "+to_string(i); hero.score-=freeze.price; totalScore+=freeze.price; usedFreeze = true;
                   // command = "FIRE "+to_string(i); hero.score-=fire.price; totalScore+=fire.price;

                }else if( hero.score>=7 and fireInTheHole(i) ){
                    command = "FIRE "+to_string(i); totalScore+=fire.price;
                    hero.score-=7;
                }else if( i == seeker and sittingDuck(i) and hero.score>=7 ){
                    command = "FIRE "+to_string(i); totalScore+=fire.price;
                    hero.score-=7;
                }else if( i == seeker ){
                    command = "MOVE "+to_string(i)+" "+to_string(enemy.curr_flag_pos.x)+" "+to_string(enemy.curr_flag_pos.y);
                }else if(i == keeper){
                    command = "MOVE "+to_string(i)+" "+to_string(hero.curr_flag_pos.x)+" "+to_string(hero.curr_flag_pos.y);
                }else if( ((hero.bots[i].curr_pos == hero.bots[i].target_pos) or (hero.bots[i].target_pos == Coord(0,0)) )  ){
                    hero.bots[i].target_pos = getPathPlanner(hero.bots[i].curr_pos);
                  //  hero.bots[i].target_pos = emptyCells[emptyCounter++]; if( emptyCounter>=emptyCells.size() ) emptyCounter = 0;
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                } else{
                    command = "MOVE "+to_string(i)+" "+to_string(hero.bots[i].target_pos.x)+" "+to_string(hero.bots[i].target_pos.y);
                }
                if(  commands == "" ) commands = command;
                else commands = commands+" | "+command;
            }
        }

        cerr<<"total points so far "<<totalScore<<endl;
        cout <<commands<<endl;
    }
}
