#include <fstream>
#include <iostream>
#include <string>

#include "algorithm.h"
#include "rational.h"

int main() {
    const std::string prefix = "./data/";
    std::vector<std::string> suffix = {"1.in", "2.in", "3.in"};
    for (const std::string &s : suffix) {
        std::string file_name = prefix + s;
        std::ifstream fin(file_name);
        if (!fin.is_open()) {
            std::cout << file_name << " does not exist!" << std::endl;
            return 0;
        }
        int m, n;
        fin >> m >> n;
        matrix<number> A(m, row<number>(n));
        col<number> b(m);
        row<number> coef(n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                fin >> A[i][j];
            }
        }
        for (int i = 0; i < m; i++) {
            fin >> b[i];
        }
        for (int i = 0; i < n; i++) {
            fin >> coef[i];
        }
        fin.close();
        std::cout << "coefficient: " << std::endl;
        print(coef);
        std::cout << "\nA and b: " << std::endl;
        print(A, b);
        std::cout << std::endl;
        {
            auto [s, kv, z] = naive(A, b, coef);
            std::cout << get_name(s) << std::endl;
            if (s != NOT_FEASIBLE) {
                for (const auto &[k, v] : kv) {
                    std::cout << "x_" << k << " = " << v << std::endl;
                }
                std::cout << "z = " << z << std::endl;
            }
            std::cout << std::endl;
        }
        {
            auto [s, kv, z] = simplex(A, b, coef);
            std::cout << get_name(s) << std::endl;
            if (s != NOT_FEASIBLE) {
                for (const auto &[k, v] : kv) {
                    std::cout << "x_" << k << " = " << v << std::endl;
                }
                std::cout << "z = " << z << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::string(30, '=') << std::endl;
    }
    return 0;
}
