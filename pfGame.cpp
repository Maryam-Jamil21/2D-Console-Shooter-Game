#include<iostream>
#include<vector>
#include<cstdlib>
#include<conio.h>

using namespace std;
const int ROWS=20;
const int COLS=40;
char board[ROWS][COLS];

int playerX=11,playerY=COLS/2;
int playerHealth=3,playerLives=3,score=0;
bool gameRunning=true; //added global flag to control game loop

struct Bullet{
    int x,y;
    bool isEnemy;
};
vector<Bullet> bullets;
struct Enemy{
    int x,y; 
    int health;
    char type;

};
vector<Enemy> enemies;

string maze[]={
    "########################################",
    "#                                      #",
    "#   ###########           #########    #",
    "#                                      #",
    "#       #####         #####            #",
    "#                                      #",
    "#   #######             #######        #",
    "#                                      #",
    "#     ########   #######     ########  #",
    "#                                      #",
    "#   ###        ########        ###     #",
    "#                                      #",
    "########################################"
};
void loadMaze(){
    for (int i=0;i<13;i++)
        for (int j=0;j<COLS;j++)
            board[i][j]=maze[i][j];
    for(int i=13;i<ROWS;i++)
       for(int j=0;j<COLS;j++)
           board[i][j]=' ';

}

void clearBoard(){
    for(int i=0;i<ROWS;i++)
       for(int j=0;j<COLS;j++)
        if(board[i][j]!='#')board[i][j]=' ';

}

void placePlayer(){
    if(board[playerX][playerY]!='#')
       board[playerX][playerY]='P';
}

void placeEnemies(){
    for(Enemy& e: enemies)
       if(e.health>0 && board[e.x][e.y]!='#')
          board[e.x][e.y]=e.type;
}

void placeBullets(){
    for(Bullet& b:bullets)
       if(b.x >= 0 && b.x < ROWS && b.y >= 0 && b.y<COLS && board[b.x][b.y]!='#')
          board[b.x][b.y]=b.isEnemy?'v':'^';
}

void printBoard(){
    clearBoard();
    loadMaze();
    placePlayer();
    placeEnemies();
    placeBullets();
    system("cls");
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++)
           cout<<board[i][j];
        cout<<endl;   
    }
    cout<<"Health: "<<playerHealth<< " | Lives: " << playerLives << " | Scores: "<< score << endl;
}

void movePlayerLeft(){
    if(playerY>0 && board[playerX][playerY-1]!='#')playerY--;
}

void movePlayerRight(){
    if(playerY<COLS-1 && board[playerX][playerY+1]!='#')playerY++;
}

void fire(){
    int bulletX=playerX-1;
    if(bulletX >=0 && board[bulletX][playerY]!='#')
       bullets.push_back({bulletX,playerY,false});
}
void firefromEnemies(){
    for(Enemy& e:enemies)
       if(e.health>0 && e.x+1<ROWS && board[e.x+1][e.y]!='#')
          bullets.push_back({e.x+1,e.y,true});
}

void moveFire(){
    vector<Bullet> newBullets;
    for(Bullet& b : bullets){
        int newX=b.x+(b.isEnemy?1:-1);
        if(newX<0 || newX>=ROWS || board[newX][b.y]=='#')continue;
        if(b.isEnemy && newX==playerX && b.y==playerY){
            playerHealth--;
            if(playerHealth==0){
                playerLives--;
                playerHealth=3;
                if(playerLives==0) gameRunning=false;

            }
            continue;
        }
        if(!b.isEnemy){
            bool hit=false;
            for(Enemy& e:enemies){
                if(e.x==newX && e.y==b.y && e.health>0){
                    e.health--;
                    if(e.health==0)score+=10;
                    hit=true;
                    break;
                }
            }
            if(hit) continue;
        }
        newBullets.push_back({newX,b.y,b.isEnemy});

    }
    bullets=newBullets;
}

void moveEnemy(char type,string direction){
    for(Enemy& e:enemies){
        if(e.type==type && e.health>0){
            int nx=e.x,ny=e.y;
            if(direction=="left")ny--;
            if(direction=="right")ny++;
            if(direction=="down")nx++;
            if(nx>=0 && nx<ROWS && ny>=0 && ny<COLS && board[nx][ny]!='#'){
                e.x=nx;e.y=ny;
            }
            
        }
    }
}

bool allEnemiesDefeated(){
    for(Enemy& e:enemies)
       if(e.health>0)return false;
       return true;
}

void loadEnemies(){
    enemies.clear();
    enemies.push_back({1, 5, 2, 'E'});
    enemies.push_back({1, 20, 3, 'F'});
    enemies.push_back({1, 35, 1, 'G'});
}

void gameLoop(){
    int step=0;
    loadEnemies();
    gameRunning=true;

    while(gameRunning){
        printBoard();
        if(_kbhit()){
            char input=_getch();
            if(input=='a')movePlayerLeft();
            else if(input=='d')movePlayerRight();
            else if(input=='f')fire();
            else if(input=='q'){
                gameRunning=false;
                break;
            }

        }

        if(step%2==0)moveEnemy('E',"left");
        if(step%3==0)moveEnemy('F',"right");
        if(step%5==0)moveEnemy('G',"down");
        if(step%4==0)firefromEnemies();
        moveFire();
        step++;

        if(allEnemiesDefeated()){
            printBoard();
            cout<<"\nLEVEL 1 CLEARED!\n";
            gameRunning=false;
            break;
        }
        _sleep(150);
    
    }
    if(playerLives==0)
       cout<<"\n GAME OVER!\n";
}

int main(){
    gameLoop();

return 0;
}