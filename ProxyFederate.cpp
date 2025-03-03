#include "ProxyFederate.hpp"
#include "HLAdata/HLAdata.hpp"
#include <algorithm>
#include <iostream>

namespace HLA {

    ProxyFederate::ProxyFederate(const JSON& file) noexcept : BaseFederate(JSON::MakeJSON(file.GetRoot()->AsMap().at(L"SOM_path")->AsWstring())){}


    bool ProxyFederate::StartProxy(const JSON &proxy_file){
        std::wcout << L"Start proxy" << std::endl;
        auto root = proxy_file.GetRoot()->AsMap();
        auto ip = root.at(L"IP_local")->AsWstring();
        ip_remote = root.at(L"IP_remote")->AsWstring();
        std::wcout << L"I get ip" << std::endl;
        _ports_pub_attributes = JSON::ToMapUshortWstring(root.at(L"Ports_Publish_Attributes"));
        std::wcout << L"I create map" << std::endl;
        for(const auto& port : _ports_pub_attributes){
            try{
                _sockets[port.first] = std::make_unique<udp_socket_t>(_context);
                std::wcout << L"Socket made" << std::endl;
                _sockets[port.first] -> open(boost::asio::ip::udp::v4());
                std::wcout << L"Socket opened" << std::endl;
                _sockets[port.first] -> bind(udp_endpoint_t(boost::asio::ip::address::from_string(std::string(ip.begin(), ip.end())), port.first));
                std::wcout << L"Socket binded" << std::endl;
            }
            catch(boost::system::system_error& e){
                *logger << Logger::MSG::ERRORR
                        << L"Can't create socket with IP = "
                        << ip
                        << L" and port = "  //Артём, порты от 0 до 65535 могут быть
                        << port.first
                        << L" for publish attribute: "
                        << port.second
                        << L". Details: "
                        << Tools::widen(std::string(e.what()))
                        << Logger::Flush();
                return false;
            }
            catch(...){
                std::wcout << L"Some error" <<std::endl;
                return false;
            }
            Listen_attributes(port.first);
        }

        _ports_sub_attributes = JSON::ToMapUshortWstring(root.at(L"Ports_Subscribe_Attributes"));
        for(const auto& port : _ports_sub_attributes){
            try{
                _attributes_sub_ports[port.second] = port.first;
                _sockets[port.first] = std::make_unique<udp_socket_t>(_context);
                _sockets[port.first] -> open(boost::asio::ip::udp::v4());
                _sockets[port.first] -> bind(udp_endpoint_t(boost::asio::ip::address::from_string(std::string(ip.begin(), ip.end())), port.first));
            }
            catch(std::exception& e){
                *logger << Logger::MSG::ERRORR
                        << L"Can't create socket with IP = "
                        << ip
                        << L" and port = "
                        << port.first
                        << L" for subscribe attribute: "
                        << port.second
                        << L". Details: "
                        << Tools::widen(std::string(e.what()))
                        << Logger::Flush();
                return false;
            }
        }

        _ports_pub_interactions = JSON::ToMapUshortWstring(root.at(L"Ports_Publish_Interactions"));
        for(const auto& port : _ports_pub_interactions){
            try{
                _sockets[port.first] = std::make_unique<udp_socket_t>(_context);
                _sockets[port.first] -> open(boost::asio::ip::udp::v4());
                _sockets[port.first] -> bind(udp_endpoint_t(boost::asio::ip::address::from_string(std::string(ip.begin(), ip.end())), port.first));
            }
            catch(std::exception& e){
                *logger << Logger::MSG::ERRORR
                        << L"Can't create socket with IP = "
                        << ip
                        << L" and port = "
                        << port.first
                        << L" for publish interaction: "
                        << port.second
                        << L". Details: "
                        << Tools::widen(std::string(e.what()))
                        << Logger::Flush();
                return false;
            }
            Listen_interactions(port.first);
        }

        _ports_sub_interactions = JSON::ToMapUshortWstring(root.at(L"Ports_Subscribe_Interactions"));
        for(const auto& port : _ports_sub_attributes){
            try{
                _sockets[port.first] = std::make_unique<udp_socket_t>(_context);
                _sockets[port.first] -> open(boost::asio::ip::udp::v4());
                _sockets[port.first] -> bind(udp_endpoint_t(boost::asio::ip::address::from_string(std::string(ip.begin(), ip.end())), port.first));
            }
            catch(std::exception& e){
                *logger << Logger::MSG::ERRORR
                        << L"Can't create socket with IP = "
                        << ip
                        << L" and port = "
                        << port.first
                        << L" for subscribe interaction: "
                        << port.second
                        << L". Details: "
                        << Tools::widen(std::string(e.what()))
                        << Logger::Flush();
                return false;
            }
        }

        *logger << Logger::MSG::INFO
                << L"UDP configuration complete"
                << Logger::Flush();

        try{
            auto hla_file = JSON::MakeJSON(root.at(L"SOM_path")->AsWstring());
            return ConnectRTI(hla_file);
        }
        catch(std::runtime_error&){
            *logger << Logger::MSG::ERRORR
                    << L"Invalid SOM path"
                    << Logger::Flush();
            return false;
        }
    }

