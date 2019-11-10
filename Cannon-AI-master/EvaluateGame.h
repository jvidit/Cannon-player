#ifndef EVALUATEGAME_H
#define EVALUATEGAME_H

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


class EvaluateGame
{

    float ws;
    float wth;
    float wnam;
    float wathm;
    float wasm;
    float wnab;
    float wathb;
    float wasb;
    float wpthb;
    float wd;
    float alpha;

    string weightsFilePath;

public:
    
    float countPieces(Game* game, int color, int update = 0, float prediction = 0, float target = 0);
    float countAttacks(Game* game, int color, int update = 0, float prediction = 0, float target = 0);
    float defenseScore(Game* game, int color, int update = 0 , float prediction = 0, float target = 0);
    EvaluateGame();

    void writeWeights();
    void readWeights();
    
};

#endif