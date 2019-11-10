#include "Game.h"
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

Game::Game(Game& game)
{
    this->blackSoldiers = game.getBlackSoldiers();
    this->whiteSoldiers = game.getWhiteSoldiers();
    this->blackTownHalls = game.getBlackTownHalls();
    this->whiteTownHalls = game.getWhiteTownHalls();
    this->board = game.getBoard();
    this->n = game.getN();
    this->m = game.getM();
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

    if(whiteTownHalls.size()<=2 || blackTownHalls.size()<=2)
        return moves;


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
    if(y-3>=0 && x+3<=n-1 && board[x+1][y-1]==board[x][y] && board[x+2][y-2]==board[x][y] && board[x+3][y-3]==Empty)
        moves.pb(mp(x+3,y-3));
    if(y+3<=m-1 && x-3>=0 && board[x-1][y+1]==board[x][y] && board[x-2][y+2]==board[x][y] && board[x-3][y+3]==Empty)
        moves.pb(mp(x-3,y+3));
    
    //diagnol top left
    if(y-3>=0 && x-3>=0 && board[x-1][y-1]==board[x][y] && board[x-2][y-2]==board[x][y] && board[x-3][y-3]==Empty)
        moves.pb(mp(x-3,y-3));
    if(y+3<=m-1 && x+3<=n-1 && board[x+1][y+1]==board[x][y] && board[x+2][y+2]==board[x][y] && board[x+3][y+3]==Empty)
        moves.pb(mp(x+3,y+3));
    
    
    
    
    
    // cerr<<"SOLDIER MOVES "<<soldierPosition.first<<" "<<soldierPosition.second<<endl;
    // printVector(moves);
    // cerr<<endl;
    return moves;
}

vector<pii> Game::validBombs(pii soldierPosition, int color)
{
    vector<pii> moves;
    int x = soldierPosition.first, y = soldierPosition.second;
    
    if(whiteTownHalls.size()<=2 || blackTownHalls.size()<=2)
        return moves;
    
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
    
    // cerr<<"SOLDIER BOMBS "<<soldierPosition.first<<" "<<soldierPosition.second<<endl;
    // printVector(moves);
    // cerr<<endl;
    return moves;
}

int Game::possibleBombs(pii soldierPosition, int color)
{
    
    int x = soldierPosition.first, y = soldierPosition.second;
    int possibleBombs = 0;
    
    //check vertical cannon
    if(y-1>=0 && y+1<=m-1)
    {
        if(board[x][y-1]==board[x][y] && board[x][y+1]==board[x][y])
        {
            if (!color)
            {
                if(y==3 && board[x][0]==WhiteTownHall && board[x][1]==WhiteSoldier && board[x][5]==Empty)
                    possibleBombs++;
                if(y==5 && board[x][0]==WhiteTownHall && board[x][2]==Empty && board[x][3]==Empty)
                    possibleBombs++;
            }

            else
            {
                if(y==m-4 && board[x][m-1]==BlackTownHall && board[x][m-2]==BlackSoldier && board[x][m-6]==Empty)
                    possibleBombs++;
                if(y==m-6 && board[x][m-1]==BlackTownHall && board[x][m-3]==Empty && board[x][m-4]==Empty)
                    possibleBombs++;
            }
        }
    }
    
    return possibleBombs;
}

int Game::defenseScore(int color)
{
    int defenseScore = 0;
    
    for(int i=0;i<=n-3;i++)
    {
        int diff = 0, flag = 0;

        if(!color)
        {

            for(int j=m-1;j>=0;j--)
            {
                int whiteSoldiers = 0,blackSoldiers = 0;
                for(int k = 0;k<3;k++)
                {
                    if(board[i+k][j]==WhiteSoldier)
                        whiteSoldiers++;
                    else if(board[i+k][j]==BlackSoldier)
                        blackSoldiers++;
                }

                diff-=whiteSoldiers;
                if(diff<0)
                {
                    flag=1;
                    break;
                }
                diff+=blackSoldiers;
            }
        }
        else
        {
            for(int j=0;j<m;j++)
            {
                int whiteSoldiers = 0,blackSoldiers = 0;
                for(int k = 0;k<3;k++)
                {
                    if(board[i+k][j]==WhiteSoldier)
                        whiteSoldiers++;
                    else if(board[i+k][j]==BlackSoldier)
                        blackSoldiers++;
                }

                diff-=blackSoldiers;
                if(diff<0)
                {
                    flag=1;
                    break;
                }
                diff+=whiteSoldiers;
            }

        }

        if(!flag)
            defenseScore++;
    }

    if(n+m==16)
        return 0;

    return defenseScore;
}

vector<pii> Game::getBlackSoldiers()
{   
	// printVector(blackSoldiers);  
	return blackSoldiers; 
}

vector<pii> Game::getWhiteSoldiers()
{   
	// printVector(whiteSoldiers);  
	return whiteSoldiers;
}

vector<pii> Game::getBlackTownHalls()
{   
	// printVector(blackTownHalls); 
	return blackTownHalls;
}

vector<pii> Game::getWhiteTownHalls()
{   
	// printVector(whiteTownHalls); 
	return whiteTownHalls;
}

vector<vector<int> > Game::getBoard()
{
	return board;
}
int Game::getN()
{
	return n;
}
int Game::getM()
{
	return m;
}
void Game::printBoard()
{
	;
	cerr<<endl;

	cerr<<". . ";
	for(int i=0;i<n;i++)
		cerr<<i<<" ";
	cerr<<"\n.\n";

    for(int j=0;j<m;j++)
    {
    	cerr<<j<<"   ";
        for(int i=0;i<n;i++)
            cerr<<board[i][j]<<" ";
        cerr<<endl;
    }
    cerr<<endl;
}

void Game::printVector(vector<pii> v)
{
	;
    for(int i=0;i<v.size();i++)
        cerr<<v[i].first<<" "<<v[i].second<<endl;
}

bool Game::hasSoldier(pii position)
{
    if ((board[position.first][position.second] == WhiteSoldier) || (board[position.first][position.second] == BlackSoldier) )
        return true;
    else 
        return false;
}

bool Game::hasTownHall(pii position)
{
    if ((board[position.first][position.second] == WhiteTownHall) || (board[position.first][position.second] == BlackTownHall) )
        return true;
    else 
        return false;
}

