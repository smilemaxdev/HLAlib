﻿#ifndef BASEFEDERATE_HPP
#define BASEFEDERATE_HPP

#include <RTI/RTI1516.h>
#include <RTI/NullFederateAmbassador.h>

#include <thread>
#include <queue>
#include <condition_variable>
#include <unordered_set>

#include "Tools/JSON.hpp"
#include "Tools/Logger.hpp"

namespace HLA{

/**
* @brief logger
* Logger - Singlton for log inforamtion about connection, initialization and execution
*/
    extern std::unique_ptr<Logger> logger;

/**
* @brief The ModelMode enum
* Set of Model Modes:
* Free_Following      : Do modeling process and federate time control in the thread, which connect federate to RTI with independent modeling step (master)
* Free_Threading      : Do modeling process and federate time control in the new thread with independent modeling step (master)
* Managing_Following  : Do modeling process in the thread, which connect federate to RTI with depends on FederationManager (slave)
* Managing_Threading  : Do modeling process and federate time control in the new thread with depends on FederationManager (slave)
*/
    enum class MODELMODE : short{
        FREE_FOLLOWING      = 0,
        FREE_THREADING      = 1,
        MANAGING_FOLLOWING  = 2,
        MANAGING_THREADING  = 3
    };

/**
* @brief The State enum
* Set of Federate States:
* Prepare      : Initial federate state
* NameReserved : Federate state after succesfull name reservation in RTI
* Connected    : Federate state after succesfull connect to RTI
* Started      : Federate state after succesfull initializaton
* Processing   : Federate state during data processing
* Doing        : Federate state during time control
* Ready        : Federate state, when it waiting for next time
* Exit         : Federate state at the end of simulation
*/
    enum class STATE : short{
        PREPARE      = 0,
        NAMERESERVED = 1,
        CONNECTED    = 2,
        STARTED      = 3,
        PROCESSING   = 4,
        DOING        = 5,
        READY        = 6,
        EXIT         = -1
    };

/**
* @brief The TimeStamp enum
* READY : Stapm that notify FederationManager about ready of Federate
* GO    : Stamp that notify Federate that FederationManager allow next modeling step
*/
    enum class TIMESTAMP : short{
        READY = 0,
        GO    = 1
    };

/**
* @brief The BaseFederate class
* Basic federate of HLAlib, which provide easy API for Quick Federate Developing, based on NullFederateAmbassador (provided by HLA)
*/
    class BaseFederate: public rti1516e::NullFederateAmbassador{

//================================================================================================================================================

//          Public API

//================================================================================================================================================

