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
int maxDepth = 4;

float evaluateGame (Game* game);
float minVal(Game *state, float alpha, float beta, int depth);
float maxVal(Game *state, float alpha, float beta, int depth);

void chooseAndPlayMove();
void chooseAndPlayRandomMove(int color);
void possibleOpponentMoves(int color);



fstream f;


int main()
{
    cin>>color;
    color = color-1;
    cin>>n;
    cin>>m;
    cin>>time_left;
    
    game = new Game(n,m);
    // game->printBoard();
    string move;
    
    srand(time(0));
    
    if (color == 0)
    {
        //My Move
        // cout<<"RANDOMPLAYER'S MOVE"<<endl<<endl;
        chooseAndPlayMove();
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
        chooseAndPlayMove();
    }
    return 0;
}


float evaluateGame (Game* game)
{
    float ws = 1, wth = 100;
    int blackSoldiers = (game->getBlackSoldiers()).size();
    int whiteSoldiers = (game->getWhiteSoldiers()).size();
    int blackTownHalls = (game->getBlackTownHalls()).size();
    int whiteTownHalls = (game->getWhiteTownHalls()).size();



    float evaluationBlack = ws*(blackSoldiers - whiteSoldiers) + wth*(blackTownHalls - whiteTownHalls);

    // if(evaluationBlack>1000000)
    // {
    //  cout<<-(whiteSoldiers.size())<<" "<<wth*(blackTownHalls.size() - whiteTownHalls.size())<<" "<<evaluationBlack<<endl;
    // }

    if (!color)
        return evaluationBlack;
    else
        return -1*evaluationBlack;
}


float minVal(Game *state, float alpha, float beta, int depth)
{
    // cout<<depth<<endl;
    //pseudo leaf
    if(depth==maxDepth)
        return evaluateGame(state);

    bool hasChildren = false;

    vector<pii> opponentSoldiers;


    if (color)
        opponentSoldiers = game->getBlackSoldiers();
    else
        opponentSoldiers = game->getWhiteSoldiers();


    for(int i=0;i<opponentSoldiers.size();i++)
    {
        pii chosenSoldier = opponentSoldiers[i];
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier, (color+1)%2);
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier, (color+1)%2);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', (color+1)%2);
            float childStateValue = maxVal(childState, alpha, beta, depth+1);
            beta = min(beta,childStateValue);
            if (alpha>=beta) 
                return childStateValue;
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', (color+1)%2);
            float childStateValue = maxVal(childState, alpha, beta, depth+1);
            beta = min(beta,childStateValue);
            if (alpha>=beta) 
                return childStateValue;
        }
    }

    if(!hasChildren)
        return evaluateGame(state);

    return beta;
}

float maxVal(Game *state, float alpha, float beta, int depth)
{
    // cout<<depth<<endl;
    //pseudo leaf
    if(depth==maxDepth)
        return evaluateGame(state);

    bool hasChildren = false;

    vector<pii> mySoldiers;


    if (!color)
        mySoldiers = game->getBlackSoldiers();
    else
        mySoldiers = game->getWhiteSoldiers();


    for(int i=0;i<mySoldiers.size();i++)
    {
        pii chosenSoldier = mySoldiers[i];
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,color);
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,color);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', color);
            float childStateValue = minVal(childState, alpha, beta, depth+1);
            alpha = max(alpha,childStateValue);
            if (alpha>=beta) 
                return childStateValue;
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', color);
            float childStateValue = minVal(childState, alpha, beta, depth+1);
            alpha = max(alpha,childStateValue);
            if (alpha>=beta) 
                return childStateValue;
        }
    }

    if(!hasChildren)
        return evaluateGame(state);

    return alpha;
}


void chooseAndPlayMove()
{

    float alpha = -INT32_MAX;
    float beta = INT32_MAX;

    pii soldierPosition, finalPosition;
    char action;


    vector<pii> mySoldiers;
    if (color==0)
        mySoldiers = game->getBlackSoldiers();
    else
        mySoldiers = game->getWhiteSoldiers();


    // game->printVector(mySoldiers);
    // game->printBoard();
    for(int i=0;i<mySoldiers.size();i++)
    {
        // cout<<i<<" ";
        pii chosenSoldier = mySoldiers[i];
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,color);
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,color);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            // cout<<j<<" ";
            Game* childState = new Game(*game);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', color);

            float childStateValue = minVal(childState, alpha, beta, 1);


            // cout<<childStateValue<<endl;
            // childState->printBoard();
            if(childStateValue>alpha)
            {
                alpha = childStateValue;
                soldierPosition = mySoldiers[i];
                finalPosition = movesForChosenSoldier[j];
                action = 'M';
            }
            
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            
            Game* childState = new Game(*game);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', color);


            float childStateValue = minVal(childState, alpha, beta, 1);
            
            // cout<<childStateValue<<endl;
            // childState->printBoard();
            if(childStateValue>alpha)
            {
                alpha = childStateValue;
                soldierPosition = mySoldiers[i];
                finalPosition = bombsForChosenSoldier[j];
                action = 'B';
            }
            
        }
    }
    

    cout<< "S " + to_string(soldierPosition.first) + " " + to_string(soldierPosition.second) + " " + string(1,action) + " " + to_string(finalPosition.first) + " " + to_string(finalPosition.second)<<endl;
    game->play(soldierPosition, finalPosition, action, color);

}

//Choosing and playing Random Move
void chooseAndPlayRandomMove(int color)
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
                // game->printBoard();
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
                // game->printBoard();
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





