/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* WinPlr - open-source WINAPI audio player.
* MIT-License
**********************************************************
* Module Name: WinAudio thread-system
**********************************************************
* WinThread.cpp
* Thread system for WinPlr
*********************************************************/

#include "WinAudio.h"

Player::ErrorHandler hErr;

/*************************************************
* ThCreateNewMutex():
* Create mutex with user name and 
* return itself
*************************************************/
HANDLE 
Player::ThreadSystem::ThCreateNewMutex(
	LPCSTR lpName
)
{
	HANDLE hMutex = CreateMutexA(
		NULL,
		FALSE,
		lpName
	);
	if (!hMutex)
	{
		hErr.CreateErrorText(
			"Error! Can't create mutex!"
		);
	}
	return hMutex;
}

/*************************************************
* ThCreateNewThread:
* Create thread with user method and 
* return thread id
*************************************************/
LPDWORD 
Player::ThreadSystem::ThCreateNewThread(
	LPVOID lpFunc,
	HANDLE hMutex
)
{
	LPDWORD lpUserThreadID = NULL;

	// create window thread
	HANDLE hThread = CreateThread(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)lpFunc,
		hMutex,
		NULL,
		lpUserThreadID
	);
	if (!hThread)
	{
		hErr.CreateErrorText(
			"Error! Can't create thread"
		);
	}
	return lpUserThreadID;
}

/*************************************************
* ThSetNewThreadName:
* set user name for thread by exception
*************************************************/
VOID 
Player::ThreadSystem::ThSetNewThreadName(
	LPDWORD dwThreadID,
	LPCSTR lpName
)
{
	THREAD_NAME stName;
	stName.dwType = 0x1000;
	stName.dwThreadID = (DWORD)dwThreadID;
	stName.dwFlags = NULL;
	stName.lpName = lpName;

	// try to raise exception to set name of thread
	__try
	{
		RaiseException(
			0x406D1388,
			NULL,
			sizeof(stName) / 
			sizeof(ULONG_PTR),
			(ULONG_PTR*)&stName
		);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION) 
	{
	}
}
