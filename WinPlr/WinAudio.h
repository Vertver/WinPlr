/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* WinPlr - open-source WINAPI audio player.
* MIT-License
**********************************************************
* Module Name: WinAudio header	
**********************************************************
* WinAudio.h
* Master include file for WinPlr application.
*********************************************************/
#pragma once

#include <windows.h>
#include <string>
#include <dsound.h>			// for DirectSound
#include <mmreg.h>
#include <mmeapi.h>
#include <VersionHelpers.h>
#include <mmdeviceapi.h>	// minimal MME API
#include <ks.h>
#include <ksmedia.h>
#include <comdef.h>			// for hresult
#include <cguid.h>			// for UI
#include <strsafe.h>
#include <d2d1_3.h>
#include <dxgi.h>
#include <xaudio2.h>
#include <stdio.h>
#include <process.h>

#define DLL_EXPORTS
#ifdef DLL_EXPORTS
#define PLAYER_API decltype(dllexport)
#else
#define PLAYER_API decltype(dllimport)
#endif

template<typename T> T freadNum(FILE* f);
std::string freadStr(FILE* f, size_t len);

VOID CreateErrorText(LPCSTR lpMsgText);
VOID CreateErrorText(LPCSTR lpMsgText, HRESULT hr);
VOID CreateInfoText(LPCSTR lpMsgText);
VOID CreateWarningText(LPCSTR lpMsgText);
VOID ContinueIfYes(LPCSTR lpMsgText, LPCSTR lpMsgTitle);

#define DEBUG_MESSAGE(x)	OutputDebugStringA(x); OutputDebugStringA("\n");
#define _RELEASE(x)			if (x)					{ x->Release(); x = NULL; }	// safety release pointers
#define R_ASSERT2(x, y)		if (!SUCCEEDED(x))		{ CreateErrorText(y, x); }
#define R_ASSERT(x)			if (!SUCCEEDED(x))		{ CreateErrorText("R_ASSERT"); }
#ifndef DEBUG
#define ASSERT(x, y)		if (!x)					{ OutputDebugStringA(y); OutputDebugStringA("\n");}
#else
#define ASSERT(x, y)		if (!x)					{ OutputDebugStringA(y); OutputDebugStringA("\n"); __debugbreak();}
#endif
#define DO_EXIT(x, y)		if (!(x))				{ CreateErrorText(y); }
#define PLAYER_VERSION		"#PLAYER_VERSION: 0.1.16#"

typedef enum
{
	WAV_FILE = 1,
	AIF_FILE = 2,
	ALAC_FILE = 3,
	FLAC_FILE = 4,
	MPEG4_FILE = 5,
	MPEG3_FILE = 6,
	MPEG2_FILE = 7,
	OGG_FILE = 8,
	OPUS_FILE = 9,
	UNKNOWN_FILE = 10
} FILE_TYPE;

typedef enum
{
	HANN_WINDOW = 1,
	HAMMING_WINDOW = 2,
	BLACKMAN_WINDOW = 3,
	BLACKMANHARRIS_WINDOW = 4
} WINDOW_TYPE;

typedef struct {
	DWORD	dwType;				// thread type
	LPCSTR	lpName;				// thread name
	DWORD	dwThreadID;			// id for thread
	DWORD	dwFlags;			// flags to create thread
} THREAD_NAME; 

typedef struct
{
	LPVOID lpData;				// pointer to allocated memory
	WORD wBitrate;				// bit rate of sample
	WORD wChannels;				// count of channels
	BOOL isFloat;				// IEEE or PCM
	BOOL bReorderChannels;		// if we need to reorder channels - get this
	DWORD dwSamplerate;			// sample rate of sample
	DWORD dwTime;				// durability of sample
	LPCSTR lpPath;				// full path to file
	LPSTR lpName;				// full name of file
	BOOL bWindows;				// if we wan't to use internal WINAPI methods
} PCM_DATA, *PCM_DATA_P;

typedef struct
{
	HANDLE hFile;				// handle of file
	LPVOID lpFile;				// pointer to allocated memory
	DWORD dwSize;				// size of file
	FILE_TYPE eType;			// type of file
} FILE_DATA, *FILE_DATA_P;

typedef struct  
{
	FILE_DATA dData;			// FILE structure data
	PCM_DATA dPCM;				// PCM structure data
	HWND hwnd;					// HWND handle
} HANDLE_DATA, *HANDLE_DATA_P;

typedef struct
{
	PCM_DATA dPCM;										// all PCM data for DirectSound
	LPDIRECTSOUND lpDirectSound;						// DirectSound main object
	LPDIRECTSOUNDBUFFER lpPrimaryDirectBuffer;			// DirectSound buffer
	LPDIRECTSOUNDBUFFER lpSecondaryDirectBuffer;		// DirectSound buffer
	LPDIRECTSOUNDNOTIFY lpDirectNotify;					// DirectSound notify
	BOOL bPlaying;										// display if audio now is playing
} STREAM_DATA, *STREAM_DATA_P;

typedef struct  
{
	HWND hwnd;					// hwnd for app
	LPCSTR lpWindowName;		// title of window
	HINSTANCE hInstance;		// hInstance
} HWND_DATA, *HWND_DATA_P;

typedef struct
{
	FILE_DATA dData;			// file data struct
	PCM_DATA dPCM;				// PCM data struct
} AUDIO_FILE, *AUDIO_FILE_P;


namespace Player
{
	class Buffer
	{
	public:
		HANDLE_DATA LoadFileToBuffer(HWND hwnd, HANDLE hData, FILE_DATA dFile, PCM_DATA dPCM);
		BOOL CheckBufferFile(HANDLE_DATA hdData);
	};
	class Stream
	{
	public:
		STREAM_DATA CreateMMIOStream(FILE_DATA dData, PCM_DATA dPCM, HWND hwnd);
		STREAM_DATA CreateDirectSoundStream(FILE_DATA dData, PCM_DATA dPCM, HWND hwnd);
		VOID PlayBufferSound(STREAM_DATA streamData);
		VOID StopBufferSound(STREAM_DATA streamData);
		VOID ReleaseSoundBuffers(STREAM_DATA streamData);
	};
	class ThreadSystem
	{
	public:
		VOID ThSetNewThreadName(LPCSTR lpName);
		BOOL ThSetNewWin10ThreadName(LPCWSTR lpName);
		VOID ThBeginXAudioThread(AUDIO_FILE audioFile);
	};
	class Graphics
	{
	public:
	};
}
