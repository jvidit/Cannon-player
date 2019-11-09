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


float EvaluateGame::countPieces(Game* game, int color, int update, float prediction, float target)
{
    int blackSoldiers = (game->getBlackSoldiers()).size();
    int whiteSoldiers = (game->getWhiteSoldiers()).size();
    int blackTownHalls = (game->getBlackTownHalls()).size();
    int whiteTownHalls = (game->getWhiteTownHalls()).size();



    float evaluationBlack = ws*(blackSoldiers - whiteSoldiers) + wth*(blackTownHalls - whiteTownHalls);
    if (!color)
    {   
        ws += alpha*(target-prediction)*(blackSoldiers - whiteSoldiers);
        wth += alpha*(target-prediction)*(blackTownHalls - whiteTownHalls);
    }
    else
    {
        ws -= alpha*(target-prediction)*(blackSoldiers - whiteSoldiers);
        wth -= alpha*(target-prediction)*(blackTownHalls - whiteTownHalls);
    }
    // if(evaluationBlack>1000000)
    // {
    // 	cout<<-(whiteSoldiers.size())<<" "<<wth*(blackTownHalls.size() - whiteTownHalls.size())<<" "<<evaluationBlack<<endl;
    // }

    if(update)
        writeWeights();
    
    wth = max(0.0f,wth);
    ws = max(0.0f,ws);

    if (!color)
        return evaluationBlack;
    else
        return -1*evaluationBlack;

}

float EvaluateGame::countAttacks(Game* game, int color, int update, float prediction, float target)
{
	vector<pii> blackSoldiers = game->getBlackSoldiers();
    vector<pii>whiteSoldiers = game->getWhiteSoldiers();
    vector<pii> blackTownHalls = game->getBlackTownHalls();
    vector<pii> whiteTownHalls = game->getWhiteTownHalls();

    int nonAttackingMovesBlack = 0;
    int nonAttackingMovesWhite = 0;
    int AttackingTHMovesBlack = 0;
    int AttackingTHMovesWhite = 0;
	int AttackingSMovesBlack = 0;
    int AttackingSMovesWhite = 0;

    int nonAttackingBombsBlack = 0;
    int nonAttackingBombsWhite = 0;
    int AttackingTHBombsBlack = 0;
    int AttackingTHBombsWhite = 0;
	int AttackingSBombsBlack = 0;
    int AttackingSBombsWhite = 0;

    int possibleTHBombBlack = 0;
    int possibleTHBombWhite = 0;

    for(int i=0;i<blackSoldiers.size();i++)
    {
    	pii chosenSoldier = blackSoldiers[i];
    	vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,0);
    	vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,0);

        possibleTHBombBlack+=game->possibleBombs(chosenSoldier, 0);

    	for(int j = 0;j<movesForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(movesForChosenSoldier[j]))
            {
    			AttackingTHMovesBlack+=1;
                break;
            }
    		else
    		{
    			if (game->hasSoldier(movesForChosenSoldier[j]))
    				AttackingSMovesBlack+=1;
    			else
    				nonAttackingMovesBlack+=1;
    		}

    	}
    	for(int j = 0;j<bombsForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(bombsForChosenSoldier[j]))
    			AttackingTHBombsBlack+=1;
    		else
    		{
    			if (game->hasSoldier(bombsForChosenSoldier[j]))
    				AttackingSBombsBlack+=1;
    			else
    				nonAttackingBombsBlack+=1;
    		}

    	}
    }

    for(int i=0;i<whiteSoldiers.size();i++)
    {
    	pii chosenSoldier = whiteSoldiers[i];
    	vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,1);
    	vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,1);

        possibleTHBombWhite+=game->possibleBombs(chosenSoldier, 1);


    	for(int j = 0;j<movesForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(movesForChosenSoldier[j]))
            {
    			AttackingTHMovesWhite+=1;
                break;
            }
    		else
    		{
    			if (game->hasSoldier(movesForChosenSoldier[j]))
    				AttackingSMovesWhite+=1;
    			else
    				nonAttackingMovesWhite+=1;
    		}

    	}
    	for(int j = 0;j<bombsForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(bombsForChosenSoldier[j]))
    			AttackingTHBombsWhite+=1;
    		else
    		{
    			if (game->hasSoldier(bombsForChosenSoldier[j]))
    				AttackingSBombsWhite+=1;
    			else
    				nonAttackingBombsWhite+=1;
    		}
    	}
    }


    float countAttacksBlack = wnam*nonAttackingMovesBlack
                        +     wathm*AttackingTHMovesBlack
                        +     wasm*AttackingSMovesBlack
                        +     wnab*nonAttackingBombsBlack
                        +     wathb*AttackingTHBombsBlack
                        +     wasb*AttackingSBombsBlack
                        +     wpthb*possibleTHBombBlack;

    float countAttacksWhite = wnam*nonAttackingMovesWhite
                        +     wathm*AttackingTHMovesWhite
                        +     wasm*AttackingSMovesWhite
                        +     wnab*nonAttackingBombsWhite
                        +     wathb*AttackingTHBombsWhite
                        +     wasb*AttackingSBombsWhite
                        +     wpthb*possibleTHBombWhite;


    float wOffense = ((float) game->getBlackSoldiers().size())/10;
    float wDefense = ((float) game->getWhiteSoldiers().size())/10;

    float blackEval = wOffense * countAttacksBlack - wDefense * countAttacksWhite;
    // float blackEval = countAttacksBlack - countAttacksWhite;

    if (!color)
    {
        wnam += alpha*(target-prediction)*(wOffense*nonAttackingMovesBlack - wDefense*nonAttackingMovesWhite);
        wathm += alpha*(target-prediction)*(wOffense*AttackingTHMovesBlack - wDefense*AttackingTHMovesWhite);
        wasm += alpha*(target-prediction)*(wOffense*AttackingSMovesBlack - wDefense*AttackingSMovesWhite);
        wnab += alpha*(target-prediction)*(wOffense*nonAttackingBombsBlack - wDefense*nonAttackingBombsWhite);
        wathb += alpha*(target-prediction)*(wOffense*AttackingTHBombsBlack - wDefense*AttackingTHBombsWhite);
        wasb += alpha*(target-prediction)*(wOffense*AttackingSBombsBlack - wDefense*AttackingSBombsWhite);
        wpthb += alpha*(target-prediction)*(wOffense*possibleTHBombBlack - wDefense*possibleTHBombWhite);
    }
    else
    {
        wnam -= alpha*(target-prediction)*(wOffense*nonAttackingMovesBlack - wDefense*nonAttackingMovesWhite);
        wathm -= alpha*(target-prediction)*(wOffense*AttackingTHMovesBlack - wDefense*AttackingTHMovesWhite);
        wasm -= alpha*(target-prediction)*(wOffense*AttackingSMovesBlack - wDefense*AttackingSMovesWhite);
        wnab -= alpha*(target-prediction)*(wOffense*nonAttackingBombsBlack - wDefense*nonAttackingBombsWhite);
        wathb -= alpha*(target-prediction)*(wOffense*AttackingTHBombsBlack - wDefense*AttackingTHBombsWhite);
        wasb -= alpha*(target-prediction)*(wOffense*AttackingSBombsBlack - wDefense*AttackingSBombsWhite);
        wpthb -= alpha*(target-prediction)*(wOffense*possibleTHBombBlack - wDefense*possibleTHBombWhite);
    }

    wathb = max(0.0f,wathb);
    wathm = max(0.0f,wathm);

    wasb = max(0.0f,wasb);
    wasm = max(0.0f,wasm);

    wnab = max(0.0f,wnab);
    wnam = max(0.0f,wnam);

    wpthb = max(0.0f,wpthb);

    if(update)
        writeWeights();

	if (!color)
        return blackEval;
    else
        return -1*blackEval;
}





