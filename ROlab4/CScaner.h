#pragma once

#include "CImg.h"
using namespace cimg_library;

struct tagLetter
{
	int X = 0;
	int Y = 0;
	unsigned char **pLetter = NULL;
	tagLetter *pNext = NULL;
};

class CScaner
{
public:
	CScaner() {};
	~CScaner() {};

public:
	int CreateAlphabet();
	void Interpolation(tagLetter *Letter);
	int DownloadAlphabet();
	int UploadAlphabet();
	int ScanText(char *filename);
	int WritingToFile(char *filename);

protected:
	int AlphLetterSize = 40;
	int LetterHeight = 0;
	tagLetter *Alphabet = 0;




};