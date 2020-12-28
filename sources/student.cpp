// Copyright 2020 Olga Molchun olgamolchun5@gmail.com

#include "student.hpp"

Student::Student(const json &obj) {
    if (obj.empty()) throw invalid_argument("Object for Student cannot be empty");

    _name = obj.at("name").get<string>();
    _group = any{obj.at("group")};
    _debt= any{obj.at("debt")};

    if      (obj.at("avg").is_string()) _avg = stod(obj.at("avg").get<string>());
    else if (obj.at("avg").is_number()) _avg = obj.at("avg").get<double>();
    else throw invalid_argument("The type of the _avg variable is undefined!!!");
}

Student::Student() = default;

const string &Student::get_name()  const { return _name;  }
const any    &Student::get_group() const { return _group; }
double        Student::get_avg()   const { return _avg;   }
const any    &Student::get_debt()  const { return _debt;  }

void Student::set_name( const json &name ) { _name  = name.get<string>(); }
void Student::set_group(const json &group) { _group = any{group};         }
void Student::set_debt( const json &debt ) { _debt  = any{debt};          }
void Student::set_avg(  const json &avg  ) {
    if      (avg.is_string()) _avg = stod(avg.get<string>());
    else if (avg.is_number()) _avg = avg.get<double>();
    else throw invalid_argument("The type of the _avg variable is undefined!!!");
}

void Student::from_json(const json &obj) {
    if (obj.empty()) throw invalid_argument("Object for Student cannot be empty");

    _name = obj.at("name").get<string>();
    _group = any{obj.at("group")};
    _debt = any{obj.at("debt")};

    if      (obj.at("avg").is_string()) _avg = stod(obj.at("avg").get<string>());
    else if (obj.at("avg").is_number()) _avg = obj.at("avg").get<double>();
    else throw invalid_argument("The type of the _avg variable is undefined!!!");
}


void from_json(const json &j, Student &s) {
    if (j.empty()) throw invalid_argument("Object for Student cannot be empty");

    s.set_name(j.at("name"));
    s.set_group(j.at("group"));
    s.set_avg(j.at("avg"));
    s.set_debt(j.at("debt"));
}