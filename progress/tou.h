#pragma once
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<string>

const int NUM = 102;
const int baifenbi = 100;
const char zifu = '-';
const char jiantou = '>';

void qingkong();
void xianshi(void (*zhege)(int));
void moni(int);
