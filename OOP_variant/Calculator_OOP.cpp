#include "Calculator_OOP.h"

template <typename T>
Calculator<T>::Calculator(std::string expression)
{
    try
    {
        calculate_expression(expression);
        print_last_result();
    }
    catch (const char *str)
    {
        std::cout << str << std::endl;
    }
}

//вычисление выражений из файла и вывод результатов в другой файл
template <typename T>
std::vector<T> Calculator<T>::calculate_expr_in_file(const std::string &fname)
{
    auto exprCount = 1;
    const auto output_fname = "Result.txt"; //имя выходного файла
    std::string expression;
    std::ifstream fin;
    std::ofstream fout;

    fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try
    {
        fin.open(fname);
        fout.open(output_fname);

        if (fin.peek() == EOF)
            throw "File \"" + fname + "\" is empty";

        while (!fin.eof())
        {
            fin >> expression;
            auto result = calculate_expression(expression);
            fout << exprCount++ << ". " << expression << "=" << result << std::endl;
            results_vec.push_back(result);
        }
    }
    catch (const std::string &str)
    {
        fin.close();
        fout.close();
        throw str;
    }
    catch (const std::exception &ex)
    {
        fout.close();
        throw ex;
    }
    fin.close();
    fout.close();

    return results_vec;
}

//вычисление выражения
template <typename T>
T Calculator<T>::calculate_expression(const std::string &expression)
{
    check_expression(expression);
    std::stack<T> operands;
    std::stack<char> operations;

    for (auto i = 0; i < expression.size(); ++i)
        if (isdigit(expression[i]))
        {
            //!!!надо модифицировать при изменении типа вычисляемого выражения!!!
            operands.push(atof(&expression[i]));
            for (; i < expression.size() && (isdigit(expression[i + 1]) || expression[i + 1] == '.'); ++i)
                ;
        }
        else if (expression[i] == '(')
            operations.push(expression[i]);
        else if (expression[i] == ')')
        {
            while (operations.top() != '(')
                calculate_operation(operations, operands);
            operations.pop();
        }
        else
        {
            while (!operations.empty() && operation_priority(operations.top()) >= operation_priority(expression[i]))
                calculate_operation(operations, operands);
            operations.push(expression[i]);
        }

    while (!operations.empty())
        calculate_operation(operations, operands);

    result = operands.top();

    return result;
}

//проверка выражения на корректность
//(проверка в ходе вычесления выражения может привести к лишним вычислениям и неправильным сообщениям об ошибках)
template <typename T>
void Calculator<T>::check_expression(const std::string &expression) const
{
    auto operation_possible = false, open_bracket_possible = true,
         close_bracket_possible = false, number_possible = true,
         dot_possible = false;
    auto open_brackets_count = 0, close_brackets_count = 0;

    for (auto i = 0; i < expression.size(); ++i)
        if (expression[i] >= 48 && expression[i] <= 57)
        {
            if (number_possible)
            {
                number_possible = true;
                dot_possible = true;
                operation_possible = true;
                open_bracket_possible = false;
                close_bracket_possible = true;
            }
            else
                throw "That input is invalid (a number cannot follow immediately after a closing bracket)";
        }
        else if (expression[i] == '.')
        {
            if (dot_possible)
            {
                number_possible = true;
                dot_possible = false;
                operation_possible = false;
                open_bracket_possible = false;
                close_bracket_possible = false;
            }
            else
                throw "That input is invalid (a dote can follow only immediately after a number)";
        }
        //!!!надо модифицировать при добавлении новой операции!!!
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
            if (operation_possible)
            {
                number_possible = true;
                dot_possible = false;
                operation_possible = false;
                open_bracket_possible = true;
                close_bracket_possible = false;
            }
            else
                throw "That input is invalid (an operation cannot follow immediately after an open bracket or another operation)";
        else if (expression[i] == '(')
        {
            if (open_bracket_possible)
            {
                number_possible = true;
                dot_possible = false;
                operation_possible = false;
                open_bracket_possible = true;
                close_bracket_possible = false;
                ++open_brackets_count;
            }
            else
                throw "That input is invalid (an open bracket cannot follow immediately after a number)";
        }
        else if (expression[i] == ')')
        {
            if (close_bracket_possible)
            {
                number_possible = false;
                dot_possible = false;
                operation_possible = true;
                open_bracket_possible = true;
                close_bracket_possible = true;
                ++close_brackets_count;
            }
            else
                throw "That input is invalid (a close bracket cannot follow immediately after an operation or open bracket)";
        }
        else
            throw "That input is invalid (an unknown symbol)";

    if (open_brackets_count > close_brackets_count)
        throw "Missing a closing bracket";
    else if (open_brackets_count < close_brackets_count)
        throw "Missing an open bracket";
}

//получение приоритета операции
//!!!надо модифицировать при добавлении новой операции!!!
template <typename T>
int Calculator<T>::operation_priority(const char operation) const noexcept
{
    if (operation == '(' || operation == ')')
        return 0;
    else if (operation == '+' || operation == '-')
        return 1;
    else if (operation == '*' || operation == '/')
        return 2;
    else
        return -1;
}

//выполнение операции со стека операций над двумя операндами со стека операндов
template <typename T>
void Calculator<T>::calculate_operation(std::stack<char> &operations, std::stack<T> &operands) const
{
    T firstArg, secondArg;
    secondArg = operands.top();
    operands.pop();
    firstArg = operands.top();
    operands.pop();

    //вызов функции соответсвующей символу операции
    //!!!надо модифицировать при добавлении новой операции!!!
    switch (operations.top())
    {
    case '+':
        operands.push(add_operation(firstArg, secondArg));
        break;
    case '-':
        operands.push(subtract_operation(firstArg, secondArg));
        break;
    case '*':
        operands.push(multiply_operation(firstArg, secondArg));
        break;
    case '/':
        operands.push(divide_operation(firstArg, secondArg));
        break;
    }

    operations.pop();
}

template <typename T>
void Calculator<T>::print_last_result() const
{
    std::cout << "Calculation result:" << std::endl;
    std::cout << result << std::endl;
}

template <typename T>
void Calculator<T>::print_last_results_vec() const
{
    std::cout << "Calculation results:" << std::endl;
    for (const auto &element : results_vec)
        std::cout << element << std::endl;
}

//функции выполняющие соответсвующие операции
template <typename T>
T Calculator<T>::add_operation(T firstArg, T secondArg) const noexcept
{
    return firstArg + secondArg;
}

template <typename T>
T Calculator<T>::subtract_operation(T firstArg, T secondArgs) const noexcept
{
    return firstArg - secondArgs;
}

template <typename T>
T Calculator<T>::multiply_operation(T firstArg, T secondArgs) const noexcept
{
    return firstArg * secondArgs;
}

template <typename T>
T Calculator<T>::divide_operation(T firstArg, T secondArg) const
{
    if (secondArg == 0)
        throw "Division by zero";
    else
        return firstArg / secondArg;
}
