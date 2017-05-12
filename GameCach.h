#ifndef GAMECACH_H_
#define GAMECACH_H_
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>  

using namespace std;
class GameCach {
protected:
	//移动用户数量
	int MU_Num;
	//小基站数量
	int FBS_Num;
	//文件数量
	int File_Num;
	//基站容量（容纳文件数）
	int CacheSize;
	//基站文件缓存矩阵
	vector<vector<int> > CacheMatrix;
	//基站用户服务矩阵
	vector<vector<int> > ServeMatrix;
	//文件受欢迎度分布
	vector<float>Pf;
	//选择基站用于训练
	vector<int> Kt;
	//所选基站效用函数
	vector<float> utility;
private:
	//初始化
	void initial();
	//计算效用函数
	float Calutility(int id_Kt, vector<vector<int> > mCacheMatrix);
	//选择一个基站集合进行策略更新
	void SelectPlayers(int knum=0);
	//对所选基站进行策略寻找和更新
	void ExploreNewStrategy(int knum = 0,double beta=0);
	static bool compare(int a, int b);
public:
	GameCach(int  mu = 0,int fbs = 0,int file = 0,int cachesize = 0);
	virtual ~GameCach();
	//训练函数
	void train(int nIter);
	void displayCacheMatrix();
};
#endif