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
#include "EvaluateGame.h"

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

float const timeThreshold = 200;
int const soldierThreshold = 0;

Game*game;
EvaluateGame*evalGame;
int maxDepth = 3;


float evaluateGame (Game* game);
float minVal(Game *state, float alpha, float beta, int depth);
float maxVal(Game *state, float alpha, float beta, int depth);

void chooseAndPlayMove();
void chooseAndPlayRandomMove(int color);
void possibleOpponentMoves(int color);
float expectedValue(vector<float> v);
float expectiVal(Game *state, float alpha, float beta, int depth);


fstream f;


int main()
{


    cin>>color;
    color = color-1;
    cin>>n;
    cin>>m;
    cin>>time_left;
    
    game = new Game(n,m);
    evalGame = new EvaluateGame();
    // game->printBoard();
    string move;
    
    srand(time(0));
    
    if (color == 0)
    {
        //My Move
        // cout<<"RANDOMPLAYER'S MOVE"<<endl<<endl;
        float tStart = clock();
        chooseAndPlayMove();
        time_left -= (clock() - tStart)/1000000;
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
        float tStart = clock();
        chooseAndPlayMove();
        time_left -= (clock() - tStart)/1000000;


        vector<pii> ownSoldiers; 
        if(color)
            ownSoldiers = game->getWhiteSoldiers();
        else
            ownSoldiers = game->getBlackSoldiers();

        if(time_left>=timeThreshold && ownSoldiers.size()<=soldierThreshold)
            maxDepth = 5;
        else
            maxDepth = 3;
    }
    return 0;
}


float evaluateGame (Game* game)
{
    float pieceEval = evalGame->countPieces(game, color);
    float attackEval = evalGame->countAttacks(game, color);
    float totEval = pieceEval + attackEval;
    totEval = (float)((int)totEval/7);

    return totEval;
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
        opponentSoldiers = state->getBlackSoldiers();
    else
        opponentSoldiers = state->getWhiteSoldiers();


    for(int i=0;i<opponentSoldiers.size();i++)
    {
        pii chosenSoldier = opponentSoldiers[i];
        vector<pii> movesForChosenSoldier = state->validMoves(chosenSoldier, (color+1)%2);
        vector<pii> bombsForChosenSoldier = state->validBombs(chosenSoldier, (color+1)%2);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', (color+1)%2);
            float childStateValue = maxVal(childState, alpha, beta, depth+1);

            // f.open("/home/ashish/Desktop/COL333/A2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // f<<bombsForChosenSoldier[j].first<<" "<<bombsForChosenSoldier[j].first<<" "<<childStateValue<<endl;
            // f.close();
            beta = min(beta,childStateValue);

            delete childState;
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

            delete childState;
            if (alpha>=beta) 
                return childStateValue;
        }
    }

    if(!hasChildren)
        return evaluateGame(state);

    return beta;
}


float expectedValue(vector<float> v)
{
    float totSum = 0;
    vector<float> weights;
    for(int i=0;i<v.size();i++)
    {
        weights.pb(exp(-v[i]));
        totSum+=weights[i];
    }

    //generates random value between 0 and 1
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1
    float val = dis(gen);

    val*=(totSum-0.00001);

    int ind = -1;

    while(val>0)
    {
        val-=weights[ind+1];
        ind++;
    }


    return v[ind];
}

float expectiVal(Game *state, float alpha, float beta, int depth)
{
    // cout<<depth<<endl;
    //pseudo leaf
    if(depth==maxDepth)
        return evaluateGame(state);

    bool hasChildren = false;

    vector<pii> opponentSoldiers;


    if (color)
        opponentSoldiers = state->getBlackSoldiers();
    else
        opponentSoldiers = state->getWhiteSoldiers();


    vector<float> childStateValues;

    for(int i=0;i<opponentSoldiers.size();i++)
    {
        pii chosenSoldier = opponentSoldiers[i];
        vector<pii> movesForChosenSoldier = state->validMoves(chosenSoldier, (color+1)%2);
        vector<pii> bombsForChosenSoldier = state->validBombs(chosenSoldier, (color+1)%2);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', (color+1)%2);
            float childStateValue = maxVal(childState, alpha, beta, depth+1);

            // f.open("/home/ashish/Desktop/COL333/A2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==2 && chosenSoldier.second ==4)
            //     f<<movesForChosenSoldier[j].first<<" M "<<movesForChosenSoldier[j].second<<" "<<childStateValue<<endl;
            // f.close();

            childStateValues.pb(childStateValue);

            delete childState;
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', (color+1)%2);
            float childStateValue = maxVal(childState, alpha, beta, depth+1);

            // f.open("/home/ashish/Desktop/COL333/A2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==2 && chosenSoldier.second ==4)
            //     f<<bombsForChosenSoldier[j].first<<" B "<<bombsForChosenSoldier[j].second<<" "<<childStateValue<<endl;
            // f.close();

            childStateValues.pb(childStateValue);

            delete childState;
        }
    }

    if(!hasChildren)
        return evaluateGame(state);

    return expectedValue(childStateValues) ;
}

