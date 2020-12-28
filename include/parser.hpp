// Copyright 2020 Olga Molchun olgamolchun5@gmail.com

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include "student.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct len_fields {
    size_t len_name;
    size_t len_group;
    size_t len_avg;
    size_t len_debt;
};
class Parser {
public:
    Parser();
    explicit Parser(const string &path);

    void parser(const string &path);

    [[nodiscard]] const vector<Student> &get_students() const;
    [[nodiscard]] const len_fields &get_len() const;

    [[nodiscard]] string get_separator() const;
    [[nodiscard]] bool empty_json_obj() const;

    void set_json_str(const string &JSON);

    void print_row(ostream& out, const Student &student) const;
    void print_data();
    friend ostream& operator <<(ostream& out, Parser& p);

    ~Parser();
private:
    std::vector<Student> students;
    len_fields l{15, 8, 6, 15};
    void set_len();
};

#endif  // INCLUDE_PARSER_HPP_