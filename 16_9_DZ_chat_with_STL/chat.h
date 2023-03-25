#pragma once

#include "sha1.h"
#include <string>
#include <vector>
#include <exception>
#include <memory>
#include <unordered_map>
#include <iostream>

using namespace std;
#define SIZE 10
#define LOGINLENGTH 10

struct UserLoginEx : public exception
{
    const char* what() const noexcept override { return "����� ��� �����������, ������� ������ �������"; }
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
            name(""),
            pass_sha1_hash(0),
            status(CellStatus::free) {
        }
        ~AuthData() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }
        // �������� �����, �������� ������ ���
        AuthData(string _login, uint* sh1, string _name) {
            login = _login;
            pass_sha1_hash = sh1;
            name = _name;
            status = CellStatus::engaged;
        }
        // �������� ��
        AuthData& operator = (const AuthData& other) {
            login = other.login;

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            status = other.status;

            return *this;
        }
        string login;
        string name;
        uint* pass_sha1_hash;

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
    shared_ptr <AuthData> getUserName(const string& _name) const;

    vector <AuthData> userArr;
    vector <Message> messageArr;
    shared_ptr <AuthData> currentUser = nullptr;
    bool work_ = false;
    unordered_map<string, int > data;
};