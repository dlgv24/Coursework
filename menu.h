#pragma once

#define XCONST 2
#define YCONST 1
#define UP 72
#define DOWN 80
#define ENTER 13

#include <conio.h>
#include <windows.h>

#include <iostream>
#include <string>

#include "database.h"
#include "list.h"
#include "student.h"

class Menu : private Database {
   private:
    HANDLE handle;

    void set_cursor_visible(bool show) {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = show;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }

    void set_cursor_position(short x, short y) { SetConsoleCursorPosition(handle, {x, y}); }

    void focus_text(bool focus) {
        WORD color = focus ? 13 : 15;
        SetConsoleTextAttribute(handle, color);
    }

    short menu_box(List<std::string> lines) {
        system("cls");
        short selected_index = 0;
        char ch;
        bool exit = false;
        while (!exit) {
            short x = XCONST, y = YCONST;
            for (short i = 0; i < lines.size(); i++) {
                set_cursor_position(x, y++);
                focus_text(i == selected_index);
                std::cout << lines[i];
            }
            focus_text(false);
            ch = _getch();
            switch (ch) {
                case UP:
                    selected_index = (selected_index + lines.size() - 1) % lines.size();
                    break;
                case DOWN:
                    selected_index = (selected_index + 1) % lines.size();
                    break;
                case ENTER:
                    exit = true;
            }
            system("cls");
        }
        return selected_index;
    }

    std::string prompt_box(List<std::string> lines) {
        system("cls");
        short x = XCONST, y = YCONST;
        for (short i = 0; i < lines.size(); i++) {
            set_cursor_position(x, y++);
            std::cout << lines[i];
        }
        set_cursor_position(x, y);
        focus_text(true);
        set_cursor_visible(true);
        std::string text;
        std::getline(std::cin, text);
        set_cursor_visible(false);
        focus_text(false);
        system("cls");
        return text;
    }

    void alert_box(List<std::string> lines) {
        system("cls");
        short x = XCONST, y = YCONST;
        for (short i = 0; i < lines.size(); i++) {
            set_cursor_position(x, y++);
            std::cout << lines[i];
        }
        set_cursor_position(x, y);
        focus_text(true);
        std::cout << "ОК";
        char ch = _getch();
        while (ch != ENTER) ch = _getch();
        focus_text(false);
        system("cls");
    }

    bool confirm_box(List<std::string> lines) {
        system("cls");
        short selected_index = 0;
        char ch;
        bool exit = false;
        while (!exit) {
            short x = XCONST, y = YCONST;
            for (short i = 0; i < lines.size(); i++) {
                set_cursor_position(x, y++);
                std::cout << lines[i];
            }
            set_cursor_position(x, y++);
            focus_text(0 == selected_index);
            std::cout << "ОК";
            set_cursor_position(x, y);
            focus_text(1 == selected_index);
            std::cout << "Отмена";
            focus_text(false);
            ch = _getch();
            switch (ch) {
                case UP:
                    selected_index = (selected_index - 1) % 2;
                    break;
                case DOWN:
                    selected_index = (selected_index + 1) % 2;
                    break;
                case ENTER:
                    exit = true;
            }
            system("cls");
        }
        return !selected_index;
    }

    bool file_menu() {
        short selected_index;
        bool check, exit = false;
        std::string name;
        while (!exit) {
            selected_index = menu_box(
                {"Создать базу данных", "Выбрать базу данных", "Выбрать зашифрованную базу данных", "Выход"});
            if (selected_index == 0) {
                name = prompt_box({"Введите название новой БД", "(только цифры и английские буквы)"});

                check = check_database_name(name);
                if (check) {
                    check = check_database_file(name + ".bin");
                    if (check) {
                        alert_box({"Такая БД существует"});
                    } else {
                        check = create_database_file(name + ".bin");
                        if (check) {
                            alert_box({"БД создана!"});
                            set_path(name + ".bin");
                            break;
                        } else {
                            alert_box({"Ошибка при создании БД!"});
                        }
                    }
                } else {
                    alert_box({"Неправильное имя БД!"});
                }

            } else if (selected_index == 1) {
                name = prompt_box({"Введите название существующей БД", "(только цифры и английские буквы)"});
                check = check_database_name(name);
                if (check) {
                    check = check_database_file(name + ".bin");
                    if (check) {
                        alert_box({"БД выбрана!"});
                        set_path(name + ".bin");
                        break;
                    } else
                        alert_box({"БД не существует или существует в зашифрованном виде!"});
                } else {
                    alert_box({"Неправильное имя БД!"});
                }
            } else if (selected_index == 2) {
                name = prompt_box({"Введите название существующей БД", "(только цифры и английские буквы)"});
                check = check_database_name(name);
                if (check) {
                    check = check_database_file(name + ".bin.enc");
                    if (check) {
                        std::string passwd = prompt_box({
                            "Введите пароль от БД",
                        });
                        check = decrypt_file(std::string(name + ".bin.enc").c_str(),
                                             std::string(name + ".bin").c_str(), passwd.c_str());
                        if (check) {
                            alert_box({"БД выбрана!"});
                            set_path(name + ".bin");
                            break;
                        } else
                            alert_box({"Вы ввели неправильный пароль!"});
                    } else
                        alert_box({"БД не существует или существует в незашифрованном виде!"});
                } else
                    alert_box({"Неправильное имя БД!"});
            } else if (selected_index == 3) {
                exit = confirm_box({"Подтвердите выход."});
            }
        }
        return exit;
    }

