// Copyright 2020 Olga Molchun olgamolchun5@gmail.com

#include "parser.hpp"

#include <algorithm>
using std::left;
using std::setw;
using std::any_cast;
using std::to_string;
using std::cout;
using std::out_of_range;
using std::ifstream;
using std::setprecision;

Parser::Parser() = default;
Parser::Parser(const string &path) { parser(path); }

const vector<Student> &Parser::get_students() const { return students; }
const len_fields      &Parser::get_len()      const { return l;        }

void Parser::print_row(ostream &out, const Student &student) const {
    out << left
        << "|"
        << setw(l.len_name)
        << student.get_name()
        << "|";


    if (any_cast<json>(student.get_group()).is_number())
        out << setw(l.len_group)
            << any_cast<json>(student.get_group()).get<int>();

    else if (any_cast<json>(student.get_group()).is_string())
        out << setw(l.len_group)
            << any_cast<json>(student.get_group()).get<string>();

    else
        throw invalid_argument("The type of the _group variable is undefined!!!");


    out << "|" << setprecision(3) << setw(l.len_avg)
        << student.get_avg() << "|";


    if (any_cast<json>(student.get_debt()).is_null())
        out << setw(l.len_debt) << "null";

    else if (any_cast<json>(student.get_debt()).is_array()) {
        string it = to_string(any_cast<json>(student.get_debt())
                                      .get<vector<string>>()
                .size()) + " items";
        out << setw(l.len_debt) << it;

    } else if (any_cast<json>(student.get_debt()).is_string())
        out << setw(l.len_debt)
            << any_cast<json>(student.get_debt()).get<string>();

    else
        throw invalid_argument("The type of the _debt variable is undefined!!!");


    out << "|";
}
string Parser::get_separator() const {
    string sep = "|";

    for (size_t i = 0; i < l.len_name;  ++i) sep += "-";
    sep += "|";
    for (size_t i = 0; i < l.len_group; ++i) sep += "-";
    sep += "|";
    for (size_t i = 0; i < l.len_avg;   ++i) sep += "-";
    sep += "|";
    for (size_t i = 0; i < l.len_debt;  ++i) sep += "-";
    sep += "|";
    return sep;
}

void Parser::parser(const string &path) {
    if (path.empty())
        throw invalid_argument("The file path cannot be empty!!!");

    ifstream json_file(path);

    if (!json_file.is_open())
        throw out_of_range("The file with the specified name: "
                           + path
                           + " does not exist!!!");

    json data;
    json_file >> data;

    if (!data.at("items").is_array())
        throw invalid_argument("Items is not array!!!");

    if (data.at("items").size() != data.at("_meta").at("count").get<size_t>())
        throw out_of_range("Items length don't equal _meta.count!!!");

    for (auto const &student : data.at("items"))
        students.emplace_back(student);

    set_len();
}

void Parser::print_data() { cout << *this; }

bool Parser::empty_json_obj() const { return students.empty(); }

void Parser::set_json_str(const string &JSON) {

    json data = json::parse(JSON);

    if (!data.at("items").is_array())
        throw std::invalid_argument("Items is not array!!!");

    if (data.at("items").size() != data.at("_meta").at("count").get<size_t>())
        throw std::invalid_argument("Items length don't equal _meta.count!!!");

    for (auto const &student : data.at("items"))
        students.emplace_back(student);

    set_len();
}
void Parser::set_len() {
    for (const auto &student : students) {

        if (student.get_name().size() > l.len_name)
            l.len_name = student.get_name().size() + 1;

        if (any_cast<json>(student.get_group()).is_number()) {
            if (to_string(any_cast<json>(student.get_group())
                                  .get<int>()).size() > l.len_group)
                l.len_group = to_string(any_cast<json>(student.get_group())
                                                .get<int>()).size() + 1;
        } else {
            if (any_cast<json>(student.get_group()).get<string>().size() > l.len_group)
                l.len_group = any_cast<json>(student.get_group())
                                      .get<string>().size() + 1;
        }

        if (   any_cast<json>(student.get_debt()).is_string()
               && any_cast<json>(student.get_debt()).get<string>().size()
                  >  l.len_debt)
            l.len_debt = any_cast<json>(student.get_debt())
                                 .get< string>().size() + 1;
    }
}
ostream &operator<<(ostream &out, Parser &p) {
    out << left
        << "|" << setw(p.l.len_name)  << "name"
        << "|" << setw(p.l.len_group) << "group"
        << "|" << setw(p.l.len_avg)   << "avg"
        << "|" << setw(p.l.len_debt)  << "debt"
        << "|" << '\n';

    string separator = p.get_separator();
    out << separator << "\n";

    for (const auto &student : p.students) {
        p.print_row(out, student);
        out << '\n';
        out << separator << "\n";
    }
    return out;
}

Parser::~Parser() = default;