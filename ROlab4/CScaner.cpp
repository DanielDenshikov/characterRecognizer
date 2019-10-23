#include "stdafx.h"
#include "CScaner.h"
#include <math.h>
#include <locale.h>

int CScaner::CreateAlphabet()
{
	setlocale(LC_ALL, "RUS");

	CImg<unsigned char> AlphFile("Alphabet.bmp");
	if (!AlphFile) return 1;

	int X = AlphFile._height;
	int Y = AlphFile._width;

	Alphabet = new tagLetter;
	tagLetter *Buffer = Alphabet;

	unsigned char **Alph = new unsigned char*[X];
	for (int i = 0; i < X; i++) Alph[i] = new unsigned char[Y];

	for (int i = 0; i < X; i++)
		for (int j = 0; j < Y; j++)
		{
			if (AlphFile(j, i) < 128) Alph[i][j] = 0;
			else Alph[i][j] = 255;
		}

	int i = 0;
	while (i < X)
	{
		int S = 0;
		for (int j = 0; j < Y; j++)
			if (!Alph[i][j]) S++;

		if (S)
		{
			int StringTop = i;
			while (S)
			{
				S = 0;
				i++;
				for (int j = 0; j < Y; j++)
					if (!Alph[i][j]) S++;
			}
			int StringBottom = i;

			for (int j = 0; j < Y; j++)
			{
				S = 0;
				for (i = StringTop; i < StringBottom; i++)
					if (!Alph[i][j]) S++;

				if (S)
				{
					int LetterLeft = j;
					while (S)
					{
						S = 0;
						j++;
						for (i = StringTop; i < StringBottom; i++)
							if (!Alph[i][j]) S++;
					}
					int LetterRight = j;

					int LetterTop;
					int LetterBottom;


					for (int f = StringTop; f < StringBottom; f++)
					{
						S = 0;
						for (int p = LetterLeft; p < LetterRight; p++)
							if (!Alph[f][p]) S++;

						if (S)
						{
							LetterTop = f;
							break;
						}
					}
					for (int f = StringBottom - 1; f >= LetterTop; f--)
					{
						S = 0;
						for (int p = LetterLeft; p < LetterRight; p++)
							if (!Alph[f][p]) S++;

						if (S)
						{
							LetterBottom = f + 1;
							break;
						}
					}

					Buffer->X = LetterBottom - LetterTop;
					Buffer->Y = LetterRight - LetterLeft;

					Buffer->pLetter = new unsigned char*[Buffer->X];
					for (int k = 0; k < Buffer->X; k++) Buffer->pLetter[k] = new unsigned char[Buffer->Y];

					for (int k = 0; k < Buffer->X; k++)
						for (int l = 0; l < Buffer->Y; l++)
							Buffer->pLetter[k][l] = Alph[LetterTop + k][LetterLeft + l];

					Buffer->pNext = new tagLetter;
					Buffer = Buffer->pNext;
				}
			}
			i = X;
		}
		i++;
	}

	unsigned char LetterNom = 192;
	char *letterfilename = new char[15];
	Buffer = Alphabet;

	while (Buffer->pNext)
	{
		if (LetterNom == 219) LetterNom++;
		sprintf(letterfilename, "%c.bmp", LetterNom);
		LetterNom++;
		Interpolation(Buffer);

		CImg<unsigned char> ImgOutFile(Buffer->Y, Buffer->X);

		for (int i = 0; i < Buffer->X; i++)
			for (int j = 0; j < Buffer->Y; j++)
				ImgOutFile(j, i) = Buffer->pLetter[i][j];

		ImgOutFile.save(letterfilename);

		Buffer = Buffer->pNext;
	}
	delete[] letterfilename;

	for (int i = 0; i < X; i++) delete[] Alph[i];
	delete[] Alph;

	return 0;
}

