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
	cout << "�����û�������վ�����ļ�������վ�����С��ѵ������" << endl;
	cin >> mu >> fbs >> file >> cachesize >> nIter;
	GameCach test1(mu, fbs, file, cachesize);
	test1.train(nIter);
	test1.displayCacheMatrix();
	return 0;	
}