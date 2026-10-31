// Sqrt Tree for Range Minimum/Maximum Query.
// Build: O(n log log n), Query: O(1), Memory: O(n log log n).
// Query: min (or max with std::greater<T>) on half-open [l, r), 0 <= l < r <= n.
// query() is const: no mutation, no allocations.

#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <bit>
#include <limits>
#include <type_traits>
#include <algorithm>

inline namespace rmq {

template <typename T, typename Cmp = std::less<T>>
struct SqrtTree {
    static constexpr const char* name = "SqrtTree";
    std::size_t n, lg;
    std::vector<T> a;
    std::vector<std::size_t> LOG, layers, onLayer;
    std::vector<std::vector<T>> pref, suf, between;
    Cmp cmp{};

    explicit SqrtTree(const std::vector<T>& input) {
        n = input.size();
        a = input;
        lg = (n <= 1) ? 0 : std::bit_width(n - 1);

        LOG.assign(std::size_t{1} << lg, 0);
        for (std::size_t i = 1; i < LOG.size(); ++i)
            LOG[i] = LOG[i >> 1] + 1;

        layers.push_back(lg);
        for (std::size_t tlg = lg; tlg > 1; )
            layers.push_back(tlg = (tlg + 1) / 2);

        onLayer.assign(lg + 1, 0);
        for (std::size_t p = lg + 1; p --> 0; ) {
            std::size_t k = 0;
            while (k + 1 < layers.size() && layers[k + 1] >= p + 1) ++k;
            onLayer[p] = k;
        }

        pref.assign(layers.size(), std::vector<T>(n));
        suf.assign(layers.size(), std::vector<T>(n));
        between.assign(layers.size(), std::vector<T>(std::size_t{1} << lg, neutral()));

        build(0, 0, n);
    }
    SqrtTree(const SqrtTree&) = delete;
    SqrtTree& operator=(const SqrtTree&) = delete;

    // [l, r)
    T query(std::size_t l, std::size_t r) const { return query_impl(l, r - 1); }

private:
    static constexpr T neutral() {
        if constexpr (std::is_same_v<Cmp, std::greater<T>>) return std::numeric_limits<T>::lowest();
        else                                                return std::numeric_limits<T>::max();
    }
    T cmb(const T& a, const T& b) const { return cmp(a, b) ? a : b; }

    void buildBlock(std::size_t layer, std::size_t l, std::size_t r) {
        pref[layer][l] = a[l];
        for (std::size_t i = l + 1; i < r; ++i)
            pref[layer][i] = cmb(pref[layer][i-1], a[i]);
        suf[layer][r-1] = a[r-1];
        for (std::size_t i = r - 1; i --> l; )
            suf[layer][i] = cmb(a[i], suf[layer][i+1]);
    }
    void buildBetween(std::size_t layer, std::size_t lBound, std::size_t rBound) {
        std::size_t bSzLog = (layers[layer] + 1) / 2, bCntLog = layers[layer] / 2;
        std::size_t bSz = std::size_t{1} << bSzLog;
        std::size_t bCnt = (rBound - lBound + bSz - 1) >> bSzLog;
        for (std::size_t i = 0; i < bCnt; ++i) {
            T ans = neutral();
            for (std::size_t j = i; j < bCnt; ++j) {
                T add = suf[layer][lBound + (j << bSzLog)];
                ans = (i == j) ? add : cmb(ans, add);
                between[layer][lBound + (i << bCntLog) + j] = ans;
            }
        }
    }
    void build(std::size_t layer, std::size_t lBound, std::size_t rBound) {
        if (layer >= layers.size()) return;
        std::size_t bSz = std::size_t{1} << ((layers[layer] + 1) / 2);
        for (std::size_t l = lBound; l < rBound; l += bSz) {
            std::size_t r = std::min(l + bSz, rBound);
            buildBlock(layer, l, r);
            build(layer + 1, l, r);
        }
        buildBetween(layer, lBound, rBound);
    }
    T query_impl(std::size_t l, std::size_t r) const {
        if (l == r) return a[l];
        if (l + 1 == r) return cmb(a[l], a[r]);
        std::size_t layer = onLayer[LOG[l ^ r]];
        std::size_t bSzLog = (layers[layer] + 1) / 2, bCntLog = layers[layer] / 2;
        std::size_t lBound = (l >> layers[layer]) << layers[layer];
        std::size_t lBlock = ((l - lBound) >> bSzLog) + 1;
        std::size_t rBlock = ((r - lBound) >> bSzLog) - 1;
        T ans = suf[layer][l];
        if (lBlock <= rBlock)
            ans = cmb(ans, between[layer][lBound + (lBlock << bCntLog) + rBlock]);
        return cmb(ans, pref[layer][r]);
    }
};

} // namespace rmq