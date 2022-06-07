#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>
#include "shuffle.h"
#include "platform.h"
#include <chrono>
#include <thread>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


char letterInput() {
	string c;
	vector<string> options {"r", "c", "q", "n"};
	// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
	do {
		cout << "Press r to reset, c to continue editing file, n to start new challenge, q to quit: "; 
		cin >> c;
		if (find(options.begin(), options.end(), c) !=  options.end()) {
			return c.at(0);
		}
	}
	while(true);
}

string stringInput() {
	string c;
	do {
		cout << "Please enter the challenge string:" << endl;
		cin >> c;
		return c; 
		
	} while (true);
}

string openLink(string url) {
	cout << "Opening link" << endl;	
	if (PLATFORM_NAME == "osx") {
		cout << "OSX" << endl;
		string command ("open " + url);
		system(command.c_str());
	} else if (PLATFORM_NAME == "linux") {
		cout << "LINUS" << endl;
		string command ("xdg-open " + url);
		system(command.c_str());
	} else {
		cout << "Windows" << endl;
		string command ("start " + url);
		system(command.c_str());
	}
	return "cool";
}

float roundTo2Decimal(float var) {
	float val = (int)(var * 100 + 0.5);
	return (float)val / 100;
}

const string WHITESPACE = " \n\r\t\f\v";
string rtrim(string s) {
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

bool equalIgnoringTrailingSpace(string original, string vim) {
	string originalLine;
	string vimLine;
	size_t posOrig = 0, posVim = 0;
	string delimiter = "\n";
	while ((posOrig = original.find(delimiter)) != string::npos
		&& (posVim = vim.find(delimiter)) != string::npos
	) {
		originalLine = rtrim(original.substr(0, posOrig));
		vimLine = rtrim(original.substr(0, posVim));
		if (originalLine.compare(vimLine) != 0) {
			return false;
		}
		original.erase(0, posOrig + delimiter.length());
		vim.erase(0, posVim + delimiter.length());
	}

	if (count(original.begin() , original.end(), '\n') 
		== count(vim.begin(), vim.end(), '\n')) {
		originalLine = rtrim(original);
		vimLine = rtrim(vim);
		return originalLine.compare(vimLine) == 0;
	}
	return false;
}

int main() 
{
	string tmpFileName = "user.py";

	cout << "Welcome to\033[1;31m vim racer!\033[0m" << endl;
	do {
		string randomSource = stringInput();
		// openLink("https://google.com");
		ifstream infile("sample.py");
		string origin_str;
		if (infile)
		{
			ostringstream ss;
			ss << infile.rdbuf();
			origin_str = ss.str();
			infile.close();
		}
		
		// string x = vimShuffle(origin_str, randomSource);
		string x = origin_str;
		ofstream outfile(tmpFileName);
		outfile << x;
		outfile.close();

		float total_time = 0.00;
		// Start timer
		bool reset = false;
		do {
			steady_clock::time_point begin = steady_clock::now();

			string vimCommand = "vim " + tmpFileName + " -c 'set nofixendofline'";
			system(vimCommand.c_str());

			ifstream userSaveFile(tmpFileName);
			string userSaveString;
			ostringstream ss;
			ss << userSaveFile.rdbuf();
			userSaveString = ss.str();
			userSaveFile.close();
			steady_clock::time_point end = steady_clock::now();
			if (!equalIgnoringTrailingSpace(userSaveString, origin_str)
				// userSaveString.compare(origin_str) != 0
				) {
				cout << "Your output\033[1;31m did not\033[0m match the expected file." << endl;
				char c = letterInput();
				if (c == 'r') {
					total_time = 0.00; 
					string x = origin_str;
					// string x = vimShuffle(origin_str, randomSource);
					ofstream outfile(tmpFileName);
					outfile << x;
					outfile.close();
				} else if (c == 'c') {
					total_time += duration_cast<microseconds>(end - begin).count() / (1000000.0000);
				} else if (c == 'q') {
					cout << "Ended challenge" << endl;
					break;
				} else {
					reset = true;
					continue;
				}
			} else {
				total_time += duration_cast<microseconds>(end - begin).count() / (1000000.0000);
				cout << "Congratulations you completed the challenge in : \033[1;31m"<< roundTo2Decimal(total_time)
						<<" seconds \033[0m" << endl;
				break;
			}
		} while(true);
		if (reset) {
			continue;
		} else {
			break;
		}
	} while (true);

	return 0;
}