void CScaner::Interpolation(tagLetter *Letter)
{
	if (!(Letter->X ^ AlphLetterSize)) return;

	int ResX = AlphLetterSize;

	float IntFactor = (float)ResX / (Letter->X - 1);
	int ResY = IntFactor * Letter->Y;

	unsigned char **imgOutArray = new unsigned char*[ResX];
	for (int i = 0; i < ResX; i++)
	{
		imgOutArray[i] = new unsigned char[ResY];
	}

	//Обработка
	for (int i = 0; i < ResX; i++)
		for (int j = 0; j < ResY; j++)
		{
			float Xp = (float)i / IntFactor;
			float Yp = (float)j / IntFactor;
			int Xq = Xp + 0.5;
			int Yq = Yp + 0.5;
			/*int R1 = (Xp - Xq) * Letter->pLetter[Xq + 1][Yq] + (1 - Xp + Xq) * Letter->pLetter[Xq][Yq];
			int R2 = (Xp - Xq) * Letter->pLetter[Xq + 1][Yq + 1] + (1 - Xp + Xq) * Letter->pLetter[Xq][Yq + 1];
			if (((Yp - Yq) * R2 + (1 - Yp + Yq) * R1) > 128) imgOutArray[i][j] = 255;
			else imgOutArray[i][j] = 0;*/
			imgOutArray[i][j] = Letter->pLetter[Xq][Yq];

		}

	for (int i = 0; i < Letter->X; i++) delete[] Letter->pLetter[i];
	delete[] Letter->pLetter;

	Letter->pLetter = imgOutArray;
	Letter->X = ResX;
	Letter->Y = ResY;
}

int CScaner::DownloadAlphabet()
{
	setlocale(LC_ALL, "RUS");

	if (Alphabet) UploadAlphabet();
	unsigned char LetterNom = 192;
	char *letterfilename = new char[15];
	
	Alphabet = new tagLetter;
	tagLetter *Buffer = Alphabet;

	while (LetterNom < 224)
	{
		if (LetterNom == 219) LetterNom++;
		sprintf(letterfilename, "%c.bmp", LetterNom);
		LetterNom++;

		CImg<unsigned char> LetterFile(letterfilename);

		Buffer->X = LetterFile._height;
		Buffer->Y = LetterFile._width;

		Buffer->pLetter = new unsigned char*[Buffer->X];
		for (int i = 0; i < Buffer->X; i++) Buffer->pLetter[i] = new unsigned char[Buffer->Y];

		for (int i = 0; i < Buffer->X; i++)
			for (int j = 0; j < Buffer->Y; j++)
				Buffer->pLetter[i][j] = LetterFile(j, i);

		Buffer->pNext = new tagLetter;
		Buffer = Buffer->pNext;
	}
	delete[] letterfilename;

	return 0;
}

int CScaner::UploadAlphabet()
{
	if (!Alphabet) return 1;
	tagLetter *Buffer;
	while (Alphabet->pNext)
	{
		Buffer = Alphabet;
		Alphabet = Alphabet->pNext;
		for (int i = 0; i < Buffer->X; i++) delete[] Buffer->pLetter[i];
		delete[] Buffer->pLetter;
		delete Buffer;
	}
	delete[] Alphabet;
	Alphabet = NULL;
	return 0;
}

