#include "busybox.hpp"
#include "unit.hpp"
namespace rcl{
std::vector<std::pair<unit,std::string> > make_dictionary_v(){
std::vector<std::pair<unit,std::string> > answer;

unit adim = unit();
answer.push_back(std::pair<unit,std::string>(adim,"adim"));
unit lenght = unit(1,0,0,0,0,0);
answer.push_back(std::pair<unit,std::string>(lenght,"longitud"));
unit mass = unit(0,0,1,0,0,0);
answer.push_back(std::pair<unit,std::string>(mass,"masa"));
unit time = unit(0,1,0,0,0,0);
answer.push_back(std::pair<unit,std::string>(time,"tiempo"));
unit charge = unit(0,0,0,0,1,0);
answer.push_back(std::pair<unit,std::string>(charge,"carga"));
unit speed = lenght/time;
answer.push_back(std::pair<unit,std::string>(speed,"velocidad"));
unit acceleration = speed/time;
answer.push_back(std::pair<unit,std::string>(acceleration,"aceleracion"));
unit momentum = mass*speed;
answer.push_back(std::pair<unit,std::string>(momentum,"momento"));
unit force = mass*acceleration;
answer.push_back(std::pair<unit,std::string>(force,"fuerza"));
unit energy = force*lenght;
answer.push_back(std::pair<unit,std::string>(energy,"energia"));
unit power = force*speed;
answer.push_back(std::pair<unit,std::string>(power,"potencia"));
unit surface = lenght*lenght;
answer.push_back(std::pair<unit,std::string>(surface,"superficie"));
unit presure = force/surface;
answer.push_back(std::pair<unit,std::string>(presure,"presion"));
unit volume = surface*lenght;
answer.push_back(std::pair<unit,std::string>(volume,"volumen"));
unit density = mass/volume;
answer.push_back(std::pair<unit,std::string>(density,"densidad"));
unit energy_density = energy/volume;
answer.push_back(std::pair<unit,std::string>(energy_density,"densidad_de_energia"));
unit charge_density = charge/volume;
answer.push_back(std::pair<unit,std::string>(charge_density,"densidad_de_carga"));
unit current_intensity = charge/time;
answer.push_back(std::pair<unit,std::string>(current_intensity,"intensidad"));
unit current = current_intensity / surface;
answer.push_back(std::pair<unit,std::string>(current,"corriente"));
unit displacement_field = charge_density*lenght;
answer.push_back(std::pair<unit,std::string>(displacement_field,"desplazamiento"));
unit electric_field = force/charge;
answer.push_back(std::pair<unit,std::string>(electric_field,"campo_electrico"));
unit electric_potential = energy/charge;
answer.push_back(std::pair<unit,std::string>(electric_potential,"potencial_electrico"));
unit electric_capacity = charge/electric_potential;
answer.push_back(std::pair<unit,std::string>(electric_capacity,"capacidad"));
unit impedance = electric_potential/current_intensity;
answer.push_back(std::pair<unit,std::string>(impedance,"impedancia"));
unit admitance = current_intensity/electric_potential;
answer.push_back(std::pair<unit,std::string>(admitance,"admitancia"));
unit electrical_induction= impedance*time;
answer.push_back(std::pair<unit,std::string>(electrical_induction,"induccion_electrica"));
unit magnetic_field = current*lenght;
answer.push_back(std::pair<unit,std::string>(magnetic_field,"campo_magnetico"));
unit magnetic_induction = force/speed/charge;
answer.push_back(std::pair<unit,std::string>(magnetic_induction,"induccion"));
unit magnetic_potential = magnetic_induction*lenght;
answer.push_back(std::pair<unit,std::string>(magnetic_potential,"potencial_magnetico"));
unit vacuum_permitivity = displacement_field/electric_field;
answer.push_back(std::pair<unit,std::string>(vacuum_permitivity,"permitividad"));
unit vacuum_permeability = magnetic_induction/magnetic_field;
answer.push_back(std::pair<unit,std::string>(vacuum_permeability,"permeabilidad"));
unit gravity_constant = acceleration*surface/mass;
answer.push_back(std::pair<unit,std::string>(gravity_constant,"constante_gravitacional"));
unit wave_number = adim/lenght;
answer.push_back(std::pair<unit,std::string>(wave_number,"numero_de_onda"));
unit frequency = adim/time;
answer.push_back(std::pair<unit,std::string>(frequency,"frecuencia"));

return answer;
}

std::map<std::string,unit> make_dictionary_s(const std::vector<std::pair<unit,std::string> >& dic){
  std::map<std::string,unit> answer;
  for(std::vector<std::pair<unit,std::string> >::const_iterator itr=dic.begin();itr!=dic.end();itr++)
  answer.insert(std::pair<std::string,unit>(itr->second,itr->first)); 
  return answer;
}

std::map<unit,std::string> make_dictionary_u(const std::vector<std::pair<unit,std::string> >& dic){
  std::map<unit,std::string> answer;
  for(std::vector<std::pair<unit,std::string> >::const_iterator itr=dic.begin();itr!=dic.end();itr++)
  answer.insert(std::pair<unit,std::string>(itr->first,itr->second)); 
  return answer;
}

const std::vector<std::pair<unit,std::string> >& dic_v = make_dictionary_v();
const std::map<std::string,unit>& dic_su = make_dictionary_s(make_dictionary_v());
const std::map<unit,std::string>& dic_us = make_dictionary_u(make_dictionary_v());

std::string to_exp(const std::string& str){
  std::string answer;
  for (std::string::const_iterator itr=str.begin();itr!=str.end();itr++){
    switch(*itr){
      case '0':
        answer+="⁰";
        break;
      case '1':
        answer+="¹";
        break;
      case '2':
        answer+="²";
        break;
      case '3':
        answer+="³";
        break;
      case '4':
        answer+="⁴";
        break;
      case '5':
        answer+="⁵";
        break;
      case '6':
        answer+="⁶";
        break;
      case '7':
        answer+="⁷";
        break;
      case '8':
        answer+="⁸";
        break;
      case '9':
        answer+="⁹";
        break;
      case '-':
        answer+="⁻";
        break;
      default:
        answer+=*itr;
        break;
      }
    }
  return answer;
}

std::string to_str(const unit& u){
  std::string answer;
  bool first_used = false;
  
  if (u[0]!=0) {
    answer+="L";
    first_used = true;
    std::stringstream ss;
    ss << u[0];
    if (u[0]!=1) answer+=to_exp(ss.str());
    }
  if (u[1]!=0) {
    if (first_used) answer+=" × ";
    first_used = true;
    answer+="T";
    std::stringstream ss;
    ss << u[1];
    if (u[1]!=1) answer+=to_exp(ss.str());
    }
  if (u[2]!=0) {
    if (first_used) answer+=" × ";
    first_used = true;
    answer+="M";
    std::stringstream ss;
    ss << u[2];
    if (u[2]!=1) answer+=to_exp(ss.str());
    }
  if (u[3]!=0) {
    if (first_used) answer+=" × ";
    first_used = true;
    answer+="θ";
    std::stringstream ss;
    ss << u[3];
    if (u[3]!=1) answer+=to_exp(ss.str());
    }
  if (u[4]!=0) {
    if (first_used) answer+=" × ";
    first_used = true;
    answer+="Q";
    std::stringstream ss;
    ss << u[4];
    if (u[4]!=1) answer+=to_exp(ss.str());
    }
  if (u[5]!=0) {
    if (first_used) answer+=" × ";
    first_used = true;
    answer+="Φ";
    std::stringstream ss;
    ss << u[5];
    if (u[5]!=1) answer+=to_exp(ss.str());
    }
    return answer;   
  }

void parse_string(std::map<std::string,unit>& bank,const std::string& expr){
  std::stringstream ss(expr);
  std::vector<std::string> stack;
  while (!ss.eof()){
    std::string chunk;
    ss >> chunk;
    stack.push_back(chunk);
    }
  enum type_of_statement{noop,print,assigment,list};
  type_of_statement type=noop;
  
  if (stack.size() > 0) if (stack[0]=="list") type = list;
  if (stack.size() > 0) if (stack[0]=="print") type = print;
  if (stack.size() > 1) if (stack[1]=="=") type = assigment;
  
  switch(type){
    case list:
    for (auto itr=bank.begin();itr!=bank.end();itr++) std::cout << itr->first << " ";
    std::cout << std::endl;
    break;
    case print:
    for (int idx=1;idx<stack.size();idx++){
      std::string& key = stack[idx];  
      unit value;  
      if (bank.find(key)!=bank.end()) {
        if (dic_us.find(bank[key])==dic_us.end()) std::cout << to_str(bank[key]) << " ";
        else                                      std::cout << dic_us.find(bank[key])->second << " ";}
      else std::cout << key << " ";
    }
    std::cout << std::endl;
    break;
    case assigment:
    unit answer;
    for (unsigned int idx=2;idx<stack.size();idx++){
    if (stack[idx]!="*" && stack[idx]!="/") 
      {
      if (stack[idx-1]=="/") answer/=bank[stack[idx]];
      else answer*=bank[stack[idx]];
      }
    }
    bank[stack[0]]=answer;
    break;
    }
  return;
}
}


