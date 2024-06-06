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
int length;
string _temp;
map<unsigned char, pair<int, unsigned long long> > _hashmap;
const int sizes = 10240;
string releasename = "unzip";
struct huffnode
{
    int node;
    bool isleaf;
    unsigned char c;
    huffnode* left;
    huffnode* right;
};
class hufftree
{
public:
    huffnode* root;
    hufftree(map<unsigned char, pair< int, unsigned long long> > a)
    {
        root = new huffnode;
        root->isleaf = false;
        root->left = NULL;
        root->right = NULL;
        huffnode* temp;
        for (auto it = a.begin(); it != a.end(); it++)
        {
            temp = root;
            string ss = "";
            unsigned long long tt = (it->second).second;
            int n = it->second.first;
            while (n--)
            {
                ss += char(tt % 2 + '0');
                tt /= 2;
            }
            for (int i = 0; i < ss.size(); i++)
            {
                if (ss[i] == '0')
                {
                    if (!temp->left)
                    {
                        huffnode* t = new huffnode;
                        t->isleaf = false;
                        t->left = NULL;
                        t->right = NULL;
                        temp->left = t;
                    }
                    temp = temp->left;
                }
                else
                {
                    if (!temp->right)
                    {
                        huffnode* t = new huffnode;
                        t->isleaf = false;
                        t->left = NULL;
                        t->right = NULL;
                        temp->right = t;
                    }
                    temp = temp->right;
                }
            }
            temp->isleaf = true;
            temp->c = it->first;
        }
    }
    ~hufftree()
    {
        queue<huffnode*> q;
        q.push(root);
        while (q.size())
        {
            huffnode* tt = q.front();
            q.pop();
            if (tt->left)q.push(tt->left);
            if (tt->right)q.push(tt->right);

        }
    }
};
void ReCreateTree(FILE* fp)
{
    int mapSize = 0;
    if (fp == nullptr)return;
    fread(&mapSize, sizeof(mapSize), 1, fp);
    pair<unsigned char, pair<int, unsigned long long> > data;
    while (mapSize > 0)
    {
        fread(&data, sizeof(data), 1, fp);
        _hashmap[data.first] = data.second;
        mapSize--;
    }
}
bool is(string s1, string s2, int head, int tail)
{
    if (s2.size() != tail - head + 1)return false;
    for (int i = head; i <= tail; i++)
    {
        if (s1[i] != s2[i - head])return false;
    }
    return true;
}
void GetSuffixName(FILE* fp)
{
    int sufSize = 0;
    fread(&sufSize, sizeof(sufSize), 1, fp);
    char s;
    while (sufSize > 0)
    {
        fread(&s, 1, 1, fp);
        releasename += s;
        sufSize--;
    }
}

string ucharToBinaryString(unsigned char value)
{
    string b = "";
    for (int n = 0x80; n > 0; n >>= 1)
    {
        if (n & value)
            b += '1';
        else
            b += '0';
    }
    return b;
}
bool rezip()
{
    FILE* frp;
    fopen_s(&frp, "zip.txt", "rb");
    if (frp == NULL)
    {
        return false;
    }

    GetSuffixName(frp);
    ReCreateTree(frp);
    hufftree h(_hashmap);
    unsigned char s;
    string ans = "";
    FILE* fwp;
    fopen_s(&fwp, releasename.c_str(), "wb");
    bool u = false;



    unsigned long long s_;
    huffnode* it = h.root;
    while (!feof(frp))
    {
        int k = fread(&s_, 1, sizeof(s_), frp);
        long eight = 64;
        while (eight > 0 && ((64 - eight) / 8) <= k)
        {
            int x = (s_ >> (64 - eight)) & 1;
            if (x == 0)it = it->left;
            if (x == 1)it = it->right;
            if (!it->left && !it->right)
            {
                fwrite(&it->c, sizeof(it->c), 1, fwp);
                it = h.root;
            }
            eight--;
        }
    }
    fclose(fwp);
    return true;
}