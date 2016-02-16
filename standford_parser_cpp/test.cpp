#include "tag.h"
#include <fstream>
#include <algorithm>

void printdep(TD& a) {
	std::cout<<a.reln<<" ";
	std::cout<<a.gov<<" ";
	std::cout<<a.dep<<std::endl;
}

int main(){
	tag *t = new tag();
	std::ifstream fin("C:\\dym\\wiki.txt");
	std::ofstream fout("C:\\dym\\wiki.lemma.txt");
	int counter = 0;
	std::string line;
	while(getline(fin, line)){
		counter++;
		if(counter % 10000 == 0) std::cout << counter << std::endl;
		int word_start = 0;
		int word_end = 0;
		//while(word_end < line.size()){
		//	while(word_end < line.size() && line[word_end] != ' ') word_end++;
			//t->lemma(line.substr(word_start, word_end - word_start), "VB");
			//std::cout << t->lemma(line.substr(word_start, word_end - word_start), "VB") << std::endl;
		//	word_end++;
		//	word_start = word_end;
		//}
		std::vector<TD> temp = t->depTag(line) ;
		std::for_each(temp.begin(), temp.end(), printdep);
	}
	delete t;
	system("pause");
}