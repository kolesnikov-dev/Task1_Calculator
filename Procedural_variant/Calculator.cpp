#include "Calculator.h"

#include <iostream>
#include <fstream>

using namespace std;

//реализация меню программы
int main()
{
    int choice;

    print_menu();
    do
    {
        cout << endl
             << "Please, choose menu item (input its number):" << endl;
        cout << ">";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            string expression;

            cout << "Input an mathematical expression (without spaces) to calculate (allowed symbols: + - * / ( )):" << endl;
            cout << ">";
            cin >> expression;
            try
            {
                auto result = calculate_expression<expr_type>(expression);
                cout << "Calculation result:" << endl;
                cout << result << endl;
            }
            catch (const string &str)
            {
                cout << str << endl;
            }
            break;
        }
        case 2:
        {
            string fname;

            cout << "Input a filename:" << endl;
            cout << ">";
            cin >> fname;
            try
            {
                auto results = calculate_expr_in_file<expr_type>(fname);
                cout << "Calculation results:" << endl;
                for (const auto &element : results)
                    cout << element << endl;
            }
            catch (const string &str)
            {
                cout << str << endl;
            }
            catch (const exception &ex)
            {
                cout << ex.what() << endl;
            }
            break;
        }
        case 3:
            break;
        default:
            cout << "That input is invalid" << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(32767, '\n');
            }
            else
                cin.ignore(32767, '\n');
            break;
        }
    } while (choice != 3);

    cout << "Program finished" << endl;

    return 0;
}

void print_menu() noexcept
{
    cout << "\tMENU" << endl;
    cout << "1. Calculate expression entered into the console" << endl;
    cout << "2. Calculate expression(s) entered into the file" << endl;
    cout << "3. Exit" << endl
         << endl;
}

//вычисление выражений из файла и вывод результатов в другой файл
template<typename T>
vector<T> calculate_expr_in_file(const string &fname)
{
    auto exprCount = 1;
    const auto output_fname = "Result.txt"; //имя выходного файла
    string expression;
    ifstream fin;
    ofstream fout;
    vector<T> results_vec;

    fin.exceptions(ifstream::badbit | ifstream::failbit);
    try
    {
        fin.open(fname);
        fout.open(output_fname);

        if (fin.peek() == EOF)
            throw "File \"" + fname + "\" is empty";

        while (!fin.eof())
        {
            fin >> expression;
            auto result = calculate_expression<expr_type>(expression);
            fout << exprCount++ << ". " << expression << "=" << result << endl;
            results_vec.push_back(result);
        }
    }
    catch (const string &str)
    {
        fin.close();
        fout.close();
        throw str;
    }
    catch (const exception &ex)
    {
        fout.close();
        throw ex;
    }
    fin.close();
    fout.close();

    return results_vec;
}

//вычисление выражения
template<typename T>
T calculate_expression(const string &expression)
{
    check_expression(expression);
    stack<T> operands;
    stack<char> operations;

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
                calculate_operation<expr_type>(operations, operands);
            operations.pop();
        }
        else
        {
            while (!operations.empty() && operation_priority(operations.top()) >= operation_priority(expression[i]))
                calculate_operation<expr_type>(operations, operands);
            operations.push(expression[i]);
        }

    while (!operations.empty())
        calculate_operation<expr_type>(operations, operands);

    return operands.top();
}

//проверка выражения на корректность
//(проверка в ходе вычесления выражения может привести к лишним вычислениям и неправильным сообщениям об ошибках)
void check_expression(const string &expression)
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
int operation_priority(const char operation) noexcept
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

//получение указателя на функцию соответсвующую символу операции
//!!!надо модифицировать при добавлении новой операции!!!
function get_function(const char operation) noexcept
{
    switch (operation)
    {
    case '+':
        return add_operation;
    case '-':
        return subtract_operation;
    case '*':
        return multiply_operation;
    case '/':
        return divide_operation;
    default:
        return NULL;
    }
}

//выполнение операции со стека операций над двумя операндами со стека операндов
template<typename T>
void calculate_operation(stack<char> &operations, stack<T> &operands)
{
    T firstArg, secondArg;
    secondArg = operands.top();
    operands.pop();
    firstArg = operands.top();
    operands.pop();
    operands.push(get_function(operations.top())(firstArg, secondArg));
    operations.pop();
}

//функции выполняющие соответсвующие операции
template <typename T>
T add_operation(T firstArg, T secondArg) noexcept
{
    return firstArg + secondArg;
}

template <typename T>
T subtract_operation(T firstArg, T secondArg) noexcept
{
    return firstArg - secondArg;
}

template <typename T>
T multiply_operation(T firstArg, T secondArg) noexcept
{
    return firstArg * secondArg;
}

template <typename T>
T divide_operation(T firstArg, T secondArg)
{
    if (secondArg == 0)
        throw "Division by zero";
    else
        return firstArg / secondArg;
}