#ifndef TETSFIXEDRECORD_HPP
#define TETSFIXEDRECORD_HPP
#include "test_runner.hpp"
#include "profile.hpp"
#include "../HLAfixedRecod.hpp"
#include "../HLAstring.hpp"
#include "../HLAtypes.hpp"
#include "../HLAvector.hpp"

struct Position{
    double x = 0;
    double y = 0;
};

class HLAPosition : public HLA::BaseFixedRecord<Position,8>{
public:
    void getDataMax(void *ptrSource, unsigned long uiMaxSize){
        HLA::Float64BE field0;
        HLA::Float64BE field1;
        unsigned offset = 0;
        auto_offset(offset,ptrSource,uiMaxSize,field0,field1);
    }

    void get(const Position &obj){
        HLA::Float64BE field0;
        HLA::Float64BE field1;
        unsigned offset = 0, uiSize;
        m_uiSizeData=0;
        auto_geter_first(offset,uiSize,field0,obj.x,field1,obj.y);
        auto_geter_second(offset,uiSize,field0,field1);
    }

    void set(Position &obj){
        HLA::Float64BE field0;
        HLA::Float64BE field1;
        unsigned offset = 0, uiSize;
        auto_seter(offset,uiSize,field0,obj.x,field1,obj.y);
    }
};

void TestPosition(){
    Position pos1{4.0,2.2};
    rti1516e::VariableLengthData v1 = HLA::cast_to_rti<HLAPosition>(pos1);
    Position pos2 = HLA::cast_from_rti<HLAPosition>(v1);
    ASSERT_EQUAL(pos1.x,pos2.x)
    ASSERT_EQUAL(pos1.y,pos2.y)

}

using HLAPosition_ = HLA::Struct_wrapper<Position, 8, HLA::Float64BE, HLA::Float64BE>;

void TestPosition_(){
    Position pos1{4.0,2.2};
    rti1516e::VariableLengthData v1 = HLA::cast_to_rti<HLAPosition_>(pos1);
    Position pos2 = HLA::cast_from_rti<HLAPosition_>(v1);
    ASSERT_EQUAL(pos1.x,pos2.x)
    ASSERT_EQUAL(pos1.y,pos2.y)

}

struct Book{
    int pages = 0;
    std::string name;
};

class HLABook : public HLA::BaseFixedRecord<Book,8>{
public:
    void getDataMax(void *ptrSource, unsigned long uiMaxSize){
        HLA::Integer32BE field0;
        HLA::String field1;
        unsigned offset = 0;
        auto_offset(offset,ptrSource,uiMaxSize,field0,field1);
    }

    void get(const Book &obj){
        HLA::Integer32BE field0;
        HLA::String field1;
        unsigned offset = 0, uiSize;
        m_uiSizeData=0;
        auto_geter_first(offset,uiSize,field0,obj.pages,field1,obj.name);
        auto_geter_second(offset,uiSize,field0,field1);
    }

    void set(Book &obj){
        HLA::Integer32BE field0;
        HLA::String field1;
        unsigned offset = 0, uiSize;
        auto_seter(offset,uiSize,field0,obj.pages,field1,obj.name);
    }
};

