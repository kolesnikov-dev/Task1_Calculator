#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

template<typename T>
class Calculator
{
public:
    Calculator() = default;
    Calculator(std::string);

    T add_operation(T, T) const noexcept;
    T subtract_operation(T, T) const noexcept;
    T multiply_operation(T, T) const noexcept;
    T divide_operation(T, T) const;

    std::vector<T> calculate_expr_in_file(const std::string &);
    T calculate_expression(const std::string &);
    void print_last_result() const;
    void print_last_results_vec() const;

private:
    void check_expression(const std::string &) const;
    int operation_priority(const char) const noexcept;
    void calculate_operation(std::stack<char> &, std::stack<T> &) const;

    T result{};
    std::vector<T> results_vec;
};

#include "Calculator_OOP.cpp"