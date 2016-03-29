// trim functions
// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}

// class to hold player info
class player {
public:
	string name;
	int mmr;
	player* duo;
	bool inTeam;

	// default constructor
	player() {
		name = "";
		mmr = 0;
		duo = NULL;
	}

	// parameterized constructor, takes name and mmr value
	player(string name0, int mmr0) {
		name = name0;
		mmr = mmr0;
		inTeam = false;
	}

	// function to link two players who will play together
	void addDuo(player* d) {
		duo = d;
		if (d != NULL)
			d->duo = this;
	}
};

// class to hold list of ranks and mmr values
class rankTable {
private:

	//private class to hold rank name and value
	class entry {
	public:
		string rank;
		int mmr;

		entry() {
			rank = "";
			mmr = 0;
		}

		entry(string rank0, int mmr0) {
			rank = rank0;
			mmr = mmr0;
		}
	};

	vector <entry*> table;
	int entryCt;

public:
	// default constructor
	rankTable() {
		entryCt = 0;
	}

	// parameterized constructor, builds list of ranks from text file
	rankTable(string fname) {
		ifstream ifile;
		ifile.open(fname);
		string line;
		int numLines = 0;
		entryCt = 0;

		while (getline(ifile, line, '\n')) {
			stringstream ss;
			string div;
			int mmr;
			ss << line;
			getline(ss, div, ',');
			ss >> mmr;
			add(new entry(div, mmr));
		}

		ifile.close();
	}

	// function to add entry to list of ranks
	void add(entry *e) {
		table.push_back(e);
		entryCt++;
	}

	// function to search table for string and return corresponding MMR value
	int getMMR(string s) {
		for (int i = 0; i < entryCt; i++) {
			if (table[i]->rank == s) {
				return table[i]->mmr;
			}
		}
		return -1;
	}

	string getRank(int mmr) {
		for each (entry *e in table) {
			if (e->mmr == mmr)
				return e->rank;
		}
		return "error";
	}

	// print function to test contents of table
	void print() {
		for (int i = 0; i < entryCt; i++) {
			cout << table[i]->rank << " " << table[i]->mmr << endl;
		}
		cout << endl;
	}
};

// class to hold a team of players
class teamTable {
public:
	vector<player*> table;
	int playerCt;
	int totalMMR;

	// default constructor
	teamTable() {
		totalMMR = 0;
		playerCt = 0;
	}

	// function to add player to table list
	void add(player* p) {
		if (p->inTeam == false) {
			p->inTeam = true;
			table.push_back(p);
			totalMMR += p->mmr;
			playerCt++;
			if (p->duo != NULL) {
				totalMMR += 25;
				add(p->duo);
			}
		}
	}

	void remove(player *p) {
		int spot = -1;
		for (int i = 0; i < playerCt; i++) {
			if (table[i] == p) {
				spot = i;
			}
		}

		if (spot != -1) {
			playerCt--;
			swap(table[spot], table[table.size() - 1]);
			table.pop_back();
		}
	}

	// function to return average MMR of team table
	int avgMMR() {
		if (playerCt != 0) {
			int sum = 0;
			for each (player *p in table)
				sum += p->mmr;
			return sum / playerCt;
		}
		else
			return 0;
	}
};

// class to hold a list of players
class playerTable {
private:

	vector<player*> table;

	// function to find player and return index of player in table
	int find(string s) {
		int index = -1;
		for (int i = 0; i < playerCt; i++) {
			if (table[i]->name == s) {
				index = i;
			}
		}
		return index;
	}

	// function to find player in table and return pointer to player
	player* findPlayer(string s) {
		for (int i = 0; i < playerCt; i++) {
			if (table[i]->name == s) {
				return table[i];
			}
		}
		return NULL;
	}

	// function to sort list of players in descending order
	void sortTable() {
		int i, j;
		int iMax = 0;

		for (j = 0; j < playerCt - 1; j++) {
			iMax = j;
			for (i = j + 1; i < playerCt; i++) {
				if (table[i]->mmr > table[iMax]->mmr) {
					iMax = i;
				}
			}

			if (iMax != j) {
				swap(table[j], table[iMax]);
			}
		}
	}

