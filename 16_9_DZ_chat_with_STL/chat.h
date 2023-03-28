#pragma once

#include "sha1.h"
#include <string>
#include <vector>
#include <exception>
#include <memory>
#include <unordered_map>
#include <iostream>

using namespace std;

const size_t MAXCOUNTUSER = 10;

struct UserLoginEx : public exception
{
    const char* what() const noexcept override { return "Логин уже использован, введите другой вариант"; }
};

class Chat {
private:
    struct AuthData;
public:

    void startChat();
    void showLoginMenu();
    void showUserMenu();
    bool work() const { return work_; }
    shared_ptr <Chat::AuthData> getcurrentUser() const { return currentUser; }

private:

    enum CellStatus {
        free,
        engaged,
        deleted
    };

    struct AuthData {
        AuthData() :
            login(""),
            pass_sha1_hash(0),
            name(""),
            status(CellStatus::free) {
        }
        /*~AuthData() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }*/
        // копирует логин, забирает внутрь хеш
        AuthData(string _login, uint* sh1, string _name) {
            login = _login;
            pass_sha1_hash = sh1;
            name = _name;
            status = CellStatus::engaged;
        }
        // копирует всё
        AuthData& operator = (const AuthData& other) {
            login = other.login;

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            name = other.name;
            status = other.status;

            return *this;
        }
        string login;
        uint* pass_sha1_hash;
        string name;

        CellStatus status;
    };

    struct Message
    {
        Message(const string& messageFrom_, const string& messageTo_,
            const string& text_)
            : messageFrom(messageFrom_), messageTo(messageTo_), text(text_)
        {}

        string messageFrom;
        string messageTo;
        string text;
    };

    void userRegistration();
    void userLogin();
    void addMessage();
    void showChat() const;
    void showAllUsers() const;
    shared_ptr <AuthData> getUserLog(const string& _login) const;
    //shared_ptr <AuthData> getUserName(const string& _name) const;

    vector <AuthData> userArr;
    vector <Message> messageArr;
    shared_ptr <AuthData> currentUser = nullptr;
    bool work_ = false;
    unordered_map<string, size_t > mapUser;  //индексы пользователей
    vector<vector<size_t>> vvTo;  // индексы мессаджей To
    vector<vector<size_t>> vvFrom;   // индексы мессаджей From
};