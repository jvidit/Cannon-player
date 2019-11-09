#ifndef GAME_H
#define GAME_H

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
    Game(Game &game);
    
    void play(pii soldierPosition, pii finalPosition, char action, int color);          //0 for black, 1 for white
    vector<pii> validMoves(pii soldierPosition, int color);
    vector<pii> validBombs(pii soldierPosition, int color);
    
    vector<pii> getBlackSoldiers();
    vector<pii> getBlackTownHalls();
    vector<pii> getWhiteSoldiers();
    vector<pii> getWhiteTownHalls();
    vector<vector<int> > getBoard();
    int getN();
    int getM();

    void printBoard();
    void printVector(vector<pii> v);

    bool hasSoldier(pii position);
    bool hasTownHall(pii position);
    int possibleBombs(pii soldierPosition, int color);
    int defenseScore(int color);
    
};

#endif