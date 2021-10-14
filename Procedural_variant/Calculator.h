#pragma once

#include <string>
#include <vector>
#include <stack>

using expr_type = double; //тип выражения

using function = expr_type (*)(expr_type, expr_type); //указатель на функции операций

template <typename T>
T add_operation(T, T) noexcept;
template <typename T>
T subtract_operation(T, T) noexcept;
template <typename T>
T multiply_operation(T, T) noexcept;
template <typename T>
T divide_operation(T, T);

void print_menu() noexcept;

template <typename T>
std::vector<T> calculate_expr_in_file(const std::string &);

template <typename T>
T calculate_expression(const std::string &);

template <typename T>
void calculate_operation(std::stack<char> &, std::stack<T> &);

void check_expression(const std::string &);
int operation_priority(const char) noexcept;
function get_function(const char) noexcept;