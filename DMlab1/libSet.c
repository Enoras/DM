#include "libSet.h"

 list_sets list;
 
//����� ��������� �� �����
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

//����� ���� �������� �� �����
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

//�������� ��������� (������ � ���������)
set_t* SetCreate(char* name, error_t* err) {
  int i = 0, j=0;
  set_t* set = (set_t*)malloc(sizeof(set_t));
  if (!set) {
    *err = MEMORY;
    return NULL;
  }
  bool_t EXISTS = FALSE;
  set_t* temp = list.head;
  //��� ���������
  for (i = 0; i < NAME_SIZE; i++) {

    if (name[i] != '"' && name[i] != '\n') {
      set->name[j] = name[i];
      j++;
    }

    if (name[i] == '\n') {
      set->name[j] = '\0';
      i = NAME_SIZE; //����� �� �����
    }
  }
  set->next = NULL;
  set->elem = NULL;

  //��������, ��� ��������� � ����� ������ ������ ���
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

  //��������� ����� ��������� � ������ ��������
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

  //����� �� �����, �� �� ����� ���������
  return NULL;
}

//�������� ������������� ��������
void SetDestroy(set_t* set) {
  if (!set)
    return;

  elem_t* temp = set->elem, *temp2 = set->elem;
  set_t* findset = list.head;

  //������� ������
  if (findset == set) 
    list.head = findset->next;

  //������� ������, ������ � �.�...
  else {
    while (findset->next != set)
      findset = findset->next;

    //���� ������� ��������� ���������, �� ��������� tail �� �����
    if (set->next == NULL)
      tail = findset;

    //�� ������ �����
    findset->next = findset->next->next;
  }
  //������� ���������
  while (temp2) {
    temp2 = temp2->next;
    free(temp);
    temp = temp2;
  }
  if(temp)
    free(temp);
  free(set);
}

//������� �������� ���������
int SetPower(set_t* set) {
  int power = 0;
  elem_t* temp = set->elem;
  while (temp) {
    temp = temp->next;
    power++;
  }
  return power;
}

//����������� ��������� (��������������� �������)
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

//����������� ��������
set_t* SetUnion(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  elem_t* temp1 = NULL, * temp2 = NULL, * newelem = NULL, * temp = set3->elem;
  elem_t* search = NULL;

  //����� ��������� �� ���������� � ����������
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //������ ��������� ������
  else if (!set1->elem) {

    //������ ��������� ��� ����������, ��� ������
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;//������� ������ ���������
    }
    //������� ������ ���������, ���� ��� �� ������ 
    else {
      free(set3);
      set3 = SetCopy(set2);
      SetAddToList(set3);
      return set3;
    }
  }

  //������ ��������� ������
  else if (!set2->elem) {

    //���� ������ ���� ������, �� ����
    //������ ���������� ����
    free(set3);
    set3 = SetCopy(set1);
    SetAddToList(set3);
    return set3;
  }

  //��� ��������� ���������� � ��������
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

    else { //�������� �����, ��������� ������ ����
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
    //���� ������ �������� �� ��������� ���, ������� ���
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

  //���������� ����� �� ������ �� �������� � ���������
  if (temp1 == NULL)
    temp1 = temp2;

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //��������
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

  //������� ��������� � ������ ��������
  SetAddToList(set3);
  return set3;
}

