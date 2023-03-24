#include <gnum.h>
#include <stdio.h>


GNum::GNum() {}

GNum::GNum(unsigned char v) {
	value = v;
}

GNum::GNum(const GNum& v) {
	value = v.value;
}

GNum GNum::operator+(GNum other) {
	return GNum(value ^ other.value);
}

void GNum::operator+=(GNum other) {
	value ^= other.value;
}

GNum GNum::operator-(GNum other) {
	return GNum(value ^ other.value);
}

void GNum::operator-=(GNum other) {
	value ^= other.value;
}

GNum GNum::operator*(GNum other) {
	if (value == 0 || other.value == 0)
		return GNum(0);

	return GNum(pow[(log[value] + log[other.value]) % (filed_size - 1)]);
};

void GNum::operator*=(GNum other) {
	if (value == 0 || other.value == 0)
		value = 0;
	else
		value = pow[(log[value] + log[other.value]) % (filed_size - 1)];
};

GNum GNum::operator/(GNum other) {
	if (value == 0 || other.value == 0)
		return GNum(0);

	return GNum(pow[(log[value] + (filed_size - 1) - log[other.value]) % (filed_size - 1)]);
};

void GNum::operator/=(GNum other) {
	if (value == 0 || other.value == 0)
		value = 0;
	else
		value = pow[(log[value] + (filed_size - 1) - log[other.value]) % (filed_size - 1)];
};

bool GNum::operator==(GNum other) {
	return value == other.value;
}

bool GNum::operator!=(GNum other) {
	return value != other.value;
}

GNum GNum::npow(int p) {
	return GNum(pow[(log[value] * p) % (filed_size - 1)]);
}