    void change_sessions(Student& record) {
        List<std::string> lines = {"Выход"};
        for (int i = 0; i < 9; i++) {
            lines.push_back("Сессия " + std::to_string(i + 1));
        }
        short selected_index;
        bool exit = false;
        while (!exit) {
            selected_index = menu_box(lines);
            if (selected_index == 0) {
                exit = true;
            } else {
                change_exams(record, selected_index - 1);
            }
        }
    }

    void change_exams(Student& record, short session_index) {
        List<std::string> lines;
        short selected_index, subject_index;
        bool exit = false;
        std::string tmp_str;
        while (!exit) {
            lines.push_back("Выход");
            for (char i = 0; i < 10; i++) {
                lines.push_back("Предмет [" + record.get_subject_name(session_index, i) + "] Оценка [" +
                                record.get_subject_mark(session_index, i) + "]");
            }
            selected_index = menu_box(lines);
            if (selected_index == 0) {
                exit = true;
            } else {
                subject_index = selected_index - 1;
                selected_index =
                    menu_box({"Выход", "Изменить название предмета", "Изменить оценку за предмет"});
                if (selected_index == 1) {
                    tmp_str = prompt_box({"Введите название предмета:"});
                    if (record.set_subject_name(tmp_str, session_index, subject_index))
                        alert_box({"Вы неправильно ввели название предмета!"});
                    else
                        alert_box({"Название предмета изменено."});
                } else if (selected_index == 2) {
                    selected_index = menu_box({"Незачет", "Зачет", "Отлично", "Хорошо", "Удовлетворительно",
                                               "Неудовлетворительно"});
                    record.set_subject_mark(selected_index, session_index, subject_index);
                }
            }
            lines.clear();
        }
    }

    bool change_student_record(Student& record) {
        Student copied_record = record;
        std::string tmp_str;
        short selected_index;
        bool exit = false;
        while (!exit) {
            selected_index =
                menu_box({"Сохранить", "Выход без сохранения", "Редактирование списка сессий",
                          "Изменить фамилию [" + copied_record.get_surname() + "]",
                          "Изменить имя [" + copied_record.get_name() + "]",
                          "Изменить отчество [" + copied_record.get_patronymic() + "]",
                          "Изменить факультет [" + copied_record.get_faculty() + "]",
                          "Изменить кафедру [" + copied_record.get_department() + "]",
                          "Изменить группу [" + copied_record.get_group() + "]",
                          "Изменить номер зачетной книжки [" + copied_record.get_record_book_number() + "]",
                          "Изменить год поступления [" + copied_record.get_start_year() + "]",
                          "Изменить дату рождения [" + copied_record.get_date_of_birth() + "]",
                          "Изменить пол [" + copied_record.get_sex() + "]"});

            if (selected_index == 0) {
                if (confirm_box({"Сохранить запись?"})) {
                    if (copied_record.get_surname() == "" || copied_record.get_name() == "" ||
                        copied_record.get_patronymic() == "" || copied_record.get_faculty() == "" ||
                        copied_record.get_department() == "" || copied_record.get_group() == "" ||
                        copied_record.get_record_book_number() == "")
                        alert_box({"Ошибка!", "Введены не все данные о студенте."});
                    else {
                        record = copied_record;
                        alert_box({"Запись сохранена."});
                        exit = true;
                    }
                }
            } else if (selected_index == 1) {
                exit = confirm_box({"Подтвердите выход."});
            } else if (selected_index == 2) {
                change_sessions(copied_record);
            } else if (selected_index == 3) {
                tmp_str = prompt_box({"Введите фамилию:"});
                if (copied_record.set_surname(tmp_str))
                    alert_box({"Вы неправильно ввели фамилию!"});
                else
                    alert_box({"Фамилия изменена."});
            } else if (selected_index == 4) {
                tmp_str = prompt_box({"Введите имя:"});
                if (copied_record.set_name(tmp_str))
                    alert_box({"Вы неправильно ввели имя!"});
                else
                    alert_box({"Имя изменено."});
            } else if (selected_index == 5) {
                tmp_str = prompt_box({"Введите отчество:"});
                if (copied_record.set_patromymic(tmp_str))
                    alert_box({"Вы неправильно ввели отчество!"});
                else
                    alert_box({"Отчество изменено."});
            } else if (selected_index == 6) {
                tmp_str = prompt_box({"Введите название факультета:"});
                if (copied_record.set_faculty(tmp_str))
                    alert_box({"Вы неправильно ввели название факультета!"});
                else
                    alert_box({"Название факультета изменено."});
            } else if (selected_index == 7) {
                tmp_str = prompt_box({"Введите название кафедры:"});
                if (copied_record.set_department(tmp_str))
                    alert_box({"Вы неправильно название кафедры!"});
                else
                    alert_box({"Название кафедры изменено."});
            } else if (selected_index == 8) {
                tmp_str = prompt_box({"Введите название группы:"});
                if (copied_record.set_group(tmp_str))
                    alert_box({"Вы неправильно ввели название группы!"});
                else
                    alert_box({"Название группы изменено."});
            } else if (selected_index == 9) {
                tmp_str = prompt_box({"Введите номер зачетной книжки:"});
                if (copied_record.set_record_book_number(tmp_str, record_book_number_in_database(tmp_str)))
                    alert_box({"Вы неправильно ввели номер зачетной книжки!"});
                else
                    alert_box({"Номер зачетной книжки изменен."});
            } else if (selected_index == 10) {
                tmp_str = prompt_box({"Введите год поступления:"});
                if (copied_record.set_start_year(tmp_str))
                    alert_box({"Вы неправильно ввели год поступления!"});
                else
                    alert_box({"Год поступления изменен."});
            } else if (selected_index == 11) {
                tmp_str = prompt_box({"Введите дату рождения:"});
                if (copied_record.set_date_of_birth(tmp_str))
                    alert_box({"Вы неправильно ввели дату рождения!"});
                else
                    alert_box({"Дата рождения изменена."});
            } else if (selected_index == 12) {
                selected_index = menu_box({"Мужчина", "Женщина"});
                copied_record.set_sex(selected_index);
            }
        }
        return (selected_index == 1);
    }

