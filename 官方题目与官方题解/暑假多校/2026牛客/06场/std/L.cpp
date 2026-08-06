#include <bits/stdc++.h>

using namespace std;

/*
最后修改:
20231218
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SUFFIXARRAY__
#define __OY_SUFFIXARRAY__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace OY {
    namespace SA {
        using size_type = uint32_t;
        template <bool Rank, bool Height, size_type MAX_LEN>
        struct SuffixArray {
            size_type m_length;
            std::vector<size_type> m_sa, m_rank, m_height;
            static bool s_bool_buffer[MAX_LEN << 1];
            static size_type s_lms_map_buffer[MAX_LEN + 1], s_buffer[MAX_LEN * 6];
            template <typename Sequence>
            void _sa_is(const Sequence &seq, size_type length, size_type alpha, bool *ls, size_type *buffer) {
                if (length == 1) {
                    m_sa[0] = 0;
                    return;
                }
                if (length == 2) {
                    if (seq[0] < seq[1])
                        m_sa[0] = 0, m_sa[1] = 1;
                    else
                        m_sa[0] = 1, m_sa[1] = 0;
                    return;
                }
                ls[length - 1] = false;
                for (size_type i = length - 2; ~i; i--) ls[i] = (seq[i] == seq[i + 1]) ? ls[i + 1] : (seq[i] < seq[i + 1]);
                size_type *buf = buffer, *sum_l = buffer + alpha, *sum_s = buffer + alpha * 2;
                std::fill_n(sum_l, alpha, 0);
                std::fill_n(sum_s, alpha, 0);
                for (size_type i = 0; i != length; i++)
                    if (!ls[i])
                        sum_s[seq[i]]++;
                    else
                        sum_l[seq[i] + 1]++;
                for (size_type i = 0; i != alpha; i++) {
                    sum_s[i] += sum_l[i];
                    if (i + 1 != alpha) sum_l[i + 1] += sum_s[i];
                }
                auto induce = [&](size_type *lms, size_type *lms_end) {
                    std::fill_n(m_sa.begin(), length, -1);
                    std::copy_n(sum_s, alpha, buf);
                    for (auto it = lms; it != lms_end; ++it) {
                        auto d = *it;
                        if (d == length) continue;
                        m_sa[buf[seq[d]]++] = d;
                    }
                    std::copy_n(sum_l, alpha, buf);
                    m_sa[buf[seq[length - 1]]++] = length - 1;
                    for (size_type i = 0; i != length; i++) {
                        typename std::make_signed<size_type>::type v = m_sa[i];
                        if (v >= 1 && !ls[v - 1]) m_sa[buf[seq[v - 1]]++] = v - 1;
                    }
                    std::copy_n(sum_l, alpha, buf);
                    for (size_type i = length - 1; ~i; i--) {
                        typename std::make_signed<size_type>::type v = m_sa[i];
                        if (v >= 1 && ls[v - 1]) m_sa[--buf[seq[v - 1] + 1]] = v - 1;
                    }
                };
                size_type *lms = buffer + alpha * 3, *lms_end = lms;
                s_lms_map_buffer[0] = s_lms_map_buffer[length] = -1;
                size_type m = 0;
                for (size_type i = 1; i != length; i++)
                    if (!ls[i - 1] && ls[i])
                        s_lms_map_buffer[i] = m++, *lms_end++ = i;
                    else
                        s_lms_map_buffer[i] = -1;
                induce(lms, lms_end);
                if (m) {
                    size_type *sorted_lms = lms_end, *sorted_lms_end = sorted_lms;
                    for (size_type i = 0; i != length; i++) {
                        size_type v = m_sa[i];
                        if (~s_lms_map_buffer[v]) *sorted_lms_end++ = v;
                    }
                    size_type *rec_s = sorted_lms_end, rec_alpha = 1;
                    rec_s[s_lms_map_buffer[sorted_lms[0]]] = 0;
                    for (size_type i = 1; i != m; i++) {
                        size_type l = sorted_lms[i - 1], r = sorted_lms[i], end_l = (s_lms_map_buffer[l] + 1 < m) ? lms[s_lms_map_buffer[l] + 1] : length, end_r = (s_lms_map_buffer[r] + 1 < m) ? lms[s_lms_map_buffer[r] + 1] : length;
                        if (end_l - l != end_r - r)
                            rec_alpha++;
                        else {
                            while (l < end_l && seq[l] == seq[r]) l++, r++;
                            if (l == length || seq[l] != seq[r]) rec_alpha++;
                        }
                        rec_s[s_lms_map_buffer[sorted_lms[i]]] = rec_alpha - 1;
                    }
                    _sa_is(rec_s, m, rec_alpha, ls + length, rec_s + m);
                    for (size_type i = 0; i != m; i++) sorted_lms[i] = lms[m_sa[i]];
                    induce(lms_end, sorted_lms_end);
                }
            }
            void _get_rank() {
                m_rank.resize(m_length);
                for (size_type i = 0; i != m_length; i++) m_rank[m_sa[i]] = i;
            }
            template <typename Sequence>
            void _get_height(const Sequence &seq) {
                m_height.resize(m_length);
                for (size_type i = 0, h = 0; i != m_length; i++) {
                    if (h) h--;
                    if (m_rank[i])
                        while (m_sa[m_rank[i] - 1] + h < m_length && seq[i + h] == seq[m_sa[m_rank[i] - 1] + h]) h++;
                    m_height[m_rank[i]] = h;
                }
            }
            SuffixArray() = default;
            template <typename InitMapping>
            SuffixArray(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            SuffixArray(Iterator first, Iterator last, size_type alpha = 0) { reset(first, last, alpha); }
            SuffixArray(const std::string &seq) : SuffixArray(seq.begin(), seq.end()) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                using value_type = decltype(mapping(0));
                m_length = length;
                std::vector<value_type> text;
                text.reserve(m_length);
                size_type Mx = 0;
                bool less_than_zero = false;
                for (size_type i = 0; i != m_length; i++) {
                    auto elem = mapping(i);
                    Mx = std::max<size_type>(Mx, elem);
                    less_than_zero |= elem < 0;
                    text.push_back(elem);
                }
                m_sa.resize(m_length);
                if (Mx < MAX_LEN && !less_than_zero)
                    _sa_is(text, m_length, Mx + 1, s_bool_buffer, s_buffer);
                else {
                    std::vector<value_type> items(text);
                    std::sort(items.begin(), items.end());
                    items.erase(std::unique(items.begin(), items.end()), items.end());
                    std::vector<size_type> ord(m_length);
                    for (size_type i = 0; i != m_length; i++) ord[i] = std::lower_bound(items.begin(), items.end(), text[i]) - items.begin();
                    _sa_is(ord, m_length, items.size(), s_bool_buffer, s_buffer);
                }
                if constexpr (Rank) {
                    _get_rank();
                    if constexpr (Height) _get_height(text);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, uint32_t alpha = 0) {
                using value_type = typename std::decay<decltype(*first)>::type;
                m_length = last - first;
                m_sa.resize(m_length);
                if (alpha)
                    _sa_is(first, m_length, alpha, s_bool_buffer, s_buffer);
                else {
                    size_type Mx = 0;
                    bool less_than_zero = false;
                    for (auto it = first; it != last; ++it) {
                        auto &&elem = *it;
                        Mx = std::max<size_type>(Mx, elem);
                        less_than_zero |= elem < 0;
                    }
                    if (Mx < MAX_LEN && !less_than_zero)
                        _sa_is(first, m_length, Mx + 1, s_bool_buffer, s_buffer);
                    else {
                        std::vector<value_type> items(first, last);
                        std::sort(items.begin(), items.end());
                        items.erase(std::unique(items.begin(), items.end()), items.end());
                        std::vector<size_type> ord(m_length);
                        for (size_type i = 0; i != m_length; i++) ord[i] = std::lower_bound(items.begin(), items.end(), *(first + i)) - items.begin();
                        _sa_is(ord, m_length, items.size(), s_bool_buffer, s_buffer);
                    }
                }
                if constexpr (Rank) {
                    _get_rank();
                    if constexpr (Height) _get_height(first);
                }
            }
            size_type query_sa(size_type rank) const { return m_sa[rank]; }
            size_type query_rank(size_type pos) const {
                static_assert(Rank, "Rank Must Be True");
                return m_rank[pos];
            }
            size_type query_height(size_type rank) const {
                static_assert(Height, "Height Must Be True");
                return m_height[rank];
            }
        };
        template <bool Rank, bool Height, size_type MAX_LEN>
        bool SuffixArray<Rank, Height, MAX_LEN>::s_bool_buffer[MAX_LEN << 1];
        template <bool Rank, bool Height, size_type MAX_LEN>
        size_type SuffixArray<Rank, Height, MAX_LEN>::s_lms_map_buffer[MAX_LEN + 1];
        template <bool Rank, bool Height, size_type MAX_LEN>
        size_type SuffixArray<Rank, Height, MAX_LEN>::s_buffer[MAX_LEN * 6];
    }
}

#endif

/*
最后修改:
20240904
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CATTREE__
#define __OY_CATTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace CAT {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename SemiGroup, size_t MAX_LEVEL = 30>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub[MAX_LEVEL];
            size_type m_size, m_depth;
            void _update(size_type i) {
                auto sub = m_sub[0].data();
                for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1) {
                    auto cur = m_sub[j].data();
                    size_type l = i & -(1 << (j + 1));
                    if (i >> j & 1) {
                        size_type j = i, end = std::min(l + k, m_size);
                        cur[j] = (j == l + (k >> 1)) ? sub[j] : group::op(cur[j - 1], sub[j]);
                        while (++j != end) cur[j] = group::op(cur[j - 1], sub[j]);
                    } else {
                        if (m_size <= l + k / 2) continue;
                        size_type j = i + 1;
                        cur[j - 1] = (j == l + (k >> 1)) ? sub[j - 1] : group::op(sub[j - 1], cur[j]);
                        while (--j != l) cur[j - 1] = group::op(sub[j - 1], cur[j]);
                    }
                }
            }
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                for (size_type i = 0; i != m_depth; i++) m_sub[i].resize(m_size);
                auto sub = m_sub[0].data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                    auto cur = m_sub[j].data();
                    for (l = 0; l + k <= m_size; l += k) {
                        size_type i = l + (k >> 1);
                        cur[i - 1] = sub[i - 1];
                        while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        i = l + (k >> 1);
                        cur[i] = sub[i];
                        while (++i != l + k) cur[i] = group::op(cur[i - 1], sub[i]);
                    }
                    if (l != m_size && (l + (k >> 1) < m_size)) {
                        size_type i = l + (k >> 1);
                        cur[i - 1] = sub[i - 1];
                        while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        i = l + (k >> 1);
                        cur[i] = sub[i];
                        while (++i != m_size) cur[i] = group::op(cur[i - 1], sub[i]);
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            void modify(size_type i, value_type val) { m_sub[0][i] = val, _update(i); }
            value_type query(size_type i) const { return m_sub[0][i]; }
            value_type query(size_type left, size_type right) const {
                if (left == right) return m_sub[0][left];
                size_type d = std::bit_width(left ^ right) - 1;
                return group::op(m_sub[d][left], m_sub[d][right]);
            }
            value_type query_all() const { return query(0, m_size - 1); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left];
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ (m_size - 1));
                while (d && left < m_size) {
                    size_type split = (left & -(1 << (d - 1))) | (1 << (d - 1));
                    if (m_size <= split)
                        while (--d && (left >> (d - 1) & 1)) {}
                    else {
                        value_type a = group::op(val, m_sub[d - 1][left]);
                        if (judge(a))
                            val = a, --d, left = split;
                        else
                            while (--d && (left >> (d - 1) & 1)) {}
                    }
                }
                if (left < m_size && judge(group::op(val, m_sub[0][left]))) left++;
                return std::min(left, m_size) - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right];
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = group::op(m_sub[d - 1][right], val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(group::op(m_sub[0][right], val))) right--;
                return right + 1;
            }
        };
        template <typename Ostream, typename SemiGroup, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = CAT::Table<CAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(CAT::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = CAT::Table<CAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatMaxTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::ChoiceByCompare<Tp, std::less<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatMinTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::ChoiceByCompare<Tp, std::greater<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatGcdTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::FpTransfer<Tp, std::gcd<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatLcmTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::FpTransfer<Tp, std::lcm<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitAndTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_and<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitOrTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_or<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitXorTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_xor<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatSumTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::plus<Tp>>, MAX_LEVEL>;
}

#endif

/*
最后修改:
20240905
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SQRTTREE__
#define __OY_SQRTTREE__

namespace OY {
    namespace SQRT {
        using size_type = uint32_t;
        using CAT::BaseSemiGroup;
        using CAT::ChoiceByCompare;
        using CAT::FpTransfer;
        template <size_type BlockSize = 16>
        struct StaticController {
            void reserve(size_type capacity) {}
            static constexpr bool is_first(size_type i) { return i % BlockSize == 0; }
            static constexpr size_type block_id(size_type i) { return i / BlockSize; }
            static constexpr size_type block_first(size_type i) { return i / BlockSize * BlockSize; }
            static constexpr size_type block_size() { return BlockSize; }
            static constexpr size_type block_count(size_type length) { return (length + BlockSize - 1) / BlockSize; }
        };
        template <size_type DefaultDepth = 5>
        struct RandomController {
            size_type m_mask = (1 << DefaultDepth) - 1, m_depth = DefaultDepth;
            void reserve(size_type capacity) { m_depth = (std::bit_width(capacity) - 1) / 2, m_mask = (1 << m_depth) - 1; }
            bool is_first(size_type i) const { return !(i & m_mask); }
            size_type block_id(size_type i) const { return i >> m_depth; }
            size_type block_first(size_type i) const { return i & ~m_mask; }
            size_type block_size() const { return m_mask + 1; }
            size_type block_count(size_type length) const { return (length + m_mask) >> m_depth; }
        };
        template <size_type DefaultDepth = 5>
        struct NonRandomController {
            size_type m_mask = (1 << DefaultDepth) - 1, m_depth = DefaultDepth;
            void reserve(size_type capacity) { m_depth = capacity >= 32 ? std::bit_width<size_type>(std::bit_width(capacity / std::bit_width(capacity)) - 1) : (std::bit_width(capacity) - 1) / 2, m_mask = (size_type(1) << m_depth) - 1; }
            bool is_first(size_type i) const { return !(i & m_mask); }
            size_type block_id(size_type i) const { return i >> m_depth; }
            size_type block_first(size_type i) const { return i & ~m_mask; }
            size_type block_size() const { return m_mask + 1; }
            size_type block_count(size_type length) const { return (length + m_mask) >> m_depth; }
        };
        template <typename SemiGroup, typename Controller = RandomController<>, size_t MAX_LEVEL = 30>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
            using inner_table = CAT::Table<group, MAX_LEVEL>;
        private:
            inner_table m_table;
            std::vector<value_type> m_data, m_prefix, m_suffix;
            size_type m_size;
            Controller m_ctrl;
            template <typename Judger>
            size_type _max_right(size_type left, size_type end, Judger &&judge) const {
                value_type val = m_data[left];
                if (judge(val))
                    while (++left != end) {
                        value_type a = group::op(val, m_data[left]);
                        if (!judge(a)) break;
                        val = a;
                    }
                return left - 1;
            }
            template <typename Judger>
            size_type _max_right2(size_type left, size_type end, Judger &&judge) const {
                size_type low = left, high = end;
                while (low != high) {
                    size_type mid = (low + high) / 2;
                    if (judge(m_prefix[mid]))
                        low = mid + 1;
                    else
                        high = mid;
                }
                return low - 1;
            }
            template <typename Judger>
            size_type _min_left(size_type end, size_type right, Judger &&judge) const {
                value_type val = m_data[right];
                if (judge(val))
                    while (--right != end) {
                        value_type a = group::op(m_data[right], val);
                        if (!judge(a)) break;
                        val = a;
                    }
                return right + 1;
            }
            template <typename Judger>
            size_type _min_left2(size_type end, size_type right, Judger &&judge) const {
                size_type low = end, high = right;
                while (low != high) {
                    size_type mid = (low + high + 1) / 2;
                    if (judge(m_suffix[mid]))
                        high = mid - 1;
                    else
                        low = mid;
                }
                return low + 1;
            }
            void _update(size_type i) {
                size_type cur = m_ctrl.block_first(i), nxt = std::min(cur + m_ctrl.block_size(), m_size);
                m_prefix[i] = (i == cur) ? m_data[i] : group::op(m_prefix[i - 1], m_data[i]);
                for (size_type j = i + 1; j != nxt; j++) m_prefix[j] = group::op(m_prefix[j - 1], m_data[j]);
                m_suffix[i] = (i == nxt - 1) ? m_data[i] : group::op(m_data[i], m_suffix[i + 1]);
                for (size_type j = i - 1; j != cur - 1; j--) m_suffix[j] = group::op(m_data[j], m_suffix[j + 1]);
                m_table.modify(m_ctrl.block_id(i), m_suffix[cur]);
            }
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            size_type size() const { return m_size; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_ctrl.reserve(m_size);
                m_data.resize(m_size);
                for (size_type i = 0; i != m_size; i++) m_data[i] = mapping(i);
                m_prefix = m_suffix = m_data;
                for (size_type i = 1; i != m_size; i++)
                    if (!m_ctrl.is_first(i)) m_prefix[i] = group::op(m_prefix[i - 1], m_prefix[i]);
                for (size_type i = m_size - 1; i; i--)
                    if (!m_ctrl.is_first(i)) m_suffix[i - 1] = group::op(m_suffix[i - 1], m_suffix[i]);
                m_table.resize(m_ctrl.block_count(m_size), [&](size_type i) { return m_suffix[i * m_ctrl.block_size()]; });
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, value_type val) { m_data[i] = val, _update(i); }
            value_type query(size_type i) const { return m_data[i]; }
            value_type query(size_type left, size_type right) const {
                size_type l = m_ctrl.block_id(left), r = m_ctrl.block_id(right);
                if (l == r) {
                    value_type res = m_data[left];
#ifndef __clang__
#pragma GCC unroll 64
#endif
                    for (size_type i = left + 1; i <= right; i++) res = group::op(res, m_data[i]);
                    return res;
                } else if (l + 1 == r)
                    return group::op(m_suffix[left], m_prefix[right]);
                else
                    return group::op(group::op(m_suffix[left], m_table.query(l + 1, r - 1)), m_prefix[right]);
            }
            value_type query_all() const { return m_table.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_suffix[left];
                if (!judge(val)) return _max_right(left, std::min(m_size, m_ctrl.block_first(left) + m_ctrl.block_size()), judge);
                size_type l = m_ctrl.block_id(left);
                if (l + 1 == m_table.size()) return m_size - 1;
                size_type r = m_table.max_right(l + 1, [&](const value_type &x) { return judge(group::op(val, x)); });
                if (r + 1 == m_table.size()) return m_size - 1;
                if (r > l) val = group::op(val, m_table.query(l + 1, r));
                return _max_right2((r + 1) * m_ctrl.block_size(), std::min(m_size, (r + 2) * m_ctrl.block_size()), [&](const value_type &x) { return judge(group::op(val, x)); });
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_prefix[right];
                if (!judge(val)) return _min_left(m_ctrl.block_first(right) - 1, right, judge);
                size_type r = m_ctrl.block_id(right);
                if (!r) return 0;
                size_type l = m_table.min_left(r - 1, [&](const value_type &x) { return judge(group::op(x, val)); });
                if (!l) return 0;
                if (l < r) val = group::op(m_table.query(l, r - 1), val);
                return _min_left2(((l - 1) * m_ctrl.block_size()) - 1, (l * m_ctrl.block_size()) - 1, [&](const value_type &x) { return judge(group::op(x, val)); });
            }
        };
        template <typename Ostream, typename SemiGroup, typename Controller, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, Controller, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename Controller = SQRT::RandomController<>, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = SQRT::Table<SQRT::BaseSemiGroup<Tp, Operation>, Controller, MAX_LEVEL>>
    auto make_SqrtTree(SQRT::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Controller = SQRT::RandomController<>, size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = SQRT::Table<SQRT::BaseSemiGroup<Tp, Operation>, Controller, MAX_LEVEL>>
    auto make_SqrtTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtMaxTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, SQRT::ChoiceByCompare<Tp, std::less<Tp>>>, Controller, MAX_LEVEL>;
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtMinTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, SQRT::ChoiceByCompare<Tp, std::greater<Tp>>>, Controller, MAX_LEVEL>;
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtGcdTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, SQRT::FpTransfer<Tp, std::gcd<Tp>>>, Controller, MAX_LEVEL>;
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtLcmTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, SQRT::FpTransfer<Tp, std::lcm<Tp>>>, Controller, MAX_LEVEL>;
    template <typename Tp, typename Controller = SQRT::RandomController<>, size_t MAX_LEVEL = 30>
    using SqrtBitAndTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, std::bit_and<Tp>>, Controller, MAX_LEVEL>;
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtBitOrTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, std::bit_or<Tp>>, Controller, MAX_LEVEL>;
    template <typename Tp, typename Controller = SQRT::RandomController<>, size_t MAX_LEVEL = 30>
    using SqrtBitXorTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, std::bit_xor<Tp>>, Controller, MAX_LEVEL>;
    template <typename Tp,typename Controller = SQRT::RandomController<>,  size_t MAX_LEVEL = 30>
    using SqrtSumTable = SQRT::Table<SQRT::BaseSemiGroup<Tp, std::plus<Tp>>, Controller, MAX_LEVEL>;
}

#endif

/*
最后修改:
20240425
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SALCP__
#define __OY_SALCP__

namespace OY {
    namespace SALCP {
        using size_type = uint32_t;
        template <size_type MAX_LEN>
        struct LCP {
            size_type m_length;
            SA::SuffixArray<true, true, MAX_LEN> m_table;
            SqrtMinTable<size_type, SQRT::RandomController<>, 15> m_inner_table;
            template <typename Iterator>
            LCP(Iterator first, Iterator last) : m_length(last - first), m_table(first, last), m_inner_table(m_length, [&](size_type i) { return m_table.query_height(i); }) {}
            LCP(const std::vector<int> &seq) : LCP(seq.begin(), seq.end()) {}
            LCP(const std::string &seq) : LCP(seq.begin(), seq.end()) {}
            size_type lcp(size_type a, size_type b, size_type limit) const {
                if (a == b) return limit;
                size_type ra = m_table.query_rank(a), rb = m_table.query_rank(b);
                if (ra > rb) std::swap(ra, rb);
                return std::min<size_type>(limit, m_inner_table.query(ra + 1, rb));
            }
            size_type lcs(size_type a, size_type b, size_type limit) const {
                if (a == b) return a + 1;
                size_type low = 0, high = limit;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    size_type ra = m_table.query_rank(a - mid + 1), rb = m_table.query_rank(b - mid + 1);
                    if (ra > rb) std::swap(ra, rb);
                    if (m_inner_table.query(ra + 1, rb) >= mid)
                        low = mid;
                    else
                        high = mid - 1;
                }
                return low;
            }
            size_type lcp(size_type a, size_type b) const {
                if (a == b) return m_length - a;
                return lcp(a, b, m_length - std::max(a, b));
            }
            size_type lcs(size_type a, size_type b) const {
                if (a == b) return a + 1;
                return lcs(a, b, std::min(a, b) + 1);
            }
            int compare(size_type l1, size_type r1, size_type l2, size_type r2) const {
                if (l1 == l2) return r1 < r2 ? -1 : (r1 == r2 ? 0 : 1);
                size_type len1 = r1 - l1 + 1, len2 = r2 - l2 + 1, ra = m_table.query_rank(l1), rb = m_table.query_rank(l2);
                if (ra < rb)
                    if (len1 < len2 || m_inner_table.query(ra + 1, rb) < len2)
                        return -1;
                    else
                        return len1 > len2;
                else if (len2 < len1 || m_inner_table.query(rb + 1, ra) < len1)
                    return 1;
                else
                    return len1 < len2 ? -1 : 0;
            }
        };
    }
    template <SALCP::size_type MAX_LEN, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(const std::vector<int> &seq) -> TableType { return TableType(seq.begin(), seq.end()); }
    template <SALCP::size_type MAX_LEN, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(const std::string &seq) -> TableType { return TableType(seq.begin(), seq.end()); }
    template <SALCP::size_type MAX_LEN, typename ValueType, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(ValueType *first, ValueType *last) -> TableType { return TableType(first, last); }
}

#endif

/*
最后修改:
20240107
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PAM__
#define __OY_PAM__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

namespace OY {
    namespace PAM {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void add_child(size_type index, size_type child) { m_child[index] = child; }
            void remove_child(size_type index) { m_child[index] = 0; }
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename ChildGetter, typename Sequence>
        struct Automaton {
            using value_type = typename Sequence::value_type;
            struct node : ChildGetter {
                size_type m_length, m_fail, m_ancestor, m_size_when_appear;
            };
            struct series {
                size_type m_longest, m_shortest, m_delta;
            };
            Sequence m_text;
            std::vector<node> m_data;
            std::vector<size_type> m_node;
            static constexpr size_type odd_root() { return 0; }
            static constexpr size_type even_root() { return 1; }
            static constexpr bool is_node(size_type node_index) { return node_index >> 1; }
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            void _init() { m_data.resize(2), m_data[0].m_length = m_data[0].m_fail = -1, m_data[1].m_length = m_data[1].m_fail = 0, m_data[1].m_ancestor = 1, m_node.push_back(0); }
            size_type _jump(size_type node_index, size_type index, const value_type &elem) const {
                if (~node_index)
                    while (index == m_data[node_index].m_length || m_text[index - m_data[node_index].m_length - 1] != elem) node_index = m_data[node_index].m_fail;
                return node_index;
            }
            Automaton() { _init(); }
            template <typename InitMapping>
            Automaton(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Automaton(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                reserve(length);
                for (size_type i = 0; i != length; i++) push_back(mapping(i));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type length) { m_text.clear(), m_text.reserve(length), m_data.clear(), m_data.reserve(length + 2), m_node.clear(), m_node.reserve(length + 1), _init(); }
            void clear() { m_text.clear(), m_data.clear(), m_node.clear(), _init(); }
            size_type size() const { return m_text.size(); }
            bool empty() const { return m_text.empty(); }
            void push_back(const value_type &elem) {
                size_type index = m_text.size();
                m_text.push_back(elem);
                size_type last = _jump(m_node.back(), index, elem), child = m_data[last].get_child(elem);
                if (!child) {
                    child = _newnode();
                    m_data[last].add_child(elem, child);
                    m_data[child].m_length = m_data[last].m_length + 2;
                    size_type fail = _jump(m_data[last].m_fail, index, elem);
                    fail = ~fail ? m_data[fail].get_child(elem) : 1;
                    m_data[child].m_fail = fail;
                    m_data[child].m_ancestor = m_data[m_data[fail].m_fail].m_length + m_data[child].m_length == m_data[fail].m_length * 2 ? m_data[fail].m_ancestor : fail;
                    m_data[child].m_size_when_appear = m_text.size();
                }
                m_node.push_back(child);
            }
            void pop_back() {
                if (m_data.back().m_size_when_appear == m_text.size()) {
                    const auto &elem = m_text.back();
                    size_type last = _jump(m_node[m_node.size() - 2], m_text.size() - 1, elem);
                    m_data[last].remove_child(elem);
                    m_data.pop_back(), m_text.pop_back(), m_node.pop_back();
                } else
                    m_text.pop_back(), m_node.pop_back();
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            size_type query_fail(size_type node_index) const { return m_data[node_index].m_fail; }
            size_type query_ancestor(size_type node_index) const { return m_data[node_index].m_ancestor; }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
            const node *get_fail_node(size_type node_index) const { return get_node(query_fail(node_index)); }
            node *get_fail_node(size_type node_index) { return get_node(query_fail(node_index)); }
            bool query(size_type left, size_type right) const {
                size_type len = right - left + 1, cur = m_node[right + 1], cur_len = m_data[cur].m_length;
                if (cur_len < len) return false;
                while (true) {
                    size_type fail = m_data[cur].m_fail, nxt = m_data[cur].m_ancestor, shortest = m_data[nxt].m_length;
                    if (!~shortest) shortest = 1;
                    size_type delta = fail == nxt ? cur_len - shortest : cur_len - m_data[fail].m_length;
                    if (len >= shortest) return (len - shortest) % delta == 0;
                    cur = nxt, cur_len = shortest;
                }
                return false;
            }
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (size_type i = 2; i != m_data.size(); i++) call(i);
            }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_data.size() - 1; is_node(i); i--) call(i);
            }
            template <typename Callback>
            void do_for_each_series(size_type i, Callback &&call) {
                size_type cur = m_node[i + 1], cur_len = m_data[cur].m_length;
                while (cur_len > 1) {
                    size_type fail = m_data[cur].m_fail, nxt = m_data[cur].m_ancestor, shortest = m_data[nxt].m_length;
                    if (!shortest) shortest = 1;
                    if (fail == nxt)
                        call(series{cur_len, shortest, cur_len - shortest});
                    else
                        call(series{cur_len, shortest, cur_len - m_data[fail].m_length});
                    cur = nxt, cur_len = shortest;
                }
            }
            template <typename Callback>
            void do_for_each_node(size_type init_node_index, Callback &&call) const {
                size_type node_index = init_node_index;
                do call(node_index);
                while (is_node(node_index = m_data[node_index].m_fail));
            }
        };
    }
    template <PAM::size_type ChildCount = 26>
    using StaticPAM_string = PAM::Automaton<PAM::StaticChildGetter<ChildCount>, std::string>;
}

#endif

using PAM = OY::StaticPAM_string<>;

constexpr int N = 1e6 + 10;

void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    vector<pair<int, int>> qu(q);
    for (int i = 0; i < q; i++) {
        cin >> qu[i].first >> qu[i].second;
        qu[i].first--;
        qu[i].second--;
    }
    vector<int> ans(q);
    auto work = [&]() -> void {
        string t = s + "#";
        reverse(s.begin(), s.end());
        t += s;
        reverse(s.begin(), s.end());
        auto lcp = OY::make_SA_LCP<N>(t);
        auto get = [&](int i, int j) -> int {
            if (i < 0 || j >= n) return 0;
            else return lcp.lcp(j, 2 * n - i);
        };
        PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });
        vector<int> p(n + 1);
        for (int i = 0; i < n; i++) p[i + 1] = max<int>(p[i], pam.get_node(pam.query_node_index(i))->m_length);
        for (int i = 0; i < q; i++) {
            auto [l, r] = qu[i];
            ans[i] = max(ans[i], p[l]);
            if (l > 0 && r < n - 1) {
                ans[i] = max(ans[i], 2 * get(l - 1, r + 1));
                pam.do_for_each_series(l - 1, [&](PAM::series w) -> void {
                    int d = w.m_delta;
                    int k = get(l - 1 - w.m_shortest, r + 1) / d;
                    int m = (w.m_longest - w.m_shortest) / d;
                    for (auto x : {m - k, m - k - 1, m - k + 1}) if (x >= 0 && x <= m) {
                        ans[i] = max<int>(ans[i], w.m_shortest + d * x + 2 * get(l - 1 - w.m_shortest - d * x, r + 1));
                    }
                });
                ans[i] = max(ans[i], 2 * get(l - 2, r + 1) + 1);
            }
        }
    };
    work();
    reverse(s.begin(), s.end());
    for (int i = 0; i < q; i++) {
        swap(qu[i].first, qu[i].second);
        qu[i].first = n - qu[i].first - 1;
        qu[i].second = n - qu[i].second - 1;
    }
    work();
    for (auto v : ans) cout << v << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();    
}