void TestBook(){
    Book book1 = {784,"C++ Templates"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLABook>(book1);
    Book book2 = HLA::cast_from_rti<HLABook>(v);
    ASSERT_EQUAL(book1.name,book2.name)
    ASSERT_EQUAL(book1.pages,book2.pages)

}

using HLABook_ = HLA::Struct_wrapper<Book, 4, HLA::Integer32BE, HLA::String>;

void TestBook_(){
    Book book1 = {784,"C++ Templates"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLABook_>(book1);
    Book book2 = HLA::cast_from_rti<HLABook_>(v);
    ASSERT_EQUAL(book1.name,book2.name)
    ASSERT_EQUAL(book1.pages,book2.pages)

}

struct Juice{
    int volume = 0;
    double price = 0;
    std::string type = "";
};

class HLAJuice : public HLA::BaseFixedRecord<Juice,8>{
public:
    void getDataMax(void *ptrSource, unsigned long uiMaxSize){
        HLA::Float64BE field0;
        HLA::Integer32BE field1;
        HLA::String field2;
         unsigned offset = 0;
         auto_offset(offset,ptrSource,uiMaxSize,field0,field1,field2);
    }

    void get(const Juice &obj){
        HLA::Float64LE field0;
        HLA::Integer32BE field1;
        HLA::String field2;
        unsigned offset = 0, uiSize;
        m_uiSizeData=0;
        auto_geter_first(offset,uiSize,field0,obj.price,field1,obj.volume,field2,obj.type);
        auto_geter_second(offset,uiSize,field0,field1,field2);
    }

    void set(Juice &obj){
        HLA::Float64LE field0;
        HLA::Integer32BE field1;
        HLA::String field2;
        unsigned offset = 0, uiSize;
        auto_seter(offset,uiSize,field0,obj.price,field1,obj.volume, field2, obj.type);
    }
};


void TestJuice(){
    Juice j1 = {2,2.33,"Orange"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAJuice>(j1);
    Juice j2 = HLA::cast_from_rti<HLAJuice>(v);
    ASSERT_EQUAL(j1.volume,j2.volume)
    ASSERT_EQUAL(j1.type,j2.type)
    ASSERT_EQUAL(j1.price,j2.price)
}

using HLAJuice_ = HLA::Struct_wrapper<Juice, 8, HLA::Integer32BE, HLA::Float64BE, HLA::String>;

void TestJuice_(){
    Juice j1 = {2,2.33,"Orange"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAJuice_>(j1);
    Juice j2 = HLA::cast_from_rti<HLAJuice_>(v);
    ASSERT_EQUAL(j1.volume,j2.volume)
    ASSERT_EQUAL(j1.type,j2.type)
    ASSERT_EQUAL(j1.price,j2.price)
}

struct Person{
    std::wstring first_name;
    std::wstring second_name;
    int age;
    double weight;
    double height;
};

class HLAPerson : public HLA::BaseFixedRecord<Person,8>{
public:
    void getDataMax(void *ptrSource, unsigned long uiMaxSize){
        HLA::Wstring field0;
        HLA::Wstring field1;
        HLA::Integer32BE field2;
        HLA::Float64BE field3;
        HLA::Float64BE field4;
         unsigned offset = 0;
         auto_offset(offset,ptrSource,uiMaxSize,field0,field1,field2,field3,field4);
    }

    void get(const Person &obj){
        HLA::Wstring field0;
        HLA::Wstring field1;
        HLA::Integer32BE field2;
        HLA::Float64BE field3;
        HLA::Float64BE field4;
        unsigned offset = 0, uiSize;
        m_uiSizeData=0;
        auto_geter_first(offset,uiSize,field0,obj.first_name,
                                       field1,obj.second_name,
                                       field2,obj.age,
                                       field3,obj.weight,
                                       field4,obj.height);

        auto_geter_second(offset,uiSize,field0,field1,field2,field3,field4);
    }

    void set(Person &obj){
        HLA::Wstring field0;
        HLA::Wstring field1;
        HLA::Integer32BE field2;
        HLA::Float64BE field3;
        HLA::Float64BE field4;
        unsigned offset = 0, uiSize;
        auto_seter(offset,uiSize,field0,obj.first_name,
                   field1,obj.second_name,
                   field2,obj.age,
                   field3,obj.weight,
                   field4,obj.height);
    }
};

using HLAPerson_ = HLA::Struct_wrapper<Person, 8,
                                                HLA::Wstring,
                                                HLA::Wstring,
                                                HLA::Integer32BE,
                                                HLA::Float64BE,
                                                HLA::Float64BE>;

void TestPerson(){
    Person p1 = {L"Ivan",L"Ivanov",30,80.4,179.3};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAPerson>(p1);
    Person p2 = HLA::cast_from_rti<HLAPerson>(v);
    ASSERT_EQUAL(p1.first_name,p2.first_name)
    ASSERT_EQUAL(p1.second_name,p2.second_name)
    ASSERT_EQUAL(p1.age,p2.age)
    ASSERT_EQUAL(p1.weight,p2.weight)
    ASSERT_EQUAL(p1.height,p2.height)
}

void TestPerson_(){
    Person p1 = {L"Ivan",L"Ivanov",30,80.4,179.3};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAPerson_>(p1);
    Person p2 = HLA::cast_from_rti<HLAPerson_>(v);
    ASSERT_EQUAL(p1.first_name,p2.first_name)
    ASSERT_EQUAL(p1.second_name,p2.second_name)
    ASSERT_EQUAL(p1.age,p2.age)
    ASSERT_EQUAL(p1.weight,p2.weight)
    ASSERT_EQUAL(p1.height,p2.height)
}

struct Button{
    int push;
    std::string name1;
    std::string name2;
};

class HLAButton : public HLA::BaseFixedRecord<Button,8>{
public:
    void getDataMax(void *ptrSource, unsigned long uiMaxSize){
        HLA::Integer32BE field0;
        HLA::String field1;
        HLA::String field2;
        unsigned offset = 0;
         auto_offset(offset,ptrSource,uiMaxSize,field0,field1,field2);
    }

    void get(const Button &obj){
        HLA::Integer32BE field0;
        HLA::String field1;
        HLA::String field2;
        unsigned offset = 0, uiSize;
        m_uiSizeData=0;
        auto_geter_first(offset,uiSize,field0,obj.push,
                                       field1,obj.name1,
                                       field2,obj.name2);

        auto_geter_second(offset,uiSize,field0,field1,field2);
    }

    void set(Button &obj){
        HLA::Integer32BE field0;
        HLA::String field1;
        HLA::String field2;
        unsigned offset = 0, uiSize;
        auto_seter(offset,uiSize,field0,obj.push,
                   field1,obj.name1,
                   field2,obj.name2);
    }
};

void TestButton(){
    Button b1 = {10,"Ivan","Ivanov"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAButton>(b1);
    Button b2 = HLA::cast_from_rti<HLAButton>(v);
    ASSERT_EQUAL(b1.push,b2.push)
    ASSERT_EQUAL(b1.name1,b2.name1)
    ASSERT_EQUAL(b1.name2,b2.name2)
}


using HLAButton_ = HLA::Struct_wrapper<Button, 8,
                                       HLA::Integer32BE,
                                       HLA::String,
                                       HLA::String>;

void TestButton_(){
    Button b1 = {120,"Ivan","Ivanov"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLAButton_>(b1);
    Button b2 = HLA::cast_from_rti<HLAButton_>(v);
    ASSERT_EQUAL(b1.push,b2.push)
    ASSERT_EQUAL(b1.name1,b2.name1)
    ASSERT_EQUAL(b1.name2,b2.name2)
}

struct Signal{
    uint64_t size;
    std::vector<int> signal;
};

using HLASignal = HLA::Struct_wrapper<Signal, 8,
                                                HLA::Unsigned64LE,
                                                HLA::Vector<HLA::Integer32BE, 4>>;

void TestSignal(){
    Signal sig1{100, std::vector<int>(100, 23)};
    auto v1 = HLA::cast_to_rti<HLASignal>(sig1);
    auto sig2 = HLA::cast_from_rti<HLASignal>(v1);
    ASSERT_EQUAL(sig1.size, sig2.size);
    ASSERT_EQUAL(sig1.signal, sig2.signal);
}


void TestfixedRecord(){
    LOG_DURATION("Fixed Record")
    TestPosition();
    TestPosition_();
    TestBook();
    TestBook_();
    TestJuice();
    TestJuice_();
    TestPerson();
    TestPerson_();
    TestButton();
    TestButton_();
    TestSignal();
}
#endif // TETSFIXEDRECORD_HPP
