#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

enum {
	IF=0, FOR=1, WHILE=2
};

class ModuleNode 
{
	public: 
		vector<string> variables;
		vector<ModuleNode *> nodes;
		int moduleType;

		ModuleNode(int k) {
			moduleType = k;
		}


		string toString() {
			string innerBlock = "";
			string result = "";

			innerBlock.append(generateBlockFromVariables());
			for (auto & node : nodes) {
				innerBlock.append(node -> toString());
				innerBlock.append(generateBlockFromVariables());
			}
			int i = rand() % variables.size();
			switch(moduleType) {
				case FOR:
					result.append("for " + variables[i] + " in range("
							+ to_string(rand() % 100) +"):\n");
					result.append(indent(innerBlock));
					break;
				case WHILE:
					result.append("while (True):\n");
					result.append(indent(innerBlock));
					break;
				default:
					result.append("if (!" + variables[0] + ") :\n");
					result.append(indent(innerBlock));
					break;
			}
			
			return result;
		}

		string indent(string block) {
			/*
			Add tabs to every line 
				a\n
				b\n
			becomes 
				\ta\n
				\tb\n
			*/
			string delimiter = "\n";
			string result = "";
			size_t pos = 0;
			while ((pos = block.find("\n")) != string::npos) {
				string line = block.substr(0, pos);
				result.append("\t" + line + "\n");
				block.erase(0, pos + delimiter.length());
			}
			result.append("\t" + block + "\n");
			return result;
		} 

		string generateBlockFromVariables() {
			/*
				Generate block of code
				a \n
				b \n
				c \n
			*/
			int RandIndex = rand() % variables.size();
			string result = "";
			int numberOfTimes = rand() % 4  + 1;
			for (unsigned i = 0 ; i < numberOfTimes; i++) {
				result.append(generateLineFromVarialbles() + "\n");
			}
			
			return result;
		}

		string generateLineFromVarialbles() {
			/*
				Generate single line with \n
				eg. a = (b + c) % 2 
			*/
			int varSize = variables.size();
			int RandIndex = rand() % varSize; 
			string result = "";
			// naive string generation from variables
			result.append(variables[RandIndex]).append(" = ");
			for (unsigned i = 0 ; i < varSize; i++) {
				int len = rand() % 3 + 3;
				for (unsigned i = 0 ; i < len; i++) {
					result.append(variables[rand() % varSize]);
					int sign = rand() % varSize;
					switch (sign % 6) {
						case 0: result.append(" + "); break;
						case 1: result.append(" - "); break;
						case 2: result.append(" / "); break;
						case 3: result.append(" * "); break;
						case 4: result.append(" % "); break;
						
					}
				} 
				result.append(variables[RandIndex] + " + ");
			}
			return result;	
		}

	
};


string generate() {
	srand(time(NULL));
	ModuleNode lnode(0);
	lnode.variables.push_back("a");
	lnode.variables.push_back("b");
	lnode.variables.push_back("c");

	ModuleNode lnode1(1);
	lnode1.variables.push_back("a");
	lnode1.variables.push_back("b");

	ModuleNode lnode2(2);
	lnode2.variables.push_back("a");
	lnode2.variables.push_back("b");

	lnode.nodes.push_back(&lnode1);
	lnode.nodes.push_back(&lnode2);




	return lnode.toString();
}