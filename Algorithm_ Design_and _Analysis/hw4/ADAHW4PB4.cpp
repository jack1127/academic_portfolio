#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>
#include "hex.h"

using namespace std;
typedef pair<int, int> pii;

const int maxn = 11;

bool board[maxn][maxn];
bool link3[3];
bool link4[4];
bool link5[5];
bool needrandom;
pii last,last2;

int n;

void init(int _n) {
    n = _n;
    memset(board, 0, sizeof board);
    memset(link3, 0, sizeof link3);
    memset(link4, 0, sizeof link4);
    memset(link5, 0, sizeof link5);
    needrandom = false;
    srand(12345);
}
pii decide(pii p) {
    pii res;
    if (p != make_pair(-1, -1))
        board[p.first][p.second] = true;
    /*vector<pii> unused;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (!board[i][j])
                unused.push_back({i, j});
    pii res = unused[rand() % unused.size()];
    board[res.first][res.second] = true;*/
    if(n==3){
        if (p == make_pair(-1, -1)){
            res = {n/2, n/2};
            board[res.first][res.second] = true;
            return res;
        }
        if(link3[0]==false && link3[2]==false){
            if (p.first==0){
                if(board[0][1] == false){
                    res = {0, 1};
                }
                else if(board[0][2] == false){
                    res = {0, 2};
                }
                link3[0] = true;
            }
            else if(p.first==2){
                if(board[2][1] == false){
                    res = {2, 1};
                }
                else if(board[2][0] == false){
                    res = {2, 0};
                }
                link3[2] = true;
            }
            else{
                res = {0, 1};
                link3[0] == true;
            }
        }
        else{
            if(link3[0] == false){
                if(board[0][1] == false){
                    res = {0, 1};
                }
                else if(board[0][2] == false){
                    res = {0, 2};
                }
            }
            else if(link3[2] == false){
                if(board[2][1] == false){
                    res = {2, 1};
                }
                else if(board[2][0] == false){
                    res = {2, 0};
                }
            }
        }
    }
    else if(n==4){
        if (p == make_pair(-1, -1)){
            res = {1, 2};
            last = {1, 2};
            board[res.first][res.second] = true;
            return res;
        }
        
        if(link4[0]==false && p.first==0 && p.second==2){
            res = {0, 3};
            link4[0]=true;
        }
        else if(link4[0]==false && p.first==0 && p.second==3){
            res = {0, 2};
            link4[0]=true;
        }
        else if(link4[2]==false && p.first==2 && p.second==1){
            res = {2, 2};
            last = {2, 2};
            link4[2]=true;
        }
        else if(link4[2]==false && p.first==2 && p.second==2){
            res = {2, 1};
            last = {2, 1};
            link4[2]=true;
        }
        else if(board[3][1] == false){
            res = {3, 1};
            link4[3]=true;
        }
        else if(link4[2]==false){
            if(board[2][1] == false){
                res = {2, 1};
                last = {2, 1};
                link4[2]=true;
            }
            else if(board[2][2] == false){
                res = {2, 2};
                last = {2, 2};
                link4[2]=true;
            }
            else{
                needrandom = true;
            }
        }    
        else if(link4[3]==false){
            if(board[last.first+1][last.second] == false){
                res = {last.first+1, last.second};
                last = {last.first+1, last.second};
                link4[last.first+1]=true;
            }
            else if(board[last.first+1][last.second-1] == false){
                res = {last.first+1, last.second-1};
                last = {last.first+1, last.second-1};
                link4[last.first+1]=true;
            }
            else if(board[last.first][last.second-1] == false){
                res = {last.first, last.second-1};
                last = {last.first, last.second-1};
            }
            else if(board[last.first][last.second+1] == false){
                res = {last.first, last.second+1};
                last = {last.first, last.second+1};
            }
            else{
                needrandom=true;
            }
        }
        else if(link4[0]==false){
            if(board[0][2] == false){
                res = {0, 2};
                link4[0]=true;
            }
            else if(board[0][3] == false){
                res = {0, 3};
                link4[0]=true;
            }
            else{
                needrandom=true;
            }
        }
        else{
            needrandom=true;
        }
        if(res.first > n-1 || res.first <= -1 || res.second > n-1 || res.second < 0 || needrandom == true){
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (!board[i][j])
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
        }
    }
   
    else if(n==5){
        if (p == make_pair(-1, -1)){
            res = {n/2, n/2};
            last = {n/2, n/2};
            last2 = {n/2, n/2};
            board[res.first][res.second] = true;
            return res;
        }
        if(p.first < n/2){
            if(board[last.first-1][last.second] == false){
                res = {last.first-1, last.second};
                last = {last.first-1, last.second};
            }
            else if(board[last.first-1][last.second+1] == false){
                res = {last.first-1, last.second+1};
                last = {last.first-1, last.second+1};
            }
            else if(board[last.first][last.second+1] == false){
                res = {last.first, last.second+1};
                last = {last.first, last.second+1};
            }
            else if(board[last.first][last.second-1] == false){
                res = {last.first, last.second-1};
                last = {last.first, last.second-1};
            }
            else{
                needrandom = true;
            }
        }
        else {
            if(board[last2.first+1][last2.second] == false){
                res = {last2.first+1, last2.second};
                last2 = {last2.first+1, last2.second};
            }
            else if(board[last2.first+1][last2.second-1] == false){
                res = {last2.first+1, last2.second-1};
                last2 = {last2.first+1, last2.second-1};
            }
            else if(board[last2.first][last2.second+1] == false){
                res = {last2.first, last2.second+1};
                last2 = {last2.first, last2.second+1};
            }
            else if(board[last2.first][last2.second-1] == false){
                res = {last2.first, last2.second-1};
                last2 = {last2.first, last2.second-1};
            }
            else{
                vector<pii> unused;
                for (int i = n-1; i >= 0; i--)
                    for (int j = n-1; j >= 0; j--)
                        if (!board[i][j])
                            unused.push_back({i, j});
                res = unused[rand() % unused.size()];
            }
        }
        if(last.first < 0 || last.second > n-1 || last.second < 0){
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (!board[i][j])
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
        }
        else if(last2.first > n-1 || last2.second > n-1 || last2.second < 0){
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (!board[i][j])
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
        }
    }    
    else if(n==8){
        if (p == make_pair(-1, -1)){
            res = {0, n/2};
            last = {0, n/2};
            board[res.first][res.second] = true;
            return res;
        }
        if(board[last.first+1][last.second] == false){
            res = {last.first+1, last.second};
            last = {last.first+1, last.second};
        }
        else if(board[last.first+1][last.second-1] == false){
            res = {last.first+1, last.second-1};
            last = {last.first+1, last.second-1};
        }
        else if(board[last.first][last.second+1] == false){
            res = {last.first, last.second+1};
            last = {last.first, last.second+1};
        }
        else if(board[last.first][last.second-1] == false){
            res = {last.first, last.second-1};
            last = {last.first, last.second-1};
        }
        else if(board[last.first-1][last.second+1] == false){
            res = {last.first-1, last.second+1};
            last = {last.first-1, last.second+1};
        }
        else{
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (!board[i][j])
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
        }
        if(last.first > n-1 || last.second > n-1 || last.second < 0){
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (!board[i][j])
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
        }
    }

    board[res.first][res.second] = true;
    return res;
}

