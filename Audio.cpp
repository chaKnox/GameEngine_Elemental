#include "Audio.h"
#include "Wave.h"


Audio::Audio()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&m_Engine)))
	{
		CoUninitialize();
		MessageBox( NULL,"Audio: Create XAudio2: Failed", "Error", NULL );
	}

	if (FAILED(m_Engine->CreateMasteringVoice(&m_Master)))
	{
		m_Engine->Release();
		CoUninitialize();
		MessageBox(NULL, "Audio: Create XAudio2: Failed", "Error", NULL);
	}
	m_MusicVolume = 0.5f;
	m_FXVolume = 0.5f;
}


Audio::~Audio()
{
	m_Engine->Release();
	ClearList();
}

void Audio::Load(char * File, bool Loop)
{
	Wave* l_TempWave = new Wave();
	
	if (Loop)
	{
		l_TempWave->XaBuffer()->LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if (!l_TempWave->Load(File))
	{
		m_Engine->Release();
		CoUninitialize();
	}
	else
	{
		m_AudioList.push_back(l_TempWave);
	}
}

void Audio::PlayAudio(int Enum)
{
	m_Engine->CreateSourceVoice(&m_MusicSource, m_AudioList[Enum]->WavForm());
	m_MusicSource->SetVolume(m_MusicVolume, XAUDIO2_COMMIT_ALL);
	m_MusicSource->Start();
	m_MusicSource->SubmitSourceBuffer(m_AudioList[Enum]->XaBuffer());
}

void Audio::PlaySingle(int Enum)
{
	m_Engine->CreateSourceVoice(&m_FXSource, m_AudioList[Enum]->WavForm());
	m_FXSource->SetVolume(m_MusicVolume, XAUDIO2_COMMIT_ALL);
	m_FXSource->Start();
	m_FXSource->SubmitSourceBuffer(m_AudioList[Enum]->XaBuffer());
}

void Audio::StopAudio(int Enum)
{
	m_MusicSource->Stop();
}

void Audio::ClearList()
{
	for (unsigned int i = 0; i < m_AudioList.size(); i++)
	{
		delete m_AudioList[i];
	}
	m_AudioList.clear();
}
