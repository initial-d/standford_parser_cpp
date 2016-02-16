#ifndef POSDEP
#define POSDEP

#include<string>
struct typedDependency{
	std::string reln;//依存关系名，relation
	std::string gov;//主词 governor
	int gov_position;//主词位置
	std::string dep;//依存词depennor
	int dep_position;//依存词位置

	typedDependency(){}
	typedDependency(std::string s, std::string t, int m, std::string p, int n):reln(s), gov(t), gov_position(m)\
																				,dep(p), dep_position(n){}
};

typedef struct typedDependency TD;

struct posTag{
	int position;//句中位置
	std::string word;//词
	std::string pos;//词性
	posTag(int position, std::string word, std::string pos):position(position), word(word), pos(pos){}
};

typedef struct posTag POS;

#endif