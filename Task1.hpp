#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>

struct Person
{
    std::string firstname;
    std::string lastname;
    std::optional<std::string> patronymic;
};

struct PhoneNumber
{
    int country_code;
    int city_code;
    std::string number;
    std::optional<int> additional_number;
};

class PhoneBook
{
private:
    std::vector<std::pair<Person, PhoneNumber>> m_data;
public:
    PhoneBook(std::istream &file);
    friend std::ostream& operator<<(std::ostream &out, const PhoneBook &pb);
    void SortByName();
    void SortByPhone();
    std::pair<std::string, std::vector<PhoneNumber>> GetPhoneNumber(const std::string &firstname);
    void ChangePhoneNumber(const Person &p, const PhoneNumber &pn);
};

std::optional<std::string> getOptStr(std::string &s);
std::ostream& operator<<(std::ostream &out, const Person &p);
bool operator<(const Person &p1, const Person &p2);
bool operator==(const Person &p1, const Person &p2);

bool operator<(const PhoneNumber &p1, const PhoneNumber &p2);
std::optional<int> getOptInt(std::string &s);
std::ostream& operator<<(std::ostream &out, const PhoneNumber &p);
