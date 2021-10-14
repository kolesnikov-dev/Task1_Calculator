#include "Calculator_OOP.h"

using namespace std;
using expr_type = double; //тип выражения

void print_menu();

//реализация меню программы
int main()
{
    Calculator<expr_type> calc;
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
                calc.calculate_expression(expression);
                calc.print_last_result();
            }
            catch (const char *str)
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
                calc.calculate_expr_in_file(fname);
                calc.print_last_results_vec();
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

void print_menu()
{
    cout << "\tMENU" << endl;
    cout << "1. Calculate expression entered into the console" << endl;
    cout << "2. Calculate expression(s) entered into the file" << endl;
    cout << "3. Exit" << endl
         << endl;
}