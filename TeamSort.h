class player {
public:
	string name;
	int mmr;

	player() {
		name = "";
		mmr = 0;
	}

	player(string name0, int mmr0) {
		name = name0;
		mmr = mmr0;
	}
};

class rankTable {
private:

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
	rankTable() {
		entryCt = 0;
	}

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

	void add(entry *e) {
		table.push_back(e);
		entryCt++;
	}

	int getMMR(string s) {
		for (int i = 0; i < entryCt; i++) {
			if (table[i]->rank == s) {
				return table[i]->mmr;
			}
		}
		return -1;
	}

	void print() {
		for (int i = 0; i < entryCt; i++) {
			cout << table[i]->rank << " " << table[i]->mmr << endl;
		}
		cout << endl;
	}
};

class teamTable {
public:
	vector<player*> table;
	int playerCt;
	int totalMMR;

	teamTable() {
		totalMMR = 0;
		playerCt = 0;
	}

	void add(player* p) {
		table.push_back(p);
		totalMMR += p->mmr;
		playerCt++;
	}

	int avgMMR() {
		if (playerCt != 0)
			return totalMMR / playerCt;
		else
			return 0;
	}
};

class playerTable {
private:

	vector<player*> table;

	int find(string s) {
		int index = -1;
		for (int i = 0; i < playerCt; i++) {
			if (table[i]->name == s) {
				index = i;
			}
		}
		return index;
	}

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
		teams.resize(numTeams);
		for (int i = 0; i < numTeams; i++) {
			teams[i] = new teamTable;
		}

		//sort here
		for (int i = 0; i < playerCt; i++) {
			int jMin = 0;
			for (int j = 1; j < numTeams; j++) {
				int x = teams[jMin]->totalMMR;
				int y = teams[j]->totalMMR;
				if (y < x) {
					jMin = j;
				}
			}
			teams[jMin]->add(table[i]);
		}

		return teams;
	}

public:
	int playerCt;

	playerTable() {
		playerCt = 0;
	}

	playerTable(string fname, rankTable ranks) {
		int numPlayers = 0;
		ifstream ifile;
		ifile.open(fname);
		string line;

		while (getline(ifile, line,'\n')) {
			stringstream ss;
			string name;
			string rank;

			ss << line;
			getline(ss, name, ',');
			ss >> rank;
			
			addPlayer(new player(name, ranks.getMMR(rank)));
		}

		ifile.close();
	}

	void addPlayer(player *p) {
		table.push_back(p);
		playerCt++;
	}

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

	void printTableTo(ofstream &outputFile) {
		
		int mmrTotal = 0;
		for (int i = 0; i < playerCt; i++) {
			outputFile << table[i]->name + " ";
			outputFile << table[i]->mmr << endl;
			mmrTotal += table[i]->mmr;
		}
		outputFile << "Team mmr avg: " + mmrTotal / playerCt << endl;
		/*
		for (int i = 0; i < playerCt; i++) {
			cout << table[i]->name << " " << table[i]->mmr << endl;
			mmrTotal += table[i]->mmr;
		}
		cout << "Team mmr avg: " << mmrTotal / playerCt << endl;*/
	}

	void printTable(string fname) {
		
		ofstream tableFile;
		tableFile.open(fname);
		for (int i = 0; i < playerCt; i++) {
			tableFile << table[i]->name + " ";
			tableFile << table[i]->mmr;
			tableFile << '\n';
		}
		tableFile.close();
		/*
		int mmrTotal = 0;
		for(int i = 0; i < playerCt; i++) {
			cout << table[i]->name << " " << table[i]->mmr << endl;
			mmrTotal += table[i]->mmr;
		}
		cout << "Team mmr avg: " << mmrTotal / playerCt << endl;*/
	}
};