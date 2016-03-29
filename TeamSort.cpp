/*
	Team Sorter for League Tournaments
	Created by:		Antonio Ayala
	Last revised:	2/26/2016
*/

#include <Windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

#include "TeamSort.h"

void printTeams(vector<teamTable*>);
void printTeamsInfo(vector<teamTable*>, rankTable);

int main() {
	wstring s(L".\\test");
	CreateDirectory(s.c_str(), NULL);

	//load up tables from ranks and players files
	rankTable ranks("ranks.txt");
	playerTable players("players.txt", ranks);

	//create teams
	vector<teamTable*> teams = players.createTeams(5);

	players.testTeams(teams);
	players.printTable("./test/debug3.txt");

	//print teams to file
	printTeams(teams);
	printTeamsInfo(teams, ranks);

	system("pause");
	return 0;
}

// function to print teams to text file
void printTeams(vector<teamTable*> teams) {
	int teamNum = 1;
	ofstream out;
	out.open("teams.txt");

	for each(teamTable *t in teams) {
		out << "Team " << teamNum << endl;
		for each(player* p in t->table) {
			out << p->name << endl;
		}
		out << "Avg: " << t->avgMMR() << endl;
		out << endl;
		teamNum++;
	}

	out.close();
}

void printTeamsInfo(vector<teamTable*> teams, rankTable ranks) {
	int teamNum = 1;
	ofstream out;
	out.open("./test/debug1.txt");

	for each (teamTable *t in teams) {
		out << "Team " << teamNum << endl;
		for each (player *p in t->table) {
			string s = "s";
			if (p->duo) {
				s = p->duo->name;
			}
			out << p->name << " " << ranks.getRank(p->mmr) << " " << p->mmr << " " << s << endl;
		}
		out << "Avg: " << t->avgMMR() << endl;
		out << endl;
		teamNum++;
	}

	out.close();
}