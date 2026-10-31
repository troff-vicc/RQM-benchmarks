// Sparse Table for Range Minimum Query.
//
// Build:  O(n log n)
// Query:  O(1)
// Memory: O(n log n)
//
// Query answers min on half-open interval [l, r), 0 <= l < r <= n.

#include<bits/stdc++.h>

using namespace std;

struct SparseTable {
	vector<int> LOG;
	vector<vector<int>> nodes;
	SparseTable(const vector<int> &a) {
		build(a);
	}

	void build(const vector<int> &a) {
		int n = a.size();

		// LOG[i] = floor(log2(i))
		LOG.resize(n + 1, 0);
		for (int i = 2; i <= n; ++i) {
			LOG[i] = LOG[i / 2] + 1;
		}

		int K = LOG[n] + 1;
		nodes.resize(K, vector<int>(n));
		for (int i = 0; i < n; ++i) {
			nodes[0][i] = a[i];
		}

		for (int k = 1; k < K; ++k) {
			int len = (1 << k);

			for (int i = 0; i + len <= n; ++i) {
				nodes[k][i] = min(nodes[k - 1][i],
									nodes[k - 1][i + len / 2]);
			}
		}
	}

	// [l, r)
	int query(int l, int r) {
		int k = LOG[r - l];
		int len = (1 << k);

		// two overlapping blocks of length 2^k cover [l, r)
		return min(nodes[k][l], nodes[k][r - len]);
	}
};

int main() {
	vector<int> a = {1, 2, 3, 4, 5};

	SparseTable st(a);

	cout << st.query(0, 5) << endl; // 1
	cout << st.query(2, 4) << endl; // 3
	cout << st.query(4, 5) << endl; // 5

	return 0;
}