int unit_calculator_main(int argc,char **argv){
  std::vector<std::string> args(argv,argv+argc);
  
  if (argc==1){
  bool run_loop=true;
  std::map<std::string,rcl::unit> bank(rcl::dic_su);
  while (run_loop){
    std::string input;
    std::getline(std::cin,input);
    if (input=="quit") {run_loop=false;}
    else parse_string(bank,input);
    }
    return 0;
  }
  if (args[1]=="testcmp"){
    std::vector<std::pair<rcl::unit,std::string> > dic(rcl::make_dictionary_v());
    std::vector<rcl::unit> vec;
    for (auto itr=dic.begin();itr!=dic.end();itr++) vec.push_back(itr->first);
    std::cout << "Generado vector de tamaño "<< vec.size() << " elementos" << std::endl;
    std::sort(vec.begin(),vec.end());
    std::cout << "Ordenado vector" << std::endl;
    for (unsigned int idx = 0;idx<vec.size();idx++)
      std::cout << idx+1 << " " << to_str(vec[idx]) << std::endl;
    return 0;
    }
  if (args[1]=="testdic"){
    std::cout << "dic_su size = " << rcl::dic_su.size() << std::endl;
    for (auto itr=rcl::dic_su.begin();itr!=rcl::dic_su.end();itr++)
      std::cout << itr->first << " >> " << rcl::to_str(itr->second) << std::endl;
    std::cout << " ==================================================" << std::endl;
    std::cout << "dic_us size = " << rcl::dic_us.size() << std::endl;
    for (auto itr=rcl::dic_us.begin();itr!=rcl::dic_us.end();itr++)
      std::cout << rcl::to_str(itr->first) << " >> " << itr->second << std::endl;
    std::cout << " ==================================================" << std::endl;
    for (auto itr=rcl::dic_su.begin();itr!=rcl::dic_su.end();itr++)
      std::cout << std::setw(24) << itr->first << " --  " << rcl::dic_us.find(itr->second)->second << std::endl;
    std::cout << " ==================================================" << std::endl;
    rcl::unit antiguo;
    for (auto itr=rcl::dic_us.begin();itr!=rcl::dic_us.end();itr++){
      std::cout  << antiguo;
      if (antiguo < itr->first)  std::cout << " <  ";
      else                       std::cout << " >= ";
      std::cout << itr->first << std::endl;
      antiguo = itr->first;
    }
    return 0;
  }
  return 0;
}
static rcl::reg_app reg("unit_calculator_tool",unit_calculator_main);
/*
for (auto itr=dic_su.begin();itr!=dic_su.end();itr++)
    std::cout << itr->first << " = "  << to_str(itr->second) << std::endl;
*/