    public:

/**
* @brief BaseFederate
* Default constructor, initialization nothing specific
*/
        BaseFederate() = delete;

/**
* @brief BaseFederate
* @param file JSON file with necessary parameters:
* Name           : Name of Federate
* Type           : Type of federate, which match object in FOM
* FOMpath        : Path to FOM file
* FederationName : Name of Federation
* Address        : IP - address of CRC
* LogFileName    : Prefix for log - file
* Constructor for JSON file with lvalue reference
*/
        BaseFederate(const JSON& file) noexcept;

/**
* @brief BaseFederate
* @param file JSON file with necessary parameters:
* Name           : Name of Federate
* Type           : Type of federate, which match object in FOM
* FOMpath        : Path to FOM file
* FederationName : Name of Federation
* Address        : IP - address of CRC
* LogFileName    : Prefix for log - file
* Constructor for JSON file with rvalue reference
*/
        BaseFederate(JSON&& file) noexcept;

/**
* @brief ~BaseFederate
* Destructor of basic federate, which set Federate State to EXIT
*/
        virtual ~BaseFederate() override;

/**
* @brief BaseFederate
* Copy constructor of Federate deleted, only unique entity can exist
*/
        BaseFederate(const BaseFederate&) = delete;

/**
* @brief BaseFederate
* Move constructor of Federate deleted, only unique entity can exist
*/
        BaseFederate(BaseFederate&&) = delete;


/**
* @brief operator =
* Copy assignment operator of Federate deleted, only unique entity can exist
* @return BaseFederate&
*/
        BaseFederate& operator=(const BaseFederate&) = delete;

/**
* @brief operator =
* Move assignment operator of Federate deleted, only unique entity can exist
* @return BaseFederate&
*/
        BaseFederate& operator=(BaseFederate&&) = delete;


/**
* @brief ConnectRRTI
* Connect to RTI. In order to connect we need to create federation based on FOM (isn't nessary, if federation already exist) and join there. After that federate initialized in RTI and go to the his main loop. Use only to lvalue class samples
* @return flag of success execution
*/
         bool ConnectRTI(const JSON& file) &;

/**
* @brief isActive
* Method notifying about the status of the federate
* @return flag of active execution
*/
         bool isActive() const & noexcept;

/**
* @brief operator ()
* @param step Modeling Step in milliseconds
* Operator that run federate with modelin_step in milliseconds = step like functional object
* @return flag of success execution
*/
        virtual bool operator()(const JSON& file);

/**
* @brief GetName
* Return name of Federate
* @return std::wstring
*/
        const std::wstring GetName() const noexcept;

/**
* @brief GetType
* Return type of Federate, which match object in FOM
* @return std::wstring
*/
        const std::wstring GetType() const noexcept;

/**
* @brief GetFederation
* Return name of Federation
* @return std::wstring
*/
        const std::wstring GetFederation() const noexcept;

/**
* @brief GetFOMpath
* Return Path to FOM
* @return std::wstring
*/
        const std::wstring GetFOMpath() const noexcept;

/**
* @brief GetIP
* return IP - address of CRC
* @return std::wstring
*/
        const std::wstring GetIP() const noexcept;

/**
* @brief GetCallbackMode
* Return current Callback mode :
* HLA_IMMEDIATE : Asynchronous Mode
* HLA_EVOKED    : Synchronous Mode, requires requests to RTI for every call
* @return rti1516e::CallbackModel
*/
        rti1516e::CallbackModel GetCallbackMode() const noexcept;

/**
* @brief GetModelMode
* Return current Federate Mode
* @return ModelMode
*/
        MODELMODE GetModelMode() const noexcept;

/**
* @brief GetState
* Return current Federate State
* @return State
*/
        STATE GetState() const noexcept;


//================================================================================================================================================

//          Private Methods, structures and alias

//================================================================================================================================================

private:

/**
* @brief LoadSOMFromJSON
* @param file file JSON file with necessary parameters:
* ModelingStep          : Step of Modeling in milliseconds
* CallbackMode          : Asynchronous/Synchronous mode match 0/1
* ModelingMode          : Modeling Mode parameter, look ModelMode enumeration class
* Method for JSON file with lvalue reference
* @return Sample reference of current Federate
*/
        BaseFederate& LoadSOMFromJSON(const JSON& file);

/**
* @brief LoadSOMFromJSON
* @param file file JSON file with necessary parameters:
* ModelingStep          : Step of Modeling in milliseconds
* CallbackMode          : Asynchronous/Synchronous mode match 0/1
* ModelingMode          : Modeling Mode parameter, look ModelMode enumeration class
* Method for JSON file with rvalue reference
* @return Sample reference of current Federate
*/
        BaseFederate& LoadSOMFromJSON(JSON&& file);

/**
* @brief NameMap
* Alias for data structure std::unordered_map<std::wstring, std::vector<std::wstring>>
*/
        using NameMap = std::unordered_map<std::wstring, std::vector<std::wstring>>;

/**
* @brief NameList
* Alias for data structure std::vector<std::wstring>
*/
        using NameList = std::vector<std::wstring>;

/**
* @brief SetModelingStep
* @param step in milliseconds
* Setup Federate Modeling step (_step) to step value
* @return Sample reference of current Federate
*/
        BaseFederate& SetModelingStep(int step) noexcept;

/**
* @brief SetModelMode
* @param mode Value of Model Mode
* Setup Modeling Mode parameter, look ModelMode enumeration class
* @return Sample reference of current Federate
*/
        BaseFederate& SetModelMode(MODELMODE mode) noexcept;

/**
* @brief SetSyncCallbackMode
* @param is_sync Boolean value for setup Synchronous mode
* Change Callback mode to Synchronize if is_sync = true, default Asynchronize
* @return Sample reference of current Federate
*/
       BaseFederate& SetSyncCallbackMode(bool is_sync) noexcept;

/**
* @brief The ObjectClassHash struct
* Hash for Object Class, it necessary for hash structures
*/
        struct ObjectClassHash{
            size_t operator()(const rti1516e::ObjectClassHandle& _Object) const noexcept;
        };

/**
* @brief The InteractionClassHash struct
* Hash for Interaction Class, it necessary for hash structures
*/
        struct InteractionClassHash{
            size_t operator()(const rti1516e::InteractionClassHandle& _Interaction) const noexcept;
        };

/**
* @brief The CallbackInformation struct
* Data structure for Callback Message {info, data}
* info   : Some mark, using for differ identifiers
* data   : Map of data with VariableLengthData (ByteArray)
* handle : Handle of recived type
*/
        template<typename T,
                 typename Handle>
        struct CallbackInformation{
            rti1516e::VariableLengthData info;
            T data;
            Handle handle;
        };

/**
* @brief CallbackAttributesInformation
* CallbackInformation struct with data = AttributeHandleValueMap
*/
        using CallbackAttributesInformation = CallbackInformation<rti1516e::AttributeHandleValueMap, rti1516e::ObjectClassHandle>;

/**
* @brief CallbackParametersInformation
* CallbackInformation struct with data = ParameterHandleValueMap
*/
        using CallbackParametersInformation = CallbackInformation<rti1516e::ParameterHandleValueMap, rti1516e::InteractionClassHandle>;

/**
* @brief ObjectHandleMap
* Alias for data structure std::unordered_map<std::wstring, rti1516e::ObjectClassHandle>
*/
        using ObjectHandleMap = std::unordered_map<std::wstring, rti1516e::ObjectClassHandle>;

/**
* @brief InteractionHandleMap
* Alias for data structure std::unordered_map<std::wstring, rti1516e::InteractionClassHandle>
*/
        using InteractionHandleMap = std::unordered_map<std::wstring, rti1516e::InteractionClassHandle>;

/**
* @brief AttributeHandleMap
* Alias for data structure std::unordered_map<rti1516e::ObjectClassHandle, std::unordered_map<std::wstring, rtigetglobaleHandle>, ObjectClassHash>
*/
        using AttributeHandleMap = std::unordered_map<rti1516e::ObjectClassHandle, std::unordered_map<std::wstring, rti1516e::AttributeHandle>, ObjectClassHash>;

/**
* @brief AttributeHandleMap
* Alias for data structure std::unordered_map<rti1516e::InteractionClassHandle, std::unordered_map<std::wstring, rti1516e::ParameterHandle>, InteractionClassHash>
*/
        using ParameterHandleMap = std::unordered_map<rti1516e::InteractionClassHandle, std::unordered_map<std::wstring, rti1516e::ParameterHandle>, InteractionClassHash>;


/**
* @brief MakeRTIambassador
* @return std::unique_ptr<rti1516e::RTIambassador>
* Create RTIambassador pointer(unique_ptr) object, that provide access to RTI services
*/
        std::unique_ptr<rti1516e::RTIambassador> MakeRTIambassador() const &;

/**
* @brief Init
* Initialized federate (it's object type in FOM and attributes), environment in federation (other types and attributes indicated in _ObjectsNames) and their connections for this federate.
*/
        void Init(const JSON& file);


/**
* @brief InitClassesAndAttributes
* Initializerd federate object and his attributes indicated in _AttributeNagetglobalmental objects and attributes indicated in _ObjectNames
*/
        void InitClassesAndAttributes(const NameList&,
                                      const NameMap&,
                                      std::unordered_map<rti1516e::ObjectClassHandle,rti1516e::AttributeHandleSet,ObjectClassHash>&,
                                      rti1516e::AttributeHandleSet&);


/**
* @brief InitInteractionsAndParameters
* Initializerd federate interactions and their parameters indicated in _MyInteractionsNames and environmental interactions and paramters indicated in _InteractionsNames
*/
        void InitInteractionsAndParameters(const NameMap&,
                                           const NameMap&,
                                           std::unordered_set<rti1516e::InteractionClassHandle, InteractionClassHash>&,
                                           std::unordered_set<rti1516e::InteractionClassHandle, InteractionClassHash>&);

/**
* @brief SubscribeAttributes
* Call RTI to subscribe on Objects and Attributes from _ObjectNames
*/
        void SubscribeAttributes(const std::unordered_map<rti1516e::ObjectClassHandle,rti1516e::AttributeHandleSet,ObjectClassHash>&) const;

/**
* @brief PublishAttributes
* Call RTI to publish the _MyClass with attributes from _AttributeNames
*/
        void PublishAttributes(const rti1516e::AttributeHandleSet&) const;

/**
* @brief SubscribeParameters
* Call RTI to subscribe on Interactions and Parameters from _MyInteractionsNames
*/
        void SubscribeInteractions(const std::unordered_set<rti1516e::InteractionClassHandle, InteractionClassHash>&) const;

/**
* @brief PublishParameters
* Call RTI to publish the Interactions and Parameters from _InteractionsNames
*/
        void PublishInteractions(const std::unordered_set<rti1516e::InteractionClassHandle, InteractionClassHash>&) const;

/**
* @brief RegisterName
* Call RTI to reserve and register federate name (_federate_name) and init this object as _MyInstanceID
*/
        void RegisterName();

/**
* @brief ReadyToGo
* Method that sends the FederationManager a stamp that the federate is ready for the next modeling step
*/
        void ReadyToGo() const;

/**
* @brief CacheID
* Method caching the handle of the resulting object class by the hash of the class id
*/
        void CacheID(const rti1516e::ObjectInstanceHandle&) const;

/**
* @brief AttributeProcess
* Function that process queue of reflected attributes step by step in time order, should be override
*/
        void AttributeProcessMain();

/**
* @brief ParameterProcess
* Function that process queue of recived parameters of interactions step by step in time order, should be override
*/
        void ParameterProcessMain();

