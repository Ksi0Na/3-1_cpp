// Copyright 2020 Olga Molchun olgamolchun5@gmail.com

#ifndef INCLUDE_STUDENT_HPP_
#define INCLUDE_STUDENT_HPP_

#include <any>
#include <exception>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using nlohmann::json;
using std::string;
using std::vector;
using std::ostream;
using std::any;
using std::stod;
using std::invalid_argument;


class Student {
public:
    explicit Student(const json &obj);
    Student();

    [[nodiscard]] const string &get_name() const;
    [[nodiscard]] const any &get_group() const;
    [[nodiscard]] double get_avg() const;
    [[nodiscard]] const any &get_debt() const;

    void set_name(const json &name);
    void set_group(const json &group);
    void set_avg(const json &avg);
    void set_debt(const json &debt);

    void from_json(const json &j);

private:
    string _name;
    any    _group;
    double _avg = 0;
    any    _debt;
};

void from_json(const json &j, Student &s);

#endif  // INCLUDE_STUDENT_HPP_