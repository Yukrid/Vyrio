#ifndef VYRON_EXCEPTION_HPP
#define VYRON_EXCEPTION_HPP

#include <iostream>

//===================================//
//    Namespace: vyron::exception    //
//===================================//
namespace vyron::exception{
    
    //-----------------------//
    //    Type Definition    //
    //-----------------------//
    //(    vyron::exception::Base Void    )//
    struct Base
    {
        //+    Member Function    +//
        //_ Constructor
        Base (const std::string&, const std::string&, const std::string&) noexcept;
    };


    //(    vyron::exception::VYRInvalidFormat Void    )//
    struct VYRInvalidFormat : public Base
    {
        //+    Member Function    +//
        //_ Constructor
        VYRInvalidFormat (const std::string& func_, const std::string& err_str_) noexcept;
    };


    //(    vyron::exception::FailedStream Void    )//
    struct FailedStream : public Base
    {
        //+    Member Function    +//
        //_ Constructor
        FailedStream (const std::string& func_) noexcept;
    };


    //(    vyron::exception::VYRStreamMismatch Void    )//
    struct VYRStreamMismatch : public Base
    {
        //+    Member Function    +//
        //_ Constructor
        VYRStreamMismatch (const std::string& func_) noexcept;
    };


    //(    vyron::exception::VYRCatchInvalidStream Void    )//
    struct VYRCatchInvalidStream : public Base
    {
        //+    Member Function    +//
        //_ Constructor
        VYRCatchInvalidStream (const std::string& func_) noexcept;
    };


    //(    vyron::exception::MajorVersionMismatch Void    )//
    struct MajorVersionMismatch : public Base
    {
        //+    Member Function    +//
        //_ Constructor
        MajorVersionMismatch (const std::string& func_, uint16_t this_, uint16_t that_) noexcept;
    };
}


#endif