        void TryToStopModellingThread();




//================================================================================================================================================

//          Protected API and fields

//================================================================================================================================================

    protected:
/**
* @brief RunFederate
* The main loop of federate ( default: empty method without loop)
*/
        virtual void RunFederate();

/**
* @brief UpdateAttributes
* Update VariableLengthData Map of federate values to RTI, should be override
* Convert data of federate and send map of byte arrays to RTI
*/
        virtual void UpdateAttributes() const;

/**
* @brief SendParameters
* Update VariableLengthData Map of federate interactions to RTI, should be override
* Convert data of federate and send map of byte arrays to RTI
*/
        virtual void SendParameters() const;


/**
* @brief UpdateAttributes
* @param data
* @param info
*/
        void UpdateAttributes(const rti1516e::AttributeHandleValueMap& data,
                              const rti1516e::VariableLengthData info = rti1516e::VariableLengthData()) const;

/**
* @brief SendParameters
* @param handle
* @param data
* @param info
*/
        void SendParameters(const rti1516e::InteractionClassHandle& handle,
                            const rti1516e::ParameterHandleValueMap& data,
                            const rti1516e::VariableLengthData info = rti1516e::VariableLengthData()) const;
/**
* @brief AttributeProcess
* @param handle
* @param data
* @param info
*/
        virtual void AttributeProcess(rti1516e::ObjectClassHandle& handle,
                                      rti1516e::AttributeHandleValueMap& data,
                                      rti1516e::VariableLengthData& info);

/**
* @brief ParameterProcess
* @param handle
* @param data
* @param info
*/
        virtual void ParameterProcess(rti1516e::InteractionClassHandle& handle,
                                      rti1516e::ParameterHandleValueMap& data,
                                      rti1516e::VariableLengthData& info);


