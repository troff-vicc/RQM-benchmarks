// Treap (implicit key) with Range Minimum Query.
//
// Build:  O(n)
// Query:  O(log n)
// Memory: O(n)
//
// Query answers min on half-open interval [l, r), 0 <= l < r <= n.

#include<bits/stdc++.h>

using namespace std;

mt19937 rnd(time(nullptr));

struct Treap {
	struct Node {
		int val;
		int priority;

		int mn;        // min in subtree
		int sz;        // subtree size

		Node* l = nullptr;
		Node* r = nullptr;

		Node(int x) {
			val = x;
			priority = (int)rnd();
			sz = 1;
			mn = x;
		}
	};

	Node* root = nullptr;

	Treap(const vector<int>& a) {
		root = build(a);
	}

	// O(n): popped nodes become cur's left subtree, cur becomes new top's right child.
	Node* build(const vector<int>& a) {
		vector<Node*> st;

		for (int x : a) {
			Node* cur = new Node(x);
			Node* last = nullptr;

			while (!st.empty() && st.back()->priority < cur->priority) {
				last = st.back();
				upd(last);
				st.pop_back();
			}

			cur->l = last;
			if (!st.empty()) {
				st.back()->r = cur;
			}
			st.push_back(cur);
		}

		if (st.empty()) {
			return nullptr;
		}

		while (st.size() > 1) {
			Node* top = st.back();
			st.pop_back();
			upd(top);
		}
		upd(st[0]);

		return st[0];
	}

	int sz(Node* t) {
		if (!t) {
			return 0;
		}
		return t->sz;
	}
	int mn(Node* t) {
		if (!t) {
			return INT_MAX;
		}
		return t->mn;
	}

	// Call after any change to t's children.
	void upd(Node* t) {
		t->sz = 1 + sz(t->l) + sz(t->r);
		t->mn = min({mn(t->l), t->val, mn(t->r)});
	}

	// (a, b): a = first k elements, b = rest.
	pair<Node*, Node*> split(Node* t, int k) {
		if (!t) {
			return {nullptr, nullptr};
		}

		if (sz(t->l) >= k) {
			auto [a, b] = split(t->l, k);
			t->l = b;

			upd(t);
			return {a, t};
		} else {
			auto [a, b] = split(t->r, k - sz(t->l) - 1);
			t->r = a;

			upd(t);
			return {t, b};
		}
	}

	// All of a comes before all of b; order is implicit, no val comparisons.
	Node* merge(Node* a, Node* b) {
		if (!a) {
			return b;
		}
		if (!b) {
			return a;
		}

		if (a->priority > b->priority) {
			a->r = merge(a->r, b);

			upd(a);
			return a;
		} else {
			b->l = merge(a, b->l);

			upd(b);
			return b;
		}
	}

	// [l, r)
	int query(int l, int r) {
		auto [a, bc] = split(root, l);
		auto [b, c]  = split(bc, r - l);

		int res = mn(b);
		root = merge(a, merge(b, c));

		return res;
	}
};

int main() {
	vector<int> a = {1, 2, 3, 4, 5};

	Treap trp(a);

	cout << trp.query(0, 5) << endl; // 1
	cout << trp.query(2, 4) << endl; // 3
	cout << trp.query(4, 5) << endl; // 5

	return 0;
}
