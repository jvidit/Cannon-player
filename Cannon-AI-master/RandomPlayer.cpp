#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <fstream>
#include <queue>
#include <deque>
#include <stdio.h>
#include <random>
#include <chrono>
#include <tuple>

using namespace std;

#define ll long long
#define ld long double
#define tc  ll t; cin>>t; while(t--)
#define io ios::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL);
#define fi first
#define sec second
#define mp make_pair
#define pb push_back
#define pll pair<ll,ll>

#define pii pair<int, int>

enum Pieces
{
    Empty = 0,
    BlackSoldier = 1,
    WhiteSoldier = 2,
    BlackTownHall = 3,
    WhiteTownHall = 4
};

class Game
{
    vector<vector<int> > board;
    
    //    --------> n/i/x
    //    |    ---------------------------------
    //    |    ---------------------------------
    //    |    ---------------------------------
    //    V    ---------------------------------
    //    m    ---------------------------------
    
    
    int n;
    int m;
    vector<pii> blackSoldiers;
    vector<pii> whiteSoldiers;
    vector<pii> blackTownHalls;
    vector<pii> whiteTownHalls;
    void removePiece(pii piecePosition, int pieceType);
    void movePiece(pii initialPosition, pii finalPosition, int pieceType);
public:
    
    Game(int n, int m);
    
    void play(pii soldierPosition, pii finalPosition, char action, int color);          //0 for black, 1 for white
    vector<pii> validMoves(pii soldierPosition, int color);
    vector<pii> validBombs(pii soldierPosition, int color);
    
    vector<pii> getBlackSoldiers();
    vector<pii> getBlackTownHalls();
    vector<pii> getWhiteSoldiers();
    vector<pii> getWhiteTownHalls();
    
    void printBoard();
    void printVector(vector<pii> v);
    
};


void Game::removePiece(pii piecePosition, int pieceType)
{
    if(pieceType==Empty)
        return;
    else if(pieceType==BlackSoldier)
    {
        for(int i=0;i<blackSoldiers.size();i++)
        {
            if(blackSoldiers[i]==piecePosition)
            {
                blackSoldiers.erase(blackSoldiers.begin()+i);
                break;
            }
        }
    }
    else if(pieceType==WhiteSoldier)
    {
        for(int i=0;i<whiteSoldiers.size();i++)
        {
            if(whiteSoldiers[i]==piecePosition)
            {
                whiteSoldiers.erase(whiteSoldiers.begin()+i);
                break;
            }
        }
    }
    else if(pieceType==BlackTownHall)
    {
        for(int i=0;i<blackTownHalls.size();i++)
        {
            if(blackTownHalls[i]==piecePosition)
            {
                blackTownHalls.erase(blackTownHalls.begin()+i);
                break;
            }
        }
    }
    else if(pieceType==WhiteTownHall)
    {
        for(int i=0;i<whiteTownHalls.size();i++)
        {
            if(whiteTownHalls[i]==piecePosition)
            {
                whiteTownHalls.erase(whiteTownHalls.begin()+i);
                break;
            }
        }
    }
    board[piecePosition.first][piecePosition.second]=Empty;
}

void Game::movePiece(pii initialPosition, pii finalPosition, int pieceType)
{
    if(pieceType==Empty)
        return;
    else if(pieceType==BlackSoldier)
    {
        for(int i=0;i<blackSoldiers.size();i++)
        {
            if(blackSoldiers[i]==initialPosition)
            {
                blackSoldiers[i]=finalPosition;
                break;
            }
        }
    }
    else if(pieceType==WhiteSoldier)
    {
        for(int i=0;i<whiteSoldiers.size();i++)
        {
            if(whiteSoldiers[i]==initialPosition)
            {
                whiteSoldiers[i]=finalPosition;
                break;
            }
        }
    }
    board[initialPosition.first][initialPosition.second]=Empty;
    board[finalPosition.first][finalPosition.second]=pieceType;
}

