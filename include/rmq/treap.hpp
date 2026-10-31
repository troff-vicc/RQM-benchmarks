// Treap (implicit key) for Range Minimum/Maximum Query.
// Build: O(n) expected, Query: O(log n) expected, Memory: O(n).
// Query: min (or max with std::greater<T>) on half-open [l, r), 0 <= l < r <= n.
// Note: query() mutates the tree internally (split/merge); not thread-safe.
// Nodes are intentionally leaked — process is short-lived; no destructor.
// Default seed is random (std::random_device); pass explicit seed for reproducibility.

#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <random>
#include <utility>

inline namespace rmq {

template <typename T, typename Cmp = std::less<T>>
struct Treap {
    static constexpr const char* name = "Treap";

    struct Node {
        T val;
        std::uint32_t priority;
        T mn;
        std::size_t sz;
        Node* l = nullptr;
        Node* r = nullptr;
        Node(T x, std::uint32_t p) : val(x), priority(p), mn(x), sz(1) {}
    };

    Node* root = nullptr;
    Cmp cmp{};
    std::mt19937 rng;

    explicit Treap(const std::vector<T>& a, std::uint32_t seed = std::random_device{}())
        : rng(seed) {
        if (!a.empty()) root = build(a);
    }
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap(Treap&& o) noexcept : root(o.root), cmp(o.cmp), rng(o.rng) { o.root = nullptr; }
    Treap& operator=(Treap&& o) noexcept {
        if (this != &o) { root = o.root; cmp = o.cmp; rng = o.rng; o.root = nullptr; }
        return *this;
    }

    T query(std::size_t l, std::size_t r) {
        auto [a, bc] = split(root, l);
        auto [b, c]  = split(bc, r - l);
        T res = mn(b);
        root = merge(a, merge(b, c));
        return res;
    }

private:
    static constexpr T neutral() {
        if constexpr (std::is_same_v<Cmp, std::less<T>>)         return std::numeric_limits<T>::max();
        else if constexpr (std::is_same_v<Cmp, std::greater<T>>) return std::numeric_limits<T>::lowest();
        else                                                     return T{};
    }
    T pick(const T& x, const T& y) const { return cmp(x, y) ? x : y; }
    T pick3(const T& a, const T& b, const T& c) const { return pick(a, pick(b, c)); }

    static std::size_t sz(Node* t) { return t ? t->sz : 0; }
    static T mn(Node* t) { return t ? t->mn : neutral(); }

    void upd(Node* t) {
        t->sz = 1 + sz(t->l) + sz(t->r);
        t->mn = pick3(mn(t->l), t->val, mn(t->r));
    }

    Node* build(const std::vector<T>& a) {
        std::vector<Node*> st;
        for (const T& x : a) {
            Node* cur = new Node(x, static_cast<std::uint32_t>(rng()));
            Node* last = nullptr;
            while (!st.empty() && st.back()->priority < cur->priority) {
                last = st.back(); upd(last); st.pop_back();
            }
            cur->l = last;
            if (!st.empty()) st.back()->r = cur;
            st.push_back(cur);
        }
        while (st.size() > 1) { Node* top = st.back(); st.pop_back(); upd(top); }
        upd(st[0]);
        return st[0];
    }

    std::pair<Node*, Node*> split(Node* t, std::size_t k) {
        if (!t) return {nullptr, nullptr};
        if (sz(t->l) >= k) {
            auto [a, b] = split(t->l, k);
            t->l = b; upd(t);
            return {a, t};
        } else {
            auto [a, b] = split(t->r, k - sz(t->l) - 1);
            t->r = a; upd(t);
            return {t, b};
        }
    }

    Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->priority > b->priority) { a->r = merge(a->r, b); upd(a); return a; }
        else                           { b->l = merge(a, b->l); upd(b); return b; }
    }
};

}