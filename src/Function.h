#ifndef FUNCTION_H
#define FUNCTION_H

#include "Value.h"

//#include <stdlib.h>
//#include <cstring>
//#include <string>
//#include <iostream>
#include <functional>

namespace gve {

class Function {

  private:
    std::string _name;
    int _interval;
    bool _periodic;
    long _last;
    std::function<void(Value&)> _function;

  public:
    Function(const char* name){
      _name = name;
      _periodic = false;
      _interval = 0;
      _last = 0;
    };
    Function(const std::string& name){
      _name = name;
      _periodic = false;
      _interval = 0;
      _last = 0;
    };
    
    ~Function(){};
    
    operator bool(){
      return _function != nullptr;
    }

    std::string name(){
      return _name;
    }
    
    bool periodic() const{
      return _periodic;
    }
    
    long last() const{
      return _last;
    }
    
    long interval() const{
      return _interval;
    }
    
    void last(long i){
      _last = i;
    }
    
    void execute(Value& v) const{
      _function(v);
    }
    
    void operator()(Value& v){
      _function(v);
    }

    void operator=(std::function<void(Value&)> f){
      _function = f;
    }

    void operator=(int i){
      _interval = i;
    }

    friend Function& operator<< (Function& topic, int i){
      topic._interval = i;
      topic._periodic = true;
    }

    friend Function& operator<< (Function& topic, std::function<void(Value&)> f){
      topic._function = f;
    }

    friend Function& operator>> (Function& topic, std::function<void(Value&)> f){
      topic._function = f;
    }

    friend Function& operator<< (Function& topic, const char* msg){
      Value any;
      any = msg;
      topic._function(any);
    }

};

}

#endif