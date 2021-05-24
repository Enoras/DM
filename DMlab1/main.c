#include "libSet.h"
#include <crtdbg.h> //��� �������� ������
#define _CRTDBG_MAP_ALLOC

int main(void) {

  char input[120];

  // ��������� ���������
  Start();

  bool_t ok = TRUE;
  // �������� ����
  do {
    fgets(input, 120, stdin);
    ok = Run(input);
  } while (ok);

  //�������� �� ������ ������
  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  return 0;
}
