// Sparse Table for Range Minimum/Maximum Query.
// Build: O(n log n), Query: O(1), Memory: O(n log n).
// Query: min (or max with std::greater<T>) on half-open [l, r), 0 <= l < r <= n.
// query() is const: no mutation, no allocations.

#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <bit>
#include <limits>
#include <type_traits>

inline namespace rmq {

template <typename T, typename Cmp = std::less<T>>
struct SparseTable {
    static constexpr const char* name = "SparseTable";
    std::vector<std::vector<T>> nodes;
    Cmp cmp{};

    explicit SparseTable(const std::vector<T>& a) {
        if (a.empty()) return;
        std::size_t n = a.size(), K = std::bit_width(n);
        nodes.assign(K, std::vector<T>(n));
        nodes[0] = a;
        for (std::size_t k = 1; k < K; ++k) {
            std::size_t half = std::size_t{1} << (k - 1), len = half << 1;
            for (std::size_t i = 0; i + len <= n; ++i)
                nodes[k][i] = cmb(nodes[k-1][i], nodes[k-1][i + half]);
        }
    }
    SparseTable(const SparseTable&) = delete;
    SparseTable& operator=(const SparseTable&) = delete;

    // [l, r)
    T query(std::size_t l, std::size_t r) const {
        std::size_t k = std::bit_width(r - l) - 1, len = std::size_t{1} << k;
        return cmb(nodes[k][l], nodes[k][r - len]);
    }

private:
    T cmb(const T& a, const T& b) const { return cmp(a, b) ? a : b; }
};

}
