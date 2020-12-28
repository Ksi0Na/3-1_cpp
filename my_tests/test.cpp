#include <gtest/gtest.h>
#include <sstream>
#include "parser.hpp"

// Тесты для класса Parser
std::string getFullPath(const std::string& name) {
    std::string s(__FILE__);
    for (size_t i = 0; i < 8; ++i) {
        s.pop_back();
    }
    return s + name;
}
TEST(Parser, EmptyParser) {
    Parser a;
    ASSERT_TRUE(a.empty_json_obj());
    Parser b;
    b.set_json_str(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
    ASSERT_FALSE(b.empty_json_obj());
}
TEST(Parser, Throw) {
    EXPECT_THROW(Parser(""), std::invalid_argument);
    EXPECT_THROW(Parser("Wrong.json"), std::out_of_range);
    EXPECT_THROW(Parser().set_json_str(R"({
  "items":
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    }
}
)"),
                 std::invalid_argument);
    EXPECT_THROW(Parser().set_json_str(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 4
  }
})"),
                 std::invalid_argument);
}
TEST(Parser, LengthOfFields) {
    ASSERT_EQ(Parser().get_len().len_name, 15);
    ASSERT_EQ(Parser().get_len().len_group, 8);
    ASSERT_EQ(Parser().get_len().len_avg, 6);
    ASSERT_EQ(Parser().get_len().len_debt, 15);
    Parser b;
    b.set_json_str(R"({
  "items": [
    {
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++ Java Python C#"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31-2019-2025",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
    ASSERT_EQ(b.get_len().len_name, 22);  // 21+1
    ASSERT_EQ(b.get_len().len_group, 17);  // 16+1
    ASSERT_EQ(b.get_len().len_avg, 6);
    ASSERT_EQ(b.get_len().len_debt, 19);  // 18+1
}
TEST(Parser, NumberOfStudents) {
    ASSERT_EQ(Parser().get_students().size(), 0);
    Parser b;
    b.set_json_str(R"({
  "items": [
    {
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++ Java Python C#"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31-2019-2025",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
    ASSERT_EQ(b.get_students().size(), 3);  // 21+1
}
TEST(Parser, Separator) {
    std::string s = "|---------------|--------|------|---------------|";
    ASSERT_EQ(Parser().get_separator(), s);
}
TEST(Parser, PrintRow) {
    Parser p;
    Student s(json::parse(R"({
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })"));
    std::stringstream ss;
    p.print_row(ss, s);
    ASSERT_EQ(ss.str(), "|Ivanov Petr    |1       |4.25  |null           |");
    s.set_group(json::parse(R"({"group": 25})").at("group"));
    s.set_debt(json::parse(R"({"debt": ["C++"]})").at("debt"));
    ss.str(string());
    p.print_row(ss, s);
    ASSERT_EQ(ss.str(), "|Ivanov Petr    |25      |4.25  |1 items        |");
    s.set_debt(json::parse(R"({"debt": "C++"})").at("debt"));
    ss.str(string());
    p.print_row(ss, s);
    ASSERT_EQ(ss.str(), "|Ivanov Petr    |25      |4.25  |C++            |");
    s.set_group(json::parse(R"({"group": []})").at("group"));
    ss.str(string());
    ASSERT_THROW(p.print_row(ss, s), invalid_argument);
    s.set_debt(json::parse(R"({"debt": 25})").at("debt"));
    ss.str(string());
    ASSERT_THROW(p.print_row(ss, s), invalid_argument);
}
TEST(Parser, ConstructorThrow) {
    ASSERT_THROW(Parser(getFullPath("WrongCount.json")), std::out_of_range);
    ASSERT_THROW(Parser(getFullPath("NotArray.json")), invalid_argument);
}
TEST(Parser, Constructor) {
    Parser b(getFullPath("students.json"));
    ASSERT_EQ(b.get_len().len_name, 15);
    ASSERT_EQ(b.get_len().len_group, 10);  // 9+1
    ASSERT_EQ(b.get_len().len_avg, 6);
    ASSERT_EQ(b.get_len().len_debt, 15);
}
TEST(Parser, PrintData) {
    std::stringstream ss;
    Parser p(getFullPath("students.json"));
    ss << p;
    std::string correct(R"(|name           |group     |avg   |debt           |
|---------------|----------|------|---------------|
|Ivanov Petr    |1         |4.25  |null           |
|---------------|----------|------|---------------|
|Sidorov Ivan   |123456789 |4     |C++            |
|---------------|----------|------|---------------|
|Pertov Nikita  |IU8-31    |3.33  |3 items        |
|---------------|----------|------|---------------|
)");
    ASSERT_EQ(ss.str(), correct);
    std::cout << "\n\n" << p << "\n\n";
}
//Тесты для класса Student
TEST(Student, CorrectTypesOfFieldsNull) {
    Student s{json::parse(R"({
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })")};
    ASSERT_EQ(s.get_name(), "Ivanov Petr");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<string>(), "1");
    ASSERT_DOUBLE_EQ(s.get_avg(), 4.25);
    ASSERT_TRUE(std::any_cast<json>(s.get_debt()).is_null());
}
TEST(Student, CorrectTypesOfFieldsString) {
    Student s{json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })")};
    ASSERT_EQ(s.get_name(), "Sidorov Ivan");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<int>(), 31);
    ASSERT_DOUBLE_EQ(s.get_avg(), 4.00);
    ASSERT_TRUE(std::any_cast<json>(s.get_debt()).is_string());
}
TEST(Student, CorrectTypesOfFieldsArray) {
    Student s{json::parse(R"({
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    })")};
    ASSERT_EQ(s.get_name(), "Pertov Nikita");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<std::string>(), "IU8-31");
    ASSERT_DOUBLE_EQ(s.get_avg(), 3.33);
    ASSERT_TRUE(std::any_cast<json>(s.get_debt()).is_array());
}
TEST(Student, ThrowStudent) {
    ASSERT_THROW(Student{json::parse(R"({})")}, std::invalid_argument);
}
TEST(Student, From_json_methods) {
    Student s, m;
    s.from_json(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })"));
    ASSERT_EQ(s.get_name(), "Sidorov Ivan");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<int>(), 31);
    ASSERT_DOUBLE_EQ(s.get_avg(), 4.00);
    ASSERT_EQ(std::any_cast<json>(s.get_debt()).get<std::string>(), "C++");
    m.from_json(json::parse(R"({
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })"));
    ASSERT_EQ(m.get_name(), "Ivanov Petr Petrovich");
    ASSERT_EQ(std::any_cast<json>(m.get_group()).get<std::string>(), "1");
    ASSERT_DOUBLE_EQ(m.get_avg(), 4.25);
    ASSERT_TRUE(std::any_cast<json>(m.get_debt()).is_null());
}
TEST(Student, From_json_function) {
    Student s;
    from_json(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })"),
              s);
    ASSERT_EQ(s.get_name(), "Sidorov Ivan");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<int>(), 31);
    ASSERT_DOUBLE_EQ(s.get_avg(), 4.00);
    ASSERT_EQ(std::any_cast<json>(s.get_debt()).get<std::string>(), "C++");
    ASSERT_THROW(from_json(json::parse(R"({})"), s), std::invalid_argument);
}
TEST(Student, StudentSetMethods) {
    Student s;
    json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })");
    s.set_name(j.at("name"));
    s.set_group(j.at("group"));
    s.set_avg(j.at("avg"));
    s.set_debt(j.at("debt"));
    ASSERT_EQ(s.get_name(), "Sidorov Ivan");
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<int>(), 31);
    s.set_group(json::parse("24"));
    ASSERT_EQ(std::any_cast<json>(s.get_group()).get<int>(), 24);
    ASSERT_DOUBLE_EQ(s.get_avg(), 4.00);
    ASSERT_EQ(std::any_cast<json>(s.get_debt()).get<std::string>(), "C++");
}
TEST(Student, ThrowFromJsonMethod) {
    Student s;
    ASSERT_THROW(s.from_json(json::parse(R"({})")), std::invalid_argument);
    json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": "31",
      "avg": [],
      "debt": "C++"
    })");
    ASSERT_THROW(s.from_json(j), std::invalid_argument);
}
TEST(Student, ThrowSetMethod) {
    Student s;
    json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": [],
      "debt": "C++"
    })");

    ASSERT_THROW(s.set_avg("avg"), std::invalid_argument);
    ASSERT_THROW(s.set_avg(j.at("avg")), std::invalid_argument);
}
TEST(Student, ThrowConstructor) {
    ASSERT_THROW(Student(json::parse(R"({})")), std::invalid_argument);
    ASSERT_THROW(Student(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": [],
      "debt": "C++"
    })")),
                 std::invalid_argument);
}