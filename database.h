#pragma once

#include <cryptopp/default.h>
#include <cryptopp/files.h>

#include <fstream>
#include <string>

#include "list.h"
#include "student.h"

class Database {
   private:
    std::string path;

   protected:
    bool check_database_name(std::string name) {
        bool flag = true;
        for (int i = 0; i < name.length(); i++) {
            if (!(isalnum(name[i]) || name[i] == '_')) {
                flag = false;
                break;
            }
        }
        return flag;
    }

    bool check_database_file(std::string path) {
        std::ifstream file(path, std::ios::binary);
        bool check = file.is_open();
        file.close();
        return check;
    }

    bool create_database_file(std::string path) {
        std::ofstream file(path, std::ios::binary);
        bool check = file.is_open();
        file.close();
        return check;
    }

    bool encrypt_file(const char* fin, const char* fout, const char* passwd) {
        try {
            CryptoPP::FileSource* file_source = new CryptoPP::FileSource(
                fin, true, new CryptoPP::DefaultEncryptor(passwd, new CryptoPP::FileSink(fout)));
            delete file_source;
        } catch (CryptoPP::FileStore::OpenErr) {
            remove(fout);
            return false;
        }
        remove(fin);
        return true;
    }

    bool decrypt_file(const char* fin, const char* fout, const char* passwd) {
        try {
            CryptoPP::FileSource f(fin, true,
                                   new CryptoPP::DefaultDecryptor(passwd, new CryptoPP::FileSink(fout)));
        } catch (CryptoPP::KeyBadErr) {
            remove(fout);
            return false;
        }
        remove(fin);
        return true;
    }

    void set_path(std::string path) { this->path = path; }

    std::string get_path() { return path; }

    int count_of_records() {
        std::ifstream file(path, std::ios::binary);
        file.seekg(0, std::ios::end);
        int length = file.tellg();
        file.close();
        return length / sizeof(t_student);
    }

    Student get_record(int index) {
        t_student rec;
        std::ifstream file(path, std::ios::binary);
        file.seekg(index * sizeof(t_student), std::ios::beg);
        file.read((char*)&rec, sizeof(t_student));
        file.close();
        Student record;
        record.set_student(rec);
        return record;
    }

    void set_record(Student record, int index) {
        t_student rec = record.get_student();
        std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
        file.seekg(index * sizeof(t_student), std::ios::beg);
        file.write((char*)&rec, sizeof(t_student));
        file.close();
    }

    void delete_record(int index) {
        std::ofstream fout(path + ".tmp", std::ios::binary);
        std::ifstream fin(path, std::ios::binary);
        t_student rec;
        int count = count_of_records();
        for (int i = 0; i < count; i++) {
            if (i != index) {
                fin.read((char*)&rec, sizeof(t_student));
                fout.write((char*)&rec, sizeof(t_student));
            }
        }
        fout.close();
        fin.close();
        remove(path.c_str());
        rename((path + ".tmp").c_str(), path.c_str());
    }

    bool push_record(Student record) {
        std::fstream file(path, std::ios::binary | std::ios::app);
        if (!file.is_open()) return false;
        t_student rec = record.get_student();
        file.write((char*)&rec, sizeof(t_student));
        file.close();
        return true;
    }

    bool record_book_number_in_database(std::string record_book_number) {
        bool flag = false;
        std::ifstream file(path, std::ios::binary);
        t_student rec;
        for (int i = 0; i < count_of_records(); i++) {
            file.read((char*)&rec, sizeof(t_student));
            if (std::string(rec.record_book_number) == record_book_number) {
                flag = true;
                break;
            }
            file.seekg((i + 1) * sizeof(t_student), std::ios::beg);
        }
        file.close();
        return flag;
    }

    List<Student> list_of_records_by_group(std::string group) {
        List<Student> records;
        std::ifstream file(path, std::ios::binary);
        Student record;
        t_student rec;
        for (int i = 0; i < count_of_records(); i++) {
            file.read((char*)&rec, sizeof(t_student));
            if (std::string(rec.group) == group) {
                record.set_student(rec);
                records.push_back(record);
            }
            file.seekg((i + 1) * sizeof(t_student), std::ios::beg);
        }
        file.close();
        return records;
    }
};