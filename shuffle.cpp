#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <functional>
#include "hash.h"

using namespace std;

size_t prevHash = 0;
size_t nextRandom() {
	prevHash = hash3(prevHash);
	return prevHash;
}

void duplicateLine (vector<string>& lines) {
	vector<string>::iterator it = lines.begin();
	int start = nextRandom() % lines.size();
	int len = nextRandom() % ((lines.size()) - start);
	vector<string> copy;
	copy.insert(copy.begin(), it+start, it+start+len);

	start = nextRandom() % lines.size();
	lines.insert(lines.begin() + start, copy.begin(), copy.end());
}

int strpos(string haystack, char needle, int nth)
{
	// Will return position of n-th occurence of a char in a string.
	string read;    // A string that will contain the read part of the haystack
	for (int i=1 ; i<nth+1 ; ++i)
	{
			std::size_t found = haystack.find(needle);
			read += haystack.substr(0,found+1); // the read part of the haystack is stocked in the read string
			haystack.erase(0, found+1);     // remove the read part of the haystack up to the i-th needle
			if (i == nth)
			{
					return read.size() - 1;
			}
	}
	return -1;
}

void changeSigns(vector<string>& lines) {
	// Exanded: other signs
	for (int i = 0 ; i < lines.size(); i++) {
		string& line = lines[i];
		int cntPlus = count(line.begin(), line.end(), '+');
		if (cntPlus > 0) {
			if (nextRandom() % 10 < 4) { // 0.4 probability of switching
				int targetIndex = strpos(line, '+', nextRandom() % cntPlus + 1);
				line.replace(targetIndex, 1, "%");
			}
		}
	}
}

void changeBetweenBrackets (vector<string>& lines) {
	// Expansion: more diverse substitutions
	for (int i = 0 ; i < lines.size() ; i++) {
		string line = lines[i];
		int cntLeft = count(line.begin(), line.end(), '(');
		int cntRight = count(line.begin(), line.end(), ')');
		if (cntLeft > 0 && cntRight > 0) {
			int leftIndex = strpos(line, '(', nextRandom() % cntLeft + 1);
			int rightIndex = strpos(line, ')', nextRandom() % cntRight + 1);
			if (nextRandom() % 10 < 3) {
				lines[i] = line.substr(0, leftIndex + 1) + "vim_substitution" + line.substr(rightIndex, line.size() -rightIndex);
			}
		}
	}
}

vector<vector<string> > groupLines (vector<string>& lines) {
	vector<vector<string> > blocks;
	vector<string>block;
	block.push_back(lines[0]);
	for (int i = 1 ; i < lines.size() ; i++) {
		if (nextRandom() % 20 < 2) {
			blocks.push_back(block);
			vector<string> tmp;
			tmp.push_back(lines[i]);
			block = tmp;
		} else {
			block.push_back(lines[i]);
		}
	}
	blocks.push_back(block);	
	return blocks;
}

vector<vector<string> > groupLinesGuaranteed (vector<string>& lines, int k) {
	vector<int> choices;
	while (choices.size() < k) {
		int c = nextRandom() % lines.size();
		if ((c < lines.size() - 1) && find(choices.begin(), choices.end(), c) == choices.end()) {
			choices.push_back(c);
		}
	}
	sort(choices.begin(), choices.end());
	
	int current = 0;
	vector<vector<string> > blocks;
	vector<string> block; 
	for (int i = 0 ; i < lines.size() ; i++) {
		block.push_back(lines[i]);
		if ((current < k) && (i == choices[current])){
			blocks.push_back(block);
			block = vector<string>();
			current ++;
		} 
	}
	blocks.push_back(block);

	int cnt = 0;
	for (auto & block : blocks) {
		cnt += block.size();
	}
	return blocks;
}

vector<string> reduce2dVectorTo1d(vector<vector<string> >& blocks) {
	vector<string> lines = vector<string>();
	for (vector<string> & block: blocks) {
		for (string line : block) {
			lines.push_back(line);
		}
	}
	return lines;
}

void indentBlocksOfCode(vector<string>& lines) {
	vector<vector<string> > blocks = groupLinesGuaranteed(lines, 5);

	int cnt = 0 ;
	for (vector<string>& block : blocks) {
		cnt += block.size();
		bool willIndent = nextRandom() % 5 < 2;
		if (willIndent) {
			for (string& s : block) {
				s.insert(0, "\t\t");
			}
		}
	}
	lines = reduce2dVectorTo1d(blocks);
	// lines = vector<string>();
	// for (vector<string> & block: blocks) {
	// 	for (string line : block) {
	// 		lines.push_back(line);
	// 	}
	// }
}

void shuffleVector(vector<vector<string> > & v) {
	int sizeOfVector = v.size();
	for (int k = 0 ; k < sizeOfVector; k++) {
		int r = k + nextRandom() % (sizeOfVector - k);
		swap(v[k], v[r]);
	}
}

void swappingBlocksOfCode(vector<string>& lines){
	vector<vector<string> > blocks = groupLinesGuaranteed(lines, 5);
	shuffleVector(blocks);
	lines = reduce2dVectorTo1d(blocks);
}

string flatten(vector<string> *lines) {
	vector<string>::iterator it;
	string result = "";
	for (it = lines->begin(); it != lines->end() ; ++it) {
		result.append(*it);
		result.append("\n");
	}
	// remove last \n
	if (result.at(result.length() - 1) == '\n') {
		result = result.substr(0, result.length() - 1);
	}
	return result;
}

string vimShuffle(string original, string randomSource) {
	hash<string> hasher;
	prevHash = hasher(randomSource);
	vector<string> lines;
	size_t pos = 0;
	string delimiter = "\n";
	while ((pos = original.find(delimiter)) != string::npos) {
		lines.push_back(original.substr(0, pos));
		original.erase(0, pos + delimiter.length());
	}
	lines.push_back(original);

	changeBetweenBrackets(lines);
	changeSigns(lines);

	bool willIndent = nextRandom() % 2 < 1;
	if (willIndent){
		indentBlocksOfCode(lines);
	} else {
		duplicateLine(lines);
		swappingBlocksOfCode(lines);
	}

	string result = flatten(&lines);
	return result;
}