Game::Game(int nIn, int mIn)
{
    this->n = nIn;
    this->m = mIn;
    
    for(int i=0;i<n;i++)
    {
        vector<int> temp;
        
        for(int j=0;j<m;j++)
            temp.pb(Empty);
        
        board.pb(temp);
    }
    
    //placing peices onto the board
    for(int i=0;i<n;i++)
    {
        //placing townhalls
        if(i%2==0)
            board[i][0]=WhiteTownHall;
        else
            board[i][m-1]=BlackTownHall;
        
        
        
        //placing soldiers, 3 in a column
        if(i%2==0)
        {
            for(int j=m-1;j>=m-3;j--)
                board[i][j]=BlackSoldier;
        }
        else
        {
            for(int j=0;j<3;j++)
                board[i][j]=WhiteSoldier;
        }
    }
    
    //adding townhalls to vector
    for(int i=0;i<n/2;i++)
    {
        whiteTownHalls.pb(mp(2*i,0));
        blackTownHalls.pb(mp(2*i+1,m-1));
    }
    
    //adding soldiers to vector
    for(int i=0;i<n/2;i++)
    {
        for(int j=0;j<3;j++)
        {
            blackSoldiers.pb(mp(2*i,m-j-1));
            whiteSoldiers.pb(mp(2*i+1,j));
        }
    }
}

void Game::play(pii soldierPosition, pii finalPosition, char action, int color)
{
    //move is of the form  :      S x y M/B a b
    
    removePiece(finalPosition, board[finalPosition.first][finalPosition.second]);
    
    //handling bombs
    if(action=='B')
        return;
    
    //handle move
    movePiece(soldierPosition,finalPosition, board[soldierPosition.first][soldierPosition.second]);
}