       template<MODELMODE>
/**
* @brief Modeling
* Polymorphic Main function of modeling, look at specializations
*/
       void Modeling();



/**
* @brief objectInstanceNameReservationSucceeded
* @param theObjectInstanceName
* Callback function from RTI. Call when name registration in RegisterName is successful
*/
        void objectInstanceNameReservationSucceeded (std::wstring const & theObjectInstanceName)
        throw (rti1516e::FederateInternalError) override;

/**
* @brief objectInstanceNameReservationFailed
* @param theObjectInstanceName
* Callback function from RTI. Call when name registration in RegisterName is failed
*/
        void objectInstanceNameReservationFailed(const std::wstring &theObjectInstanceName)
        throw (rti1516e::FederateInternalError) override;

/**
* @brief reflectAttributeValues
* @param theObject          : Instanse Handle of Federate sender (something like id)
* @param theAttributeValues : Map of VariableLengthData (byte arrays)
* @param info : Mark of message
* Callback function from RTI. Call when subscribed federate update his attributes.
* Info of reflected attributes in theUserSuppliedTag
* Data of reflected attributes in theAttributeValues
* Handle of reflected object
*/
        void reflectAttributeValues(rti1516e::ObjectInstanceHandle theObject,
                                            const rti1516e::AttributeHandleValueMap &theAttributeValues,
                                            const rti1516e::VariableLengthData &info,
                                            rti1516e::OrderType ,
                                            rti1516e::TransportationType,
                                            rti1516e::SupplementalReflectInfo)
        throw (rti1516e::FederateInternalError) override;

/**
* @brief receiveInteraction
* @param theInteraction     : Handle of received interaction
* @param theParameterValues : Map of VariableLengthData (byte arrays)
* @param info : Mark of message
* Callback function from RTI. Call when subscribed interation with his parameters sends.
* Info of recived interaction in theUserSuppliedTag
* Data of recived interaction in theParameterValues
* Handle of recived interaction
*/
        void receiveInteraction (rti1516e::InteractionClassHandle theInteraction,
                                         rti1516e::ParameterHandleValueMap const & theParameterValues,
                                         rti1516e::VariableLengthData const & info,
                                         rti1516e::OrderType sentOrder,
                                         rti1516e::TransportationType theType,
                                         rti1516e::SupplementalReceiveInfo theReceiveInfo)
        throw (rti1516e::FederateInternalError) override;

/**
* @brief receiveInteraction
* @param theInteraction : Handle of recived interaction (default GO/READY)
* @param theTime        : Time stamp, using for separate queue of interactions and managing tools
* Callback function from RTI with time stamp. Call when federate send stamp READY or when FederationManager send GO
*/
        void receiveInteraction (rti1516e::InteractionClassHandle theInteraction,
                                rti1516e::ParameterHandleValueMap const & ,
                                rti1516e::VariableLengthData const & ,
                                rti1516e::OrderType ,
                                rti1516e::TransportationType ,
                                rti1516e::LogicalTime const & theTime,
                                rti1516e::OrderType ,
                                rti1516e::SupplementalReceiveInfo )
       throw (rti1516e::FederateInternalError) override;



/**
* @brief connectionLost
* @param faultDescription : Description of lost
* Callback function from RTI. Call when federate lost connection with RTI. After that federate go to EXIT state
*/
       void connectionLost (std::wstring const & faultDescription)
          throw (rti1516e::FederateInternalError) override;


/** @brief _federate_name
* Name of federate
*/
        const std::wstring _federate_name;

/** @brief _federate_type
* Type of federate, must be matched with FOM
*/
        const std::wstring _federate_type;

/** @brief _FOMname
* Path to FOM file
*/
        const std::wstring _FOMname;

/** @brief _federation_name
* Name of Federation
*/
        const std::wstring _federation_name;

/** @brief _host_IP_address
* IP address of CRC
*/
        const std::wstring _host_IP_address;

/**
* @brief _state
* State of Federate, look at enumeration class State
*/
        STATE _state = STATE::PREPARE;

/**
* @brief _mode
* Model Mode of Federate, look at enumeration class ModelMode
*/
        MODELMODE _mode = MODELMODE::FREE_THREADING;
/**
* @brief _f_modeling
* Flag of modeling execution
*/
        bool _f_modeling     = false;


/** @brief _callback_mode
* RTI Callback mode :
* HLA_IMMEDIATE : Asynchronous Mode
* HLA_EVOKED    : Synchronous Mode, requires requests to RTI for every call
*/
        rti1516e::CallbackModel _callback_mode = rti1516e::HLA_IMMEDIATE;

/**
* @brief _rtiAmbassador
* Pointer for RTI-ambassador object
*/
        mutable std::unique_ptr<rti1516e::RTIambassador> _rtiAmbassador;

/**
* Self Object Class and Object Instance
* @brief _MyClass
* Self object class of federate
*/

