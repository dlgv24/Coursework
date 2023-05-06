#pragma once

#include <fstream>
#include <locale>
#include <sstream>
#include <string>

#include "list.h"

typedef struct s_date {
    char day;
    char month;
    short year;
} t_date;

typedef struct s_subject {
    char name[31];
    char mark;
} t_subject;

typedef struct s_student {
    char surname[21];
    char name[21];
    char patronymic[21];
    char faculty[31];
    char department[31];
    char group[11];
    char record_book_number[8];
    short start_year;
    t_date date_of_birth;
    char sex;
    t_subject subjects[9][10];
} t_student;

class Student {
   private:
    t_student student;

    bool str_is_digit(std::string str) {
        for (int i = 0; i < str.length(); i++) {
            if (!std::isdigit(str[i])) return false;
        }
        return true;
    }

    bool str_is_alpha(std::string str) {
        for (int i = 0; i < str.length(); i++) {
            if (!std::isalpha(str[i], std::locale("rus_rus.1251"))) return false;
        }
        return true;
    }

   public:
    Student() {
        student.surname[0] = 0;
        student.name[0] = 0;
        student.patronymic[0] = 0;
        student.faculty[0] = 0;
        student.department[0] = 0;
        student.group[0] = 0;
        student.record_book_number[0] = 0;
        student.start_year = 2000;
        student.date_of_birth.day = 1;
        student.date_of_birth.month = 1;
        student.date_of_birth.year = 2000;
        student.sex = 0;
        for (char i = 0; i < 9; i++) {
            for (char j = 0; j < 10; j++) {
                student.subjects[i][j].name[0] = 0;
                student.subjects[i][j].mark = 0;
            }
        }
    }

    void set_student(t_student student) { this->student = student; }

    t_student get_student() { return student; }

    bool set_surname(std::string surname) {
        if (surname.length() > 20 || !str_is_alpha(surname)) return true;
        strcpy_s(student.surname, surname.c_str());
        return false;
    }

    std::string get_surname() { return std::string(student.surname); }

    bool set_name(std::string name) {
        if (name.length() > 20 || !str_is_alpha(name)) return true;
        strcpy_s(student.name, name.c_str());
        return false;
    }

    std::string get_name() { return std::string(student.name); }

    bool set_patromymic(std::string patronymic) {
        if (patronymic.length() > 20 || !str_is_alpha(patronymic)) return true;
        strcpy_s(student.patronymic, patronymic.c_str());
        return false;
    }

    std::string get_patronymic() { return std::string(student.patronymic); }

    bool set_faculty(std::string faculty) {
        if (faculty.length() > 30) return true;
        strcpy_s(student.faculty, faculty.c_str());
        return false;
    }

    std::string get_faculty() { return std::string(student.faculty); }

    bool set_department(std::string department) {
        if (department.length() > 30) return true;
        strcpy_s(student.department, department.c_str());
        return false;
    }

    std::string get_department() { return std::string(student.department); }

    bool set_group(std::string group) {
        if (group.length() != 10) return true;
        strcpy_s(student.group, group.c_str());
        return false;
    }

    std::string get_group() { return std::string(student.group); }

    bool set_record_book_number(std::string record_book_number, bool record_book_number_in_database) {
        if (record_book_number.length() != 7 || record_book_number_in_database) return true;
        strcpy_s(student.record_book_number, record_book_number.c_str());
        return false;
    }

    std::string get_record_book_number() { return std::string(student.record_book_number); }

    bool set_start_year(std::string start_year) {
        if (start_year.length() != 4) return true;
        if (!str_is_digit(start_year)) return true;
        int tmp = std::stoi(start_year);
        student.start_year = tmp;
        return false;
    }

    std::string get_start_year() { return std::to_string(student.start_year); }

    bool set_date_of_birth(std::string date_of_birth) {
        List<std::string> tmp_vector;
        std::stringstream tmp_stream(date_of_birth);
        std::string tmp_str;
        while (std::getline(tmp_stream, tmp_str, '.')) {
            tmp_vector.push_back(tmp_str);
        }

        if (tmp_vector.size() != 3) return true;
        if (tmp_vector[0].size() < 1 || tmp_vector[0].size() > 2 || !str_is_digit(tmp_vector[0]) ||
            tmp_vector[1].size() < 1 || tmp_vector[1].size() > 2 || !str_is_digit(tmp_vector[1]) ||
            tmp_vector[2].size() != 4 || !str_is_digit(tmp_vector[2]))
            return true;

        t_date tmp_date;
        tmp_date.day = std::stoi(tmp_vector[0]);
        tmp_date.month = std::stoi(tmp_vector[1]);
        tmp_date.year = std::stoi(tmp_vector[2]);

        if (tmp_date.month < 1 || tmp_date.month > 12) return true;
        int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (tmp_date.year % 400 == 0 || tmp_date.year % 4 == 0 && tmp_date.year % 100 != 0)
            days_in_month[1] = 29;
        if (tmp_date.day < 1 || tmp_date.day > days_in_month[tmp_date.month - 1]) return true;

        student.date_of_birth = tmp_date;
        return false;
    }

    std::string get_date_of_birth() {
        std::string date_of_birth = "";
        if (student.date_of_birth.day < 10) date_of_birth += "0";
        date_of_birth += std::to_string(student.date_of_birth.day) + ".";
        if (student.date_of_birth.month < 10) date_of_birth += "0";
        date_of_birth += std::to_string(student.date_of_birth.month) + ".";
        date_of_birth += std::to_string(student.date_of_birth.year);
        return date_of_birth;
    }

    void set_sex(char sex) { student.sex = sex; }

    std::string get_sex() {
        std::string sex[] = {"Мужчина", "Женщина"};
        return sex[student.sex];
    }

    bool set_subject_name(std::string name, short session_index, short subject_index) {
        if (name.length() > 30) return true;
        strcpy_s(student.subjects[session_index][subject_index].name, name.c_str());
        return false;
    }

    std::string get_subject_name(short session_index, short subject_index) {
        return std::string(student.subjects[session_index][subject_index].name);
    }

    void set_subject_mark(char mark, short session_index, short subject_index) {
        student.subjects[session_index][subject_index].mark = mark;
    }

    std::string get_subject_mark(short session_index, short subject_index) {
        std::string mark[] = {
            "Незачет", "Зачет", "Отлично", "Хорошо", "Удовлетворительно", "Неудовлетворительно"};
        return mark[student.subjects[session_index][subject_index].mark];
    }

    friend static bool compare_start_years(Student& a, Student& b);
};

bool compare_start_years(Student& a, Student& b) { return (a.student.start_year < b.student.start_year); }