//for minVAL
float maxVal(Game *state, float alpha, float beta, int depth)
{
    // cout<<depth<<endl;
    //pseudo leaf
    if(depth==maxDepth)
        return evaluateGame(state);

    bool hasChildren = false;

    vector<pii> mySoldiers;


    if (!color)
        mySoldiers = state->getBlackSoldiers();
    else
        mySoldiers = state->getWhiteSoldiers();


    for(int i=0;i<mySoldiers.size();i++)
    {
        pii chosenSoldier = mySoldiers[i];
        vector<pii> movesForChosenSoldier = state->validMoves(chosenSoldier,color);
        vector<pii> bombsForChosenSoldier = state->validBombs(chosenSoldier,color);

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', color);
            
            float childStateValue = minVal(childState, alpha, beta, depth+1);

            // f.open("/home/ashish/Desktop/COL333/A2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==2 && chosenSoldier.second ==4)
            //     f<<movesForChosenSoldier[j].first<<" M "<<movesForChosenSoldier[j].second<<" "<<childStateValue<<endl;
            // f.close();

            alpha = max(alpha,childStateValue);

            delete childState;
            if (alpha>=beta) 
                return childStateValue;
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            hasChildren = true;
            Game* childState = new Game(*state);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', color);
            float childStateValue = minVal(childState, alpha, beta, depth+1);

            // f.open("/home/ashish/Desktop/COL333/A2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==2 && chosenSoldier.second ==4)
            //     f<<bombsForChosenSoldier[j].first<<" B "<<bombsForChosenSoldier[j].second<<" "<<childStateValue<<endl;
            // f.close();

            alpha = max(alpha,childStateValue);


            delete childState;
            if (alpha>=beta) 
                return childStateValue;
        }
    }

    if(!hasChildren)
        return evaluateGame(state);

    return alpha;
}


//for expectiVal
// float maxVal(Game *state, float alpha, float beta, int depth)
// {
//     // cout<<depth<<endl;
//     //pseudo leaf
//     if(depth==maxDepth)
//         return evaluateGame(state);

//     bool hasChildren = false;

//     vector<pii> mySoldiers;


//     if (!color)
//         mySoldiers = state->getBlackSoldiers();
//     else
//         mySoldiers = state->getWhiteSoldiers();


//     for(int i=0;i<mySoldiers.size();i++)
//     {
//         pii chosenSoldier = mySoldiers[i];
//         vector<pii> movesForChosenSoldier = state->validMoves(chosenSoldier,color);
//         vector<pii> bombsForChosenSoldier = state->validBombs(chosenSoldier,color);

//         for(int j = 0;j<movesForChosenSoldier.size();j++)
//         {
//             hasChildren = true;
//             Game* childState = new Game(*state);
//             childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', color);
//             float childStateValue = expectiVal(childState, alpha, beta, depth+1);
//             alpha = max(alpha,childStateValue);

//             delete childState;
//             if (alpha>=beta) 
//                 return childStateValue;
//         }

//         for(int j = 0;j<bombsForChosenSoldier.size();j++)
//         {
//             hasChildren = true;
//             Game* childState = new Game(*state);
//             childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', color);
//             float childStateValue = expectiVal(childState, alpha, beta, depth+1);
//             alpha = max(alpha,childStateValue);


//             delete childState;
//             if (alpha>=beta) 
//                 return childStateValue;
//         }
//     }

//     if(!hasChildren)
//         return evaluateGame(state);

//     return alpha;
// }

