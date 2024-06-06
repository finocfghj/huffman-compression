#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<map>
#include<algorithm>
#include<cstring>
#include<queue>
#include<vector>
#include<stdlib.h>
#include<ctime>
using namespace std;
const int N = 1e5;
int wrong = 0;
int check(string s1)
{
    string temp1, temp2;
    char filename[100];
    strcpy(filename, s1.c_str());
    FILE* fp;
    unsigned char s[N];
    fopen_s(&fp, filename, "rb");

    FILE* fq;
    fopen_s(&fq, "zip.txt", "rb");
    unsigned char ss[N];


    while (feof(fp))
    {
        fread(&s, 1, sizeof(s), fp);

        fread(&ss, 1, sizeof(ss), fq);

        for (int i = 0; i < N; i++)
        {
            if (s[i] != ss[i])wrong++;
        }
    }
  
    fclose(fq);
    fclose(fp);

    return wrong;
}