    void list_of_student_records() {
        List<std::string> lines;
        bool exit = false;
        short selected_index, save_index;
        while (!exit) {
            int count = count_of_records();
            if (count == 0) {
                alert_box({ "Нет записей." });
                return;
            }
            lines.push_back("Выход");
            for (int i = 0; i < count; i++) {
                Student record = get_record(i);
                lines.push_back(record.get_surname() + " " + record.get_name()[0] + "." +
                                record.get_patronymic()[0] + ". " + record.get_record_book_number() + " " +
                                record.get_start_year());
            }
            selected_index = menu_box(lines);
            if (selected_index == 0) {
                exit = true;
            } else {
                save_index = selected_index - 1;
                selected_index = menu_box({"Выход", "Изменить запись", "Удалить запись"});
                if (selected_index == 1) {
                    Student record = get_record(save_index);
                    if (!change_student_record(record)) set_record(record, save_index);
                } else if (selected_index == 2) {
                    if (confirm_box({"Подтвердите удаление."})) {
                        delete_record(save_index);
                    }
                }
            }
            lines.clear();
        }
    }

    void option_25() {
        std::string group = prompt_box({"Введите название группы:"});
        List<Student> records = list_of_records_by_group(group);
        if (records.empty()) {
            alert_box({"В этой группе нет студентов!"});
        } else {
            records.sort(compare_start_years);
            List<std::string> lines;
            for (int i = 0; i < records.size(); i++) {
                lines.push_back(records[i].get_surname() + " " + records[i].get_name()[0] + "." +
                                records[i].get_patronymic()[0] + ". " + records[i].get_record_book_number() +
                                " " + records[i].get_start_year());
            }
            alert_box(lines);
        }
    }

    void student_menu() {
        short selected_index;
        bool exit = false;
        while (!exit) {
            selected_index =
                menu_box({"Добавить запись о студенте", "Список студентов (удалить или изменить записи)",
                          "Выполнить вариант 25", "Выход с шифрованием файла", "Выход"});

            if (selected_index == 0) {
                Student record;
                if (!change_student_record(record)) push_record(record);
            } else if (selected_index == 1) {
                list_of_student_records();
            } else if (selected_index == 2) {
                option_25();
            } else if (selected_index == 3) {
                exit = confirm_box({"Подтвердите выход."});
                if (exit) {
                    std::string passwd = prompt_box({
                        "Введите пароль от БД",
                    });
                    std::string path = get_path();
                    if (encrypt_file(path.c_str(), (path + ".enc").c_str(), passwd.c_str())) {
                        alert_box({"Файл зашифрован."});
                    } else {
                        alert_box({"Ошибка при шифровании!"});
                        exit = false;
                    }
                }
            } else if (selected_index == 4) {
                exit = confirm_box({"Подтвердите выход."});
            }
        }
    }

   public:
    Menu() {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTitle(L"База данных студентов");
        set_cursor_visible(false);
        focus_text(false);
    }

    void menu() {
        if (!file_menu()) student_menu();
    }
};