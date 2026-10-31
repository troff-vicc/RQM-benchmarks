// Splay tree (implicit key) with Range Minimum Query.
//
// Build:  O(n)
// Query:  O(log n) amortized — via split/merge
// Memory: O(n)
//
// Positions are implicit: stored in subtree sizes, not in keys.
// Query answers min on half-open interval [l, r), 0 <= l < r <= n.

#include<bits/stdc++.h>

using namespace std;

struct SplayTree {
	struct Node {
		int val;
		int mn;         // min in subtree
		int sz;         // subtree size

		Node* l = nullptr;
		Node* r = nullptr;
		Node* p = nullptr;

		Node(int x) {
			val = x;
			mn = x;
			sz = 1;
		}
	};

	Node* root = nullptr;

	SplayTree(const vector<int> &a) {
		root = build(0, (int)a.size(), a);
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

	// Syncs parent pointers of children — so callers don't have to.
	void upd(Node* t) {
		t->sz = 1 + sz(t->l) + sz(t->r);
		t->mn = min({mn(t->l), t->val, mn(t->r)});

		if (t->l) {
			t->l->p = t;
		}
		if (t->r) {
			t->r->p = t;
		}
	}

	// O(n): median split → perfectly balanced initial tree.
	Node* build(int l, int r, const vector<int> &a) {
		if (l >= r) {
			return nullptr;
		}

		int m = (l + r) / 2;

		Node* t = new Node(a[m]);
		t->l = build(l, m, a);
		t->r = build(m + 1, r, a);

		upd(t);
		return t;
	}

	// Rotate x up over its parent p.
	void rotate(Node* x) {
		Node* p = x->p;
		Node* g = p->p;

		if (p->l == x) {
			p->l = x->r;
			if (x->r) x->r->p = p;
			x->r = p;
		} else {
			p->r = x->l;
			if (x->l) x->l->p = p;
			x->l = p;
		}
		p->p = x;
		x->p = g;
		if (g) {
			if (g->l == p) g->l = x;
			else           g->r = x;
		}
		upd(p);
		upd(x);
	}

	// Bring x to the root of its tree. Zig-zig / zig-zag → amortized O(log n).
	void splay(Node* x) {
		while (x->p) {
			Node* p = x->p;
			Node* g = p->p;

			if (g) {
				if ((g->l == p) == (p->l == x)) {
					rotate(p);
				} else {
					rotate(x);
				}
			}
			rotate(x);
		}
		root = x;
	}

	// Find k-th node (0-indexed) and splay it to the root.
	Node* find_k(int k) {
		Node* cur = root;
		while (cur) {
			int left = sz(cur->l);

			if (k < left) {
				cur = cur->l;
				
			} else if (k == left) {
				splay(cur);

				return cur;
			} else {
				k -= left + 1;
				cur = cur->r;
			}
		}
		return nullptr;
	}

	// (a, b): a = first k elements, b = rest.
	pair<Node*, Node*> split(Node* t, int k) {
		if (!t) {
			return {nullptr, nullptr};
		}
		if (k == 0) {
			return {nullptr, t};
		}
		if (k >= sz(t)) {
			return {t, nullptr};
		}

		root = t;
		find_k(k);
		Node* a = root->l;
		if (a) {
			a->p = nullptr;
		}
		root->l = nullptr;

		upd(root);
		return {a, root};
	}

	// All of a comes before all of b; order is implicit, no val comparisons.
	Node* merge(Node* a, Node* b) {
		if (!a) {
			return b;
		}
		if (!b) {
			return a;
		}

		root = a;
		Node* cur = a;

		while (cur->r) {
			cur = cur->r;
		}
		splay(cur);

		cur->r = b;
		b->p = cur;

		upd(cur);
		return cur;
	}

	// [l, r): isolate [l, r) via two splits, read mn, merge back.
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

	SplayTree spl(a);

	cout << spl.query(0, 5) << endl; // 1
	cout << spl.query(2, 4) << endl; // 3
	cout << spl.query(4, 5) << endl; // 5

	return 0;
}