        rti1516e::ObjectClassHandle _MyClass;
/**
* @brief _MyInstanceID
* Self instance class of federate with registered federate name in RTI (something like ID in RTI)
*/
        rti1516e::ObjectInstanceHandle _MyInstanceID;


/**
* @brief _ObjectClasses
* Hash-map of extern Object Classes [ClassName,ClassID]
*/
        ObjectHandleMap _ObjectClasses;

/**
* @brief _InteractionClasses
* Hash-map of all Interaction Classes [ClassName,ClassID]
*/
        InteractionHandleMap _InteractionClasses;

/**
* @brief _AttributesMap
* Hash-map of all used Object Classes and their Attributes Hash-map [ClassID,[AttributeName,AttributeID]]
*/
        AttributeHandleMap _AttributesMap;

//
/**
* @brief _ParametersMap
* Hash-map of all used Interaction Classes and their Parameters Hash-map [ClassID,[ParameterName,ParameterID]]
*/
        ParameterHandleMap _ParametersMap;

/**
* @brief _CacheID
* Hash map for caching [ObjectID; ObjectClassID] in format [HashOF(ObjectID); PointerOF(ObjectClassID)] for better memory using
* it valueable for switching and parsing queue of attributes
*/
        mutable std::unordered_map<size_t, rti1516e::ObjectClassHandle> _CacheID;

/**
* @brief _qAttributes
* Queue of reflected attributes with transport information
*/
        std::queue<CallbackAttributesInformation> _qAttributes;

/**
* @brief _qParameters
* Queue of recived parameters with transport information
*/
        std::queue<CallbackParametersInformation> _qParameters;

/**
* @brief _amutex
* Mutex for attributes queue
*/
        mutable std::mutex _amutex;

/**
* @brief _pmutex
* Mutex for parameters queue
*/
        mutable std::mutex _pmutex;

/**
* @brief _smutex
* Mutex for _f_state when ModelGuard using
*/