EvaluateGame::EvaluateGame()
{
    // wth = 10000;
    // ws = 100;
    
    // wathb = 80;
    // wathm = 50;

    // wasb = 20;
    // wasm = 10;

    // wnab = 5;
    // wnam = 0;

    // wpthb =  20;  //possible town hall bomb  
    alpha = 0.01;
    weightsFilePath="./weights";

    readWeights();

}


void EvaluateGame::writeWeights()
{
    ofstream file;
    file.open(weightsFilePath);

    file<<wth<<endl;
    file<<ws<<endl;

    file<<wathb<<endl;
    file<<wathm<<endl;

    file<<wasb<<endl;
    file<<wasm<<endl;

    file<<wnab<<endl;
    // file<<wnam<<endl;
    file<<0<<endl;
    file<<wpthb<<endl;

    file.close();
}

void EvaluateGame::readWeights()
{
    ifstream file;
    file.open(weightsFilePath);

    file>>wth;
    file>>ws;

    file>>wathb;
    file>>wathm;

    file>>wasb;
    file>>wasm;

    file>>wnab;
    file>>wnam;

    file>>wpthb;


    wth = max(0.0f,wth);
    ws = max(0.0f,ws);

    wathb = max(0.0f,wathb);
    wathm = max(0.0f,wathm);

    wasb = max(0.0f,wasb);
    wasm = max(0.0f,wasm);

    wnab = max(0.0f,wnab);
    wnam = max(0.0f,wnam);

    wpthb = max(0.0f,wpthb);


    file.close();
}








// 10000
// 100 
// 80
// 50
// 20
// 10
// 5
// 0
// 20




