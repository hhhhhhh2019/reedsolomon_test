#include <gpoly.h>
#include <gnum.h>
#include <stdio.h>
#include <stdarg.h>


GPoly::GPoly() {}

GPoly::GPoly(int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 1; i <= count; i++) {
		data.push_back(va_arg(args, GNum));
	}

	va_end(args);
};


void GPoly::fit(int size) {
	if ((int)data.size() >= size)
		return;

	int l = size - data.size();

	for (int i = 0; i <= l; i++)
		data.push_back(0);
}

void GPoly::normalize() {
	while (data.size() > 0 && data.back().value == 0) {
		data.pop_back();
	}
}

void GPoly::print(const char* name, bool new_line) {
	printf(name);
	putc('{', stdout);
	if (data.size() > 0) {
		for (int i = 0; i < data.size() - 1; i++)
			printf("%d ", data[i].value);
		printf("%d", data[data.size()-1].value);
	}
	putc('}', stdout);
	if (new_line)
		putc('\n', stdout);
}


GPoly GPoly::operator+(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	GPoly res;

	for (int i = 0; i < size; i++)
		res.data.push_back(data[i] + other.data[i]);

	return res;
}

void GPoly::operator+=(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	for (int i = 0; i < size; i++)
		data[i] += other.data[i];
}

GPoly GPoly::operator-(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	GPoly res;

	for (int i = 0; i < size; i++)
		res.data.push_back(data[i] - other.data[i]);

	return res;
}

void GPoly::operator-=(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	for (int i = 0; i < size; i++)
		data[i] -= other.data[i];
}

GPoly GPoly::operator*(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	GPoly res;

	for (int i = 0; i < size; i++) {
		GPoly v;

		for (int j = 0; j < size; j++)
			v.data.push_back(other.data[j] * data[i]);

		for (int j = 0; j < i; j++)
			v.data.insert(v.data.begin(), GNum(0));

		res += v;
	}

	res.normalize();

	return res;
}

void GPoly::operator*=(GPoly other) {
	int size = std::max(data.size(), other.data.size());

	fit(size);
	other.fit(size);

	GPoly res;

	for (int i = 0; i < size; i++) {
		GPoly v;

		for (int j = 0; j < size; j++)
			v.data.push_back(other.data[j] * data[i]);

		for (int j = 0; j < i; j++)
			v.data.insert(v.data.begin(), GNum(0));

		res += v;
	}

	res.normalize();

	data = res.data;
}

GPoly GPoly::operator/(GPoly other) {
	normalize();
	other.normalize();

	int size = std::max(data.size(), other.data.size());

	GPoly res;
	GPoly a;
	a.data = data;

	for (int i = data.size() - 1; i >= (int)other.data.size() - 1; i--) {
		GNum m = a.data[i] / other.data.back();

		res.data.insert(res.data.begin(), m);

		GPoly s = other.scale(m);
		s = s.mulXPow(a.data.size()-s.data.size() - (a.data.size()-i) + 1);

		a -= s;
	}

	res.normalize();

	return res;
}

void GPoly::operator/=(GPoly other) {
	normalize();
	other.normalize();

	int size = std::max(data.size(), other.data.size());

	GPoly res;
	GPoly a;
	a.data = data;

	for (int i = data.size() - 1; i >= (int)other.data.size() - 1; i--) {
		GNum m = a.data[i] / other.data.back();

		res.data.insert(res.data.begin(), m);

		GPoly s = other.scale(m);
		s = s.mulXPow(a.data.size()-s.data.size() - (a.data.size()-i) + 1);

		a -= s;
	}

	res.normalize();

	data = res.data;
}

GPoly GPoly::operator%(GPoly other) {
	normalize();
	other.normalize();

	int size = std::max(data.size(), other.data.size());

	GPoly res;
	GPoly a;
	a.data = data;

	for (int i = data.size() - 1; i >= (int)other.data.size() - 1; i--) {
		GNum m = a.data[i] / other.data.back();

		res.data.insert(res.data.begin(), m);

		GPoly s = other.scale(m);
		s = s.mulXPow(a.data.size()-s.data.size() - (a.data.size()-i) + 1);

		a -= s;
	}

	a.normalize();

	return a;
}

void GPoly::operator%=(GPoly other) {
	normalize();
	other.normalize();

	int size = std::max(data.size(), other.data.size());

	GPoly res;
	GPoly a;
	a.data = data;

	for (int i = data.size() - 1; i >= (int)other.data.size() - 1; i--) {
		GNum m = a.data[i] / other.data.back();

		res.data.insert(res.data.begin(), m);

		GPoly s = other.scale(m);
		s = s.mulXPow(a.data.size()-s.data.size() - (a.data.size()-i) + 1);

		a -= s;
	}

	a.normalize();

	data = a.data;
}


bool GPoly::operator==(GPoly other) {
	normalize();
	other.normalize();

	if (data.size() != other.data.size())
		return false;

	for (int i = 0; i < data.size(); i++) {
		if (data[i].value != other.data[i].value)
			return false;
	}

	return true;
}


GPoly GPoly::mulXPow(int p) {
	GPoly r(0);

	for (int i = 0; i < p; i++)
		r.data.push_back(GNum(0));

	for (int i = 0; i < data.size(); i++)
		r.data.push_back(data[i]);

	return r;
}

GPoly GPoly::scale(GNum x) {
	GPoly r(0);

	for (int i = 0; i < data.size(); i++) {
		r.data.push_back(data[i] * x);
	}

	return r;
}

GNum GPoly::solve(GNum x) {
	GNum result(0);

	for (int i = 0; i < data.size(); i++) {
		result += x.npow(i) * data[i];
	}

	return result;
}

GPoly GPoly::copy() {
	GPoly result;

	for (int i = 0; i < data.size(); i++)
		result.data.push_back(data[i]);

	return result;
}

GPoly GPoly::reverse() {
	GPoly result;

	for (int i = data.size() - 1; i >= 0; i--) {
		result.data.push_back(data[i]);
	}

	return result;
}
