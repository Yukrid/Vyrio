#ifndef VYRON_ICHANNEL_HPP
#define VYRON_ICHANNEL_HPP

#include <iostream>
#include <sstream>
#include <functional>
#include <map>

#include "vyrio.hpp"
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
    

    //-----------------------//
    //    Type Definition    //
    //-----------------------//
    //(    vyron::vyrio::IBase Class    )//
    class IBase
    {        
        protected:
        //+    Member Variable    +//
        std::istream&  _is;
        StreamType     _type;
        char           _status;
        std::streampos _head_pos;
        std::streampos _prev_pos;

        public:
        //+    Member Function     +//
        //_ Constructor
        IBase (void);
        IBase (std::istream&);

        //_ Destructor
        virtual ~IBase (void);

        //_ Constant Accessor
                auto type    (void) const noexcept -> StreamType;
                auto fail    (void) const noexcept -> bool;
                auto good    (void) const noexcept -> bool;
                auto eof     (void) const noexcept -> bool;
                auto headpos (void) const noexcept -> std::streampos;
        virtual auto null    (void) const          -> bool;
        virtual auto found   (void) const          -> bool;
        virtual auto size    (void) const          -> size_t;
        virtual auto sname   (void) const          -> const std::string&;
        virtual auto vname   (void) const          -> const std::string&;

        //_ Accessor
                auto stream (void)               -> std::istream&;
        virtual auto at     (const std::string&) -> IValue&;

        //_ Destructive Function
                void head     (void)               noexcept;
                void tail     (void)               noexcept;
                auto tellg    (void)               noexcept -> std::streampos;
                void seekg    (std::streampos)     noexcept;
        virtual void seekg    (void);
        virtual void clear    (void);
        virtual void skip     (void);
        virtual auto consume  (void)                        -> std::string;
        virtual auto contains (const std::string&)          -> bool;

        //_ Unary Operator
        auto operator ! (void) const -> bool;

        //_ Binary Operator
        template <typename T> inline auto operator >> (T&)                                    -> IBase&;
                              inline auto operator >> (std::string&)                          -> IBase&;
                              inline auto operator >> (const std::function<IBase& (IBase&)>&) -> IBase&;
                              inline auto operator >> (INumber&)                              -> IBase&;
                              inline auto operator >> (IString&)                              -> IBase&;
                              inline auto operator >> (IArray&)                               -> IBase&;
                              inline auto operator >> (IMap&)                                 -> IBase&;
                              inline auto operator >> (ITuple&)                               -> IBase&;
                              inline auto operator >> (IStructure&)                           -> IBase&;
                              inline auto operator >> (IChannel&)                             -> IBase&;
                              inline auto operator >> (IValue&)                               -> IBase&;
        template <typename T> inline auto operator >  (T&)                                    -> IBase&;

        //_ Binary Operator
        virtual auto operator [] (const std::string&) -> IValue&;
        

        //+    Static Function    +//
        static auto rm_comma    (const std::string& str_)                         -> std::string_view;
        static auto read_string (const std::string&, std::istream&, std::string&) -> bool;
        static auto skip_string (const std::string&, std::istream&)               -> bool;

        //+   constant expression function   +//
        static constexpr char type_code[] = "vyron::vyrio::IBase";
    };


    
    //(    vyron::vyrio::INumber Class    )//
    class INumber : public IBase
    {
        //+    Member Variable    +//
        bool _next_eof;

        public:
        //+    Member Function    +//
        //_ Constructor
        INumber (void)          = default;
        INumber (std::istream&);

        //_ Destructor
        ~INumber (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const -> bool               override;
        [[noreturn]] auto found (void) const -> bool               override;
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destuctive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::INumber";
    };


    //(    vyron::vyrio::IString Class    )//
    class IString : public IBase
    {
        //+    Member Variable    +//
        bool _next_eof;
        
        public:
        //+    Member Function    +//
        //_ Constructor
        IString (void)          = default;
        IString (std::istream&);

        //_ Destructor
        ~IString (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const -> bool               override;
        [[noreturn]] auto found (void) const -> bool               override;
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;
        
        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destructive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept                override;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IString";
    };



    //(    vyron::vyrio::IArray Class    )//
    class IArray : public IBase
    {
        //+    Member Variable    +//
        size_t _size;
        bool   _const_size;

        public:
        //+    Member Function    +//
        //_ Constructor
        IArray (void)          = default;
        IArray (std::istream&);

        //_ Destructor
        ~IArray (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const          -> bool               override;
        [[noreturn]] auto found (void) const          -> bool               override;
                     auto size  (void) const noexcept -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destructive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept                override;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IArray";
    };



    //(    vyron::vyrio::IMap Class    )//
    class IMap : public IBase
    {
        //+    Member Variable    +//
        size_t _size;
        bool   _const_size;

        public:
        //+    Member Function    +//
        //_ Constructor
        IMap (void)          = default;
        IMap (std::istream&);

        //_ Destructor
        ~IMap (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const          -> bool               override;
        [[noreturn]] auto found (void) const          -> bool               override;
                     auto size  (void) const noexcept -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destructive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept                override;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IMap";
    };



    //(    vyron::vyrio::ITuple Class    )//
    class ITuple : public IBase
    {
        public:
        //+    Member Function    +//
        //_ Constructor
        ITuple (void)          = default;
        ITuple (std::istream&);

        //_ Destructor
        ~ITuple (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const -> bool               override;
        [[noreturn]] auto found (void) const -> bool               override;
        [[noreturn]] auto size  (void) const -> size_t             override;
        [[noreturn]] auto sname (void) const -> const std::string& override;
        [[noreturn]] auto vname (void) const -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destructive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept                override;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::ITuple";
    };



    //(    vyron::vyrio::IStructure Class    )//
    class IStructure : public IBase
    {
        //+    Member Variable    +//
        std::string                   _sname;
        std::string                   _vname;
        bool                          _vname_enable;
        bool                          _scanned_all;
        bool                          _found;
        std::map<std::string, IValue> _table;
        
        //+    Static Variable    +//
        static std::istringstream s_iss;
        static IValue             s_fail_chn;

        public:
        //+    Member Function    +//
        //_ Constructor
        IStructure (void)          = default;
        IStructure (std::istream&);

        //_ Destructor
        ~IStructure (void) override = default;

        //_ Constant Accessor
        [[noreturn]] auto null  (void) const          -> bool               override;
                     auto found (void) const noexcept -> bool               override;
                     auto size  (void) const noexcept -> size_t             override;
                     auto sname (void) const noexcept -> const std::string& override;
                     auto vname (void) const noexcept -> const std::string& override;
   
        //_ Accessor
        auto at (const std::string&) noexcept -> IValue& override;

        //_ Destructive Function
        void seekg    (void)               noexcept                override;
        void clear    (void)               noexcept                override;
        void skip     (void)               noexcept                override;
        auto consume  (void)               noexcept -> std::string override;
        auto contains (const std::string&) noexcept -> bool        override;

        //_ Binary Operator
        auto operator [] (const std::string&) noexcept -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IStructure";
    };


    //(    vyron::vyrio::IValue Class    )//
    class IValue : public IBase
    {
        //+    Member Variable    +//
        bool _null;
        bool _next_eof;

        public:
        //+    Member Function    +//
        //_ Constructor
        IValue (void)          = default;
        IValue (std::istream&);

        //_ Destructor
        ~IValue (void) override = default;

        //_ Constant Accessor
                     auto null  (void) const noexcept -> bool               override;
        [[noreturn]] auto found (void) const          -> bool               override;
        [[noreturn]] auto size  (void) const          -> size_t             override;
        [[noreturn]] auto sname (void) const          -> const std::string& override;
        [[noreturn]] auto vname (void) const          -> const std::string& override;

        //_ Accessor
        [[noreturn]] auto at (const std::string&) -> IValue& override;

        //_ Destuctive Function
                     void seekg    (void)               noexcept                override;
                     void clear    (void)               noexcept                override;
                     void skip     (void)               noexcept                override;
                     auto consume  (void)               noexcept -> std::string override;
        [[noreturn]] auto contains (const std::string&)          -> bool        override;

        //_ Binary Operator
        [[noreturn]] auto operator [] (const std::string&) -> IValue& override;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IValue";
    };


    //(    vyron::vyrio::IChannel Class    )//
    class IChannel
    {
        //+    Member Variable    +//
        IBase* _chn;
        bool   _ref_mode;

        public:
        //+    Member Function    +//
        //_ Constructor
        IChannel (void);
        IChannel (std::istream&);
        IChannel (IBase&);

        //_ Destructor
        ~IChannel (void);

        //_ Constant Accessor
        auto type  (void) const noexcept -> StreamType;
        auto fail  (void) const noexcept -> bool;
        auto good  (void) const noexcept -> bool;
        auto eof   (void) const noexcept -> bool;
        auto null  (void) const          -> bool;
        auto found (void) const          -> bool;
        auto size  (void) const          -> size_t;
        auto sname (void) const          -> const std::string&;
        auto vname (void) const          -> const std::string&;

        //_ Accessor
        auto stream (void)               -> std::istream&;
        auto at     (const std::string&) -> IValue&;

        //_ Destructive Function
        void head     (void)                noexcept;
        void tail     (void)                noexcept;
        void seekg    (std::streampos)      noexcept;
        void seekg    (void)                noexcept;
        auto tellg    (void)                noexcept -> std::streampos;
        void clear    (void)                noexcept;
        void skip     (void);
        auto consume  (void)                         -> std::string;
        auto contains (const std::string&)           -> bool;

        //_ Unary Operator
        auto operator ! (void) const -> bool;

        //_ Binary Operator
                                     auto operator [] (const std::string&) -> IValue&;
        template <typename T> inline auto operator >> (T&)                 -> IChannel&;
        template <typename T> inline auto operator >  (T&)                 -> IChannel&;


        //+   Constant Expression Function  +//
        static constexpr char type_code[] = "vyron::vyrio::IChannel";
    };



    //---------------------------//
    //    Variable Definition    //
    //---------------------------//
    inline const std::function<IBase& (IBase&)> dammy = [](IBase& ich_) -> IBase&
    {
        ich_.skip();
        return ich_;
    };


    //---------------------------//
    //    Function Definition    //
    //---------------------------//
    template <typename T> inline auto IBase::operator >> (T& t_)
        -> IBase&
    {
        std::istringstream iss(consume());
        iss >> t_;
        return *this;
    }
    

    inline auto IBase::operator >> (std::string& str_)
        -> IBase&
    {
         str_ = consume();
        return *this;
    }


    inline auto IBase::operator >> (const std::function<IBase& (IBase&)>& func_)
        ->  IBase&
    {
        return func_(*this);
    }


    inline auto IBase::operator >> (INumber& chn_)
        -> IBase&
    {
        seekg();
        chn_.~INumber();
        new(&chn_) INumber(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (IString& chn_)
        -> IBase&
    {
        seekg();
        chn_.~IString();
        new(&chn_) IString(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (IArray& chn_)
        -> IBase&
    {
        seekg();
        chn_.~IArray();
        new(&chn_) IArray(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (IMap& chn_)
        -> IBase&
    {
        seekg();
        chn_.~IMap();
        new(&chn_) IMap(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (ITuple& chn_)
        -> IBase&
    {
        seekg();
        chn_.~ITuple();
        new(&chn_) ITuple(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (IStructure& chn_)
        -> IBase&
    {
        seekg();
        chn_.~IStructure();
        new(&chn_) IStructure(_is); 
        skip();
        return *this;
    }


    inline auto IBase::operator >> (IChannel& chn_)
        -> IBase&
    {
        seekg();
        chn_.~IChannel();
        new(&chn_) IChannel(_is); 
        skip();
        return *this;
    }

    
    template <typename T> inline auto IBase::operator > (T& t_)
        -> IBase&
    {
        _is >> t_;
        return *this;
    }


    template <typename T> inline auto IChannel::operator >> (T& t_)
        -> IChannel&
    {
        _chn -> operator>>(t_);
        return *this;
    }


    template <typename T> inline auto IChannel::operator > (T& t_)
        -> IChannel&
    {
        _chn -> operator>(t_);
        return *this;
    }

}
#endif
