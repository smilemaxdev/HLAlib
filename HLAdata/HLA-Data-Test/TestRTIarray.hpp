#ifndef TESTARRAY_HPP
#define TESTARRAY_HPP
#include "test_runner.hpp"
#include "profile.hpp"
#include "../HLAarray.hpp"
#include "../HLAstring.hpp"
#include "../HLAtypes.hpp"


void TestArrayInt16(){
    std::array<int16_t,100> v1;
    v1.fill(2);
    rti1516e::VariableLengthData vd1 = HLA::cast_to_rti<HLA::Integer16LE,100,2>(v1);
    std::array<int16_t,100> v2 = HLA::cast_from_rti<HLA::Integer16LE,100,2>(vd1);
    ASSERT_EQUAL(v1,v2)

    std::array<int16_t,200> v3;
    v3.fill(3);
    rti1516e::VariableLengthData vd2 = HLA::cast_to_rti<HLA::Integer16BE,200,2>(v3);
    std::array<int16_t,200> v4 = HLA::cast_from_rti<HLA::Integer16BE,200,2>(vd2);
    ASSERT_EQUAL(v3,v4)
}

void TestArrayInt32(){
    std::array<int,100> v1;
    v1.fill(52);
    rti1516e::VariableLengthData vd1 = HLA::cast_to_rti<HLA::Integer32LE,100,4>(v1);
    std::array<int,100> v2 = HLA::cast_from_rti<HLA::Integer32LE,100,4>(vd1);
    ASSERT_EQUAL(v1,v2)

    std::array<int,200> v3;
    v3.fill(44);
    rti1516e::VariableLengthData vd2 = HLA::cast_to_rti<HLA::Integer32BE,200,4>(v3);
    std::array<int,200> v4 = HLA::cast_from_rti<HLA::Integer32BE,200,4>(vd2);
    ASSERT_EQUAL(v3,v4)
}

void TestArrayInt64(){
    std::array<int64_t,100> v1;
    v1.fill(5322);
    rti1516e::VariableLengthData vd1 = HLA::cast_to_rti<HLA::Integer64BE,100,8>(v1);
    std::array<int64_t,100> v2 = HLA::cast_from_rti<HLA::Integer64BE,100,8>(vd1);
    ASSERT_EQUAL(v1,v2)
    std::array<int64_t,300> v3;
    v3.fill(12313);
    rti1516e::VariableLengthData vd2 = HLA::cast_to_rti<HLA::Integer64LE,300,8>(v3);
    std::array<int64_t,300> v4 = HLA::cast_from_rti<HLA::Integer64LE,300,8>(vd2);
    ASSERT_EQUAL(v3,v4)
}

void TestArrayFloat(){
    std::array<float,100> v1BE;
    v1BE.fill(2.11);
    rti1516e::VariableLengthData v1 = HLA::cast_to_rti<HLA::Float32BE,100,4>(v1BE);
    std::array<float,100> v2BE = HLA::cast_from_rti<HLA::Float32BE,100,4>(v1);
    ASSERT_EQUAL(v1BE,v2BE)

    std::array<float,200> v1LE;
    v1LE.fill(-21.2);
    rti1516e::VariableLengthData v2 = HLA::cast_to_rti<HLA::Float32LE,200,4>(v1LE);
    std::array<float,200> v2LE = HLA::cast_from_rti<HLA::Float32LE,200,4>(v2);
    ASSERT_EQUAL(v1LE,v2LE)
}

void TestArrayDouble(){
    std::array<double,100> v1BE;
    v1BE.fill(123.11);
    rti1516e::VariableLengthData v1 = HLA::cast_to_rti<HLA::Float64BE,100,8>(v1BE);
    std::array<double,100> v2BE = HLA::cast_from_rti<HLA::Float64BE,100,8>(v1);
    ASSERT_EQUAL(v1BE,v2BE)

    std::array<double,200> v1LE;
    v1LE.fill(-222.12);
    rti1516e::VariableLengthData v2 = HLA::cast_to_rti<HLA::Float64LE,200,8>(v1LE);
    std::array<double,200> v2LE = HLA::cast_from_rti<HLA::Float64LE,200,8>(v2);
    ASSERT_EQUAL(v1LE,v2LE)
}

void TestArrayBool(){
    std::array<bool,2> v1 = {true,false};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLA::Bool,2,1>(v1);
    std::array<bool,2> v2 = HLA::cast_from_rti<HLA::Bool,2,1>(v);
    ASSERT_EQUAL(v1,v2);
}

void TestArrayChar(){
    std::array<char,3> v1 = {'a','g','1'};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLA::CharLE,3,1>(v1);
    std::array<char,3> v2 = HLA::cast_from_rti<HLA::CharLE,3,1>(v);
    ASSERT_EQUAL(v1,v2)

}

void TestArrayString(){
    std::array<std::string,4> v1 = {"Im","Name","Value","BigStringNameToTest"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLA::String,4,4>(v1);
    std::array<std::string,4> v2 = HLA::cast_from_rti<HLA::String,4,4>(v);
    ASSERT_EQUAL(v1,v2)
}

void TestArrayWstring(){
    std::array<std::wstring,4> v1 = {L"Im",L"Name",L"Value",L"BigStringNameToTest"};
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLA::Wstring,4,4>(v1);
    std::array<std::wstring,4> v2 = HLA::cast_from_rti<HLA::Wstring,4,4>(v);
    ASSERT_EQUAL(v1,v2)
}

void TestBigIntArray(){
    std::array<int,1'000'000> a1;
    for(int i = 0; i< a1.size();i++)
        a1[i] = i;
    rti1516e::VariableLengthData v = HLA::cast_to_rti<HLA::Integer32BE,1'000'000,4>(a1);
    std::array<int,1'000'000> a2 = HLA::cast_from_rti<HLA::Integer32BE,1'000'000,4>(v);
    ASSERT_EQUAL(a1,a2)
}

void Testarray(){
    LOG_DURATION("Array")
    TestArrayInt16();
    TestArrayInt32();
    TestArrayInt64();
    TestArrayFloat();
    TestArrayDouble();
    TestArrayBool();
    TestArrayChar();
    TestArrayString();
    TestArrayWstring();
    TestBigIntArray();
}
#endif // TESTARRAY_HPP
