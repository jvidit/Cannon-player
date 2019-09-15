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

float EvaluateGame::countPieces(Game* game, int color)
{
    int blackSoldiers = (game->getBlackSoldiers()).size();
    int whiteSoldiers = (game->getWhiteSoldiers()).size();
    int blackTownHalls = (game->getBlackTownHalls()).size();
    int whiteTownHalls = (game->getWhiteTownHalls()).size();



    float evaluationBlack = ws*(blackSoldiers - whiteSoldiers) + wth*(blackTownHalls - whiteTownHalls);

    // if(evaluationBlack>1000000)
    // {
    // 	cout<<-(whiteSoldiers.size())<<" "<<wth*(blackTownHalls.size() - whiteTownHalls.size())<<" "<<evaluationBlack<<endl;
    // }

    if (!color)
        return evaluationBlack;
    else
        return -1*evaluationBlack;
}

float EvaluateGame::countAttacks(Game* game, int color)
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

    for(int i=0;i<blackSoldiers.size();i++)
    {
    	pii chosenSoldier = blackSoldiers[i];
    	vector<pii> movesForChosenSoldier = game->validMoves(chosenSoldier,0);
    	vector<pii> bombsForChosenSoldier = game->validBombs(chosenSoldier,0);

    	for(int j = 0;j<movesForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(movesForChosenSoldier[j]))
    			AttackingTHMovesBlack+=1;
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

    	for(int j = 0;j<movesForChosenSoldier.size();j++)
    	{
    		if (game->hasTownHall(movesForChosenSoldier[j]))
    			AttackingTHMovesWhite+=1;
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

    float countAttacksBlack = wnam*(nonAttackingMovesBlack -nonAttackingMovesWhite);
    countAttacksBlack += wathm*(AttackingTHMovesBlack - AttackingTHMovesWhite);
	countAttacksBlack += wasm*(AttackingSMovesBlack -AttackingSMovesWhite);
	countAttacksBlack +=  wnab*(nonAttackingBombsBlack - nonAttackingBombsWhite);
	countAttacksBlack +=  wathb*(AttackingTHBombsBlack - AttackingTHBombsWhite);
	countAttacksBlack += wasb*(AttackingSBombsBlack - AttackingSBombsWhite);

	if (!color)
        return countAttacksBlack;
    else
        return -1*countAttacksBlack;
    

}


EvaluateGame::EvaluateGame()
{
    wth = 10000;
    ws = 100;
    
    wathb = 80;
    wathm = 50;

    wasb = 20;
    wasm = 10;

    wnab = 5;
    wnam = 1;
    
}
















