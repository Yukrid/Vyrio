#include "ochannel.hpp"

//===============================//
//    Namespace: vyron::vyrio    //
//===============================//
namespace vyron::vyrio{

    //---------------------------//
    //    Function Definition    //
    //---------------------------//
    //(    vyron::vyrio::OBase Class    )//
    //_ Constructor
    OBase::OBase (void)
        : _os     { *reinterpret_cast<std::ostream*>(NULL) }
        , _type   { StreamType::UNDEFINED }
        , _status { 0x11 }
        , _init   { true }
        , _depth  { 0 }
    {
        return;
    }


    OBase::OBase (std::ostream& os_, size_t depth_)
        : _os     { os_ }
        , _type   { StreamType::BASE }
        , _status { static_cast<char>((!os_) ? 0x01 : 0x00) }
        , _init   { true }
        , _depth  { depth_ }
    {
        return;
    }


    //_ Destructor
    OBase::~OBase (void)
    {
        return;
    }


    //_ Constant Accessor
    auto OBase::type (void) const noexcept
        -> StreamType
    {
        return _type;
    }


    auto OBase::fail (void) const noexcept
        -> bool
    {
        return (_status & 0x01) == 0x01;
    }

    
    auto OBase::good (void) const noexcept
        -> bool
    {
        return !_status;
    }


    auto OBase::eof (void) const noexcept
        -> bool
    {
        return (_status & 0x10) == 0x10;
    }


    auto OBase::depth (void) const noexcept
        -> size_t
    {
        return _depth;
    }


