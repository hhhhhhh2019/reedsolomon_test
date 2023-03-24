#ifndef GPOLY_H
#define GPOLY_H


#include <gnum.h>
#include <vector>


struct GPoly {
	std::vector<GNum> data;

	GPoly();
	GPoly(int ...);

	void fit(int);
	void normalize();
	void print(const char*, bool=1);

	GPoly operator+(GPoly);
	void operator+=(GPoly);
	GPoly operator-(GPoly);
	void operator-=(GPoly);
	GPoly operator*(GPoly);
	void operator*=(GPoly);
	GPoly operator/(GPoly);
	void operator/=(GPoly);
	GPoly operator%(GPoly);
	void operator%=(GPoly);

	bool operator==(GPoly);

	GPoly mulXPow(int);
	GPoly scale(GNum);
	GNum solve(GNum);

	GPoly copy();
	GPoly reverse();
};


#endif // GPOLY_H
