//
// Created by nikita on 14.05.18.
//

#include <iostream>
#include <set>
#include <string>



#define DECLARE_METHOD( caller, method ) \
method_str = #caller + (std::string)"::" + #method; \
if (caller::get_par_static()) { \
    caller::func_box_self.insert(method_str); \
} else \
    caller::func_box_ch.insert(method_str); \


#define VIRTUAL_CALL_ERROR(method_str) \
std::cout << "ERROR : method '" << method_str << "' not found!\n"; \


#define VIRTUAL_CALL(caller, method) \
method_str = (*caller).name_self + "::" + #method; \
if(!((*caller).get_par_field())) { \
    if((*caller).func_is_found_ch(method_str)) { \
        std::cout << method_str << "  a = " << (*caller).a << std::endl; \
    } else { \
        method_str = (*caller).name_par + "::" + #method; \
        if((*caller).func_is_found_par(method_str)) { \
            std::cout << method_str << "  a = " << (*caller).a << std::endl; \
        } else \
            VIRTUAL_CALL_ERROR(#method); \
    } \
} else { \
    if((*caller).func_is_found_par(method_str)) { \
        std::cout << method_str << "  a = " << (*caller).a << std::endl; \
    } else \
        VIRTUAL_CALL_ERROR(#method); \
} \


#define VIRTUAL_CLASS(Base) \
class Base { \
public: \
    Base(std::string caller_name = #Base, std::string caller_par_name = #Base, bool if_is_par = true) { \
        name_self = caller_name; name_par = caller_par_name; is_par_field = if_is_par; \
    }; \
    bool func_is_found_par(std::string method_str){ \
        if(func_box_self.find(method_str) != func_box_self.end()) \
            return true; \
    }; \
    bool func_is_found_ch(std::string method_str){ \
        if(func_box_ch.find(method_str) != func_box_ch.end()) \
            return true; \
    } \
    static bool get_par_static() { return is_par_static; } \
    bool get_par_field() {return is_par_field; } \
    static bool is_par_static; \
    bool is_par_field; \
    static std::set<std::string> func_box_self; \
    static std::set<std::string> func_box_ch; \
    std::string name_self; \
    std::string name_par; \


#define END(Base) \
}; \
bool Base::is_par_static = true; \
std::set<std::string> Base::func_box_self, Base::func_box_ch; \


#define VIRTUAL_CLASS_DERIVED(Derived, Base) \
class Derived : public Base { \
public: \
    Derived() : Base(#Derived, #Base, false){}; \
    static bool get_par_static(){ return is_par_static; } \
    std::string name_par = #Base; \
    std::string name_self = #Derived; \
    static bool is_par_static; \


#define END_DERIVE(Derived, Base) \
}; \
bool Derived::is_par_static = false; \







std::string method_str;

// базовый класс
VIRTUAL_CLASS( Base )
    int a;
END( Base )

// класс-наследник
VIRTUAL_CLASS_DERIVED( Derived, Base )
    int b;
END_DERIVE( Derived, Base )


int main() {
    DECLARE_METHOD( Base, Both )
    DECLARE_METHOD( Base, OnlyBase )
    DECLARE_METHOD( Derived, Both )
    DECLARE_METHOD( Derived, OnlyDerived )
    Base base; /* как-то создали базовый класс */
    base.a = 0; // работаем как со структурой
    Derived derived;/* ... как-то создали наследник */
    derived.a = 1;

    // полиморфизм
    Base* reallyDerived = reinterpret_cast<Base*>(&derived);

    VIRTUAL_CALL(&base, Both); // печатает “Base::Both a = 0”
    VIRTUAL_CALL(reallyDerived, Both); // печатает “Derived::Both b = 1”
    VIRTUAL_CALL(reallyDerived, OnlyBase);  // печатает “Base::OnlyBase”
    VIRTUAL_CALL(reallyDerived, OnlyDerived);
    return 0;
}