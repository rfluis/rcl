#include "busybox.hpp"

char *list1[]={"",""};
char *list2[]={"executable-name","This","should","return","thirty","three"};

reg_tester list_items();
reg_tester test_cmd1("command1",128,list1);
reg_tester test_cmd2("command2",6,list2);
