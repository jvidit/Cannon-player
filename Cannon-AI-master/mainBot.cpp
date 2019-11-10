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

float const timeThreshold1 = 20;
float const timeThreshold2 = 10;
float const timeThreshold3 = 5;
float const timeThreshold4 = 2;
int const soldierThreshold1 = 11;
int const soldierThreshold2 = 5;

int const backupDepth = 3;


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
void assignColumn(int m);


fstream f;
float evaluationAtState [10000];
int predictedForState [10000];
int moveNumber = 0;


int bc1;
int bc2;
int bc3;
int wc1;
int wc2;
int wc3;

int main()
{


    for(int i =0;i<10000;i++)
        predictedForState[i] = 0;

    cin>>color;
    color = color-1;
    cin>>m;
    cin>>n;
    cin>>time_left;
    
    assignColumn(n);
    game = new Game(n,m);
    evalGame = new EvaluateGame();
    // game->printBoard();
    string move;
    
    srand(time(0));

    int counter = 0;
    if(m==10)
        counter = 3;
    
    if (color == 0)
    {

        //My Move
        // cout<<"RANDOMPLAYER'S MOVE"<<endl<<endl;

        float tStart = clock();
        if(counter==3)
        {
            cout<<"S "<<bc1<<" 9 M "<<bc1<<" 6"<<endl;
            counter--;
            game->play(mp(bc1,9),mp(bc1,6),'M',color);
        }
        else
            chooseAndPlayMove();
        time_left -= (clock() - tStart)/1000000;

    }
    while(true)
    {   
        evalGame->writeWeights();
        //Opponent's Move
        // possibleOpponentMoves(color);
        // game->printBoard();
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
        // cerr<<time_left<<endl;
        // cerr<<maxDepth<<endl;


        if(counter)
        {
            action = 'M';
            if(color==0)
            {
                if(counter==2)
                {
                    cout<<"S "<<bc2<<" 9 M "<<bc2<<" 6"<<endl;
                    soldierPosition = mp(bc2,9);
                    finalPosition = mp(bc2,6);
                }
                else
                {
                    cout<<"S "<<bc3<<" 9 M "<<bc3<<" 6"<<endl;
                    soldierPosition = mp(bc3,9);
                    finalPosition = mp(bc3,6);
                }
            }
            else
            {
                if(counter==3)
                {
                    cout<<"S "<<wc1<<" 0 M "<<wc1<<" 3"<<endl;
                    soldierPosition = mp(wc1,0);
                    finalPosition = mp(wc1,3);
                }
                else if(counter==2)
                {
                    cout<<"S "<<wc2<<" 0 M "<<wc2<<" 3"<<endl;
                    soldierPosition = mp(wc2,0);
                    finalPosition = mp(wc2,3);
                }
                else
                {
                    cout<<"S "<<wc3<<" 0 M "<<wc3<<" 3"<<endl;
                    soldierPosition = mp(wc3,0);
                    finalPosition = mp(wc3,3);
                }
            }


            game->play(soldierPosition,finalPosition,action,color);


            counter--;
            time_left -= (clock() - tStart)/1000000;
            continue;
        }
        
        vector<pii> ownSoldiers; 
        vector<pii> opponentSoldiers; 

        if(color)
            ownSoldiers = game->getWhiteSoldiers();
        else
            ownSoldiers = game->getBlackSoldiers();

        if (color)
            opponentSoldiers = game->getBlackSoldiers();
        else
            opponentSoldiers = game->getWhiteSoldiers();

        if(time_left>=timeThreshold1)
        {
            if (ownSoldiers.size()<=soldierThreshold1 && opponentSoldiers.size()<=soldierThreshold1)
                maxDepth = 4;
            if (ownSoldiers.size()<=soldierThreshold2 && opponentSoldiers.size()<=soldierThreshold2)
                maxDepth = 6;
        }
        else if (time_left>=timeThreshold2)
            maxDepth = 4;
        else if (time_left>=timeThreshold3)
            maxDepth = 3;
        else if (time_left>=timeThreshold4)
            maxDepth = 2;
        else 
            maxDepth = 1;

        chooseAndPlayMove();

        time_left -= (clock() - tStart)/1000000;

    }
    return 0;
}


void assignColumn(int n)
{
    if(n==8)
    {
        bc1 = 0;
        bc2 = 2;
        bc3 = 4;
        wc1 = 7;
        wc2 = 3;
        wc3 = 5;
    }
    else
    {
        bc1 = 0;
        bc2 = 2;
        bc3 = 4;
        wc1 = 9;
        wc2 = 7;
        wc3 = 5;
    }
}

float evaluateGame (Game* game)
{
    float pieceEval = evalGame->countPieces(game, color);
    float attackEval = evalGame->countAttacks(game, color);
    float defenseEval = evalGame->defenseScore(game, color);
    float totEval = pieceEval + attackEval + defenseEval;
    totEval = (float)((int)totEval/1);

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
    
    
    // evaluationAtState[moveNumber+maxDepth] = bestPlayValue;
    // predictedForState[moveNumber+maxDepth] = 1;

    evaluationAtState[moveNumber+maxDepth] = bestPlayValue;
    predictedForState[moveNumber+maxDepth] = 1;

    if(predictedForState[moveNumber] == 1)
    {
        evalGame->countPieces(game,color,1,evaluationAtState[moveNumber],evaluateGame(game));
        evalGame->countAttacks(game,color,1,evaluationAtState[moveNumber],evaluateGame(game));
        evalGame->defenseScore(game,color,1,evaluationAtState[moveNumber],evaluateGame(game));
    }
    moveNumber+=2;

    cout<< "S " + to_string(soldierPosition.first) + " " + to_string(soldierPosition.second) + " " + string(1,action) + " " + to_string(finalPosition.first) + " " + to_string(finalPosition.second)<<endl;
    
    game->play(soldierPosition, finalPosition, action, color);
    
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
