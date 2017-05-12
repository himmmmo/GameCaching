#include"GameCach.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>  
using namespace std;

int main() {
	int  mu = 0,  fbs = 0,  file = 0,  cachesize = 0,  nIter=0;
	cout << "输入用户数，基站数，文件数，基站缓存大小，训练次数" << endl;
	cin >> mu >> fbs >> file >> cachesize >> nIter;
	GameCach test1(mu, fbs, file, cachesize);
	test1.train(nIter);
	test1.displayCacheMatrix();
	return 0;	
}