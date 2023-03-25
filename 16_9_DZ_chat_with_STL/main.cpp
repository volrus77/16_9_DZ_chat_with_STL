// 16_9_DZ_chat_with_STL.cpp : 

#include "chat.h"
#include <iostream>

int main()
{
	Chat chat;
	chat.startChat();

	while (chat.work())
	{
		chat.showLoginMenu();
		while (chat.getcurrentUser())
		{
			chat.showUserMenu();
		}
	}

	return 0;
}