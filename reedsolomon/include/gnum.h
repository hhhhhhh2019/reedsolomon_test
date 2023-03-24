#ifndef GNUM_H
#define GNUM_H

extern unsigned int filed_size;
extern unsigned int* log;
extern unsigned int* pow;


struct GNum {
	unsigned char value;

	GNum();
	GNum(unsigned char);
	GNum(const GNum&);

	GNum operator+(GNum);
	void operator+=(GNum);
	GNum operator-(GNum);
	void operator-=(GNum);
	GNum operator*(GNum);
	void operator*=(GNum);
	GNum operator/(GNum);
	void operator/=(GNum);
	bool operator==(GNum);
	bool operator!=(GNum);

	GNum npow(int);
};


#endif // GNUM_H