vector<pii> Game::validMoves(pii soldierPosition, int color)
{
    vector<pii> moves;
    int x = soldierPosition.first, y = soldierPosition.second;
    //white Soldier
    if(color)
    {
        //forward move
        if(y+1<=m-1)
        {
            for(int i=max(0,x-1);i<=min(n-1,x+1);i++)
            {
                if(board[i][y+1]!=WhiteTownHall && board[i][y+1]!=WhiteSoldier)
                    moves.pb(mp(i,y+1));
            }
        }
        
        //sideways capture, excluding forward capture
        if(x-1>=0)
        {
            if(board[x-1][y]==BlackSoldier || board[x-1][y]==BlackTownHall)
                moves.pb(mp(x-1,y));
        }
        if(x+1<=n-1)
        {
            if(board[x+1][y]==BlackSoldier || board[x+1][y]==BlackTownHall)
                moves.pb(mp(x+1,y));
        }
        
        //retreat backwards
        {
            bool isAttacked = false;
            if(y+1<=m-1)
            {
                for(int i=max(0,x-1);i<=min(n-1,x+1);i++)
                    if(board[i][y+1]==BlackSoldier)
                        isAttacked = true;
            }
            if(x-1>=0)
            {
                if(board[x-1][y]==BlackSoldier)
                    isAttacked = true;
            }
            if(x+1<=n-1)
            {
                if(board[x+1][y]==BlackSoldier)
                    isAttacked = true;
            }
            
            if(y-2>=0 && isAttacked)
            {
                if(x-2>=0 && board[x-2][y-2]!=WhiteTownHall && board[x-2][y-2]!=WhiteSoldier)
                    moves.pb(mp(x-2,y-2));
                if(board[x][y-2]!=WhiteTownHall && board[x][y-2]!=WhiteSoldier)
                    moves.pb(mp(x,y-2));
                if(x+2<=n-1 && board[x+2][y-2]!=WhiteTownHall && board[x+2][y-2]!=WhiteSoldier)
                    moves.pb(mp(x+2,y-2));
            }
        }
    }
    else
    {
        //forward move
        if(y-1>=0)
        {
            for(int i=max(0,x-1);i<=min(n-1,x+1);i++)
            {
                if(board[i][y-1]!=BlackTownHall && board[i][y-1]!=BlackSoldier)
                    moves.pb(mp(i,y-1));
            }
        }
        
        //sideways capture, excluding forward capture
        if(x-1>=0)
        {
            if(board[x-1][y]==WhiteSoldier || board[x-1][y]==WhiteTownHall)
                moves.pb(mp(x-1,y));
        }
        if(x+1<=n-1)
        {
            if(board[x+1][y]==WhiteSoldier || board[x+1][y]==WhiteTownHall)
                moves.pb(mp(x+1,y));
        }
        
        //retreat backwards
        {
            bool isAttacked = false;
            if(y-1>=0)
            {
                for(int i=max(0,x-1);i<=min(n-1,x+1);i++)
                    if(board[i][y-1]==WhiteSoldier)
                        isAttacked = true;
            }
            if(x-1>=0)
            {
                if(board[x-1][y]==WhiteSoldier)
                    isAttacked = true;
            }
            if(x+1<=n-1)
            {
                if(board[x+1][y]==WhiteSoldier)
                    isAttacked = true;
            }
            
            if(y+2<=m-1 && isAttacked)
            {
                if(x-2>=0 && board[x-2][y+2]!=BlackTownHall && board[x-2][y+2]!=BlackSoldier)
                    moves.pb(mp(x-2,y+2));
                if(board[x][y+2]!=BlackTownHall && board[x][y+2]!=BlackSoldier)
                    moves.pb(mp(x,y+2));
                if(x+2<=n-1 && board[x+2][y+2]!=BlackTownHall && board[x+2][y+2]!=BlackSoldier)
                    moves.pb(mp(x+2,y+2));
            }
        }
    }
    
    //CANNON MOVE
    //horizontal
    if(x-3>=0 && board[x-1][y]==board[x][y] && board[x-2][y]==board[x][y] && board[x-3][y]==Empty)
        moves.pb(mp(x-3,y));
    if(x+3<=n-1 && board[x+1][y]==board[x][y] && board[x+2][y]==board[x][y] && board[x+3][y]==Empty)
        moves.pb(mp(x+3,y));
    
    //vertical
    if(y-3>=0 && board[x][y-1]==board[x][y] && board[x][y-2]==board[x][y] && board[x][y-3]==Empty)
        moves.pb(mp(x,y-3));
    if(y+3<=m-1 && board[x][y+1]==board[x][y] && board[x][y+2]==board[x][y] && board[x][y+3]==Empty)
        moves.pb(mp(x,y+3));
    
    //diagnol top right
    if(y-3>=0 && x+3<=n-1 && board[x+1][y-1]==board[x][y] && board[x+2][y-2] && board[x+3][y-3]==Empty)
        moves.pb(mp(x+3,y-3));
    if(y+3<=m-1 && x-3>=0 && board[x-1][y+1]==board[x][y] && board[x-2][y+2] && board[x-3][y+3]==Empty)
        moves.pb(mp(x-3,y+3));
    
    //diagnol top left
    if(y-3>=0 && x-3>=0 && board[x-1][y-1]==board[x][y] && board[x-2][y-2] && board[x-3][y-3]==Empty)
        moves.pb(mp(x-3,y-3));
    if(y+3<=m-1 && x+3<=n-1 && board[x+1][y+1]==board[x][y] && board[x+2][y+2] && board[x+3][y+3]==Empty)
        moves.pb(mp(x+3,y+3));
    
    
    
    
    
    cout<<"SOLDIER MOVES "<<soldierPosition.first<<" "<<soldierPosition.second<<endl;
    printVector(moves);
    cout<<endl;
    return moves;
}

