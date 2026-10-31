// Segment Tree for Range Minimum Query.
//
// Build:  O(n)
// Query:  O(log n)
// Memory: O(n)
//
// Query answers min on half-open interval [l, r), 0 <= l < r <= n.

#include<bits/stdc++.h>

using namespace std;

struct SegmentTree {
	vector<int> nodes;
	int n;

	SegmentTree(const vector<int> &a) {
		n = (int)a.size();
		nodes.resize(4 * n);
		build(0, 0, n, a);
	}

	void build(int v, int l, int r, const vector<int> &a) {
		if (r - l == 1) {
			nodes[v] = a[l];
			return;
		}

		int m = (r + l) / 2;

		build(2 * v + 1, l, m, a);
		build(2 * v + 2, m, r, a);

		nodes[v] = min(nodes[2 * v + 1], nodes[2 * v + 2]);
	}

	// [lq, rq)
	int query_impl(int v, int l, int r, int lq, int rq) {
		if (r <= lq || rq <= l) {
			return INT_MAX; // no overlap
		}
		if (lq <= l && r <= rq) {
			return nodes[v]; // full cover
		}

		int m = (r + l) / 2;

		int left = query_impl(2 * v + 1, l, m, lq, rq);
		int right = query_impl(2 * v + 2, m, r, lq, rq);

		return min(left, right);
	}

	// [l, r)
	int query(int l, int r) {
		return query_impl(0, 0, n, l, r);
	}
};

int main() {
	vector<int> a = {1, 2, 3, 4, 5};

	SegmentTree seg(a);

	cout << seg.query(0, 5) << endl; // 1
	cout << seg.query(2, 4) << endl; // 3
	cout << seg.query(4, 5) << endl; // 5

	return 0;
}
