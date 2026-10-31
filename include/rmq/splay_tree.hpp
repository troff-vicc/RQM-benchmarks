// Splay tree (implicit key) for Range Minimum/Maximum Query.
// Build: O(n), Query: O(log n) amortized, Memory: O(n).
// Query: min (or max with std::greater<T>) on half-open [l, r), 0 <= l < r <= n.
// Note: query() mutates the tree internally (split/merge); not thread-safe.
// Nodes are intentionally leaked — process is short-lived; no destructor.
// upd() does not set t->p; callers must ensure it is already correct.

#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>

inline namespace rmq {

template <typename T, typename Cmp = std::less<T>>
struct SplayTree {
    static constexpr const char* name = "SplayTree";
    struct Node {
        T val, mn;
        std::size_t sz;
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        Node(T x) : val(x), mn(x), sz(1) {}
    };
    Node* root = nullptr;
    Cmp cmp{};

    explicit SplayTree(const std::vector<T>& a) { root = build(0, a.size(), a); }
    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;

    T query(std::size_t l, std::size_t r) {
        auto [a, bc] = split(root, l);
        auto [b, c]  = split(bc, r - l);
        T res = mn(b);
        root = merge(a, merge(b, c));
        return res;
    }

private:
    static constexpr T neutral() {
        if constexpr (std::is_same_v<Cmp, std::greater<T>>) return std::numeric_limits<T>::lowest();
        else                                                return std::numeric_limits<T>::max();
    }
    T cmb(const T& a, const T& b) const { return cmp(a, b) ? a : b; }
    static std::size_t sz(Node* t) { return t ? t->sz : 0; }
    static T mn(Node* t) { return t ? t->mn : neutral(); }

    void upd(Node* t) {
        t->sz = 1 + sz(t->l) + sz(t->r);
        t->mn = cmb(cmb(mn(t->l), t->val), mn(t->r));
        if (t->l) t->l->p = t;
        if (t->r) t->r->p = t;
    }
    Node* build(std::size_t l, std::size_t r, const std::vector<T>& a) {
        if (l >= r) return nullptr;
        std::size_t m = (l + r) / 2;
        Node* t = new Node(a[m]);
        t->l = build(l, m, a); t->r = build(m + 1, r, a);
        upd(t); return t;
    }
    void rotate(Node* x) {
        Node *p = x->p, *g = p->p;
        if (p->l == x) { p->l = x->r; if (x->r) x->r->p = p; x->r = p; }
        else           { p->r = x->l; if (x->l) x->l->p = p; x->l = p; }
        p->p = x; x->p = g;
        if (g) (g->l == p ? g->l : g->r) = x;
        upd(p); upd(x);
    }
    void splay(Node* x) {
        while (x->p) {
            Node *p = x->p, *g = p->p;
            if (g) rotate((g->l == p) == (p->l == x) ? p : x);
            rotate(x);
        }
        root = x;
    }
    Node* find_k(std::size_t k) {
        Node* cur = root;
        while (cur) {
            std::size_t left = sz(cur->l);
            if (k < left)        cur = cur->l;
            else if (k == left) { splay(cur); return cur; }
            else                { k -= left + 1; cur = cur->r; }
        }
        return nullptr;
    }
    std::pair<Node*, Node*> split(Node* t, std::size_t k) {
        if (!t)         return {nullptr, nullptr};
        if (k == 0)     return {nullptr, t};
        if (k >= sz(t)) return {t, nullptr};
        root = t; find_k(k);
        Node* a = root->l; if (a) a->p = nullptr;
        root->l = nullptr; upd(root);
        return {a, root};
    }
    Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        root = a;
        Node* cur = a; while (cur->r) cur = cur->r;
        splay(cur);
        cur->r = b; b->p = cur; upd(cur);
        return cur;
    }
};

} // namespace rmq