    void ProxyFederate::RunFederate(){
        _context.run();
    }

    void ProxyFederate::Listen_attributes(unsigned short port){
        auto handle = [this, port](const udp_error_t& error, size_t size){
            if(error) return;
            rti1516e::VariableLengthData data;
            data.setData(_buffers[port].data(), size);
            rti1516e::AttributeHandleValueMap map;
            map[_AttributesMap[_MyClass][_ports_pub_attributes[port]]] = data;
            this->UpdateAttributes(map);
            this->Listen_attributes(port);
        };

        _sockets[port]->async_receive_from(boost::asio::buffer(_buffers[port]),_endp, handle);
    }


    void ProxyFederate::Listen_interactions(unsigned short port){
        auto handle = [this, port](const udp_error_t& error, size_t size){
            if(error) return;
            rti1516e::VariableLengthData data;
            data.setData(_buffers[port].data(), size);
            rti1516e::ParameterHandleValueMap map;
            map[_ParametersMap[_InteractionClasses[_ports_pub_interactions[port]]].begin()->second] = data;
            BaseFederate::SendParameters(_InteractionClasses[_ports_pub_interactions[port]], map);
            this->Listen_attributes(port);
        };

        _sockets[port]->async_receive_from(boost::asio::buffer(_buffers[port]),_endp, handle);
    }


    void ProxyFederate::WriteAttributes(unsigned short port, const rti1516e::VariableLengthData& data){
        auto handle = [](const udp_error_t& , size_t ){};
        udp_endpoint_t remote_endp(boost::asio::ip::address::from_string(std::string(ip_remote.begin(), ip_remote.end())), port);
        _sockets[port]->async_send_to(boost::asio::buffer(data.data(), data.size()), remote_endp, handle);
    }

    void ProxyFederate::WriteInteractions(unsigned short port, const rti1516e::VariableLengthData & data){
        auto handle = [](const udp_error_t& , size_t ){};

        udp_endpoint_t remote_endp(boost::asio::ip::address::from_string(std::string(ip_remote.begin(), ip_remote.end())), port);
        _sockets[port]->async_send_to(boost::asio::buffer(data.data(), data.size()), remote_endp, handle);
    }

    void ProxyFederate::AttributeProcess(rti1516e::ObjectClassHandle &handle,
                                         rti1516e::AttributeHandleValueMap &data,
                                         rti1516e::VariableLengthData &info){
        // std::wcout << handle.toString() << std::endl;
        for(const auto& _data : data){
            auto name = _rtiAmbassador->getAttributeName(handle, _data.first);
            WriteAttributes(_attributes_sub_ports[name], _data.second);
        }
    }

    void ProxyFederate::ParameterProcess(rti1516e::InteractionClassHandle &handle,
                                         rti1516e::ParameterHandleValueMap &data,
                                         rti1516e::VariableLengthData &info){
        for(const auto& _data : data){
            auto name = _rtiAmbassador->getParameterName(handle, _data.first);
            auto it = std::find_if(_ports_sub_interactions.begin(), _ports_sub_interactions.end(), [&name](const auto& x){return x.second == name;});
            WriteInteractions(it->first, _data.second);
        }
    }

}

