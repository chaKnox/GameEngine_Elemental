#pragma once
#ifndef WAVE_H_
#define WAVE_H_
#include <Windows.h>	
#include <XAudio2.h>
#include <fstream>

class Wave
{
private:
	WAVEFORMATEX m_Waveform;
	XAUDIO2_BUFFER m_XAudioBuffer;
	BYTE* m_WaveData;
public:
	Wave(const char* szFile = NULL);
	Wave(const Wave& c);
	~Wave();
	XAUDIO2_BUFFER* XaBuffer()  { return &m_XAudioBuffer; }
	const WAVEFORMATEX* WavForm() const { return &m_Waveform; }
	bool Load(const char* szFile);
};

#endif // !WAVE_H_
