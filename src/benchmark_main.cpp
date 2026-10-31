#include<bits/stdc++.h>

#include "rmq/segment_tree.hpp"
#include "rmq/sparse_table.hpp"
#include "rmq/treap.hpp"
#include "rmq/splay_tree.hpp"
#include "rmq/sqrt_tree.hpp"

using namespace std;

struct Test {
	vector<int> a;
	vector<pair<int, int>> queries;

	Test(int n, int q) {
		a.resize(n);
		queries.resize(q);
	}
};

template <typename Algo>
void run(const Test &t) {
	using clock = chrono::steady_clock;
	auto t0 = clock::now();

	Algo algo(t.a);

	long long sum = 0;
	for (auto [l, r] : t.queries) {
		sum += algo.query(l, r);
	}
	auto t1 = clock::now();
	asm volatile("" : : "r,m"(sum) : "memory");

	auto ms = [](clock::duration d) {
		return std::chrono::duration<double, std::milli>(d).count();
	};

	cout << ms(t1 - t0) << endl;
}

int main() {
	int n, q;
	cin >> n >> q;

	Test t(n, q);
	for (int i = 0; i < n; ++i) {
		cin >> t.a[i];
	}
	for (int i = 0; i < q; ++i) {
		cin >> t.queries[i].first >> t.queries[i].second;
	}

	run<SegmentTree<int>>(t);
	run<SparseTable<int>>(t);
	run<Treap<int>>(t);
	run<SplayTree<int>>(t);
	run<SqrtTree<int>>(t);
}
