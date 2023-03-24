#include <reedsolomon.h>
#include <gnum.h>
#include <gpoly.h>
#include <stdio.h>


int fpow(int n, int p) {
	int r = 1;

	for (int i = 0; i < p; i++)
		r *= n;

	return r;
}


GPoly gen_g(int D) {
	GPoly g(2, GNum(2), GNum(1));

	for (int i = 2; i < D; i++) {
		g *= GPoly(2, GNum(2).npow(i), GNum(1));
	}

	return g;
}


GPoly encode(GPoly p, GPoly g, int N, int K) {
	GPoly p1 = p.mulXPow(N-K);
	return p1 + (p1 % g);
}

GPoly get_locators(GPoly S, int t) {
	GPoly L(1, GNum(1));
	GPoly L_old(1, GNum(1));

	for (int i = 0; i < t; i++) {
		GNum delta = S.data[i];

		for (int j = 1; j < L.data.size(); j++) {
			delta += L.data[j] * S.data[i - j];
		}

		L_old = L_old.mulXPow(1);

		if (delta.value != 0) {
			if (L_old.data.size() > L.data.size()) {
				GPoly L_new = L_old.scale(delta);
				L_old = L.scale(GNum(1)/delta);
				L = L_new.copy();
			}

			L += L_old.scale(delta);
		}
	}

	return L;
}

GPoly decode(GPoly c, GPoly g, int N, int K) {
	GPoly e = c % g;

	if (e.data.size() == 0) {
		GPoly result;

		for (int i = N - K; i < N; i++)
			result.data.push_back(c.data[i]);

		return result;
	}

	GPoly S;

	for (int i = 0; i < N-K; i++)
		S.data.push_back(e.solve(GNum(2).npow(i+1)));

	GPoly L = get_locators(S, (N-K));

	GPoly L1;

	for (int i = L.data.size() - 1; i >= 1; i--) {
		if ((i & 1) == 1) {
			L1.data.insert(L1.data.begin(), L.data[i]);
		} else {
			L1.data.insert(L1.data.begin(), GNum(0));
		}
	}

	GPoly X;

	for (int i = 1; i < filed_size; i++) {
		if (L.solve(GNum(i)).value == 0)
			X.data.push_back(i);
	}

	GPoly W = S * L;

	for (int i = N-K; i < N; i++)
		W.data[i] = GNum(0);

	GPoly Y;

	for (int i = 0; i < X.data.size(); i++) {
		GNum v = X.data[i];
		Y.data.push_back(W.solve(v) / L1.solve(v));
	}

	GPoly E;

	for (int i = 0; i < N; i++)
		E.data.push_back(GNum(0));

	for (int i = 0; i < Y.data.size(); i++) {
		int p = log[(GNum(1)/X.data[i]).value];
		E.data[p] = Y.data[i];
	}

	GPoly r = c + E;


	/*c.print("c = ");
	e.print("e = ");
	S.print("S = ");
	L.print("L = ");
	W.print("W = ");
	L1.print("L'= ");
	X.print("X = ");
	Y.print("Y = ");
	E.print("E = ");*/


	GPoly result;

	for (int i = N-K; i < N; i++)
		result.data.push_back(r.data[i]);

	return result;
}
