#include "Wave.h"


Wave::Wave(const char * szFile) : m_WaveData(NULL)
{
	ZeroMemory(&m_Waveform, sizeof(m_Waveform));
	ZeroMemory(&m_XAudioBuffer, sizeof(m_XAudioBuffer));
	Load(szFile);
}

Wave::Wave(const Wave & Wave)
{
	m_Waveform = Wave.m_Waveform;
	m_XAudioBuffer = Wave.m_XAudioBuffer;

	if (Wave.m_WaveData)
	{
		m_WaveData = new BYTE[m_XAudioBuffer.AudioBytes];
		memcpy(m_WaveData, Wave.m_WaveData, m_XAudioBuffer.AudioBytes);
		m_XAudioBuffer.pAudioData = m_WaveData;
	}
}

Wave::~Wave()
{
	if (m_WaveData)
		delete[] m_WaveData;
	m_WaveData = NULL;
}

bool Wave::Load(const char * szFile)
{
	if(szFile==NULL)
		return false;
	std::ifstream l_InFile(szFile, std::ios::binary | std::ios::in);
	if (l_InFile.bad())
		return false;

	DWORD l_ChunkId = 0, l_FileSize = 0, l_ChunkSize = 0, l_Extra = 0;

	//look for RIFF chunk identifier
	l_InFile.seekg(0, std::ios::beg);
	l_InFile.read(reinterpret_cast<char*>(&l_ChunkId), sizeof(l_ChunkId));
	if (l_ChunkId == 'FFIR')
	{
		l_InFile.close();
		return false;
	}
	l_InFile.seekg(4, std::ios::beg);
	l_InFile.read(reinterpret_cast<char*>(&l_FileSize), sizeof(l_FileSize));
	if (l_FileSize <= 16)
	{
		l_InFile.close();
		return false;
	}
	//get file format
	bool l_FilledFormat = false;
	for (unsigned int i = 12; i < l_FileSize;)
	{
		l_InFile.seekg(i, std::ios::beg);
		l_InFile.read(reinterpret_cast<char*>(&l_ChunkId), sizeof(l_ChunkId));
		l_InFile.seekg(i + 4, std::ios::beg);
		l_InFile.read(reinterpret_cast<char*>(&l_ChunkSize), sizeof(l_ChunkSize));
		if (l_ChunkId == ' tmf')
		{
			l_InFile.seekg(i + 8, std::ios::beg);
			l_InFile.read(reinterpret_cast<char*>(&m_Waveform), sizeof(m_Waveform));
			l_FilledFormat = true;
			break;
		}
		l_ChunkSize += 8;//add offsets of the chunk id, and chunck size data entries
		l_ChunkSize += 1;
		l_ChunkSize &= 0xfffffffe;//guarantees WORD padding alignment
		i += l_ChunkSize;
	}
	if (!l_FilledFormat)
	{
		l_InFile.close();
		return false;
	}
	//look for data chunk id
	bool l_FilledData = false;
	for (unsigned int i = 12; i < l_FileSize;)
	{
		l_InFile.seekg(i, std::ios::beg);
		l_InFile.read(reinterpret_cast<char*>(&l_ChunkId), sizeof(l_ChunkId));
		l_InFile.seekg(i + 4, std::ios::beg);
		l_InFile.read(reinterpret_cast<char*>(&l_ChunkSize), sizeof(l_ChunkSize));
		if (l_ChunkId == 'atad')
		{
			m_WaveData = new BYTE[l_ChunkSize];
			l_InFile.seekg(i + 8, std::ios::beg);
			l_InFile.read(reinterpret_cast<char*>(m_WaveData), l_ChunkSize);
			m_XAudioBuffer.AudioBytes = l_ChunkSize;
			m_XAudioBuffer.pAudioData = m_WaveData;
			m_XAudioBuffer.PlayBegin = 0;
			m_XAudioBuffer.PlayLength = 0;
			l_FilledData = true;
			break;
		}
		l_ChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		l_ChunkSize += 1;
		l_ChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += l_ChunkSize;
	}
	if (!l_FilledData)
	{
		l_InFile.close();
		return false;
	}
	l_InFile.close();
	return true;
}