//����������� ��������
set_t* SetIntersection(set_t* set1, set_t* set2) {
  set_t* set3 = (set_t*)malloc(sizeof(set_t));
  if (!set3)
    return NULL;

  set3->elem = NULL;
  set3->next = NULL;

  //���� �����-�� ��������� �� ����������
  if (!set1 || !set2) {
    free(set3);
    return NULL;
  }

  //���� �����-���� ��������� ������
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

//�������� ��������
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
  //������ ��������� ������
  else if (!set1->elem) {
    SetAddToList(set3);
    return set3;
  }

  //������ ��������� ������
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

//�������������� �������� ��������
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

  //������ ��������� ������
  if (!set1->elem) {
    //������ ��������� ������
    if (!set2->elem) {
      SetAddToList(set3);
      return set3;
    }
    
    //������ ��������
    free(set3);
    set3 = SetCopy(set2);
    SetAddToList(set3);
    return set3;
  }

  //������ ������
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

    else { //�������� �����, �� ��������� ��
      temp1 = temp1->next;
      temp2 = temp2->next;
      free(newelem);
      newelem = NULL;
    }

    //��������� newelem �� ��������� � ���������, ��� ������ �������� ��� �� ������ � ������� ���������
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
    //���� ������ �������� �� ���������� ���, ������� ���
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

  //���������� ����� �� ������ �� �������� � ���������
  if (temp1 == NULL) {
    temp1 = temp2;
    search2 = set1->elem;
  }
  else
    search2 = set2->elem;
  

  while (temp1) {
    newelem = ElemCreateEqual(temp1);

    //��������
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

//�������� ��������� set1 � set2
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

//����� ��������� ��� ������� arg � ������� �� ����������
set_t* SetFindInStr(char* str, int arg, error_t* err) {
  char* cursor = str;
  int i = 0, comma = 0; //����� ������ ������� 
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

//���������� ��� ������ ��������� (��� ������ �����������, ����������� � �.�.)
char* SetNewName(char* str) {
  char* name = (char*)malloc(NAME_SIZE * sizeof(char));
  if (!name)
    return NULL;
  int j = 0;
  char* cursor = str;
  int i = 0, comma = 0; //����� ����� ������� 
  while (i < SIZE) {

    if (*cursor == '"') {
      comma++;
      if (comma == 5)
        break;
      
    }
    cursor++;
    i++;
  }
  //������
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

//���������� ��������� � ������ ��������
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

    if (str[i] != '/') { //������ �������

      //�� �������� �� ������ ��� �������
        return TRUE;
    }

    i = 1;

    while (str[i] != '\n' && str[i] != ' ') {
        i++;
        size++;
    }

    //������� ������ ������� �� ������ ������
    for (i = 0; i < NUM_COMANDS; i++) {
        for (j = 0; j < size; j++) {
            if (str[j + 1] != comands[i][j])
                j = size; //���������� ��� �������
              //����� �������
            else if (j == size - 1 && size == commandLength[i]) {
                comandType = i; //��������� ������ �������
                i = NUM_COMANDS;
            }
        }
    }

    //����������� �������
    if (comandType == COMMAND_NOT_FOUND && str[size] != ' ' && str[size] != '\n')
        err = UNKNOWN_COMMAND;

    //����� �� �������
    else if (comandType == EXIT) {
        ExitProgramm();
        return FALSE;
    }

    else if (comandType == CREATE)
        Create(str + size + 2, &err); //�������� ����� � �����

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
    //������� ������ ��������� � �������
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
    //������� ������ ���������
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
    //�������� ��� � ������� ���������
    int i = 0, invertedcommas = 1;

    if (name[i] != '"') {
        *err = SYNTAX_ERROR;
        return;
    }
    i = 1;
    //�������� �����
    while (name[i] != '\n' && i <= NAME_SIZE) {
        //������������ ������
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

    //���� ��� � �������, �� ������� ����� ���������
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

    //������ ���������, � ������� ���� ��������
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
    //�������� ������ ��������
    if (str[0] == 'a' && str[1] == 'l' && str[2] == 'l' && str[3] == '\n') {
        SetAllPrint();
        return;
    }

    //�������� ���������� ���������
    set_t* set = SetFind(str);
    if (!set) {
        *err = SET_DOES_NOT_EXIST;
        return;
    }
    printf("Showing all elements of set:\n");
    SetPrint(set);
}

void Remove(char* str, error_t* err) {
    //������ ���������, � ������� ���� ��������
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
    char* name = SetNewName(str); //��� �������, \0 �� �����
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

    //��������, ��� ��������� � ����� ������ ������ ���
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

    //��������� �������� �� ����������
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
    //������ ���������, � ������� ���� ����� �������
    set_t* set = SetFindInStr(str, 3, err);

    if (ElemFind(set, str))
        printf("Elem exists in this set!\n");
    else if (*err)
        return;
    else
        printf("Elem does not exist in this set\n");
}

// ���������� ��� ��������
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

    return NULL; //������ �����
}

//�������� ������� � ������������ ���������
bool_t ElemAdd(set_t* set, char* str, error_t* error) {
    if (!set) //��������� �� ����������
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
        if (str[i] < ' ' || str[i]>'~') {//������������ ������
            *error = INCORRECT_SYMBOL;
            free(newelem);
            return FALSE;
        }
        //������ ���� ������������� ������ �������
        else if (str[i] == '"') {
            if (str[i + 1] == '\n' || str[i + 1] == ' ') { //����� ������
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
    //��������� ������
    if (set->elem == NULL)
        set->elem = newelem;

    //��������� ��������
    else {

        //������� ���� �������� �������

        //�������� � ������?
        if (ElemCompare(temp, newelem) == newelem) {
            newelem->next = temp;
            set->elem = newelem;
            return TRUE;
        }
        //��������� � ��������/�����
        else if (ElemCompare(temp, newelem) == temp) {

            while (temp && (ElemCompare(temp, newelem) == temp)) {
                temp2 = temp;
                temp = temp->next;
            }

            //����� ����� �� �����, ������ ��� ����� ������� � ����� �� ���������
            if (temp && (ElemCompare(temp, newelem) == NULL)) {
                *error = ELEM_EXISTS;
                free(newelem);
                return FALSE;
            }

            newelem->next = temp;
            temp2->next = newelem;
        }

        else { //�������� ���������� -> ������ ��������� ����� �������
            *error = ELEM_EXISTS;
            free(newelem);
        }
    }
    return TRUE;
}

//������� ������� �� ���������
bool_t ElemRemove(set_t* set, elem_t* elem) {
    if (!elem || !set)
        return FALSE;

    elem_t* temp = set->elem, * temp2 = NULL;

    //������� ������ �������
    if (temp == elem) {
        temp2 = set->elem->next;
        free(temp);
        set->elem = temp2;
        return TRUE;
    }

    //������� �� ������ �������
    else {
        while (temp->next != elem)
            temp = temp->next;

        temp2 = temp->next->next;
        free(temp->next);
        temp->next = temp2;
        return TRUE;
    }
}

//��������, ����������� �� ������� ���������
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

//��������, ����� �� ��� ��������
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

//������� ����� ��������
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