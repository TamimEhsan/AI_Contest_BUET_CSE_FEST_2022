#include<bits/stdc++.h>
using namespace std;
#define WALL '#'
#define UNKNOWN '?'
#define EMPTY '.'
#define BOMB 'B'
#define COIN 'C'
struct Coord{
    int x;
    int y;
    bool operator == (const Coord & crd) const {
        return (x == crd.x) and (y == crd.y);
    }
};
Coord my_flag_base,enemy_flag_base;
struct Power{
    string name;
    string price;
    string damage;
};
Power fire,freeze,mine;
struct Bot{
    int hp;
    Coord curr_pos;
    Coord target_pos;
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
int main(){

    cin >> HEIGHT >> WIDTH; cin.ignore();
    for (int i = 0; i < HEIGHT; i++) {
        cin >> grid[i]; cin.ignore();
        for(int j=0;j<grid[i].size();j++){
            if( grid[i][j] != WALL ) grid[i] = UNKNOWN;
        }
    }
    cin >> hero.flag_base.x >> hero.flag_base.y; cin.ignore();
    cin >> enemy.flag_base.x >> enemy.flag_base.y; cin.ignore();
    cin >> fire.name >> fire.price >> fire.damage; cin.ignore();
    cin >> freeze.name >> freeze.price >> freeze.damage; cin.ignore();
    cin >> mine.name >> mine.price >> mine.damage; cin.ignore();

    // game loop
    bool captured = false;
    while (1) {

        cin >> hero.score >> enemy.score; cin.ignore();

        cin >> hero.curr_flag_pos.x >> hero.curr_flag_pos.y >> hero.flag_carrier; cin.ignore();
        cin >> enemy.curr_flag_pos.x >> enemy.curr_flag_pos.y >> enemy.flag_carrier; cin.ignore();
        cin >> hero.bots_cnt; cin.ignore();
        for(int i=0;i<10;i++) hero.bots[i].hp = 0; // make it dead before resurrection U w U
        for (int i = 0; i < hero.bots_cnt; i++) {
            int id,pos_r,pos_c,health,timeout;
            cin >> id >> pos_r >> pos_c >> health >> timeout; cin.ignore();
            hero.bots[id].hp = health;
            hero.bots[id].curr_pos = {pos_r,pos_c};
            hero.bots[id].timeout = timeout;
            if( hero.bots[id].curr_pos == enemy.flag_base ) captured = true;
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
            grid[pos_r][pos_c] = COIN;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        string commands = "";
        for(int i=0;i<10;i++){
            string command;
            if( hero.bots[i].hp>0 and captured == false ){
                command = "MOVE "+to_string(i)+" "+to_string(enemy.flag_base.x)+" "+to_string(enemy.flag_base.y);
            } else if( hero.bots[i].hp>0 and captured == true ){
                command = "MOVE "+to_string(i)+" "+to_string(hero.flag_base.x)+" "+to_string(hero.flag_base.y);
            }
            if(  commands == "" ) commands = command;
            else commands = commands+" | "+command;
        }
        cout <<commands<<endl;
    }
}
