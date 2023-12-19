#ifndef VYRON_OCHANNEL_HPP
#define VYRON_OCHANNEL_HPP

#include <unordered_map>
#include <vector>
#include <functional>

#include "vyrio.hpp"
#include "exception.hpp"

//================================//
//    Namespace: vyron::vyrio     //
//================================//
namespace vyron::vyrio{
 

    //------------------------//
    //    Type Declaration    //
    //------------------------//
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
    //(    vyron::vyrio::OBase Class    )//
    class OBase
    {        
        protected:
        //+    Member Variable    +//
        std::ostream&  _os;
        StreamType     _type;
        char           _status;
        bool           _init;
        size_t         _depth;

        public:
        //+    Member Function     +//
        //_ Constructor
        OBase (void);
        OBase (std::ostream&, size_t = 0);

        //_ Destructor
        virtual ~OBase (void);

        //_ Constant Accessor
                auto type    (void) const noexcept -> StreamType;
                auto fail    (void) const noexcept -> bool;
                auto good    (void) const noexcept -> bool;
                auto eof     (void) const noexcept -> bool;
                auto depth   (void) const noexcept -> size_t;
        virtual auto size    (void) const          -> size_t;
        virtual auto sname   (void) const          -> const std::string&;
        virtual auto vname   (void) const          -> const std::string&;

        //_ Accessor
                auto stream (void)               -> std::ostream&;
        virtual auto at     (const std::string&) -> OValue&;

        //_ Destructive Function
                auto tellp     (void)               noexcept -> std::streampos;
                void seekp     (std::streampos)     noexcept;
        virtual void clear     (void);
        virtual void ends      (void);
        virtual void establish (void);
        virtual void append    (const std::string&);
        virtual auto contains  (const std::string&)          -> bool;

        //_ Unary Operator
        auto operator ! (void) const -> bool;

        //_ Binary Operator
        template <typename T> inline auto operator << (const T&)                              -> OBase&;
                              inline auto operator << (const std::string&)                    -> OBase&;
                              inline auto operator << (const char*)                           -> OBase&;
                              inline auto operator << (const std::function<OBase& (OBase&)>&) -> OBase&;
                              inline auto operator << (ONumber&)                              -> OBase&;
                              inline auto operator << (OString&)                              -> OBase&;
                              inline auto operator << (OArray&)                               -> OBase&;
                              inline auto operator << (OMap&)                                 -> OBase&;
                              inline auto operator << (OTuple&)                               -> OBase&;
                              inline auto operator << (OStructure&)                           -> OBase&;
                              inline auto operator << (OChannel&)                             -> OBase&;
                              inline auto operator << (OValue&)                               -> OBase&;
        template <typename T> inline auto operator <  (T&)                                    -> OBase&;

        //_ Binary Operator
        virtual auto operator [] (const std::string&) -> OValue&;
        
        //+   constant expression function   +//
        static constexpr char type_code[] = "vyron::vyrio::OBase";
    };


    
    //(    vyron::vyrio::ONumber Class    )//
    class ONumber : public OBase
    {
        public:
        //+    Member Function    +//
        //_ Constructor
        ONumber (void)          = default;
        ONumber (std::ostream&);

        //_ Destructor
        ~ONumber (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destuctive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::ONumber";
    };


    //(    vyron::vyrio::OString Class    )//
    class OString : public OBase
    {
        public:
        //+    Member Function    +//
        //_ Constructor
        OString (void)          = default;
        OString (std::ostream&);

        //_ Destructor
        ~OString (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;
        
        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destructive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OString";
    };


    //(    vyron::vyrio::OArray Class    )//
    class OArray : public OBase
    {
        //+    Member Variable    +//
        size_t _size;
        size_t _count;
        bool   _const_size;

        public:
        //+    Member Function    +//
        //_ Constructor
        OArray (size_t = 0);
        OArray (std::ostream&, size_t = 0, size_t = 0);

        //_ Destructor
        ~OArray (void) override = default;

        //_ Constant Accessor
                     auto size  (void) const noexcept -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destructive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OArray";
    };



    //(    vyron::vyrio::OMap Class    )//
    class OMap : public OBase
    {
        //+    Member Variable    +//
        size_t _size;
        size_t _count;
        bool   _const_size;

        public:
        //+    Member Function    +//
        //_ Constructor
        OMap (size_t = 0);
        OMap (std::ostream&, size_t = 0, size_t = 0);

        //_ Destructor
        ~OMap (void) override = default;

        //_ Constant Accessor
                     auto size  (void) const noexcept -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destructive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OMap";
    };



    //(    vyron::vyrio::OTuple Class    )//
    class OTuple : public OBase
    {
        public:
        //+    Member Function    +//
        //_ Constructor
        OTuple (void)          = default;
        OTuple (std::ostream&, size_t = 0);

        //_ Destructor
        ~OTuple (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destructive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OTuple";
    };



    //(    vyron::vyrio::OValue Class    )//
    class OValue : public OBase
    {
        public:
        //+    Member Function    +//
        //_ Constructor
        OValue (void)          = default;
        OValue (std::ostream&, size_t = 0);

        //_ Destructor
        ~OValue (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto size  (void) const          -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> OValue& override;

        //_ Destuctive Function
                     void clear     (void)               noexcept         override;
                     void ends      (void)               noexcept         override;
                     void establish (void)               noexcept         override;
                     void append    (const std::string&) noexcept         override;
        [[noreturn]] auto contains  (const std::string&)          -> bool override;


        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OValue";
    };


    
    //(    vyron::vyrio::OStructure Class    )//
    class OStructure : public OBase
    {
        //+    Member Variable    +//
        std::string                             _sname;
        std::string                             _vname;
        std::unordered_map<std::string, OValue> _table;
        std::vector<char>                       _temp_name;
        
