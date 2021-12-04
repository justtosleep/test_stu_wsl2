#include <iostream>
#include <cmath>
#include<vector>
using namespace std;

class group;
class individual;

class individual {
	friend group;
private:
	double fx;
	double fitness;
	double x;
public:
	int chromosome[16];
	individual();
	void recal();
	void operator=(const individual& ind) {
		for (int i = 0; i < 16; i++) {
			this->chromosome[i] = ind.chromosome[i];
		}
		this->x = ind.x;
		this->fx = ind.fx;
		this->fitness = ind.fitness;
	}    //重载取代操作
};		//个体类

individual::individual() {
	for (int i = 0; i < 16; i++) {
		chromosome[i] = (int)rand() % 2;
	}
	double tmp = 0;
	for (int i = 15; i >= 0; i--) {
		tmp = chromosome[i] + tmp / 2;
	}
	tmp *= 8;
	x = tmp - 1;
	fx = x * sin(x);
	fitness = exp(fx);
}

void individual::recal() {
	double tmp = 0;
	for (int i = 15; i >= 0; i--) {
		tmp = chromosome[i] + tmp / 2;
	}
	tmp *= 8;
	x = tmp - 1;
	fx = x * sin(x);
	fitness = exp(fx);
}

class group {
private:
	individual individuals[10];
	double reproductionrate = 0.8, mutationrate = 0.2;
public:
	group() {};
	void reproduction();
	void mutation();
	void selection(individual* children, int childrennum);
	void crossover(individual* children, int fathernum, int mothernum, int i);
	void coutvalue();
};  //群体类


void group::reproduction() {
	cout << "is  wkj";
	vector<int> reproductionnum;
	for (int i = 0; i < 10; i++) {
		if (rand() / double(RAND_MAX) <= reproductionrate) {
			reproductionnum.push_back(i);
		}
	}  //生殖个体选择
	individual children[10];
	for (int i = 0; i < reproductionnum.size() / 2; i++) {
		int fathernum = reproductionnum[2 * i];
		int mothernum = reproductionnum[2 * i + 1];
		crossover(children, fathernum, mothernum, i);
	}
	selection(children, reproductionnum.size() / 2);
}

void exchange(individual father, individual mother, int lcross, int rcross) {
	if (lcross == rcross) return;
	int tmp[16];
	for (int i = lcross; i <= rcross; i++) {
		tmp[i] = father.chromosome[i];
		father.chromosome[i] = mother.chromosome[i];
		mother.chromosome[i] = tmp[i];
	}
	father.recal();
	mother.recal();
}

void group::crossover(individual* children, int fathernum, int mothernum, int i) {
	int lcross, rcross;
	int tmp1 = int(rand()) % 16;
	int tmp2 = int(rand()) % 16;
	lcross = min(tmp1, tmp2);
	rcross = max(tmp1, tmp2);
	children[2 * i] = this->individuals[fathernum];
	children[2 * i + 1] = this->individuals[mothernum];
	exchange(children[2 * i], children[2 * i + 1], lcross, rcross);

	//除第一位都可变（减少遗传导致的数值差异）
}

void group::selection(individual* children, int childrennum) {
	double sumfit = 0;
	int groupnum = 10 + childrennum;
	for (int i = 0; i < 10; i++) {
		sumfit += this->individuals[i].fitness;
	}
	for (int i = 0; i < childrennum; i++) {
		sumfit += children[i].fitness;
	}
	double pinterval[20];;
	double tmp = 0;
	bool selectflag[20];
	for (int i = 0; i < 10; i++) {
		if (i == 0) {
			pinterval[i] = individuals[i].fitness / sumfit;
			tmp = pinterval[i];
			selectflag[i] = false;
			continue;
		}
		pinterval[i] = individuals[i].fitness / sumfit + tmp;
		tmp = pinterval[i];
		selectflag[i] = false;
	}
	for (int i = 0; i < childrennum; i++) {
		if (i == 0) {
			pinterval[10 + i] = children[i].fitness / sumfit;
			tmp = pinterval[10 + i];
			selectflag[10 + i] = false;
			continue;
		}
		pinterval[10 + i] = children[i].fitness / sumfit + tmp;
		tmp = pinterval[10 + i];
		selectflag[10 + i] = false;
	}
	while (groupnum > 10) {
		double selectrate = rand() / double(RAND_MAX);
		for (int i = 0; i < groupnum; i++) {
			if (i == 0 && pinterval[i] > selectrate && selectflag[i] == false) {
				selectflag[i] = true;
				groupnum--;
				break;
			}
			if (pinterval[i] > selectrate && pinterval[i - 1] <= selectrate) {
				selectflag[i] = true;
				groupnum--;
				break;
			}
		}
	}
	int winnum = 10;
	for (int i = 0; i < 10; i++) {
		if (selectflag[i]) {
			while (selectflag[winnum] || winnum <= 10 + childrennum) {
				winnum++;
			}
			individuals[i] = children[winnum - 10];
			winnum++;
		}
	}

}



void group::mutation() {
	cout << "my name";
	vector<int> mutationnum;
	for (int i = 0; i < 10; i++) {
		if (rand() / double(RAND_MAX) <= mutationrate) {
			mutationnum.push_back(i);
		}
	}  //变异个体选择
	for (int i = 0; i < mutationnum.size(); i++) {
		int num = mutationnum[i];
		for (int j = 0; j < 16; i++) {
			if (rand() / double(RAND_MAX) <= mutationrate) {
				this->individuals[num].chromosome[j] = (this->individuals[num].chromosome[j] == 0) ? 1 : 0;
			}
		}
		this->individuals[num].recal();
	}
}

void group::coutvalue() {
	for (int i = 0; i < 10; i++) {
		cout << "后代" << i << "号的X值为" << this->individuals[i].x << '\n';
		cout << "对应函数值为" << this->individuals[i].fx << endl;
	}
}

int main() {
	group example;
	cout << "hello";
	for (int i = 0; i < 1; i++) {
		example.mutation();
		example.reproduction();
	}
	example.coutvalue();
	return 0;
}