vector<pii> Game::validBombs(pii soldierPosition, int color)
{
    vector<pii> moves;
    int x = soldierPosition.first, y = soldierPosition.second;
    
    
    //White cannon
    int soldier, townHall;
    if(color)
    {
        soldier=WhiteSoldier;
        townHall=WhiteTownHall;
    }
    else
    {
        soldier=BlackSoldier;
        townHall=BlackTownHall;
    }
    
    
    //check horizontal cannon
    if(x-1>=0 && x+1<=n-1)
    {
        if(board[x-1][y]==board[x][y] && board[x+1][y]==board[x][y])
        {
            if(x-2>=0 && board[x-2][y]==Empty)
            {
                if(x-3>=0 && board[x-3][y]!=soldier && board[x-3][y]!=townHall)
                    moves.pb(mp(x-3,y));
                if(x-4>=0 && board[x-4][y]!=soldier && board[x-4][y]!=townHall)
                    moves.pb(mp(x-4,y));
            }
            if(x+2<=n-1 && board[x+2][y]==Empty)
            {
                if(x+3<=n-1 && board[x+3][y]!=soldier && board[x+3][y]!=townHall)
                    moves.pb(mp(x+3,y));
                if(x+4<=n-1 && board[x+4][y]!=soldier && board[x+4][y]!=townHall)
                    moves.pb(mp(x+4,y));
            }
        }
    }
    
    //check vertical cannon
    if(y-1>=0 && y+1<=m-1)
    {
        if(board[x][y-1]==board[x][y] && board[x][y+1]==board[x][y])
        {
            if(y-2>=0 && board[x][y-2]==Empty)
            {
                if(y-3>=0 && board[x][y-3]!=soldier && board[x][y-3]!=townHall)
                    moves.pb(mp(x,y-3));
                if(y-4>=0 && board[x][y-4]!=soldier && board[x][y-4]!=townHall)
                    moves.pb(mp(x,y-4));
            }
            if(y+2<=m-1 && board[x][y+2]==Empty)
            {
                if(y+3<=m-1 && board[x][y+3]!=soldier && board[x][y+3]!=townHall)
                    moves.pb(mp(x,y+3));
                if(y+4<=m-1 && board[x][y+4]!=soldier && board[x][y+4]!=townHall)
                    moves.pb(mp(x,y+4));
            }
        }
    }
    
    //check diagnol top right
    if(y-1>=0 && y+1<=m-1 && x-1>=0 && x+1<=n-1)
    {
        if(board[x+1][y-1]==board[x][y] && board[x-1][y+1]==board[x][y])
        {
            if(y-2>=0 && x+2<=n-1 && board[x+2][y-2]==Empty)
            {
                if(y-3>=0 && x+3<=n-1 && board[x+3][y-3]!=soldier && board[x+3][y-3]!=townHall)
                    moves.pb(mp(x+3,y-3));
                if(y-4>=0 && x+4<=n-1 && board[x+4][y-4]!=soldier && board[x+4][y-4]!=townHall)
                    moves.pb(mp(x+4,y-4));
            }
            if(y+2<=m-1 && x-2>=0 && board[x-2][y+2]==Empty)
            {
                if(y+3<=m-1 && x-3>=0 && board[x-3][y+3]!=soldier && board[x-3][y+3]!=townHall)
                    moves.pb(mp(x-3,y+3));
                if(y+4<=m-1 && x-4>=0 && board[x-4][y+4]!=soldier && board[x-4][y+4]!=townHall)
                    moves.pb(mp(x-4,y+4));
            }
        }
    }
    
    //check diagnol top left
    if(y-1>=0 && y+1<=m-1 && x-1>=0 && x+1<=n-1)
    {
        if(board[x-1][y-1]==board[x][y] && board[x+1][y+1]==board[x][y])
        {
            if(y-2>=0 && x-2>=0 && board[x-2][y-2]==Empty)
            {
                if(y-3>=0 && x-3>=0 && board[x-3][y-3]!=soldier && board[x-3][y-3]!=townHall)
                    moves.pb(mp(x-3,y-3));
                if(y-4>=0 && x-4>=0 && board[x-4][y-4]!=soldier && board[x-4][y-4]!=townHall)
                    moves.pb(mp(x-4,y-4));
            }
            if(y+2<=m-1 && x+2<=n-1 && board[x+2][y+2]==Empty)
            {
                if(y+3<=m-1 && x+3<=n-1 && board[x+3][y+3]!=soldier && board[x+3][y+3]!=townHall)
                    moves.pb(mp(x+3,y+3));
                if(y+4<=m-1 && x+4<=n-1 && board[x+4][y+4]!=soldier && board[x+4][y+4]!=townHall)
                    moves.pb(mp(x+4,y+4));
            }
        }
    }
    
    cout<<"SOLDIER BOMBS "<<soldierPosition.first<<" "<<soldierPosition.second<<endl;
    printVector(moves);
    cout<<endl;
    return moves;
}

