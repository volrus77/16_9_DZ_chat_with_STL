#include "chat.h"

void Chat::startChat()
{
	userArr.reserve(MAXCOUNTUSER);
	mapUser.reserve(MAXCOUNTUSER + 0.4 * MAXCOUNTUSER);
	vvTo.reserve(MAXCOUNTUSER);
	vvFrom.reserve(MAXCOUNTUSER);
	work_ = true;
}

shared_ptr <Chat::AuthData> Chat::getUserLog(const string& _login) const
{
	if (mapUser.find(_login) != mapUser.end())
	{
		return make_shared<AuthData>(userArr.at(mapUser.at(_login)));
	}
	return nullptr;
}

void Chat::userRegistration()
{
	cout << "Registation" << endl;
	string _login, _pass, _name;

	cout << "Login:  " << endl;
	cin >> _login;
	cout << "Password:   " << endl;
	cin >> _pass;
	cout << "Name: " << endl;
	cin >> _name;

	if (mapUser.find(_login) != mapUser.end() || _login == "all")
	{
		throw UserLoginEx();
	}

	//AuthData user = AuthData(_login, sha1(_pass.data(), _pass.length()),
	//	_name);
	AuthData user = AuthData(_login, sha1(&_pass[0], _pass.length()),
		_name);
	userArr.push_back(user);
	mapUser.emplace(_login, userArr.size() - 1);
	currentUser = make_shared <AuthData>(user);
	std::vector<size_t> v1, v2;
	vvTo.push_back(v1);
	vvFrom.push_back(v2);

}

void Chat::userLogin()
{
	{
		string _login, _pass;
		char op;

		do
		{
			cout << "Login menu" << endl;
			cout << "Login: " << endl;
			cin >> _login;
			cout << "Password: " << endl;
			cin >> _pass;

			currentUser = getUserLog(_login);
		
			uint* digest = sha1(&_pass[0], _pass.length());

			bool cmpHashes = !memcmp(
				currentUser->pass_sha1_hash,
				digest,
				SHA1HASHLENGTHBYTES);
			delete[] digest;

			if (currentUser == nullptr || !cmpHashes)
			{
				currentUser = nullptr;
				cout << "0 - exit, or any key " << endl;
				cin >> op;

				if (op == '0')
					break;
			}

		} while (!currentUser);
	}
}

void Chat::showLoginMenu()
{
	currentUser = nullptr;
	char op;
	do
	{
		cout << " 1 - Login " << endl;
		cout << " 2 - Registration " << endl;
		cout << " 0 - exit " << endl;
		cin >> op;

		switch (op)
		{
		case '1':
			userLogin();
			break;
		case '2':
			try

			{
				userRegistration();
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;
			}
			break;
		case '0':
			work_ = false;
			break;

		default:
			cout << "vvedite 1, 2, 0" << endl;
		}
	} while (!currentUser && work_);
}

void Chat::showUserMenu()
{
	char op;
	cout << "Hello, " << currentUser->login << endl;
	while (currentUser)
	{
		cout << "1 - Show chat, 2 - Add Message, 3 - showAllUser, 0 - exit"
			<< endl;
		cin >> op;


		switch (op)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsers();
			break;
		case '0':
			currentUser = nullptr;
			break;

		default:
			cout << "Vvedite 1,2,3" << endl;
		}
	}

}

void Chat::addMessage()
{
	string _to, _text;
	cout << "Vvedite login, or all" << endl;
	cin >> _to;
	cout << "vvedite message" << endl;
	cin.ignore();
	getline(cin, _text);

	if (!((_to == "all") || getUserLog(_to)))
	{
		cout << "error: " << _to << endl;
		return;
	}
		messageArr.emplace_back(currentUser->login, _to, _text);
		//vvTo.at(mapUser.at(_to)).push_back(messageArr.size() - 1);
		vvTo[mapUser.at(_to)].push_back(messageArr.size() - 1);
		//vvFrom.at(mapUser.at(currentUser->login)).push_back(messageArr.size() - 1);
		vvFrom[mapUser.at(currentUser->login)].push_back(messageArr.size() - 1);

}

void Chat::showAllUsers() const
{
	cout << "users: " << endl;

	for (auto& user : userArr)
	{
		cout << user.login;
		//cout << endl;
		if (currentUser->login == user.login)
		{
			cout << "(me)";
			//cout << endl;
		}
		cout << endl;
	}
}

void Chat::showChat() const
{
	//string from, to;
	cout << "Chat: " << endl;

	auto it1 = vvTo.at(mapUser.at(currentUser->login)).begin();
	auto it2 = vvTo.at(mapUser.at(currentUser->login)).end();
	
	for (; it1 != it2; it1++)
	{
		cout << "from: " << messageArr.at(*it1).messageFrom << " to: "
			<< messageArr.at(*it1).messageTo << "(me)" << endl;
		cout << " text: " << messageArr.at(*it1).text << endl;
	}

	auto it3 = vvFrom.at(mapUser.at(currentUser->login)).begin();
	auto it4 = vvFrom.at(mapUser.at(currentUser->login)).end();

	for (; it3 != it4; it3++)
	{
		cout << "from: " << messageArr.at(*it3).messageFrom << "(me)" << " to: "
			<< messageArr.at(*it3).messageTo << endl;
		cout << " text: " << messageArr.at(*it3).text << endl;
	}
}