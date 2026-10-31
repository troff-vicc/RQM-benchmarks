// Segment Tree for Range Minimum/Maximum Query.
// Build: O(n), Query: O(log n), Memory: O(n).
// Query: min (or max with std::greater<T>) on half-open [l, r), 0 <= l < r <= n.

#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <limits>
#include <type_traits>

inline namespace rmq {

template <typename T, typename Cmp = std::less<T>>
struct SegmentTree {
    static constexpr const char* name = "SegmentTree";
    std::vector<T> nodes;
    std::size_t n;
    Cmp cmp{};

    explicit SegmentTree(const std::vector<T>& a) : nodes(4 * a.size()), n(a.size()) {
        if (n > 0) build(0, 0, n, a);
    }
    T query(std::size_t l, std::size_t r) const { return query_impl(0, 0, n, l, r); }

private:
    static constexpr T neutral() {
        if constexpr (std::is_same_v<Cmp, std::less<T>>)         return std::numeric_limits<T>::max();
        else if constexpr (std::is_same_v<Cmp, std::greater<T>>) return std::numeric_limits<T>::lowest();
        else                                                     return T{};
    }
    T pick(const T& x, const T& y) const { return cmp(x, y) ? x : y; }
    void build(std::size_t v, std::size_t l, std::size_t r, const std::vector<T>& a) {
        if (r - l == 1) { nodes[v] = a[l]; return; }
        std::size_t m = (l + r) / 2;
        build(2 * v + 1, l, m, a);
        build(2 * v + 2, m, r, a);
        nodes[v] = pick(nodes[2 * v + 1], nodes[2 * v + 2]);
    }
    T query_impl(std::size_t v, std::size_t l, std::size_t r, std::size_t lq, std::size_t rq) const {
        if (r <= lq || rq <= l) return neutral();
        if (lq <= l && r <= rq) return nodes[v];
        std::size_t m = (l + r) / 2;
        return pick(query_impl(2 * v + 1, l, m, lq, rq), query_impl(2 * v + 2, m, r, lq, rq));
    }
};

}