vector<pii> Game::getBlackSoldiers()
{   printVector(blackSoldiers);  return blackSoldiers; }

vector<pii> Game::getWhiteSoldiers()
{   printVector(whiteSoldiers);  return whiteSoldiers;}

vector<pii> Game::getBlackTownHalls()
{   printVector(blackTownHalls); return blackTownHalls;}

vector<pii> Game::getWhiteTownHalls()
{   printVector(whiteTownHalls); return whiteTownHalls;}

void Game::printBoard()
{
    for(int j=0;j<m;j++)
    {
        for(int i=0;i<n;i++)
            cout<<board[i][j]<<" ";
        cout<<endl;
    }
}

void Game::printVector(vector<pii> v)
{
    for(int i=0;i<v.size();i++)
        cout<<v[i].first<<" "<<v[i].second<<endl;
}




int color;
int n;
int m;
float time_left;
Game*game;

//Choosing and playing my Move
void chooseAndPlayMove(int color)
{
    vector<pii> mySoldiers;
    while(true)
    {
        if (color==0)
            mySoldiers = game->getBlackSoldiers();
        else
            mySoldiers = game->getWhiteSoldiers();
        
        pii chosenSoldier = mySoldiers.at(rand()%mySoldiers.size());
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier, color);
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier, color);
        
        if(rand()%2 == 0)
        {
            if (movesForChosenSoldier.size() == 0)
                continue;
            else
            {
                pii chosenMoveForChosenSoldier = movesForChosenSoldier.at(rand()%movesForChosenSoldier.size());
                pii soldierPosition = chosenSoldier;
                pii finalPosition = chosenMoveForChosenSoldier;
                char action = 'M';
                game->play(soldierPosition,finalPosition,action,color);
                game->printBoard();
                break;
            }
        }
        else
        {
            if (bombsForChosenSoldier.size() == 0)
                continue;
            else
            {
                pii chosenBombForChosenSoldier = bombsForChosenSoldier.at(rand()%bombsForChosenSoldier.size());
                pii soldierPosition = chosenSoldier;
                pii finalPosition = chosenBombForChosenSoldier;
                char action = 'B';
                game->play(soldierPosition,finalPosition,action,color);
                game->printBoard();
                break;
            }
        }
        
    }
}

int main()
{
    cin>>color;
    color = color-1;
    cin>>n;
    cin>>m;
    cin>>time_left;
    
    game = new Game(n,m);
    game->printBoard();
    string move;
    
    srand(time(0));
    
    if (color == 0)
    {
        //My Move
        chooseAndPlayMove(color);
    }
    while(true)
    {
        //Opponent's Move
        string opponentMove;
        pii soldierPosition;
        pii finalPosition;
        char action;
        char temp;
        cin>>temp;
        cin>>soldierPosition.first;
        cin>>soldierPosition.second;
        cin>>action;
        cin>>finalPosition.first;
        cin>>finalPosition.second;
        game->play(soldierPosition,finalPosition,action,(color+1)%2);
        
        //My Move
        chooseAndPlayMove(color);
    }
    return 0;
}










