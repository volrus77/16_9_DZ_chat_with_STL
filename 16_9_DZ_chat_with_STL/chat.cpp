#include "chat.h"

void Chat::startChat()
{
	work_ = true;
}

shared_ptr <Chat::AuthData> Chat::getUserLog(const string& _login) const
{
	if (data.find(_login) != data.end())
	{
		return make_shared<AuthData>(userArr.at(data.at(_login)));
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

	if (data.find(_login) != data.end() || _login == "all")
	{
		throw UserLoginEx();
	}

	//AuthData user = AuthData(_login, sha1(_pass.data(), _pass.length()),
	//	_name);
	AuthData user = AuthData(_login, sha1(&_pass[0], _pass.length()),
		_name);
	userArr.push_back(user);
	data.emplace(_login, userArr.size() - 1);
	currentUser = make_shared <AuthData>(user);
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

	if (_to == "all")
	{
		messageArr.emplace_back(currentUser->login, "all", _text);
	}

	else
	{
		messageArr.emplace_back(currentUser->login, _to, _text);
	}

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
	string from, to;
	cout << "Chat: " << endl;

	for (auto& message : messageArr)
	{
		if (currentUser->login == message.messageFrom ||
			currentUser->login == message.messageTo ||
			message.messageTo == "all")
		{
			from = (currentUser->login == message.messageTo) ? "me" :
				message.messageFrom;
		}

		if (message.messageTo == "all")
		{
			to = "(all)";
		}

		else
		{
			to = (currentUser->login == message.messageTo) ? "me" :
				message.messageTo;
		}

		cout << "from: " << from << " to: " << to << endl;
		cout << " text: " << message.text << endl;
	}

}