int CScaner::ScanText(char *filename)
{
	setlocale(LC_ALL, "RUS");

	CImg<unsigned char> ImgInFile(filename);
	if (!ImgInFile) return 1;

	int X = ImgInFile._height;
	int Y = ImgInFile._width;
	int LetterCount = 0;

	tagLetter *LetterList = new tagLetter;
	tagLetter *LastLetter = LetterList;

	unsigned char **ImgIn = new unsigned char*[X];
	for (int i = 0; i < X; i++) ImgIn[i] = new unsigned char[Y];

	for (int i = 0; i < X; i++)
		for (int j = 0; j < Y; j++)
		{
			if (ImgInFile(j, i) < 128) ImgIn[i][j] = 0;
			else ImgIn[i][j] = 255;
		}

	for (int i = 0; i < X; i++)
	{
		int S = 0;
		for (int j = 0; j < Y; j++)
			if (!ImgIn[i][j]) S++;
		if (S)
		{
			int StringTop = i;
			while (S)
			{
				S = 0;
				i++;
				for (int j = 0; j < Y; j++)
					if (!ImgIn[i][j]) S++;
			}
			int StringBottom = i;

			for (int j = 0; j < Y; j++)
			{
				S = 0;
				for (i = StringTop; i < StringBottom; i++)
					if (!ImgIn[i][j]) S++;

				if (S)
				{
					int LetterLeft = j;
					while (S)
					{
						S = 0;
						j++;
						for (i = StringTop; i < StringBottom; i++)
							if (!ImgIn[i][j]) S++;
					}
					int LetterRight = j;

					int LetterTop;
					int LetterBottom;


					for (int f = StringTop; f < StringBottom; f++)
					{
						S = 0;
						for (int p = LetterLeft; p < LetterRight; p++)
							if (!ImgIn[f][p]) S++;

						if (S)
						{
							LetterTop = f;
							break;
						}
					}
					for (int f = StringBottom - 1; f >= LetterTop; f--)
					{
						S = 0;
						for (int p = LetterLeft; p < LetterRight; p++)
							if (!ImgIn[f][p]) S++;

						if (S)
						{
							LetterBottom = f + 1;
							break;
						}
					}

					LastLetter->X = LetterBottom - LetterTop;
					LastLetter->Y = LetterRight - LetterLeft;

					LastLetter->pLetter = new unsigned char*[LastLetter->X];
					for (int k = 0; k < LastLetter->X; k++) LastLetter->pLetter[k] = new unsigned char[LastLetter->Y];

					for (int k = 0; k < LastLetter->X; k++)
						for (int l = 0; l < LastLetter->Y; l++)
							LastLetter->pLetter[k][l] = ImgIn[LetterTop + k][LetterLeft + l];

					LetterCount++;
					LastLetter->pNext = new tagLetter;
					LastLetter = LastLetter->pNext;
				}
			}
		}
	}
	
	char *text = new char[LetterCount + 1];
	text[0] = 0;
	while (LetterList->pNext)
	{
		Interpolation(LetterList);

		tagLetter *AlphBuffer = Alphabet;
		unsigned char IfLetter = 192;
		unsigned char ThisLetter = 'n';
		unsigned char Difference = 1000;

		CImg<unsigned char> ImgOutFile(LetterList->Y, LetterList->X);

		for (int i = 0; i < LetterList->X; i++)
			for (int j = 0; j < LetterList->Y; j++)
				ImgOutFile(j, i) = LetterList->pLetter[i][j];

		ImgOutFile.save("buffer.bmp");

		while (AlphBuffer->pNext)
		{
			if (IfLetter == 219) IfLetter++;
			int Sum = 0;
			int Eq = 0;


			for (int i = 0; i < LetterList->X; i++)
				for (int j = 0; j < LetterList->Y; j++)
				{
					if((i < AlphBuffer->X) && (j < AlphBuffer->Y)) if ((AlphBuffer->pLetter[i][j] == LetterList->pLetter[i][j]) && !(AlphBuffer->pLetter[i][j])) Eq++;
					
					if (!LetterList->pLetter[i][j]) Sum++;
				}


			if (abs(Sum - Eq) <= Difference)
			{
				ThisLetter = IfLetter;
				Difference = abs(Sum - Eq);
			}
			IfLetter++;
			AlphBuffer = AlphBuffer->pNext;
		}
		if(!AlphBuffer->pNext) sprintf(text, "%s%c", text, ThisLetter);
		LastLetter = LetterList->pNext;
		for (int i = 0; i < LetterList->X; i++) delete[] LetterList->pLetter[i];
		delete[] LetterList->pLetter;
		LetterList = LastLetter;
	}

	delete[] LetterList;

	for (int i = 0; i < X; i++) delete[] ImgIn[i];
	delete[] ImgIn;

	WritingToFile(text);
	delete[] text;
	
	return 0;

}

int CScaner::WritingToFile(char *text)
{
	FILE *f = fopen("result.txt", "w");
	if (!f) return 1;
	fprintf(f, "%s", text);
	fclose(f);
	return 0;
}