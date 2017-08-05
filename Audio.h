#pragma once
#ifndef AUDIO_H_
#define AUDIO_H_
#include <vector>
#include <Windows.h>	
#include <XAudio2.h>
#include <fstream>
class Wave;

class Audio
{
private:
	IXAudio2 *m_Engine;
	IXAudio2SourceVoice *m_MusicSource, *m_FXSource;
	IXAudio2MasteringVoice *m_Master;
	std::vector<Wave*> m_AudioList;
public:
	Audio();
	~Audio();
	float m_FXVolume, m_MusicVolume;
	void Load(char* File, bool Loop);
	void PlayAudio(int Enum);
	void PlaySingle(int Enum);
	void StopAudio(int Enum);
	void ClearList();
};

#endif // !AUDIO_H_