/*if(p.first < n/2){
            if(link5[1] == false){
                if(board[1][2] == false && board[1][3] == false){
                    if(p.second < n/2){
                        res = {1, 3};
                        link5[1] = true;
                    }
                    else{
                        res = {1, 2};
                        link5[1] = true;
                    }
                }
                else if(board[1][2] == false){
                    res = {1, 2};
                    link5[1] = true;
                }
                else if(board[1][3] == false){
                    res = {1, 3};
                    link5[1] = true;
                }
                else{
                    needrandom = true;
                }
            }
            else if(link5[0] == false){
                if(board[1][2] == true){
                    if(board[0][3] == false){
                        res = {0, 3};
                        link5[0] = true;
                    }
                    else if(board[0][2] == false){
                        res = {0, 2};
                        link5[0] = true;
                    }
                    else if(board[0][4] == false){
                        res = {0, 4};
                        link5[0] = true;
                    }
                    else if(board[0][1] == false){
                        res = {0, 1};
                        link5[0] = true;
                    }
                    else if(board[0][0] == false){
                        res = {0, 0};
                        link5[0] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else if(board[1][3] == true){
                    if(board[0][3] == false){
                        res = {0, 3};
                        link5[0] = true;
                    }
                    else if(board[0][4] == false){
                        res = {0, 4};
                        link5[0] = true;
                    }
                    else if(board[0][2] == false){
                        res = {0, 2};
                        link5[0] = true;
                    }
                     else if(board[0][1] == false){
                        res = {0, 1};
                        link5[0] = true;
                    }
                    else if(board[0][0] == false){
                        res = {0, 0};
                        link5[0] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else{
                    needrandom = true;
                }
            }
            else{
                needrandom = true;
            }
        }
        else if(p.first > n/2){
            if(link5[3] == false){
                if(board[3][2] == false && board[3][1] == false){
                    if(p.second < n/2){
                        res = {3, 2};
                        link5[3] = true;
                    }
                    else{
                        res = {3, 1};
                        link5[3] = true;
                    }
                }
                else if(board[3][2] == false){
                    res = {3, 2};
                    link5[3] = true;
                }
                else if(board[3][1] == false){
                    res = {3, 1};
                    link5[3] = true;
                }
                else{
                    needrandom = true;
                }
            }
            else if(link5[4] == false){
                if(board[3][2] == true){
                    if(board[4][1] == false){
                        res = {4, 1};
                        link5[4] = true;
                    }
                    else if(board[4][2] == false){
                        res = {4, 2};
                        link5[4] = true;
                    }
                    else if(board[4][0] == false){
                        res = {4, 0};
                        link5[4] = true;
                    }
                    else if(board[4][3] == false){
                        res = {4, 3};
                        link5[4] = true;
                    }
                    else if(board[4][4] == false){
                        res = {4, 4};
                        link5[4] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else if(board[3][1] == true){
                    if(board[4][1] == false){
                        res = {4, 1};
                        link5[4] = true;
                    }
                    else if(board[4][0] == false){
                        res = {4, 0};
                        link5[4] = true;
                    }
                    else if(board[4][2] == false){
                        res = {4, 2};
                        link5[4] = true;
                    }
                     else if(board[4][3] == false){
                        res = {4, 3};
                        link5[4] = true;
                    }
                    else if(board[4][4] == false){
                        res = {4, 4};
                        link5[4] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else{
                    needrandom = true;
                }
            }
            else{
                needrandom = true;
            }
        }
        else{
            if(link5[1] == false){
                if(board[1][2] == false && board[1][3] == false){
                    if(p.second < n/2){
                        res = {1, 3};
                        link5[1] = true;
                    }
                    else{
                        res = {1, 2};
                        link5[1] = true;
                    }
                }
                else if(board[1][2] == false){
                    res = {1, 2};
                    link5[1] = true;
                }
                else if(board[1][3] == false){
                    res = {1, 3};
                    link5[1] = true;
                }
                else{
                    needrandom = true;
                }
            }
            else if(link5[3] == false){
                if(board[3][2] == false && board[3][1] == false){
                    if(p.second < n/2){
                        res = {3, 2};
                        link5[3] = true;
                    }
                    else{
                        res = {3, 1};
                        link5[3] = true;
                    }
                }
                else if(board[3][2] == false){
                    res = {3, 2};
                    link5[3] = true;
                }
                else if(board[3][1] == false){
                    res = {3, 1};
                    link5[3] = true;
                }
                else{
                    needrandom = true;
                }
            }
            else if(link5[0] == false){
                if(board[1][2] == true){
                    if(board[0][3] == false){
                        res = {0, 3};
                        link5[0] = true;
                    }
                    else if(board[0][2] == false){
                        res = {0, 2};
                        link5[0] = true;
                    }
                    else if(board[0][4] == false){
                        res = {0, 4};
                        link5[0] = true;
                    }
                    else if(board[0][1] == false){
                        res = {0, 1};
                        link5[0] = true;
                    }
                    else if(board[0][0] == false){
                        res = {0, 0};
                        link5[0] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else if(board[1][3] == true){
                    if(board[0][3] == false){
                        res = {0, 3};
                        link5[0] = true;
                    }
                    else if(board[0][4] == false){
                        res = {0, 4};
                        link5[0] = true;
                    }
                    else if(board[0][2] == false){
                        res = {0, 2};
                        link5[0] = true;
                    }
                     else if(board[0][1] == false){
                        res = {0, 1};
                        link5[0] = true;
                    }
                    else if(board[0][0] == false){
                        res = {0, 0};
                        link5[0] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else{
                    needrandom = true;
                }
            }
            else if(link5[4] == false){
                if(board[3][2] == true){
                    if(board[4][1] == false){
                        res = {4, 1};
                        link5[4] = true;
                    }
                    else if(board[4][2] == false){
                        res = {4, 2};
                        link5[4] = true;
                    }
                    else if(board[4][0] == false){
                        res = {4, 0};
                        link5[4] = true;
                    }
                    else if(board[4][3] == false){
                        res = {4, 3};
                        link5[4] = true;
                    }
                    else if(board[4][4] == false){
                        res = {4, 4};
                        link5[4] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else if(board[3][1] == true){
                    if(board[4][1] == false){
                        res = {4, 1};
                        link5[4] = true;
                    }
                    else if(board[4][0] == false){
                        res = {4, 0};
                        link5[4] = true;
                    }
                    else if(board[4][2] == false){
                        res = {4, 2};
                        link5[4] = true;
                    }
                     else if(board[4][3] == false){
                        res = {4, 3};
                        link5[4] = true;
                    }
                    else if(board[4][4] == false){
                        res = {4, 4};
                        link5[4] = true;
                    }
                    else{
                        needrandom = true;
                    }
                }
                else{
                    needrandom = true;
                }
            }
            else{
                needrandom = true;
            }
        }
        if(needrandom == true){
            vector<pii> unused;
            for (int i = n-1; i >= 0; i--)
                for (int j = n-1; j >= 0; j--)
                    if (board[i][j] == false)
                        unused.push_back({i, j});
            res = unused[rand() % unused.size()];
            needrandom = false;
        }
    }*/