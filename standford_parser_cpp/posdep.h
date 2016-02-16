#ifndef POSDEP
#define POSDEP

#include<string>
struct typedDependency{
	std::string reln;//�����ϵ����relation
	std::string gov;//���� governor
	int gov_position;//����λ��
	std::string dep;//�����depennor
	int dep_position;//�����λ��

	typedDependency(){}
	typedDependency(std::string s, std::string t, int m, std::string p, int n):reln(s), gov(t), gov_position(m)\
																				,dep(p), dep_position(n){}
};

typedef struct typedDependency TD;

struct posTag{
	int position;//����λ��
	std::string word;//��
	std::string pos;//����
	posTag(int position, std::string word, std::string pos):position(position), word(word), pos(pos){}
};

typedef struct posTag POS;

#endif