#pragma once

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <iostream>
#include <optional>
#include <utility>

#include "rational.h"

enum answer {
    OPTIMAL,
    INFINITE_OPTIMAL,
    NOT_FEASIBLE
};

std::string get_name(answer x) {
    static std::unordered_map<answer, std::string> map{
        {answer::OPTIMAL, "Optimal"},
        {answer::INFINITE_OPTIMAL, "Infinite Optimal"},
        {answer::NOT_FEASIBLE, "Not Feasible"}};
    return map.find(x)->second;
}

using kv_type = std::pair<size_t, number>;

using answer_type = std::tuple<answer, row<kv_type>, number>;

void adjust(matrix<number> &A, col<number> &b, size_t row_index, size_t col_index) {
    size_t m = A.size(), n = A[0].size();
    number out_ratio = A[row_index][col_index];
    for (size_t i = 0; i < n; i++) {
        A[row_index][i] /= out_ratio;
    }
    b[row_index] /= out_ratio;
    for (size_t i = 0; i < m; i++) {
        if (i == row_index || A[i][col_index] == 0) {
            continue;
        }
        number ratio = A[i][col_index];
        for (size_t j = 0; j < n; j++) {
            A[i][j] -= (A[row_index][j] * ratio);
        }
        b[i] -= (b[row_index] * ratio);
    }
}

bool gaussian_elimination(matrix<number> &A, col<number> &b) {
    size_t m = A.size();
    for (size_t k = 0; k < m; k++) {
        for (size_t index = k + 1; index < m && A[k][k] == 0; index++) {
            std::swap(A[k], A[index]);
            std::swap(b[k], b[index]);
        }
        if (A[k][k] == 0) {
            continue;
        }
        adjust(A, b, k, k);
    }
    for (size_t i = m - 1; i >= 1; i--) {
        if (b[i] == 0) {
            continue;
        }
        number ratio = b[i];
        for (size_t j = i - 1; j >= 0; j--) {
            b[j] -= (ratio * A[j][i]);
            A[j][i] = 0;
            if (j == 0) {
                break;
            }
        }
    }
    for (size_t i = 0; i < m; i++) {
        bool flag = true;
        for (size_t j = 0; j < m; j++) {
            if (A[i][j] != 0) {
                flag = false;
                break;
            }
        }
        if (flag && b[i] != 0) {
            return false;
        }
    }
    return true;
}

std::optional<answer_type> dfs(const matrix<number> &A, const col<number> &b, const row<number> &coef,
                               int index, matrix<number> &temp, row<size_t> &path) {
    size_t m = A.size(), n = A[0].size();
    if (index > int(n) || int(temp[0].size() + n) < index + int(m)) {
        return std::nullopt;
    }
    if (temp[0].size() == m) {
        matrix<number> copy{temp};
        col<number> solution{b};
        answer res = gaussian_elimination(copy, solution) &&
                             std::all_of(solution.begin(), solution.end(),
                                         [&](const number &n) -> bool { return n >= 0; })
                         ? answer::OPTIMAL
                         : answer::NOT_FEASIBLE;
        number cost = 0;
        std::vector<std::pair<size_t, number>> kv;
        if (res != answer::NOT_FEASIBLE) {
            for (size_t i = 0; i < m; i++) {
                cost += (solution[i] * coef[path[i] - 1]);
                kv.emplace_back(path[i], solution[i]);
            }
        }
        return std::optional<answer_type>(std::in_place, res, kv, cost);
    }
    std::optional<answer_type> res = std::nullopt;
    for (size_t i = index + 1; i < n; i++) {
        for (size_t k = 0; k < temp.size(); k++) {
            temp[k].push_back(A[k][i]);
        }
        path.push_back(i + 1);
        auto cur = dfs(A, b, coef, i, temp, path);
        if (cur.has_value()) {
            if (res == std::nullopt) {
                res = cur;
            } else if (std::get<0>(cur.value()) != answer::NOT_FEASIBLE &&
                       (std::get<0>(res.value()) == answer::NOT_FEASIBLE ||
                        std::get<2>(cur.value()) > std::get<2>(res.value()))) {
                res = std::move(cur);
            }
        }
        path.pop_back();
        for (size_t k = 0; k < temp.size(); k++) {
            temp[k].pop_back();
        }
    }
    return res;
}

answer_type naive(const matrix<number> &A, const col<number> &b, const row<number> &coef) {
    row<size_t> index_path;
    matrix<number> temp(A.size());
    auto res = dfs(A, b, coef, -1, temp, index_path);
    return res.has_value() ? res.value() : answer_type{NOT_FEASIBLE, row<kv_type>{}, std::numeric_limits<number::value_type>::min()};
}

answer_type simplex(matrix<number> a, col<number> b, const row<number> &c) {
    size_t m = a.size(), n = c.size();
    row<number> sigma(n);
    row<size_t> xj;
    number z;
    for (size_t i = n - m; i < n; i++) {
        xj.push_back(i);
    }
    answer state = NOT_FEASIBLE;
    do {
        z = 0;
        for (size_t i = 0; i < m; i++) {
            z += (c[xj[i]] * b[i]);
        }
        bool all_le_0 = true, zero = false, should_break = false;
        for (size_t j = 0; j < n; j++) {
            sigma[j] = c[j];
            bool in_base = false;
            for (size_t i = 0; i < m; i++) {
                sigma[j] -= (c[xj[i]] * a[i][j]);
                if (xj[i] == j) {
                    in_base = true;
                }
            }
            if (in_base) {
                continue;
            }
            if (sigma[j] > 0) {
                all_le_0 = false;
                bool flag = false;
                for (size_t i = 0; i < m; i++) {
                    if (a[i][j] > 0) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    should_break = true;
                    break;
                }
            } else if (sigma[j] == 0) {
                zero = true;
            }
        }
        if (should_break) {
            break;
        } else if (all_le_0) {
            state = zero ? INFINITE_OPTIMAL : OPTIMAL;
            break;
        }
        size_t index = std::max_element(std::cbegin(sigma), std::cend(sigma)) - std::cbegin(sigma);
        bool feasible = false;
        size_t row_to_choose = 0;
        number minimum;
        for (size_t i = 0; i < m; i++) {
            if (a[i][index] == 0) {
                continue;
            }
            number result = b[i] / a[i][index];
            if (result >= 0) {
                if (!feasible) {
                    feasible = true;
                    minimum = result;
                    row_to_choose = i;
                } else {
                    if (minimum > result) {
                        minimum = result;
                        row_to_choose = i;
                    } else if (minimum == result) {
                        if (rand() & 1) {
                            row_to_choose = i;
                        }
                    }
                }
            }
        }
        xj[row_to_choose] = index;
        adjust(a, b, row_to_choose, index);
    } while (true);
    row<std::pair<size_t, number>> kv;
    for (size_t i = 0; i < m; i++) {
        kv.emplace_back(xj[i] + 1, b[i]);
    }
    return std::make_tuple(state, kv, z);
}

#endif  // ALGORITHM_H_
