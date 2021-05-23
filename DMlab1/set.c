#include "header.h"

list_of_sets list;
set_t* tail = NULL;
 
//Вывод множества на экран
void SetPrint(set_t * set) {
  if (!set)
    return;

  elem_t* temp = set->elem;
  while (temp) {
    printf("%s\n", temp->str);
    temp = temp->next;
  }
}

//Вывод всех множеств на экран
void SetAllPrint() {
  set_t* temp = list.head;
  printf("All sets:\n");
  while (temp) {
    printf("\"%s\"", temp->name);
    temp = temp->next;
    if (temp)
      printf(", ");
  }
  printf("\n");
}

//Создание множества (Строка с кавычками)
set_t* SetCreate(char* name, error_t* err) {
  int i = 0, j=0;
  set_t* set = (set_t*)malloc(sizeof(set_t));
  if (!set) {
    *err = MEMORY;
    return NULL;
  }
  bool_t EXISTS = FALSE;
  set_t* temp = list.head;
  //Имя множества
  for (i = 0; i < NAMESIZE; i++) {

    if (name[i] != '"' && name[i] != '\n') {
      set->name[j] = name[i];
      j++;
    }

    if (name[i] == '\n') {
      set->name[j] = '\0';
      i = NAMESIZE; //Выход из цикла
    }
  }
  set->next = NULL;
  set->elem = NULL;

  //Проверим, что множества с таким именем больше нет
  temp = list.head;
  while (temp && EXISTS == FALSE) {
    EXISTS = TRUE;

    i = 0;
    while (temp->name[i] != '\0' || set->name[i] != '\0' || i <= NAMESIZE) {

      if (temp->name[i] != set->name[i]) {
        EXISTS = FALSE;
        break;
      }

      else if (temp->name[i] == '\0' && set->name[i] == '\0') {
        EXISTS = TRUE;
        break;
      }
      i++;
    }
    if (EXISTS)
      break;

    temp = temp->next;
  }

  if (EXISTS==TRUE) {
    *err = SET_EXISTS;
    free(set);
    return NULL;
  }

  //Добавляем новое множество в список множеств
  if (list.head == NULL) {
    list.head = set;
    tail = list.head;
    tail->next = NULL;
  }
  else {
    tail->next = set;
    tail = tail->next;
    tail->next = NULL;
  }
  return set;
}

set_t* SetFind(char* name) {
  set_t* temp = list.head;
  bool_t wasFound = TRUE;
  int i;

  while (temp) {
    wasFound = TRUE;
    for (i = 1; i < SIZE; ++i) {

      if (name[i] == '\0' || name[i] == '"') {
        i = SIZE;
      }

      else if (temp->name[i-1] != name[i])
        wasFound = FALSE;
      

    }
    if (wasFound)
      return temp;
      
    temp = temp->next;
  }

  //дошли до конца, но не нашли множество
  return NULL;
}

//Удаление существующего множеста
void SetDestroy(set_t* set) {
  if (!set)
    return;

  elem_t* temp = set->elem, *temp2 = set->elem;
  set_t* findset = list.head;

  //Удаляем первое
  if (findset == set) 
    list.head = findset->next;

  //Удаляем второе, третье и т.д...
  else {
    while (findset->next != set)
      findset = findset->next;

    //Если удаляем последнее множество, то сохраняем tail на конце
    if (set->next == NULL)
      tail = findset;

    //Не теряем хвост
    findset->next = findset->next->next;
  }
  //Удаляем множество
  while (temp2) {
    temp2 = temp2->next;
    free(temp);
    temp = temp2;
  }
  if(temp)
    free(temp);
  free(set);
}

//Вывести мощность множества
int SetPower(set_t* set) {
  int power = 0;
  elem_t* temp = set->elem;
  while (temp) {
    temp = temp->next;
    power++;
  }
  return power;
}

//Скопировать множество (вспомогательная функция)
set_t* SetCopy(set_t* set) {
  if (!set)
    return NULL;

  elem_t* temp=set->elem, *temp2;
  set_t* newset = (set_t*)malloc(sizeof(set_t));
  if (!newset)
    return NULL;

  newset->elem = ElemCreateEqual(temp);
  temp2 = newset->elem;
  temp = temp->next;

  while (temp) {
    temp2->next = ElemCreateEqual(temp);

    if (!temp2->next) {
      SetDestroy(newset);
      return NULL;
    }

    temp = temp->next;
    temp2 = temp2->next;
  }

  return newset;
}

