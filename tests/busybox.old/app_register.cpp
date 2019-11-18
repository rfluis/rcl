#define TEST_APP_REGISTER_MAIN
#include "app_register.hpp"

static Register_Caller app("Principal","Registrada aplicación principal");

int main(){
  GReg.show();
  app.get_reg().show();
  return 0;
}
