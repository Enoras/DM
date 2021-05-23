#include "header.h"

extern list_of_sets list;

bool_t Run(char* const str) {
  bool_t err = NO_ERROR;
  int size = 0;
  int i = 0, j = 0;
  command_t comandType = COMMAND_NOT_FOUND;
  char comands[ALL_COMANDS][COMMANDSIZE] = { "create", "add", "exit", "show"\
    ,"remove", "power", "contain", "union", "intersection", "difference",\
    "symmetricdifference", "help", "destroy", "exist"};
  int commandLength[ALL_COMANDS] = {6,3,4,4,6,5,7,5,12,10,19,4,7,5};

  if (str[i] != '/') { //начало команды

    //Ќе отвечаем на строки без команды
    return TRUE;
  }

  i = 1;
 
  while (str[i]!='\n' && str[i]!=' ') {
    i++;
    size++;
  }

  //Ќаходим нужную команду из списка команд
  for (i = 0; i < ALL_COMANDS; i++) {
    for (j = 0; j < size; j++) {
      if (str[j + 1] != comands[i][j])
        j = size; //пропускаем эту команду
      //нашли команду
      else if (j == size - 1 && size == commandLength[i]) {
        comandType = i; //«апомнили нужную команду
        i = ALL_COMANDS;
      }
    }
  }

  //Ќеизвестна€ команда
  if (comandType == COMMAND_NOT_FOUND && str[size] != ' ' && str[size] != '\n')
    err = UNKNOWN_COMMAND;

  //ѕоиск по команде
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
    Help(str+1, &err);

  else if (comandType == EXIST)
    IsExists(str + size + 2, &err);

    if (err)
      ErrorMessage(err);

    return TRUE;
  
}

void Destroy(char* name, error_t* err) {
  int i;
  //Ќаходим нужное множество и удал€ем
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
  //”далить каждое множество
  set_t* temp = list.head, *temp2 = NULL;
  printf("Exitting the programm...\n");

  if (temp)
    temp2 = temp->next;

  while (temp) {
    SetDestroy(temp);
    temp = temp2;
    if(temp2)
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
  //ѕолучаем им€ и создаем множество
  int i = 0, invertedcommas=1;

  if (name[i] != '"') {
    *err = SYNTAX_ERROR;
    return;
  }
  i = 1;
  //ѕроверка имени
  while (name[i] != '\n' && i<=NAMESIZE) {
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

  if (i >= NAMESIZE) {
    *err = MEMORY;
    return;
  }

  else if (invertedcommas != 2) {
    *err = SYNTAX_ERROR;
    return;
  }

  //≈сли все в пор€дке, то создаем новое множество
  if (SetCreate(name, err)&&!(*err)){
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
  //ѕоказать список множеств
  if (str[0] == 'a' && str[1] == 'l' && str[2] == 'l' && str[3] == '\n') {
    SetAllPrint();
    return;
  }

  //ѕоказать конкретное множество
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
    *err = SYNTAX_ERROR;
  
}

void Contain(char* str, error_t* err) {
  set_t* set1 = SetFindInStr(str,1,err);
  set_t* set2 = SetFindInStr(str,3,err);

  if (*err)
    return;

  bool_t CONTAIN = FALSE;

  elem_t* temp1 = set1->elem, *temp2 = set2->elem;

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

  if(CONTAIN)
    printf("Second set contains the first one!\n");
  else
    printf("Second set does not contain the first one :(\n");
}

void SetAction(char* str, error_t* err, int comand) {
  set_t* set1 = SetFindInStr(str, 1, err);
  set_t* set2 = SetFindInStr(str, 3, err);
  char* name = SetNewName(str); //без кавычек, \0 на конце
  int i = 0, j=0;
  if (*err || !name) {
    if (name)
      free(name);
    else
      *err = SYNTAX_ERROR;
    return;
  }
  bool_t EXISTS = FALSE;
  set_t* set3 = NULL, *temp = NULL;

  //ѕроверим, что множества с таким именем больше нет
  temp = list.head;
  while (temp && EXISTS == FALSE) {
    EXISTS = TRUE;

    i = 0;
    while (temp->name[i] != '\0' || name[i] != '\0' || i <= NAMESIZE) {

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

  //¬ыполн€ем действие со множеством
  if (comand == UNION)
    set3 = SetUnion(set1, set2);
  else if (comand == INTERSECTION)
    set3 = SetIntersection(set1, set2);
  else if (comand == DIFFERENCE)
    set3 = SetDifference(set1, set2);
  else if (comand == SYMMETRICDIFFERENCE)
    set3 = SetSymmetricDifference(set1, set2);

  i = 0;
  while ((name[i] != '\0' || j == 0) && i < NAMESIZE) {
    if (name[i] != '"')
      set3->name[j++] = name[i];
    i++;
  }
  set3->name[j] = '\0';
  free(name);

  if (i == NAMESIZE) {
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
  printf("/exist \"string\" \"set\" - check if string exists in this set");
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