        public:
        //+    Member Function    +//
        //_ Constructor
        OStructure (const std::string& = "");
        OStructure (std::ostream&, const std::string& = "", size_t = 0);

        //_ Destructor
        ~OStructure (void) override = default;

        //_ Constant Accessor
        auto size  (void) const noexcept -> size_t             override;
        auto sname (void) const noexcept -> const std::string& override;
        auto vname (void) const noexcept -> const std::string& override;
   
        //_ Accessor
        auto at (const std::string&) noexcept -> OValue& override;

        //_ Destructive Function
        void clear     (void)               noexcept         override;
        void ends      (void)               noexcept         override;
        void establish (void)               noexcept         override;
        void append    (const std::string&) noexcept         override;
        auto contains  (const std::string&) noexcept -> bool override;

        //_ Binary Operator
        auto operator [] (const std::string&) noexcept -> OValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OStructure";
    };


    //(    vyron::vyrio::OChannel Class    )//
    class OChannel{

        //+    Member Variable    +//
        OBase* _chn;
        bool   _ref_mode;

        public:
        //+    Member Function    +//
        //_ Constructor
        OChannel (void);
        OChannel (std::ostream&);
        OChannel (OBase&);

        //_ Destructor
        ~OChannel (void);

        //_ Constant Accessor
        auto type  (void) const noexcept -> StreamType;
        auto fail  (void) const noexcept -> bool;
        auto good  (void) const noexcept -> bool;
        auto eof   (void) const noexcept -> bool;
        auto size  (void) const          -> size_t;
        auto sname (void) const          -> const std::string&;
        auto vname (void) const          -> const std::string&;

        //_ Accessor
        auto stream (void)               -> std::ostream&;
        auto at     (const std::string&) -> OValue&;

        //_ Destructive Function
        void seekp     (std::streampos)      noexcept;
        auto tellp     (void)                noexcept -> std::streampos;
        void clear     (void)                noexcept;
        void ends      (void);
        void establish (void);
        void append    (const std::string&);
        auto contains  (const std::string&)           -> bool;

        //_ Unary Operator
        auto operator ! (void) const -> bool;

        //_ Binary Operator
                                     auto operator [] (const std::string&) -> OValue&;
        template <typename T> inline auto operator << (T&)                 -> OChannel&;
        template <typename T> inline auto operator <  (T&)                 -> OChannel&;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::OChannel";
    };


    //---------------------------//
    //    Variable Definition    //
    //---------------------------//
    inline const std::function<OBase& (OBase&)> null = [](OBase& ich_) -> OBase&
    {
        ich_.append("_");
        return ich_;
    };


    inline const std::function<OBase& (OBase&)> end = [](OBase& ich_) -> OBase&
    {
        ich_.ends();
        return ich_;
    };



    //---------------------------//
    //    Function Definition    //
    //---------------------------//
    template <typename T> inline auto OBase::operator << (const T& t_)
        -> OBase&
    {
        this -> append(std::to_string(t_));
        return *this;
    }
 

    inline auto OBase::operator << (const char* str_)
        -> OBase&
    {
        this -> append(str_);
        return *this;
    }
   

    inline auto OBase::operator << (const std::string& str_)
        -> OBase&
    {
        this -> append(str_);
        return *this;
    }


    inline auto OBase::operator << (const std::function<OBase& (OBase&)>& func_)
        ->  OBase&
    {
        return func_(*this);
    }


    inline auto OBase::operator << (ONumber& chn_)
        -> OBase&
    {
        establish();
        chn_.~ONumber();
        new(&chn_) ONumber(_os); 
        return *this;
    }

    
    inline auto OBase::operator << (OString& chn_)
        -> OBase&
    {
        establish();
        chn_.~OString();
        new(&chn_) OString(_os); 
        return *this;
    }

    
    inline auto OBase::operator << (OArray& chn_)
        -> OBase&
    {
        size_t sz_ = chn_.size();
        establish();
        chn_.~OArray();
        new(&chn_) OArray(_os, sz_, _depth + 1); 
        return *this;
    }


    inline auto OBase::operator << (OMap& chn_)
        -> OBase&
    {
        size_t sz_ = chn_.size();
        establish();
        chn_.~OMap();
        new(&chn_) OMap(_os, size(), _depth + 1); 
        return *this;
    }


    inline auto OBase::operator << (OTuple& chn_)
        -> OBase&
    {
        establish();
        chn_.~OTuple();
        new(&chn_) OTuple(_os, _depth); 
        return *this;
    }


    inline auto OBase::operator << (OStructure& chn_)
        -> OBase&
    {
        std::string sname = chn_.sname();
        establish();
        chn_.~OStructure();
        new(&chn_) OStructure(_os, sname, _depth + 1); 
        return *this;
    }


    inline auto OBase::operator << (OValue& chn_)
        -> OBase&
    {
        establish();
        chn_.~OValue();
        new(&chn_) OValue(_os, _depth); 
        return *this;
    }



    inline auto OBase::operator << (OChannel& chn_)
        -> OBase&
    {
        establish();
        chn_.~OChannel();
        new(&chn_) OChannel(_os); 
        return *this;
    }

    
    template <typename T> inline auto OBase::operator < (T& t_)
        -> OBase&
    {
        _os << t_;
        return *this;
    }


    template <typename T> inline auto OChannel::operator << (T& t_)
        -> OChannel&
    {
        _chn -> operator<<(t_);
        return *this;
    }


    template <typename T> inline auto OChannel::operator < (T& t_)
        -> OChannel&
    {
        _chn -> operator<(t_);
        return *this;
    }
}
#endif
