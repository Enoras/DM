#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#pragma once
#include <stdio.h>
#include <stdlib.h>
#define SIZE 81
#define NAMESIZE 20
#define COMMANDSIZE 20
#define ALL_COMANDS 14
#define WHOLESIZE 120

#define MAX(x,y) ((x)>(y)?(x):(y))

typedef enum {
  FALSE = 0,
  TRUE = 1,
}bool_t;

typedef struct elem_t {
  char str[SIZE];
  struct elem_t* next;
}elem_t;

typedef struct set_t{
  elem_t* elem;
  char name[NAMESIZE];
  struct set_t* next;
} set_t;

typedef struct list_of_sets {
  set_t* head;
}list_of_sets;

typedef enum {
  NO_ERROR = 0,
  MEMORY = 1,
  INCORRECT_SYMBOL = 2,
  SYNTAX_ERROR =3,
  UNKNOWN_COMMAND = 4,
  SET_DOES_NOT_EXIST = 5,
  ELEM_DOES_NOT_EXIST = 6,
  ELEM_EXISTS = 7,
  SET_EXISTS = 8,
}error_t;

typedef enum {
  COMMAND_NOT_FOUND = -1,
  CREATE = 0,
  ADD = 1,
  EXIT = 2,
  SHOW = 3,
  REMOVE = 4,
  POWER = 5,
  CONTAIN = 6,
  UNION = 7,
  INTERSECTION = 8,
  DIFFERENCE = 9,
  SYMMETRICDIFFERENCE = 10,
  HELP = 11,
  DESTROY = 12,
  EXIST = 13,
}command_t;

void SetPrint(set_t* set);
void SetAllPrint();
set_t* SetCreate(char* name, error_t* err);
void SetDestroy(set_t* set);
int SetPower(set_t* set);
elem_t* ElemCompare(elem_t* elem1, elem_t* elem2);
bool_t ElemAdd(set_t* set, char* str, error_t* error);
bool_t ElemRemove(set_t* set, elem_t* elem);
elem_t* ElemFind(set_t* set, char* str);
bool_t IsElemsEqual(elem_t* elem1, elem_t* elem2);
elem_t* ElemCreateEqual(elem_t* elem);
set_t* SetUnion(set_t* set1, set_t* set2);
set_t* SetIntersection(set_t* set1, set_t* set2);
set_t* SetDifference(set_t* set1, set_t* set2);
set_t* SetSymmetricDifference(set_t* set1, set_t* set2);
set_t* SetFind(char* name);
bool_t SetCheckContain(set_t* set1, set_t* set2);
set_t* SetCopy(set_t* set);
bool_t Run(char* const str); 
void ExitProgramm();
void ErrorMessage(error_t err);
void Create(char* name, error_t* err);
void Destroy(char* name, error_t* err);
void Show(char* str, error_t* err); 
void Add(char* str, error_t* err);
void Remove(char* str, error_t* err);
void Power(char* str, error_t* err);
void Contain(char* str, error_t* err);
void SetAction(char* str, error_t* err, int comand);
set_t* SetFindInStr(char* str, int arg, error_t* err);
char* SetNewName(char* str);
void SetAddToList(set_t* set);
void GreetingMessage();
void Help(char* str, error_t* err);
void IsExists(char* str, error_t* err);
#endif