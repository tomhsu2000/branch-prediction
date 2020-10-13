#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include <string>
using namespace std;

// all instructions
const char insts[6][6]{ "beq","bne","blt","bge","addi","li" };
// states
const char states[4][3]{ "SN","WN","WT","ST" };
int reg[32]{};
int bht_num;
int** bht;
map<string, int> lable_map;
vector<string> inst_vec;

bool lable(char inst[], int number);

int operation(int index);

int distin(const char inst[], int& rg1, int& rg2, int& imm);

bool prediction(int index, bool happen);

int main()
{
	char inst[50];
	int inst_num = 0;
	while (1) {
		cin.getline(inst, 50);
		if (strlen(inst) == 0)
			break;
		if (!lable(inst, inst_vec.size()))
			inst_vec.push_back(inst);
	}
	cin >> bht_num;
	bht = new int* [bht_num] {};
	for (int i = 0; i < bht_num; i++)
		bht[i] = new int[7]{};
	while (inst_num != inst_vec.size()) {
		inst_num = operation(inst_num);
	}
}

//store where lable appear
bool lable(char inst[], int number)
{
	char lable[20];
	for (int i = 0; inst[i] != '\0'; i++) {
		if (inst[i] == ':') {
			lable[i] = '\0';
			lable_map[lable] = number;
			return true;
		}
		// remove comment
		if (inst[i] == '/') {
			inst[--i] = '\0';
			break;
		}
		lable[i] = inst[i];
	}
	return false;
}

// execute instruction
int operation(int index)
{
	int rg1, rg2, imm;
	switch (distin(inst_vec[index].c_str(), rg1, rg2, imm))
	{
	case 0:
		if (prediction(index, reg[rg1] == reg[rg2]))
			return imm;
		break;
	case 1:
		if (prediction(index, reg[rg1] != reg[rg2]))
			return imm;
		break;
	case 2:
		if (prediction(index, reg[rg1] < reg[rg2]))
			return imm;
		break;
	case 3:
		if (prediction(index, reg[rg1] >= reg[rg2]))
			return imm;
		break;
	case 4:
		reg[rg1] = reg[rg2] + imm;
		break;
	case 5:
		reg[rg1] = imm;
		break;
	default:
		break;
	}
	return index + 1;
}

// distin instruction's index in array
int distin(const char inst[], int& rg1, int& rg2, int& imm)
{
	char inst_code[6], lable[20];
	int i, j, k;
	for (i = 0, j = 0; inst[i] != ' '; i++, j++)
		inst_code[j] = inst[i];
	inst_code[j] = '\0';
	for (j = 0; j < 6; j++)
		if (strcmp(insts[j], inst_code) == 0)
			break;
	for (i++; inst[i] != 'R'; i++);
	rg1 = (int)atoi(inst + i + 1);
	if (j != 5) {
		for (i++; inst[i] != 'R'; i++);
		rg2 = (int)atoi(inst + i + 1);
	}
	else
		rg2 = 0;
	for (i++; inst[i] != ','; i++);
	if (j == 4 || j == 5)
		imm = (int)atoi(inst + i + 1);
	else {
		for (i++, k = 0; inst[i] != '\0'; i++, k++)
			lable[k] = inst[i];
		lable[k] = '\0';
		imm = lable_map[lable];
	}
	return j;
}

// predict branch
bool prediction(int index, bool happen)
{
	int ind = index % bht_num, stat = bht[ind][0] * 2 + bht[ind][1] + 2;
	bool pred = (bht[ind][stat] >= 2);
	cout << "entry: " << ind << "   " << inst_vec[index] << endl;
	cout << "(" << bht[ind][0] << bht[ind][1] << ", " << states[bht[ind][2]] << ", " << states[bht[ind][3]]
		<< ", " << states[bht[ind][4]] << ", " << states[bht[ind][5]] << ") ";
	if (pred)
		cout << "T ";
	else
		cout << "N ";
	if (happen)
		cout << "T    misprediction: ";
	else
		cout << "N    misprediction: ";
	if (pred ^ happen) {
		bht[ind][6]++;
		if (happen)
			bht[index][stat]++;
		else
			bht[index][stat]--;
	}
	cout << bht[ind][6] << endl << endl;
	bht[index][0] = bht[index][1];
	bht[ind][1] = happen;
	return happen;
}
