#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>


using namespace std;

/***************************1******************/
struct Person
{
    string surname;
    string name;
    optional<string> patronymic;
};


ostream& operator<<(ostream& os, const Person& p)
{
    os << p.surname << p.name;
    if (p.patronymic != nullopt) os << p.patronymic.value();
    return os;
}

bool operator<(const Person& p1, const Person& p2)
{
    return tie(p1.surname, p1.name, p1.patronymic) < tie(p2.surname, p2.name, p2.patronymic);
}

bool operator==(const Person& p1, const Person& p2)
{
    return tie(p1.surname, p1.name, p1.patronymic) == tie(p2.surname, p2.name, p2.patronymic);
}

/**********************2***********************/
struct PhoneNumber
{
    int country_code;
    int city_code;
    string number;
    optional<int> add_number;
};

ostream& operator<<(ostream& os, const PhoneNumber& p)
{
    os << '+' << p.country_code << '(' << p.city_code << ')' << p.number;
    if (p.add_number != nullopt) os << ' ' << p.add_number.value();
    return os;
}

bool operator<(const PhoneNumber& p1, const PhoneNumber& p2)
{
    return tie(p1.country_code, p1.city_code, p1.number, p1.add_number) < tie(p2.country_code, p2.city_code, p2.number, p2.add_number);
}
bool operator== (const PhoneNumber& p1, const PhoneNumber& p2)
{
    return tie(p1.country_code, p1.city_code, p1.number, p1.add_number) == tie(p2.country_code, p2.city_code, p2.number, p2.add_number);
}


/************************3*************************/


class PhoneBook
{
private:
    vector<pair<Person, PhoneNumber>> m_data;
public:
    PhoneBook(ifstream& file)
    {
        if (!file)
        {
            cout << "Не удаётся открыть файл!" << endl;
            exit(-1);
        }

        for (string line; getline(file, line);)
        {
            stringstream str(line);
            vector<string> rowData;

            for (string s; getline(str, s, ';');)
            {
                rowData.push_back(s);
            }

            pair<Person, PhoneNumber> entry;

            for (size_t i = 0; i < rowData.size(); ++i)
            {
                switch (i)
                {
                case 0:
                    entry.first.surname = rowData[i];
                    break;
                case 1:
                    entry.first.name = rowData[i];
                    break;
                case 2:
                    entry.first.patronymic = rowData[i] == "" ? (optional<string>)nullopt : rowData[i];
                    break;
                case 3:
                    entry.second.country_code = stoi(rowData[i]);
                    break;
                case 4:
                    entry.second.city_code = stoi(rowData[i]);
                    break;
                case 5:
                    entry.second.number = rowData[i];
                    break;
                case 6:
                    entry.second.add_number = rowData[i] == "" ? (optional<int>)nullopt : stoi(rowData[i]);
                    break;
                }
            }
            m_data.push_back(entry);
        }
    }

    friend ostream& operator<<(ostream& os, const PhoneBook& pb)
    {
        for (const auto& [first, second] : pb.m_data)
        {
            os << first << ' ' << second << endl;
        }

        return os;
    }

    void SortByName()
    {
        sort(m_data.begin(), m_data.end(), [](const pair<Person, PhoneNumber>& lhs, const pair<Person, PhoneNumber>& rhs)
            {
                return lhs.first < rhs.first;
            });
    }

    void SortByPhone()
    {
        sort(m_data.begin(), m_data.end(), [](const pair<Person, PhoneNumber>& lhs, const pair<Person, PhoneNumber>& rhs)
            {
                return lhs.second < rhs.second;
            });
    }

    pair<string, vector<PhoneNumber>> GetPhoneNumber(const string& surname)
    {
        vector<PhoneNumber> phoneNumbers;
        int count = 0;

        for_each(m_data.begin(), m_data.end(), [&](const auto& entry)
            {
                if (entry.first.surname == surname)
                {
                    phoneNumbers.push_back(entry.second);
                    ++count;
                }
            });

        switch (count)
        {
        case 0:
            return { "not found", phoneNumbers };
        case 1:
            return { "", phoneNumbers };
        default:
            return { "found more than 1", phoneNumbers };
        }
    }

    void ChangePhoneNumber(const Person& p, const PhoneNumber& pn)
    {
        auto entry = find_if(m_data.begin(), m_data.end(), [&](const auto& entry)
            {
                return entry.first == p;
            });

        if (entry != m_data.end())
        {
            entry->second = pn;
        }
    }
};

int main()
{
    ifstream file("PhoneBook.txt");
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого     человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname)
    {
        cout << surname << "\n";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
        {
            for (size_t i = 0; i < get<1>(answer).size(); ++i)
            {
                cout << get<1>(answer)[i];
            }
        }
        else
        {
            cout << get<0>(answer);
        }
        cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

    return 0;
}
