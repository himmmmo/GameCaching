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
	//�ƶ��û�����
	int MU_Num;
	//С��վ����
	int FBS_Num;
	//�ļ�����
	int File_Num;
	//��վ�����������ļ�����
	int CacheSize;
	//��վ�ļ��������
	vector<vector<int> > CacheMatrix;
	//��վ�û��������
	vector<vector<int> > ServeMatrix;
	//�ļ��ܻ�ӭ�ȷֲ�
	vector<float>Pf;
	//ѡ���վ����ѵ��
	vector<int> Kt;
	//��ѡ��վЧ�ú���
	vector<float> utility;
private:
	//��ʼ��
	void initial();
	//����Ч�ú���
	float Calutility(int id_Kt, vector<vector<int> > mCacheMatrix);
	//ѡ��һ����վ���Ͻ��в��Ը���
	void SelectPlayers(int knum=0);
	//����ѡ��վ���в���Ѱ�Һ͸���
	void ExploreNewStrategy(int knum = 0,double beta=0);
	static bool compare(int a, int b);
public:
	GameCach(int  mu = 0,int fbs = 0,int file = 0,int cachesize = 0);
	virtual ~GameCach();
	//ѵ������
	void train(int nIter);
	void displayCacheMatrix();
};
#endif