void chooseAndPlayMove()
{

    // game->printBoard();

    float alpha = -INT32_MAX;
    float beta = INT32_MAX;

    pii soldierPosition, finalPosition;
    char action;


    vector<pii> mySoldiers;
    vector< pair< pii , pii> > bestPlays;
    if (color==0)
        mySoldiers = game->getBlackSoldiers();
    else
        mySoldiers = game->getWhiteSoldiers();

    // f.open("/Users/jvidit/Documents/sem5/col333/assignments/assignment-2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
    // cout<<"\n\n\n\n\n\n\n\n\n";
    // f.close();


    // game->printVector(mySoldiers);
    // game->printBoard();
    for(int i=0;i<mySoldiers.size();i++)
    {
        // cout<<i<<" ";
        pii chosenSoldier = mySoldiers[i];
        vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,color);
        vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,color);

        // f.open("/Users/jvidit/Documents/sem5/col333/assignments/assignment-2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
        // cout<<"\n"<<chosenSoldier.first<<" "<<chosenSoldier.second<<" S "<<endl;
        // f.close();
        

        for(int j = 0;j<movesForChosenSoldier.size();j++)
        {
            // cout<<j<<" ";
            Game* childState = new Game(*game);
            childState->play(chosenSoldier, movesForChosenSoldier[j], 'M', color);

            float childStateValue = minVal(childState, -INT32_MAX, beta, 1);

            // f.open("/Users/jvidit/Documents/sem5/col333/assignments/assignment-2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==7 && chosenSoldier.second ==3)
                
            // cout<<movesForChosenSoldier[j].first<<" "<<movesForChosenSoldier[j].second<<" M "<<childStateValue<<endl;
            // if(chosenSoldier.first==3 && chosenSoldier.second ==3)
            //     f<<movesForChosenSoldier[j].first<<" "<<movesForChosenSoldier[j].second<<" M "<<childStateValue<<endl;
            // f.close();

            // cout<<childStateValue<<endl;
            // childState->printBoard();

            if(childStateValue>alpha)
            {
                alpha = childStateValue;
                bestPlays.clear();
                bestPlays.pb(mp(mp(i, 'M'),movesForChosenSoldier[j]));
                
            }
            else if(childStateValue == alpha)
                bestPlays.pb(mp(mp(i, 'M'),movesForChosenSoldier[j]));
            delete childState;
            
        }

        for(int j = 0;j<bombsForChosenSoldier.size();j++)
        {
            
            Game* childState = new Game(*game);
            childState->play(chosenSoldier, bombsForChosenSoldier[j], 'B', color);


            float childStateValue = minVal(childState, -INT32_MAX, beta, 1);

            
            // f.open("/Users/jvidit/Documents/sem5/col333/assignments/assignment-2/Cannon-player/Cannon-AI-master/test.txt",fstream::app);
            // if(chosenSoldier.first==7 && chosenSoldier.second ==3)
            // cout<<bombsForChosenSoldier[j].first<<" "<<bombsForChosenSoldier[j].second<<" B "<<childStateValue<<endl;
            // if(chosenSoldier.first==3 && chosenSoldier.second ==3)
            //     f<<bombsForChosenSoldier[j].first<<" "<<bombsForChosenSoldier[j].second<<" B "<<childStateValue<<endl;
            // f.close();


            
            // cout<<childStateValue<<endl;
            // childState->printBoard();
            if(childStateValue>alpha)
            {
                alpha = childStateValue;
                bestPlays.clear();
                bestPlays.pb(mp(mp(i, 'B'),bombsForChosenSoldier[j]));
                
            }
            else if(childStateValue == alpha)
                bestPlays.pb(mp(mp(i, 'B'),bombsForChosenSoldier[j]));
            delete childState;            
        }


        // cout<<chosenSoldier.first<<" "<<chosenSoldier.second<<endl;
        // cout<<"MOVES\n";
        // game->printVector(movesForChosenSoldier);
        // cout<<"BOMBS\n";
        // game->printVector(bombsForChosenSoldier);
        // cout<<endl<<endl<<endl;
    }


    int bestPlayValue = -INT32_MAX;
    for(int i=0;i<bestPlays.size();i++)
    {
        Game* childState = new Game(*game);
        pii chosenSoldier = mySoldiers[bestPlays[i].first.first];
        pii playForChosenSoldier = bestPlays[i].second;
        char actionForChosenSoldier = bestPlays[i].first.second;
        childState->play(chosenSoldier, playForChosenSoldier, actionForChosenSoldier, color);

        float childStateValue = evaluateGame(childState);
        if(childStateValue > bestPlayValue)
        {
            bestPlayValue = childStateValue;
            soldierPosition = chosenSoldier;
            finalPosition = playForChosenSoldier;
            action = actionForChosenSoldier;
        }
        delete childState;

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
