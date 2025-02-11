#ifndef SIMFEDERATE_HPP
#define SIMFEDERATE_HPP
#include "BaseFederate.hpp"
#include "HLAdata/HLAdata.hpp"
namespace HLA {
    
    
    class SimFederate : public BaseFederate{
    public:
        SimFederate() noexcept;
        
        SimFederate(const std::wstring& name,
                const std::wstring& type,
                const std::wstring& FOMname,
                const std::wstring& fname,
                const std::wstring& ip = L"localhost") noexcept;
        
        SimFederate(std::wstring&& name,
                std::wstring&& type,
                std::wstring&& FOMname,
                std::wstring&& fname,
                std::wstring&& ip = L"localhost") noexcept;
        
        SimFederate(const HLA::JSON& file) noexcept;
        
        SimFederate(HLA::JSON&& file) noexcept;
        
        void SendDataToRTI(const std::unordered_map<std::wstring, rti1516e::VariableLengthData>& output_data);
        
        std::unordered_map<std::wstring, rti1516e::VariableLengthData>& getData();
        
        
    private:
        
        void ParameterProcess() override;
        
        void AttributeProcess() override;
        
        void SendParameters() const override;
        
        void UpdateAttributes() const override;
        
        std::unordered_map<std::wstring, rti1516e::VariableLengthData> inpt;
        
    };
}
#endif // TRANSFER_HPP
