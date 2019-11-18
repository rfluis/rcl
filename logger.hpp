#ifndef RCL_LOGGER
#define RCL_LOGGER

#include "tpoint.hpp"

namespace rcl{
  class logger_entry{
  private:
    std::string sourcefile;
    std::string sourceline;
    std::string kind;
    std::string content;
    std::string timestamp;
  public:
    logger_entry(){return;}
    logger_entry(const std::string& sourcefile,const std::string& sourceline,const std::string& kind,const std::string& content,const std::string& timestamp){
      this->sourcefile=sourcefile;
      this->sourceline=sourceline;
      this->kind=kind;
      this->content=content;
      this->timestamp=timestamp;
      return;
      }
    logger_entry(const rcl::spoint& addr,const rcl::spoint& event,const std::string& timestamp){
      logger_entry(addr.x(),addr.y(),event.x(),event.y(),timestamp);
      return;
      } 
    
    std::string& get_sourcefile(){return sourcefile;}
    std::string& get_sourceline(){return sourceline;}
    std::string& get_kind(){return kind;}
    std::string& get_content(){return content;}
    std::string& get_timestamp(){return timestamp;}
    
    const std::string& get_sourcefile()const{return sourcefile;}
    const std::string& get_sourceline()const{return sourceline;}
    const std::string& get_kind()const{return kind;}
    const std::string& get_content()const{return content;}
    const std::string& get_timestamp()const{return timestamp;}

  };
}

#endif