//Объединение множеств
set_t* SetUnion(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  elem_t* temp1 = NULL, * temp2 = NULL, * newelem = NULL, * temp = set3->elem;
  elem_t* search = NULL;

  //Любое множество не существует в компьютере
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //Первое множество пустое
  else if (!set1->elem) {

    //второе множество или существует, или пустое
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;//вернуть пустое множество
    }
    //Вернуть второе множество, если оно не пустое 
    else {
      free(set3);
      set3 = SetCopy(set2);
      SetAddToList(set3);
      return set3;
    }
  }

  //Второе множество пустое
  else if (!set2->elem) {

    //Если первое тоже пустое, то этот
    //случай рассмотрен выше
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  //Оба множества существуют и непустые
  temp1 = set1->elem;
  temp2 = set2->elem;
  newelem = NULL;
  temp = set3->elem;
  search = set3->elem;

  while (temp1 != NULL && temp2 != NULL) {

    newelem = ElemCompare(temp1, temp2);

    if (newelem == temp1) {
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
    }

    else if (newelem == temp2) {
      newelem = ElemCreateEqual(temp2);
      temp2 = temp2->next;
    }

    else { //элементы равны, добавляем только один
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
      temp2 = temp2->next;
    }

    //добавляем newelem во множество с проверкой, что такого элемента нет
    search = set3->elem;
    while (search) {
      if (IsElemsEqual(search, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }
    //Если такого элемента во множестве нет, добавим его
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
  }

  //Дописываем хвост от одного из множеств с проверкой
  if (temp1 == NULL)
    temp1 = temp2;

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //Проверка
    search = set3->elem;
    while (search) {
      if (IsElemsEqual(search, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }

    temp1 = temp1->next;
    if (newelem) {
      temp->next = newelem;
      temp = temp->next;
    }
  }

  //Добавим множество в список множеств
  SetAddToList(set3);
  return set3;
}

//Пересечение множеств
set_t* SetIntersection(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  //Если какое-то множество не существует
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //Если какое-либо множество пустое
  else if (!set1->elem || !set2->elem) {
    SetAddToList(set3);
    return set3;
  }
  
  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search = set2->elem;

  while (temp1 != NULL && temp2 != NULL) {

    search = set2->elem;
    while (search) {
      if (IsElemsEqual(search, temp1)) {
        newelem = ElemCreateEqual(temp1);
        if (temp == NULL) {
          set3->elem = newelem;
          temp = newelem;
        }
        else {
          temp->next = newelem;
          temp = temp->next;
        }
      }
      search = search->next;
    }
    temp1 = temp1->next;
  }
  SetAddToList(set3);
  return set3;
}

//Разность множеств
set_t* SetDifference(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }
  //Первое множество пустое
  else if (!set1->elem) {
    SetAddToList(set3);
    return set3;
  }

  //Второе множество пустое
  else if (!set2->elem) {
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search = set2->elem;

  while (temp1 != NULL && temp2 != NULL) {
    newelem = ElemCreateEqual(temp1);

    search = set2->elem;
    while (search) {
      if (IsElemsEqual(search, temp1)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      search = search->next;
    }
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
    temp1 = temp1->next;
  }
  SetAddToList(set3);
  return set3;
}

//Симметрическая разность множеств
set_t* SetSymmetricDifference(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //Первое множество пустое
  if (!set1->elem) {
    //Второе множество пустое
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;
    }
    
    //Второе непустое
    free(set3);
    set3 = SetCopy(set2);
    SetAddToList(set3);
    return set3;
  }

  //Второе пустое
  else if (!set2->elem) {
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  elem_t* temp1 = set1->elem, * temp2 = set2->elem, * newelem = NULL, * temp = set3->elem;
  elem_t* search1 = set3->elem, *search2 = NULL;

  while (temp1 != NULL && temp2 != NULL) {

    newelem = ElemCompare(temp1, temp2);

    if (newelem == temp1) {
      newelem = ElemCreateEqual(temp1);
      temp1 = temp1->next;
      search2 = set2->elem;
    }

    else if (newelem == temp2) {
      newelem = ElemCreateEqual(temp2);
      temp2 = temp2->next;
      search2 = set1->elem;
    }

    else { //элементы равны, не добавляем их
      temp1 = temp1->next;
      temp2 = temp2->next;
      free(newelem);
      newelem = NULL;
    }

    //добавляем newelem во множество с проверкой, что такого элемента нет во втором и третьем множестве
    search1 = set3->elem;
    while (newelem&&(search1||search2)) {
      if (IsElemsEqual(search1, newelem)||IsElemsEqual(search2, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      if(search1)
        search1 = search1->next;
      if (search2)
        search2 = search2->next;
    }
    //Если такого элемента во множествах нет, добавим его
    if (newelem) {
      if (temp == NULL) {
        set3->elem = newelem;
        temp = newelem;
      }
      else {
        temp->next = newelem;
        temp = temp->next;
      }
    }
  }

  //Дописываем хвост от одного из множеств с проверкой
  if (temp1 == NULL) {
    temp1 = temp2;
    search2 = set1->elem;
  }
  else
    search2 = set2->elem;
  

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //Проверка
    search1 = set3->elem;
    while (search1||search2) {
      if (IsElemsEqual(search1, newelem)|| IsElemsEqual(search2, newelem)) {
        free(newelem);
        newelem = NULL;
        break;
      }
      if(search1)
        search1 = search1->next;
      if(search2)
        search2 = search2->next;
    }

    temp1 = temp1->next;
    if (newelem) {
      temp->next = newelem;
      temp = temp->next;
    }
  }
  SetAddToList(set3);
  return set3;
}

//Проверка включения set1 в set2
bool_t SetCheckContain(set_t* set1, set_t* set2) {
  if (!set1 || !set2)
    return FALSE;

  elem_t* temp = set1->elem;
  elem_t* search = set2->elem;

  bool_t ELEM_EXISTS = FALSE;

  while (temp) {
    search = set2->elem;
    ELEM_EXISTS = FALSE;

    while (search) {
      if (IsElemsEqual(search, temp))
        ELEM_EXISTS = TRUE;

      search = search->next;
    }

    if (!ELEM_EXISTS)
      return FALSE;

    temp = temp->next;
  }
  return TRUE;
}

//Поиск множества под номером arg в команде из интерфейса
set_t* SetFindInStr(char* str, int arg, error_t* err) {
  char* cursor = str;
  int i = 0, comma = 0; //нужны вторые кавычки 
  set_t* set = NULL;
  while (i < WHOLESIZE) {

    if (*cursor == '"') {
      comma++;
      if (comma == arg) {
        set = SetFind(cursor);
        break;
      }
    }
    cursor++;
    i++;
  }

  if (!set) 
    *err = SET_DOES_NOT_EXIST;
  
  if (i == WHOLESIZE) 
    *err = MEMORY;
  
  if (*err)
    return NULL;

  return set;
}

//Возвращает имя нового множества (для команд объединения, пересечения и т.д.)
char* SetNewName(char* str) {
  char* name = (char*)malloc(NAMESIZE * sizeof(char));
  if (!name)
    return NULL;
  int j = 0;
  char* cursor = str;
  int i = 0, comma = 0; //нужны пятые кавычки 
  while (i < SIZE) {

    if (*cursor == '"') {
      comma++;
      if (comma == 5)
        break;
      
    }
    cursor++;
    i++;
  }
  //Ошибка
  if (i == SIZE) {
    free(name);
    return NULL;
  }
  j = i;
  for (i; i < (j+NAMESIZE); i++) {
    name[i-j] = str[i+1];
    if (i != j && name[i - j] == '"') {
      name[i - j] = '\0';
      break;
    }
    
  }
  return name;
}

//Добавление множества в список множеств
void SetAddToList(set_t* set) {
  if (tail == NULL) {
    list.head = set;
    tail = set;
  }
  else {
    tail->next = set;
    tail = tail->next;
  }
}