    auto OBase::size (void) const
        -> size_t
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::size" );
    }


    auto OBase::sname (void) const
        -> const std::string&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::sname" );
    }


    auto OBase::vname (void) const
        -> const std::string&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    auto OBase::stream (void)
        -> std::ostream&
    {
        return _os;
    }

    auto OBase::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    auto OBase::tellp (void) noexcept -> std::streampos
    {
        return _os.tellp();
    }

    
   
    void OBase::seekp (std::streampos pos_) noexcept
    {
        _os.seekp(pos_);
        if(!_os.good()) _status |= 0x01;
        return;
    }


    void OBase::clear (void)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::clear" );
    }


    void OBase::ends (void)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::ends" );
    }


    void OBase::append (const std::string&)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::append" );
    }


    void OBase::establish (void) 
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::establish" );
    }


    auto OBase::contains (const std::string&)
        -> bool
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::contains" );
    }


    //_ Unary Operator
    auto OBase::operator ! (void) const
        -> bool
    {
        return _status;
    }
    
    
    //_ Binary Operator
    auto OBase::operator [] (const std::string&)
        -> OValue&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::ONumber Class    )//
    //_ Constructor
    ONumber::ONumber (std::ostream& os_)
        : OBase     { os_ }
    {
        _type = StreamType::NUMBER;
        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto ONumber::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto ONumber::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto ONumber::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto ONumber::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void ONumber::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void ONumber::ends (void) noexcept
    {
        if(good()){
            _os << '_';
            _status |= 0x10;
        }
        return;
    }


    void ONumber::establish (void) noexcept
    {
        return;
    }


    void ONumber::append (const std::string& str_) noexcept
    {
        if(good()){
            try{
                size_t pos;
                std::stod(str_, &pos);
                if(pos != str_.length()){
                    _os << '_';
                    _status |= 0x01;
                    return;
                }
            }catch(...){
                _os << '_';
                _status |= 0x01;
                return;
            }
            _os << str_;
            _status |= 0x10;
        }
        return;
    }
    

    [[noreturn]] auto ONumber::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto ONumber::operator [] (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }


    //(    vyron::vyrio::OString Class    )//
    //_ Constructor
    OString::OString (std::ostream& os_)
        : OBase     { os_ }
    {
        _type = StreamType::STRING;
        if(!_status) _os << '"';
        
        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto OString::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto OString::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto OString::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto OString::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void OString::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OString::ends (void) noexcept
    {
        if(good()){
            _os << '"';
            _status |= 0x10;
        }
       
        return;
    }

    
    void OString::establish (void) noexcept
    {
        return;
    }


    void OString::append (const std::string& str_) noexcept
    {
        if(good()){
            _os << str_;
            _init = false;
        }
        return;
    }


    [[noreturn]] auto OString::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto OString::operator [] (const std::string&)
       -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::OArray Class    )//
    //_ Constructor
    OArray::OArray (size_t size_)
        : OBase       {  }
        , _size       { size_ }
        , _count      { 0 }
        , _const_size { static_cast<bool>(size_) }
    {
        return;
    }


    OArray::OArray (std::ostream& os_, size_t size_, size_t depth_)
        : OBase       { os_, depth_}
        , _size       { size_ }
        , _count      { 0 }
        , _const_size { static_cast<bool>(size_) }
    {
        if(good()){
            std::string ofs(_depth, ' ');
            if(_const_size){
                _os << '[';
                _os << std::endl << ofs << "#" << _size;
            }else{
                _os << '[';
            }
        }       
        return;
    }


    //_ Constant Accessor
    auto OArray::size (void) const noexcept
        -> size_t
    {
        return _const_size ? _size : _count;
    }


    [[noreturn]] auto OArray::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto OArray::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto OArray::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void OArray::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OArray::ends (void) noexcept
    {
        if(good()){
            _os << std::endl << "]";
            _status |= 0x10;
        }
        
        return;
    }

    
    void OArray::establish (void) noexcept
    {
        if(good()){
            if(_const_size and _count >= _size){
                _status |= 0x11;
            }else{
                std::string ofs(_depth, ' ');
                if(_init){
                    _os << std::endl << ofs;
                    ++_count;
                    _init = false;
                }else{
                    _os << ',' << std::endl << ofs;
                    ++_count;
                }
                if(_const_size and _count >= _size){
                    _status |= 0x10;
                }
            }
        }
        return;
    }


    void OArray::append (const std::string& str_) noexcept
    {
        if(good()){
            if(_const_size and _count >= _size){
                _status |= 0x11;
            }else{
                std::string ofs(_depth, ' ');
                if(_init){
                    _os << std::endl << ofs << str_;
                    ++_count;
                    _init = false;
                }else{
                    _os << ',' << std::endl << ofs << str_;
                    ++_count;
                }
                if(_const_size and _count >= _size){
                    ends();
                }
            }
        }
        return;
    }


    [[noreturn]] auto OArray::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto OArray::operator [] (const std::string&)
       -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::OMap Class    )//
    //_ Constructor
    OMap::OMap (size_t size_)
        : OBase       { }
        , _size       { size_ }
        , _count      { 0 }
        , _const_size { static_cast<bool>(size_) }
    {
        return;
    }


    OMap::OMap (std::ostream& os_, size_t size_, size_t depth_)
        : OBase       { os_, depth_ }
        , _size       { size_ }
        , _count      { 0 }
        , _const_size { static_cast<bool>(size_) }
    {
        _type = StreamType::MAP;
        if(!_status){
            if(_const_size) _os << "< #" << _size;
            else            _os << '<';
        }
        
        return;
    }


    //_ Constant Accessor
    auto OMap::size (void) const noexcept
        -> size_t
    {
        return _size;
    }


    [[noreturn]] auto OMap::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto OMap::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto OMap::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void OMap::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OMap::ends (void) noexcept
    {
        if(good() or _count >= _size){
            _os << " >";
            _status |= 0x10;
        }
        
        return;
    }

    
    void OMap::establish (void) noexcept
    {
        if(good()){
            if(_const_size and _count >= _size){
                _status |= 0x11;
            }else{
                if(_init){
                    _os << ' ';
                    ++_count;
                    _init = false;
                }else{
                    _os << ", ";
                    ++_count;
                }
                if(_const_size and _count >= _size){
                    _status |= 0x10;
                }
            }
        }
        return;
    }


    void OMap::append (const std::string& str_) noexcept
    {
        if(good()){
            if(_const_size and _count >= _size){
                _status |= 0x01;
            }else{
                if(_init){
                    _os << ' ' << str_;
                    ++_count;
                    _init = false;
                }else{
                    _os << ", " << str_;
                    ++_count;
                }
            }
        }
        return;
    }


    [[noreturn]] auto OMap::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto OMap::operator [] (const std::string&)
       -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::OTuple Class    )//
    //_ Constructor
    OTuple::OTuple (std::ostream& os_, size_t depth_)
        : OBase       { os_, depth_ }
    {
        _type = StreamType::TUPLE;
        if(!_status) _os << '(';
        
        return;
    }


    //_ Constant Accessor
    auto OTuple::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto OTuple::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto OTuple::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto OTuple::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void OTuple::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OTuple::ends (void) noexcept
    {
        if(good()){
            _os << " )";
            _status |= 0x10;
        }
        
        return;
    }

    
    void OTuple::establish (void) noexcept
    {
        if(good()){
            if(_init){
                _os << ' ';
                _init = false;
            }else{
                _os << ", ";
            }
        }
        return;
    }


    void OTuple::append (const std::string& str_) noexcept
    {
        if(good()){
            if(_init){
                _os << ' ' << str_;
                _init = false;
            }else{
                _os << ", " << str_;
            }
        }
        return;
    }


    [[noreturn]] auto OTuple::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto OTuple::operator [] (const std::string&)
       -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::OStruccture Class    )//
    //_ Constructor
    OStructure::OStructure (const std::string& str_)
        : OBase         { }
        , _sname        { str_ }
        , _vname        { "__" }
        , _table        { }
    {
        return;
    }


    OStructure::OStructure (std::ostream& os_, const std::string& str_, size_t depth_)
        : OBase         { os_, depth_ }
        , _sname        { str_ }
        , _vname        { "__" }
        , _table        { }
        , _temp_name    { }
    {
        _type = StreamType::STRUCTURE;
        if(!_status) _os << _sname << '{';
        return;
    }


    //_ Constant Accessor
    auto OStructure::size (void) const noexcept
        -> size_t
    {
        return _table.size();
    }


    auto OStructure::sname (void) const noexcept
        -> const std::string&
    {
        return _sname;
    }


    auto OStructure::vname (void) const noexcept
        -> const std::string&
    {
        return _vname;
    }


    //_ Accessor
    auto OStructure::at (const std::string& str_) noexcept
        -> OValue&
    {
        if(_vname != str_){
            _table[_vname].ends();
             if(!_table.count(str_)){
                std::string ofs(_depth, ' ');
                if(_init){
                    _os<< std::endl << ofs << str_ << ": ";
                    _init = false;
                }else{
                    _os<< ',' << std::endl << ofs << str_ << ": ";
                }
            }
            _vname = str_;
            _table.emplace(std::piecewise_construct, std::forward_as_tuple(_vname), std::forward_as_tuple( _os, _depth));
        }
        return _table[_vname];
    }


    //_ Destructive Function
    void OStructure::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OStructure::ends (void) noexcept
    {
        if(good()){
            _os << " }";
            _status |= 0x10;
        }
        
        return;
    }

    
    void OStructure::establish (void) noexcept
    {
        if(good()){
            _table[_vname].ends();            
            if(!_temp_name.size()) _temp_name.push_back('@');
            std::function<void(std::vector<char>&, std::vector<char>::iterator)> make_name = [&make_name](std::vector<char>& _temp_name, std::vector<char>::iterator begin_){
                *begin_ += 1;
                if(*begin_ == '['){
                    *begin_ = 'A';
                    if(_temp_name.end()-1 == begin_){
                        _temp_name.push_back('A');
                        return;
                    }else{
                        return make_name(_temp_name, ++begin_);
                    }
                }
                return; 
            };
            make_name(_temp_name, _temp_name.begin());
            _vname = std::string("__") + std::string(_temp_name.rbegin(), _temp_name.rend());
            _table[_vname];
            std::string ofs(_depth, ' ');
            if(_init) _os<< std::endl << ofs << _vname << ": ";
            else      _os<< ',' << std::endl << ofs << _vname << ": ";
        }
        
        return;
    }


    void OStructure::append (const std::string& str_) noexcept
    {
        if(good()){
            _table[_vname].ends();
            if(!_temp_name.size()) _temp_name.push_back('@');
            std::function<void(std::vector<char>&, std::vector<char>::iterator)> make_name = [&make_name](std::vector<char>& _temp_name, std::vector<char>::iterator begin_){
                *begin_ += 1;
                if(*begin_ == '['){
                    *begin_ = 'A';
                    if(_temp_name.end()-1 == begin_){
                        _temp_name.push_back('A');
                        return;
                    }else{
                        return make_name(_temp_name, ++begin_);
                    }
                }
                return; 
            };
            make_name(_temp_name, _temp_name.begin());
            _vname = std::string("__") + std::string(_temp_name.rbegin(), _temp_name.rend());
            _table[_vname];
            std::string ofs(_depth, ' ');
            if(_init) _os<< std::endl << ofs << _vname << ": "<<str_;
            else      _os<< ',' << std::endl << ofs << _vname << ": "<<str_;
        }
        
    }


    auto OStructure::contains (const std::string& str_) noexcept
        -> bool
    {
        return _table.count(str_);
    }


    //_ Binary Operator
    auto OStructure::operator [] (const std::string& str_) noexcept
       -> OValue&
    {
        return at(str_);
    }




    //(    vyron::vyrio::OValue Class    )//
    //_ Constructor
    OValue::OValue (std::ostream& os_, size_t depth_)
        : OBase       { os_, depth_ }
    {
        _type = StreamType::VALUE;
        return;
    }


    //_ Constant Accessor
    auto OValue::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto OValue::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto OValue::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto OValue::at (const std::string&)
        -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void OValue::clear (void) noexcept
    {
        _status = 0x00;
        _os.clear();
        _init = true;
        return;
    }


    void OValue::ends (void) noexcept
    {
        if(good()) _status |= 0x10;
        
        return;
    }

    
    void OValue::establish (void) noexcept
    {
        if(good()){
            if(_init){
                _init = false;
            }
            _status |= 0x10;
        }
        return;
    }


    void OValue::append (const std::string& str_) noexcept
    {
        if(!_status){
            if(_init){
                _init = false;
            }
            _os << str_;
            _status |= 0x10;
        }
        return;
    }


    [[noreturn]] auto OValue::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto OValue::operator [] (const std::string&)
       -> OValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::OChannel Class    )//
    //_ Constructor
    OChannel::OChannel (void)
        : _chn      { nullptr }
        , _ref_mode { false }
    {
        return;
    }
    

    OChannel::OChannel (std::ostream& os_)
        : _chn      { nullptr }
        , _ref_mode { false }
    {
        _chn = new OValue(os_);
        return;
    }


    OChannel::OChannel (OBase& ib_)
        : _chn      { &ib_ }
        , _ref_mode { true }
    {
        return;
    }


    //_ Destructor
    OChannel::~OChannel (void)
    {
        if(!_ref_mode and _chn) delete _chn;
        return;
    }



    //_ Constant Accessor
    auto OChannel::type (void) const noexcept
        -> StreamType
    {
        return _chn -> type();
    }


    auto OChannel::fail (void) const noexcept
        -> bool
    {
        return _chn -> fail();
    }


    auto OChannel::good (void) const noexcept
        -> bool
    {
        return _chn -> good() and _chn;
    }


    auto OChannel::eof (void) const noexcept
        -> bool
    {
        return _chn -> eof();
    }


    auto OChannel::size (void) const
        -> size_t
    {
        return _chn -> size();
    }


    auto OChannel::sname (void) const
        -> const std::string&
    {
        return _chn -> sname();
    }


    auto OChannel::vname (void) const
        -> const std::string&
    {
        return _chn -> vname();
    }


    //_ Accessor
    auto OChannel::stream (void)
        -> std::ostream&
    {
        return _chn -> stream();
    }

    auto OChannel::at (const std::string& key_)
        -> OValue&
    {
        return _chn -> at(key_);
    }


    //_ Destructive Function
    void OChannel::seekp (std::streampos pos_) noexcept
    {
        return _chn -> seekp(pos_);
    }


    auto OChannel::tellp (void) noexcept
        -> std::streampos
    {
        return _chn -> tellp();
    }


    void OChannel::clear (void) noexcept
    {
        return _chn -> clear();
    }


    void OChannel::ends (void)
    {
        return _chn -> ends();
    }


    void OChannel::establish (void)
    {
        return _chn -> establish();
    }



    void OChannel::append (const std::string& str_)
    {
        return _chn -> append(str_);
    }
    

    auto OChannel::contains (const std::string& key_)
        -> bool
    {
        return _chn -> contains(key_);
    }
    

    //_ Unary Operator
    auto OChannel::operator ! (void) const
        -> bool
    {
        return _chn -> operator!();
    }


    //_ Binary Operator
    auto OChannel::operator [] (const std::string& key_)
        -> OValue&
    {
        return _chn -> operator[](key_);
    }
}
