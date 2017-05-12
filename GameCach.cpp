#include"GameCach.h"

using namespace std;

GameCach::GameCach(int  mu,int fbs,int file,int cachesize) {
	MU_Num = mu;
	FBS_Num = fbs;
	File_Num = file;
	CacheSize = cachesize;
	CacheMatrix.resize(FBS_Num);
	ServeMatrix.resize(FBS_Num);
	Pf .resize(File_Num);
}

GameCach::~GameCach(){
}

void GameCach::initial() {
	//初始化文件受欢迎度分布
	float pfsum = 0, gamma = 0.6;
	for (int i = 0; i < File_Num; i++)
		pfsum += 1 / pow(i + 1, gamma);
	for (int i = 0; i < File_Num; i++)
		Pf[i]=1 / pow(i + 1, gamma) / pfsum;
	//初始化缓存矩阵
	for (int i = 0; i < FBS_Num; i++) {
		CacheMatrix[i].resize(File_Num);
		for (int j = 0; j < File_Num; j++) {
			if (j < CacheSize)
				CacheMatrix[i][j] = 1;
			else
				CacheMatrix[i][j] = 0;
		}
	}
	//初始化服务矩阵
	//假定基站与用户在一个400*400的区域，基站服务范围半径40；
	//随机产生基站坐标
	vector<int> fbs_x(FBS_Num);
	vector<int> fbs_y(FBS_Num);
	vector<int> mu_x(MU_Num);
	vector<int> mu_y(MU_Num);
	int max_x = 400; int max_y = 400; int fbs_r = 40;
	srand(time(0));
	for (int i = 0; i < FBS_Num; i++) {
		fbs_x[i] = rand() % max_x;
		fbs_y[i] = rand() % max_y;
	}
	for (int i = 0; i < MU_Num; i++) {
		mu_x[i] = rand() % max_x;
		mu_y[i] = rand() % max_y;
	}
	for (int i = 0; i < FBS_Num; i++) {
		ServeMatrix[i].resize(MU_Num);
		for (int j = 0; j < MU_Num;j++){
			if (sqrt(pow(fbs_x[i] - mu_x[j], 2) + pow(fbs_y[i] - mu_y[j], 2)) <=fbs_r)
				ServeMatrix[i][j] = 1;
			else
				ServeMatrix[i][j] = 0;
		}
	}
	float kt = 0;
	for (int j = 0; j < MU_Num; j++) {
		for (int i = 0; i < File_Num; i++) {
			for (int m = 0; m < FBS_Num; m++) {
				if (CacheMatrix[m][i] * ServeMatrix[m][j]) {
					kt += Pf[i];
					break;
				}
			}
		}
	}
	cout << "initial complete ! 缓存效用函数" << kt << endl;
}
//计算效用函数
float GameCach::Calutility(int id_Kt, vector<vector<int> > mCacheMatrix) {
	float kt = 0;
	for (int j = 0; j < MU_Num; j++) {
		if (!ServeMatrix[id_Kt][j])
			continue;
		for (int i = 0; i < File_Num; i++) {
			for (int m = 0; m < FBS_Num; m++) {
				if (mCacheMatrix[m][i] * ServeMatrix[m][j]) {
					kt += Pf[i];
					break;
				}
			}
		}
	}
	return kt;
}

void GameCach::SelectPlayers(int knum ) {
	Kt.resize(knum);
	utility.resize(knum);
	srand(time(0));
	//随机选择一组基站并计算其效用函数
	for (int i = 0; i < knum; i++) {
		Kt[i] = rand() % FBS_Num;
		utility[i] = Calutility(Kt[i], CacheMatrix);
	}
}

void GameCach::ExploreNewStrategy(int knum ,double beta) {
	vector<vector<int> >new_CacheMatrix = CacheMatrix;
	//对于每个选中的基站，找出效用最大的缓存策略矩阵
	for (int k = 0; k < knum; k++) {
		vector<vector<int> > new_CacheMatrix1=new_CacheMatrix;
		vector<vector<int> > new_CacheMatrix2 = CacheMatrix;
		//初始化选中基站缓存策略
		for (int i = 0; i < File_Num ; i++) {
			if (i < CacheSize)
				new_CacheMatrix2[Kt[k]][i] = 1;
			else
				new_CacheMatrix2[Kt[k]][i] = 0;
		}
		float max_utility = Calutility(Kt[k], new_CacheMatrix2);
		//遍历该基站缓存策略，即依次寻找缓存了CacheSize个文件的策略，其实是一个组合算法C(CacheSize,File_Num)的过程
		for (int t = 0; t < File_Num - 1; t++) {
			if (new_CacheMatrix2[Kt[k]][t] == 1 && new_CacheMatrix2[Kt[k]][t+1] == 0) { 
				swap(new_CacheMatrix2[Kt[k]][t], new_CacheMatrix2[Kt[k]][t + 1]); 
				sort(new_CacheMatrix2[Kt[k]].begin(), new_CacheMatrix2[Kt[k]].begin() + t, compare);
				//新策略已生成，计算效用函数
				float temp_utility = Calutility(Kt[k], new_CacheMatrix2);
				if (temp_utility >= max_utility) {
					for (int i = 0; i < File_Num; i++)
						new_CacheMatrix1[Kt[k]][i] = new_CacheMatrix2[Kt[k]][i];
					max_utility = temp_utility;
				}
				//生成下一个组合即缓存策略  
				t = -1;
			}
		}
		//根据概率进行更新
		double UpdateRule = exp(beta*utility[k])/(exp(beta*utility[k])+ exp(beta*max_utility));
		srand(time(0));
		if (((double)rand() / RAND_MAX)>UpdateRule) {
			new_CacheMatrix = new_CacheMatrix1;
		}
	}
	CacheMatrix = new_CacheMatrix;
}

bool GameCach::compare(int a, int b) {
	if (a > b) {
		return true;
	}
	else {
		return false;
	}
}

void GameCach::train(int nIter) {
	initial();
	int knum = FBS_Num / 5;
	double beta = 0.1;
	for (int n = 0; n < nIter; n++) {
		cout << "training " << n +1<< endl;
		SelectPlayers(knum);
		ExploreNewStrategy(knum, beta*(n+1));
		float kt = 0;
		for (int j = 0; j < MU_Num; j++) {
			for (int i = 0; i < File_Num; i++) {
				for (int m = 0; m < FBS_Num; m++) {
					if (CacheMatrix[m][i] * ServeMatrix[m][j]) {
						kt += Pf[i];
						break;
					}
				}
			}
		}
		cout <<"缓存效用函数"<< kt << endl;
	}
}

void GameCach::displayCacheMatrix() {
	for (int i = 0; i < FBS_Num; i++) {
		for (int j = 0; j < File_Num; j++) {
			cout << CacheMatrix[i][j] << " ";
		}
		cout << endl;
	}
}
