#include "exception.hpp"

//===================================//
//    Namespace: vyron::exception    //
//===================================//
namespace vyron::exception{
    
    //---------------------------//
    //    Function Definition    //
    //---------------------------//
    //(    vyron::exception::Base Void    )//
    //_ Constructor
    Base::Base (const std::string& func_, const std::string& err_, const std::string& mes_) noexcept
    {
        std::cerr << " [ exception | " << err_ << " | " << func_ << " ]" << std::endl;
        std::cerr << "  -> " << mes_ << std::endl;

        return;
    }


    //(    vyron::exception::VYRInvalidFormat Void    )//
    //_ Constructor
    VYRInvalidFormat::VYRInvalidFormat (const std::string& func_, const std::string& err_str_) noexcept
        : Base {
            func_,
            "vyr invalid format",
            std::string{'"'} + err_str_ + "\" is an unreadable string."
        }
    {
        return;
    }


    //(    vyron::exception::FailedStream Void    )//
    //_ Constructor
    FailedStream::FailedStream (const std::string& func_) noexcept
        : Base {
            func_,
            "failed stream",
            "A failed stream was detected."
        }
    {
        return;
    }

    
    //(    vyron::exception::VYRStreamMismatch Void    )//
    //_ Constructor
    VYRStreamMismatch::VYRStreamMismatch (const std::string& func_) noexcept
        : Base {
            func_,
            "vyr stream mismatch",
            "This function is not available for this stream type."
        }
    {
        return;
    }


    //(    vyron::exception::VYRCatchInvalidStream Void    )//
    //_ Constructor
    VYRCatchInvalidStream::VYRCatchInvalidStream (const std::string& func_) noexcept
        : Base {
            func_,
            "vyr catch invalid stream",
            "This stream type is inoperable because it is invalid."
        }
    {
        return;
    }


    //(    vyron::exception::MajorVersionMismatch Void    )//
    //_ Constructor
    MajorVersionMismatch::MajorVersionMismatch (const std::string& func_, uint16_t this_, uint16_t that_) noexcept
        : Base {
            func_,
            "major version mismatch",
            std::string{"Current major version is "} + std::to_string(this_) + ", but scaned major version is " + std::to_string(that_) + '.'
        }
    {
        return;
    }

};