        mutable std::mutex _smutex;

/**
* @brief _modeling_thread
* Thread of modeling that process all HLA interface
*/
        std::thread _modeling_thread;

/**
* @brief _modeling_step
* Value of modeling step in milliseconds
*/
        int _modeling_step = 100;

/**
* @brief _last_time
* Last time mark for ModelGuard in FollowModeling mode
*/
        std::chrono::time_point<std::chrono::steady_clock> _last_time;

/**
* @brief _cond
* Condition variable that check _state and launch main thread if ModelGuard using
*/

        std::condition_variable _condition;


/**
* @brief ModelGuard
* Class for time control using RAII
*/
        friend class ModelGuard;
    };





    template<>
/**
* @brief BaseFederate::Modeling<ModelMode::FREE_THREADING>
* Main function of modeling that sleep in sub-thread for modeling step and after that process queue of attributes and parameters and update attributes and send interactions
*/
    void BaseFederate::Modeling<MODELMODE::FREE_THREADING>();


    template<>
/**
* @brief BaseFederate::Modeling<ModelMode::FREE_FOLLOWING>
* Main function of modeling that sleep in call-thread for modeling step and after that process queue of attributes and parameters and update attributes and send interactions
*/
    void BaseFederate::Modeling<MODELMODE::FREE_FOLLOWING>();


    template<>
/**
* @brief BaseFederate::Modeling<ModelMode::MANAGING_FOLLOWING>
* Main function of modeling that wait for time-stamp GO from FederationManager and after that start process queue of attributes and parameters and update attributes and send interactions
*/
    void BaseFederate::Modeling<MODELMODE::MANAGING_FOLLOWING>();


    template<>
/**
* @brief BaseFederate::Modeling<ModelMode::MANAGING_THREADING>
* Main function of modeling that wait for time-stamp GO from FederationManager in sub-thread and after that start process queue of attributes and parameters and update attributes and send interactions
*/
    void BaseFederate::Modeling<MODELMODE::MANAGING_THREADING>();
}
#endif // BASEFEDERATE_HPP
