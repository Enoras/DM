#include "libSet.h"

 list_sets list;
 
//Вывод множества на экран
void SetPrint(set_t * set) {
  if (!set)
    return;

  elem_t* temp = set->elem;
  while (temp) {
    printf("\"%s\"\n", temp->str);
    temp = temp->next;
  }
}

set_t* tail = NULL;

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
  for (i = 0; i < NAME_SIZE; i++) {

    if (name[i] != '"' && name[i] != '\n') {
      set->name[j] = name[i];
      j++;
    }

    if (name[i] == '\n') {
      set->name[j] = '\0';
      i = NAME_SIZE; //Выход из цикла
    }
  }
  set->next = NULL;
  set->elem = NULL;

  //Проверим, что множества с таким именем больше нет
  temp = list.head;
  while (temp && EXISTS == FALSE) {
    EXISTS = TRUE;

    i = 0;
    while (temp->name[i] != '\0' || set->name[i] != '\0' || i <= NAME_SIZE) {

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
  while (i < SIZE2) {

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
  
  if (i == SIZE2) 
    *err = MEMORY;
  
  if (*err)
    return NULL;

  return set;
}

//Возвращает имя нового множества (для команд объединения, пересечения и т.д.)
char* SetNewName(char* str) {
  char* name = (char*)malloc(NAME_SIZE * sizeof(char));
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
  for (i; i < (j+NAME_SIZE); i++) {
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

bool_t Run(char* const str) {
    bool_t err = NO_ERROR;
    int size = 0;
    int i = 0, j = 0;
    command_t comandType = COMMAND_NOT_FOUND;
    char comands[NUM_COMANDS][COMMAND] = { "create", "add", "exit", "show"\
      ,"remove", "power", "contain", "union", "intersection", "difference",\
      "symmetricdifference", "help", "destroy", "exist" };
    int commandLength[NUM_COMANDS] = { 6,3,4,4,6,5,7,5,12,10,19,4,7,5 };

    if (str[i] != '/') { //начало команды

      //Не отвечаем на строки без команды
        return TRUE;
    }

    i = 1;

    while (str[i] != '\n' && str[i] != ' ') {
        i++;
        size++;
    }

    //Находим нужную команду из списка команд
    for (i = 0; i < NUM_COMANDS; i++) {
        for (j = 0; j < size; j++) {
            if (str[j + 1] != comands[i][j])
                j = size; //пропускаем эту команду
              //нашли команду
            else if (j == size - 1 && size == commandLength[i]) {
                comandType = i; //Запомнили нужную команду
                i = NUM_COMANDS;
            }
        }
    }

    //Неизвестная команда
    if (comandType == COMMAND_NOT_FOUND && str[size] != ' ' && str[size] != '\n')
        err = UNKNOWN_COMMAND;

    //Поиск по команде
    else if (comandType == EXIT) {
        ExitProgramm();
        return FALSE;
    }

    else if (comandType == CREATE)
        Create(str + size + 2, &err); //начинаем сразу с имени

    else if (comandType == SHOW)
        Show(str + size + 2, &err);

    else if (comandType == ADD)
        Add(str + size + 2, &err);

    else if (comandType == REMOVE)
        Remove(str + size + 2, &err);

    else if (comandType == POWER)
        Power(str + size + 2, &err);

    else if (comandType == CONTAIN)
        Contain(str + size + 2, &err);

    else if (comandType == DESTROY)
        Destroy(str + size + 2, &err);

    else if (comandType == UNION || comandType == INTERSECTION\
        || comandType == DIFFERENCE || comandType == SYMMETRICDIFFERENCE)
        SetAction(str + size + 2, &err, comandType);

    else if (comandType == HELP)
        Help(str + 1, &err);

    else if (comandType == EXIST)
        IsExists(str + size + 2, &err);

    if (err)
        ErrorMessage(err);

    return TRUE;

}

void Destroy(char* name, error_t* err) {
    int i;
    //Находим нужное множество и удаляем
    set_t* temp = SetFindInStr(name, 1, err);

    if (temp) {
        SetDestroy(temp);
        i = 1;
        printf("Set %c", name[0]);
        while (name[i] != '"') {
            printf("%c", name[i]);
            i++;
        }
        printf("%c was successfully destroyed!\n", name[i]);
    }
}

void ExitProgramm() {
    //Удалить каждое множество
    set_t* temp = list.head, * temp2 = NULL;
    printf("Exitting the programm...\n");

    if (temp)
        temp2 = temp->next;

    while (temp) {
        SetDestroy(temp);
        temp = temp2;
        if (temp2)
            temp2 = temp2->next;
    }
}

void ErrorMessage(error_t err) {
    if (err) {
        printf("ERROR: ");

        if (err == MEMORY)
            printf("not enough memory");
        else if (err == UNKNOWN_COMMAND)
            printf("unknown command");
        else if (err == INCORRECT_SYMBOL)
            printf("incorrect symbols detected");
        else if (err == SYNTAX_ERROR)
            printf("syntax error");
        else if (err == SET_DOES_NOT_EXIST)
            printf("a set with a such name does not exist");
        else if (err == ELEM_DOES_NOT_EXIST)
            printf("an elem with a such name does not exist");
        else if (err == ELEM_EXISTS)
            printf("an elem with a such name already exists in this set");
        else if (err == SET_EXISTS)
            printf("a set with a such name already exists");
        else
            printf("unexpected error");
        printf("\n");
    }
}

void Create(char* name, error_t* err) {
    //Получаем имя и создаем множество
    int i = 0, invertedcommas = 1;

    if (name[i] != '"') {
        *err = SYNTAX_ERROR;
        return;
    }
    i = 1;
    //Проверка имени
    while (name[i] != '\n' && i <= NAME_SIZE) {
        //недопустимый символ
        if (name[i] < ' ' || name[i]>'~') {
            *err = INCORRECT_SYMBOL;
            return;
        }
        else if (name[i] == '"' && name[i + 1] != '\n') {
            *err = INCORRECT_SYMBOL;
            return;
        }
        i++;

        if (name[i] == '"')
            invertedcommas++;
    }

    if (i >= NAME_SIZE) {
        *err = MEMORY;
        return;
    }

    else if (invertedcommas != 2) {
        *err = SYNTAX_ERROR;
        return;
    }

    //Если все в порядке, то создаем новое множество
    if (SetCreate(name, err) && !(*err)) {
        i = 1;
        printf("Set %c", name[0]);
        while (name[i] != '"') {
            printf("%c", name[i]);
            i++;
        }
        printf("%c was successfully created!\n", name[i]);
    }
}

void Add(char* str, error_t* err) {

    //найдем множество, в которое надо добавить
    set_t* set = SetFindInStr(str, 3, err);
    ElemAdd(set, str, err);
    int i;

    if (!(*err)) {
        i = 1;
        printf("Elem %c", str[0]);
        while (str[i] != '"') {
            printf("%c", str[i]);
            i++;
        }
        printf("%c was successfully added!\n", str[i]);
    }
}

void Show(char* str, error_t* err) {
    *err = NO_ERROR;
    //Показать список множеств
    if (str[0] == 'a' && str[1] == 'l' && str[2] == 'l' && str[3] == '\n') {
        SetAllPrint();
        return;
    }

    //Показать конкретное множество
    set_t* set = SetFind(str);
    if (!set) {
        *err = SET_DOES_NOT_EXIST;
        return;
    }
    printf("Showing all elements of set:\n");
    SetPrint(set);
}

void Remove(char* str, error_t* err) {
    //найдем множество, в которое надо добавить
    set_t* set = SetFindInStr(str, 3, err);
    elem_t* elem = ElemFind(set, str);
    int i;

    if (ElemRemove(set, elem)) {
        i = 1;
        printf("Elem %c", str[0]);
        while (str[i] != '"') {
            printf("%c", str[i]);
            i++;
        }
        printf("%c was successfully removed!\n", str[i]);
    }
}

void Power(char* str, error_t* err) {
    set_t* set = SetFind(str);
    if (set)
        printf("Power: %i\n", SetPower(set));
    else
        *err = SET_DOES_NOT_EXIST;

}

void Contain(char* str, error_t* err) {
    set_t* set1 = SetFindInStr(str, 1, err);
    set_t* set2 = SetFindInStr(str, 3, err);

    if (*err)
        return;

    bool_t CONTAIN = FALSE;

    elem_t* temp1 = set1->elem, * temp2 = set2->elem;

    while (temp1) {
        CONTAIN = FALSE;
        temp2 = set2->elem;

        while (temp2) {

            if (!ElemCompare(temp1, temp2)) {
                CONTAIN = TRUE;
                break;
            }

            temp2 = temp2->next;
        }

        if (!CONTAIN)
            break;

        temp1 = temp1->next;
    }

    if (CONTAIN)
        printf("Second set contains the first one!\n");
    else
        printf("Second set does not contain the first one :(\n");
}

void SetAction(char* str, error_t* err, int comand) {
    set_t* set1 = SetFindInStr(str, 1, err);
    set_t* set2 = SetFindInStr(str, 3, err);
    char* name = SetNewName(str); //без кавычек, \0 на конце
    int i = 0, j = 0;
    if (*err || !name) {
        if (name)
            free(name);
        else
            *err = SYNTAX_ERROR;
        return;
    }
    bool_t EXISTS = FALSE;
    set_t* set3 = NULL, * temp = NULL;

    //Проверим, что множества с таким именем больше нет
    temp = list.head;
    while (temp && EXISTS == FALSE) {
        EXISTS = TRUE;

        i = 0;
        while (temp->name[i] != '\0' || name[i] != '\0' || i <= NAME_SIZE) {

            if (temp->name[i] != name[i]) {
                EXISTS = FALSE;
                break;
            }

            else if (temp->name[i] == '\0' && name[i] == '\0') {
                EXISTS = TRUE;
                break;
            }
            i++;
        }
        if (EXISTS)
            break;

        temp = temp->next;
    }

    if (EXISTS == TRUE) {
        *err = SET_EXISTS;
        free(name);
        return;
    }

    //Выполняем действие со множеством
    if (comand == UNION)
        set3 = SetUnion(set1, set2);
    else if (comand == INTERSECTION)
        set3 = SetIntersection(set1, set2);
    else if (comand == DIFFERENCE)
        set3 = SetDifference(set1, set2);
    else if (comand == SYMMETRICDIFFERENCE)
        set3 = SetSymmetricDifference(set1, set2);

    i = 0;
    while ((name[i] != '\0' || j == 0) && i < NAME_SIZE) {
        if (name[i] != '"')
            set3->name[j++] = name[i];
        i++;
    }
    set3->name[j] = '\0';
    free(name);

    if (i == NAME_SIZE) {
        *err = SYNTAX_ERROR;
        SetDestroy(set3);
        return;
    }
    printf("A set was successfully created!\n");
}

void Start() {
    printf("/help for get information about command\n");
}

void Help(char* str, error_t* err) {
    printf("************************************************************************************************************************\n");
    printf("/create \"name\" - create a set with a name \"name\".\n");
    printf("/add \"string\" \"set\" - add a string to set. There can not be >1 identical strings in one set\n");
    printf("/destroy \"set\" - destroys a set\n");
    printf("/remove \"string\" \"set\" - remove a string from set\n");
    printf("/show \"set\" - show all elements in the set\n");
    printf("/show all - show names of all sets\n");
    printf("/power \"set\" - show power of the set\n");
    printf("/contain \"set1\" \"set2\" - check if set2 contains set1 or not\n");
    printf("/union \"set1\" \"set2\" \"name\" - create a set with name as an union of set1 and set2\n");
    printf("/intersection \"set1\" \"set2\" \"name\" - create a set with name as an intersection of set1 and set2\n");
    printf("/difference \"set1\" \"set2\" \"name\" - create a set with name as a difference of set1 and set2\n");
    printf("/symmetricdifference \"set1\" \"set2\" \"name\" - create a set with name as a symmetric difference of set1 and set2\n");
    printf("/exist \"string\" \"set\" - check if string exists in this set\n");
    printf("/exit - delete all sets and close the programm\n");
    printf("************************************************************************************************************************\n");
}

void IsExists(char* str, error_t* err) {
    //найдем множество, в котором надо найти элемент
    set_t* set = SetFindInStr(str, 3, err);

    if (ElemFind(set, str))
        printf("Elem exists in this set!\n");
    else if (*err)
        return;
    else
        printf("Elem does not exist in this set\n");
}

// Сравнивает два элемента
elem_t* ElemCompare(elem_t* elem1, elem_t* elem2) {

    int i, len1 = 0, len2 = 0;
    while (elem1->str[len1] != '\0')
        len1++;
    while (elem1->str[len2] != '\0')
        len2++;

    for (i = 0; (i < SIZE && MAX(elem1->str[i], elem2->str[i]) != '\0'); i++) {

        if (elem1->str[i] < elem2->str[i])
            return elem1;
        else if (elem1->str[i] > elem2->str[i])
            return elem2;
    }

    if (i != SIZE && elem1->str[i] == '\0' && elem2->str[i] != '\0')
        return elem1;
    else if (i != SIZE && elem2->str[i] == '\0' && elem1->str[i] != '\0')
        return elem2;

    return NULL; //Строки равны
}

//Добавить элемент в существующее множество
bool_t ElemAdd(set_t* set, char* str, error_t* error) {
    if (!set) //множество не существует
        return FALSE;

    elem_t* temp = set->elem, * temp2 = temp;
    int i = 0;

    elem_t* newelem = (elem_t*)malloc(sizeof(elem_t));
    if (!newelem)
        return FALSE;

    if (str[i] != '"') {
        *error = SYNTAX_ERROR;
        free(newelem);
        return FALSE;
    }

    for (i = 1; i <= SIZE; ++i) {
        if (str[i] < ' ' || str[i]>'~') {//недопустимый символ
            *error = INCORRECT_SYMBOL;
            free(newelem);
            return FALSE;
        }
        //Должна быть заканчивающая строку кавычка
        else if (str[i] == '"') {
            if (str[i + 1] == '\n' || str[i + 1] == ' ') { //конец строки
                newelem->str[i - 1] = '\0';
                break;
            }
            else {
                *error = INCORRECT_SYMBOL;
                free(newelem);
                return FALSE;
            }
        }
        else if (i > SIZE) {
            *error = MEMORY;
            free(newelem);
            return FALSE;
        }
        else
            newelem->str[i - 1] = str[i];
    }

    if (i > SIZE) {
        *error = MEMORY;
        free(newelem);
        return FALSE;
    }

    newelem->next = NULL;
    //Множество пустое
    if (set->elem == NULL)
        set->elem = newelem;

    //Множество непустое
    else {

        //Находим куда добавить элемент

        //Добавить в начало?
        if (ElemCompare(temp, newelem) == newelem) {
            newelem->next = temp;
            set->elem = newelem;
            return TRUE;
        }
        //Добавляем в середину/конец
        else if (ElemCompare(temp, newelem) == temp) {

            while (temp && (ElemCompare(temp, newelem) == temp)) {
                temp2 = temp;
                temp = temp->next;
            }

            //могли выйти из цикла, потому что нашли элемент с таким же названием
            if (temp && (ElemCompare(temp, newelem) == NULL)) {
                *error = ELEM_EXISTS;
                free(newelem);
                return FALSE;
            }

            newelem->next = temp;
            temp2->next = newelem;
        }

        else { //Элементы одинаковые -> нельзя добавлять такой элемент
            *error = ELEM_EXISTS;
            free(newelem);
        }
    }
    return TRUE;
}

//удалить элемент из множества
bool_t ElemRemove(set_t* set, elem_t* elem) {
    if (!elem || !set)
        return FALSE;

    elem_t* temp = set->elem, * temp2 = NULL;

    //удаляем первый элемент
    if (temp == elem) {
        temp2 = set->elem->next;
        free(temp);
        set->elem = temp2;
        return TRUE;
    }

    //Удаляем не первый элемент
    else {
        while (temp->next != elem)
            temp = temp->next;

        temp2 = temp->next->next;
        free(temp->next);
        temp->next = temp2;
        return TRUE;
    }
}

//Проверка, принадлежит ли элемент множеству
elem_t* ElemFind(set_t* set, char* str) {
    if (!set)
        return NULL;

    elem_t* temp = set->elem;
    int i = 0;
    while (temp) {

        for (i = 0; i < SIZE; i++) {
            if ((temp->str[i] != str[i + 1])) {

                if (temp->str[i] == '\0' && str[i + 1] == '"')
                    i = (SIZE - 1);

                break;
            }

            if (str[i + 1] == '\n' || str[i + 1] == '"') {
                i = (SIZE - 1);
                break;
            }
        }
        if (i == (SIZE - 1))
            return temp;

        temp = temp->next;
    }
    return NULL;
}

//Проверка, равны ли два элемента
bool_t IsElemsEqual(elem_t* elem1, elem_t* elem2) {
    if (!elem1 || !elem2)
        return FALSE;

    int i = 0;
    for (i = 0; i < SIZE; i++) {
        if (elem1->str[i] != elem2->str[i])
            return FALSE;
        if (elem1->str[i] == '\n')
            i = SIZE;
    }
    return TRUE;
}

//Сделать копию элемента
elem_t* ElemCreateEqual(elem_t* elem) {
    int i = 0;
    elem_t* newelem = (elem_t*)malloc(sizeof(elem_t));
    if (!newelem)
        return NULL;

    for (i = 0; i < SIZE; i++) {
        newelem->str[i] = elem->str[i];
        if (elem->str[i] == '\n')
            i = SIZE;
    }
    newelem->next = NULL;
    return newelem;
}