	// private function to sort players into list of teams and list of extras,
	// based on size of team specified
	vector<teamTable*> makeTeams(vector<player*> &extras, int teamSize) {
		int extraPlayers = playerCt % teamSize;
		int numTeams = playerCt / teamSize;

		for (int i = 0; i < extraPlayers; i++) {
			srand(time(NULL));
			int rNum = rand() % playerCt;
			player* p = removePlayer(table[rNum]->name);
			extras.push_back(p);
		}

		vector<teamTable*> teams;
		vector<teamTable*> fullTeams;
		teams.resize(numTeams);
		for (unsigned i = 0; i < teams.size(); i++) {
			teams[i] = new teamTable;
		}

		// sort here, **greedy algorithm implementation**

		// sort players
		for (int i = 0; i < playerCt; i++) {
			if (teams.size() > 0 && !table[i]->inTeam) {
				//move full teams to full teams list
				for (unsigned g = 0; g < teams.size(); g++) {
					if (teams[g]->playerCt >= teamSize) {
						fullTeams.push_back(teams[g]);
						swap(teams[g], teams[teams.size() - 1]);
						teams.pop_back();
					}
				}

				bool spaceFound = false;

				for (unsigned g = 0; g < teams.size(); g++) {
					if (teamSize - teams[g]->playerCt >= 2) {
						spaceFound = true;
					}
				}

				if (spaceFound || !table[i]->duo) {
					int jMin = 0;
					int jOldMin = 0;

					for (unsigned j = 1; j <= teams.size() - 1; j++) {
						int x = teams[jMin]->totalMMR;
						int y = teams[j]->totalMMR;
						if (y < x) {
							jOldMin = jMin;
							jMin = j;
						}
					}
					teams[jMin]->add(table[i]);
				}
				else {
					vector<teamTable*> temp;
					for each (teamTable* tt in teams) {
						temp.push_back(tt);
					}
					/*for each (teamTable* tt in fullTeams) {
						temp.push_back(tt);
					}*/

					//find lowest and second lowest with at least one non duo player
					int teamMin = 0;
					int teamMin2 = 0;
					for (unsigned k = 1; k < temp.size(); k++) {
						bool hasNonDuo = false;
						for each (player *p in temp[k]->table) {
							if (!p->duo) {
								hasNonDuo = true;
							}
						}
						if (hasNonDuo) {
							int x = temp[teamMin]->totalMMR;
							int y = temp[k]->totalMMR;
							int z = temp[teamMin2]->totalMMR;
							if (y < x) {
								teamMin2 = teamMin;
								teamMin = k;
							}
						}
					}

					teamTable *lowestTeam = temp[teamMin];
					teamTable *secondLowest = temp[teamMin2];

					//find closest mmr player not in duo in lowest team
					int target = table[i]->mmr;
					int closest = lowestTeam->playerCt - 1;
					for (int p = 0; p < lowestTeam->playerCt - 1; p++) {
						if (!lowestTeam->table[p]->duo) {
							int diff = abs(lowestTeam->table[p]->mmr - target);
							int closeDiff = abs(lowestTeam->table[closest]->mmr - target);
							if (diff < closeDiff) {
								closest = p;
							}
						}
					}
					player *toMove = lowestTeam->table[closest];

					//put that player in second lowest team
					lowestTeam->remove(toMove);
					toMove->inTeam = false;
					secondLowest->add(toMove);

					//add duo to lowest team
					lowestTeam->add(table[i]);
				}
			}
		}

		//if error happened, still show all teams
		if (teams.size() > 0) {
			for each (teamTable *t in teams) {
				fullTeams.push_back(t);
			}
		}

		//find highest team


		//find lowest team

		return fullTeams;
	}

public:
	int playerCt;

	// default constructor
	playerTable() {
		playerCt = 0;
	}

	// parameterized constructor, builds table from file and list of ranks
	playerTable(string fname, rankTable ranks) {
		playerCt = 0;
		ifstream ifile;
		ifile.open(fname);
		string line;

		while (getline(ifile, line,'\n')) {
			stringstream ss;
			string name;
			string rank;
			string duo;

			ss << line;
			getline(ss, name, ',');
			getline(ss, rank, ',');
			getline(ss, duo, '\n');

			trim(name);
			trim(rank);
			trim(duo);

			line = "";

			player* p = new player(name, ranks.getMMR(rank));
			p->addDuo(findPlayer(duo));

			addPlayer(p);
		}

		ifile.close();
	}

	// function to add player to table
	void addPlayer(player *p) {
		table.push_back(p);
		playerCt++;
	}

	// function to remove player from table and return pointer to player
	player *removePlayer(string s) {
		int index = find(s);
		if (index == -1) {
			return NULL;
		}
		else {
			player *r = table[index];
			swap(table[index], table[playerCt - 1]);
			table.pop_back();
			playerCt--;
			return r;
		}
	}

	// public function to create teams and auto-generate list of "extra" players
	vector<teamTable*> createTeams(int teamSize) {
		sortTable();
		vector<player*> extras;
		vector<teamTable*> teams = makeTeams(extras, teamSize);
		if (extras.size() != 0) {
			ofstream out;
			out.open("extras.txt");
			for each(player* p in extras) {
				out << p->name << endl;
			}
			out.close();
		}
		return teams;
	}

	// function to test contents by printing to file
	void printTable(string fname) {
		
		ofstream tableFile;
		tableFile.open(fname);
		for (int i = 0; i < playerCt; i++) {
			tableFile << table[i]->name + " ";
			tableFile << table[i]->mmr;
			if (table[i]->duo != NULL)
				tableFile << " " + table[i]->duo->name;
			else
				tableFile << " s";
			tableFile << '\n';
		}
		tableFile.close();
	}

	void testTeams(vector<teamTable*> teams) {
		playerTable error;

		for each (player *p in table) {
			bool found = false;

			for each (teamTable *t in teams) {
				for each (player *tp in t->table) {
					if (p == tp) {
						found = true;
					}
				}
			}
			
			if (!found) {
				error.addPlayer(p);
			}
		}

		error.printTable("./test/debug2.txt");
	}
};