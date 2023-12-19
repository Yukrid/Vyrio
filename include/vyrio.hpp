#ifndef VYRON_VYRIO_HPP
#define VYRON_VYRIO_HPP

#include "exception.hpp"

//================================//
//    Namespace: vyron::vyrio     //
//================================//
namespace vyron::vyrio{
 

    //------------------------//
    //    Type Declaration    //
    //------------------------//
    class IBase;
    class INumber;
    class IString;
    class IArray;
    class IMap;
    class ITuple;
    class IStructure;
    class IValue;
    class IChannel;
 
    class OBase;
    class ONumber;
    class OString;
    class OArray;
    class OMap;
    class OTuple;
    class OStructure;
    class OValue;
    class OChannel;
  
   
    //-----------------------//
    //    Type Definition    //
    //-----------------------//
    //(    vyron::vyrio::StreamType Enumeration    )//
    enum StreamType{
        UNDEFINED, BASE, NUMBER, STRING, ARRAY, MAP, TUPLE, STRUCTURE, VALUE
    };


    //---------------------------//
    //    Variable Definition    //
    //---------------------------//
    inline const std::function<std::string (StreamType)> type_str = [](StreamType tp_) -> std::string
    {
        switch(tp_){
            case StreamType::UNDEFINED: return "undefined";
            case StreamType::NUMBER:    return "number";
            case StreamType::STRING:    return "string";
            case StreamType::ARRAY:     return "array";
            case StreamType::MAP:       return "map";
            case StreamType::TUPLE:     return "tuple";    
            case StreamType::STRUCTURE: return "structure";
            case StreamType::VALUE:     return "value";
            default:                    return "error";
        }
    };
}
#endif
