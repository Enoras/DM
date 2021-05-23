#include "libSet.h"
#include <crtdbg.h> //��� �������� ������
#define _CRTDBG_MAP_ALLOC

int main(void) {
  bool_t ok = TRUE;
  char arr[120];

  // ��������� ���������
  Start();

  // �������� ����
  do {
    fgets(arr, 120, stdin);
    ok = Run(arr);
  } while (ok);

  //�������� �� ������ ������
  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  return 0;
}
