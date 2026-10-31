// Sqrt Tree for Range Minimum Query.
//
// Build:  O(n log log n)
// Query:  O(1)
// Memory: O(n log log n)
//
// Query answers min on half-open interval [l, r), 0 <= l < r <= n.

#include<bits/stdc++.h>

using namespace std;

struct SqrtTree {
	int n;
	int lg;                   // ceil(log2(n))
	vector<int> a;            // the source array

	vector<int> LOG;          // LOG[x] = floor(log2(x)), 0 <= x < 2^lg
	vector<int> layers;       // layers[k] = log2(segment length at layer k)
	vector<int> onLayer;      // onLayer[p] = layer for XOR-exponent p

	vector<vector<int>> pref;
	vector<vector<int>> suf;
	vector<vector<int>> between;

	SqrtTree(const vector<int> &input) {
		n  = (int)input.size();
		a  = input;
		lg = (n <= 1) ? 0 : 32 - __builtin_clz(n - 1);

		LOG.assign(1 << lg, 0);
		for (int i = 1; i < (1 << lg); ++i) {
			LOG[i] = LOG[i >> 1] + 1;
		}

		layers.push_back(lg);
		for (int tlg = lg; tlg > 1; ) {
			tlg = (tlg + 1) / 2;
			layers.push_back(tlg);
		}

		onLayer.assign(lg + 1, 0);
		for (int p = lg; p >= 0; --p) {
			int k = 0;

			while (k + 1 < (int)layers.size() && layers[k + 1] >= p + 1) {
				++k;
			}
			onLayer[p] = k;
		}

		pref.assign(layers.size(), vector<int>(n));
		suf.assign (layers.size(), vector<int>(n));
		between.assign(layers.size(), vector<int>(1 << lg, INT_MAX));

		build(0, 0, n);
	}

	void buildBlock(int layer, int l, int r) {
		pref[layer][l] = a[l];

		for (int i = l + 1; i < r; ++i) {
			pref[layer][i] = min(pref[layer][i - 1], a[i]);
		}


		suf[layer][r - 1] = a[r - 1];

		for (int i = r - 2; i >= l; --i) {
			suf[layer][i] = min(a[i], suf[layer][i + 1]);
		}
	}

	void buildBetween(int layer, int lBound, int rBound) {
		int bSzLog = (layers[layer] + 1) / 2;
		int bCntLog = layers[layer] / 2;
		int bSz = 1 << bSzLog;
		int bCnt = (rBound - lBound + bSz - 1) >> bSzLog;

		for (int i = 0; i < bCnt; ++i) {
			int ans = INT_MAX;
			for (int j = i; j < bCnt; ++j) {
				int add = suf[layer][lBound + (j << bSzLog)];

				if (i == j) {
					ans = add;
				} else {
					ans = min(ans, add);
				}

				between[layer][lBound + (i << bCntLog) + j] = ans;
			}
		}
	}

	void build(int layer, int lBound, int rBound) {
		if (layer >= (int)layers.size()) {
			return;
		}

		int bSz = 1 << ((layers[layer] + 1) / 2);

		for (int l = lBound; l < rBound; l += bSz) {
			int r = min(l + bSz, rBound);
			buildBlock(layer, l, r);
			build(layer + 1, l, r);
		}

		buildBetween(layer, lBound, rBound);
	}

	// Query on closed interval [l, r].
	int query_impl(int l, int r) {
		if (l == r) {
			return a[l];
		}
		if (l + 1 == r) {
			return min(a[l], a[r]);
		}

		int layer = onLayer[LOG[l ^ r]];
		int bSzLog = (layers[layer] + 1) / 2;
		int bCntLog = layers[layer] / 2;

		int lBound = (l >> layers[layer]) << layers[layer];
		int lBlock = ((l - lBound) >> bSzLog) + 1;
		int rBlock = ((r - lBound) >> bSzLog) - 1;

		int ans = suf[layer][l];

		if (lBlock <= rBlock) {
			ans = min(ans, between[layer][lBound + (lBlock << bCntLog) + rBlock]);
		}

		ans = min(ans, pref[layer][r]);
		return ans;
	}

	// [l, r)
	int query(int l, int r) {
		return query_impl(l, r - 1);
	}
};

int main() {
	vector<int> a = {1, 2, 3, 4, 5};

	SqrtTree st(a);

	cout << st.query(0, 5) << endl; // 1
	cout << st.query(2, 4) << endl; // 3
	cout << st.query(4, 5) << endl; // 5

	return 0;
}
