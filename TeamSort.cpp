/*
	Team Sorter for STS Tournaments
	Created by:		Antonio Ayala
	Last revised:	11/2/2015
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

#include "TeamSort.h"

void printTeams(vector<teamTable*> teams);

int main() {

	rankTable ranks("ranks.txt");

	playerTable players("players.txt", ranks);

	vector<teamTable*> teams = players.createTeams(5);

	printTeams(teams);

	system("pause");
	return 0;
}

void printTeams(vector<teamTable*> teams) {
	int teamNum = 1;
	ofstream out;
	out.open("teams.txt");

	for each(teamTable *t in teams) {
		out << "Team " << teamNum << endl;
		for each(player* p in t->table) {
			out << p->name << endl;
		}
		out << "Avg mmr: " << t->avgMMR() << endl << endl;
	}

	out.close();
}