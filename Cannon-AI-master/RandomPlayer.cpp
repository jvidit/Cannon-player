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
#include "Game.h"

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
                cout<< "S " + to_string(soldierPosition.first) + " " + to_string(soldierPosition.second) + " M " + to_string(finalPosition.first) + " " + to_string(finalPosition.second)<<endl;
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
                cout<< "S " + to_string(soldierPosition.first) + " " + to_string(soldierPosition.second) + " B " + to_string(finalPosition.first) + " " + to_string(finalPosition.second)<<endl;
                game->printBoard();
                break;
            }
        }
        
    }
}

void possibleOpponentMoves(int color)
{
    // cout<<"YOUR MOVE"<<endl<<endl;
    vector<pii> opponentsSoldiers;
    if (color==1)
        opponentsSoldiers = game->getBlackSoldiers();
    else
        opponentsSoldiers = game->getWhiteSoldiers();
    
    for (int i=0;i<opponentsSoldiers.size();i++)
    {
        pii chosenSoldier = opponentsSoldiers.at(i);
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier, (color+1)%2);
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier, (color+1)%2);
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
        // cout<<"RANDOMPLAYER'S MOVE"<<endl<<endl;
        chooseAndPlayMove(color);
    }
    while(true)
    {
        //Opponent's Move
        // possibleOpponentMoves(color);
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
        // cout<<"RANDOMPLAYER'S MOVE"<<endl<<endl;
        chooseAndPlayMove(color);
    }
    return 0;
}










