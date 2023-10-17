#pragma once

#include "CompileTime.hpp"

#pragma pack(push, 1)

int __stdcall MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType);

class PKey
{
public:
    static constexpr reference<PKey, 0x886980> const FastKey{};

private:
    char data_[0x204];
};
static_assert(sizeof(PKey) == 0x204);

class __declspec(novtable) MixFileClass
{
public:
    MixFileClass(const char* pFileName, const PKey& key);
    virtual ~MixFileClass();

private:
    char data_[0x24];
};
static_assert(sizeof(MixFileClass) == 0x28);

class __declspec(novtable) RAMFileClass
{
public:
    RAMFileClass(const void* data, int length);
    virtual ~RAMFileClass();

    virtual const char* GetFileName() const { return nullptr; }
	virtual const char* SetFileName(const char* pFileName) { return nullptr; }
	virtual int CreateFile() { return 0; }
	virtual int DeleteFile() { return 0; }
	virtual bool Exists(bool writeShared = false) { return 0; }
	virtual bool HasHandle() { return false; }
	virtual bool Open(int access) { return false; }
	virtual bool OpenEx(const char* pFileName, int access) { return false; }
	virtual int ReadBytes(void* pBuffer, int nNumBytes) { return 0; }
	virtual int Seek(int offset, int seek) { return 0; }
	virtual int GetFileSize() { return 0; }
	virtual int WriteBytes(void* pBuffer, int nNumBytes) { return 0; }
	virtual void Close() { return; }
	virtual unsigned int GetFileTime() { return 0; }
	virtual bool SetFileTime(unsigned int FileTime) { return false; }
	virtual void CDCheck(unsigned int errorCode, bool bUnk, const char* pFilename) { return; }

private:
    char data_[0x1A];
};
static_assert(sizeof(RAMFileClass) == 0x1E);

class __declspec(novtable) CCFileClass
{
public:
    CCFileClass();
    CCFileClass(const char* pFileName);
    virtual ~CCFileClass();

    virtual const char* GetFileName() const { return nullptr; }
	virtual const char* SetFileName(const char* pFileName) { return nullptr; }
	virtual int CreateFile() { return 0; }
	virtual int DeleteFile() { return 0; }
	virtual bool Exists(bool writeShared = false) { return 0; }
	virtual bool HasHandle() { return false; }
	virtual bool Open(int access) { return false; }
	virtual bool OpenEx(const char* pFileName, int access) { return false; }
	virtual int ReadBytes(void* pBuffer, int nNumBytes) { return 0; }
	virtual int Seek(int offset, int seek) { return 0; }
	virtual int GetFileSize() { return 0; }
	virtual int WriteBytes(void* pBuffer, int nNumBytes) { return 0; }
	virtual void Close() { return; }
	virtual unsigned int GetFileTime() { return 0; }
	virtual bool SetFileTime(unsigned int FileTime) { return false; }
	virtual void CDCheck(unsigned int errorCode, bool bUnk, const char* pFilename) { return; }

private:
    char data_[0x68];
};
static_assert(sizeof(CCFileClass) == 0x6C);

class CRCEngine
{
public:
    CRCEngine() : data_{ 0 } {}

    unsigned int operator()(void* buffer, int length);
private:
    char data_[0xC];
};
static_assert(sizeof(CRCEngine) == 0xC);

class __declspec(novtable) INIClass
{
public:
	INIClass();
	virtual ~INIClass();

	int Load(RAMFileClass* file, bool flag = false);
	PKey Get_PKey(bool fast = true);
private:
	char data_[0x51];
};
static_assert(sizeof(INIClass) == 0x55);

#pragma pack(pop)