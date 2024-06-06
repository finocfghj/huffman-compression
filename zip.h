#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<map>
#include<algorithm>
#include<cstring>
#include<queue>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<bitset>
#include<unordered_map>
using namespace std;
const int size_s = 10240;
double rate;
int len;
string temp;
map<unsigned char, unsigned long long> _hash;
unordered_map<unsigned char, pair<int, bitset<256>>> m;
unordered_map<unsigned char, pair<int, unsigned long long> > hash_;
vector<unsigned char> str;
struct hfut
{
    int weight;
    int index;
};
struct cmp1
{
    bool operator()(const hfut& a, const hfut& b)
    {
        return a.weight > b.weight;
    }
};
struct HFnode
{
    int weight;
    int parent, Ltree, Rtree;
};

unsigned char binaryStringToChar(string binarystring)
{
    int sum = 0;
    for (int i = 0; i < binarystring.size(); i++)
    {
        if (binarystring[i] == '1')
        {
            int j = pow(2, binarystring.size() - i - 1);
            sum += j;
        }
    }
    unsigned char ch = sum;
    return ch;
}

void dfs(bitset<256> x, HFnode* H, int index, int n)
{
    if (index <= str.size())
    {
        m.insert(make_pair(str[index - 1], make_pair(n, x)));
        hash_[str[index - 1]] = pair<int, unsigned long long>(n, x.to_ullong());
        return;
    }
    dfs(x, H, H[index].Ltree, n + 1);
    x[n] = 1;
    dfs(x, H, H[index].Rtree, n + 1);
    return;
}
void inittree(HFnode*& H, int len)
{
    H = new HFnode[len * 2];
    int index = 1;
    for (auto t = _hash.begin(); t != _hash.end(); t++)
    {
        str.push_back(t->first);
        H[index].weight = t->second;
        H[index].parent = 0;
        H[index].Ltree = 0;
        H[index].Rtree = 0;
        index++;
    }
    priority_queue<hfut, vector<hfut>, cmp1> q;
    for (int i = 1; i <= len; i++)
    {
        hfut temp;
        temp.index = i;
        temp.weight = H[i].weight;
        q.push(temp);
    }
    hfut temp1, temp2, temp3;
    for (int i = len + 1; i < len * 2; i++)
    {
        temp1 = q.top();
        q.pop();
        temp2 = q.top();
        q.pop();
        temp3.index = i;
        temp3.weight = temp2.weight + temp1.weight;
        q.push(temp3);
        H[i].weight = temp3.weight;
        H[i].Ltree = temp1.index;
        H[i].Rtree = temp2.index;
        H[temp1.index].parent = i;
        H[temp2.index].parent = i;
    }
    bitset<256> x = 0;
    dfs(x, H, 2 * len - 1, 0);
    delete H;
}
double zip(string s1,string suffixname)
{
    double l1=0;
    double l2=0;
    char filename[100];
    strcpy(filename, s1.c_str());
    FILE* fp;
    fopen_s(&fp, filename, "rb");
    if (fp == NULL)
    {
        return 0;
    }

    unsigned char s[size_s];
    while (!feof(fp))
    {
        int k = fread(s, 1, sizeof(s), fp);
        int i = 0;
        while (i < k)
        {
            l1+=8;
            _hash[s[i++]]++;
        }
    }
    fclose(fp);


    HFnode* hftree;
    len = _hash.size();
    inittree(hftree, len);


    fopen_s(&fp, filename, "rb");
    if (fp == NULL)
    {
        return 0;
    }

    FILE* ifs;
    fopen_s(&ifs, filename, "rb");

    FILE* ofs;
    fopen_s(&ofs, "zip.txt", "wb+");
    if (ofs == NULL)
    {
        return 0;
    }

    int sufSize = suffixname.size();//写入后缀
    fwrite((char*)&sufSize, sizeof(sufSize), 1, ofs);
    for (auto& r : suffixname)
    {
        fwrite(&r, sizeof(r), 1, ofs);
    }

    int weightSize = _hash.size();
    fwrite((char*)&weightSize, sizeof(weightSize), 1, ofs);

    for (auto& c : hash_)
    {
        fwrite((char*)&c, sizeof(c), 1, ofs);
    }
    int eight = 256;
    bitset<256> n = 0;
    while (!feof(ifs))
    {
        int k = fread(s, 1, sizeof(s), ifs);
        int i = 0;
        while (i < k)
        {
            int j = m[s[i]].first;
            if (j <= eight)
            {
                n |= m[s[i]].second << (256 - eight);
                eight -= j;
            }
            else
            {
                n |= m[s[i]].second << (256 - eight);
                l2 += 256;
                fwrite(&n, sizeof(n), 1, ofs);
                n = 0;
                n |= m[s[i]].second >> eight;
                eight = 256 - (j - eight);
            }
            i++;
        }
    }
    if (eight != 0) fwrite(&n, (256 - eight) / 8 + 1, 1, ofs);

    fclose(ofs);
    fclose(ifs);
    rate = l2 / l1;
    return rate;
}