#include <ichannel.hpp>

//===============================//
//    Namespace: vyron::vyrio    //
//===============================//
namespace vyron::vyrio{

    //---------------------------//
    //    Variable Definition    //
    //---------------------------//
    std::istringstream IStructure::s_iss("|");
    IValue             IStructure::s_fail_chn(s_iss);


    //---------------------------//
    //    Function Definition    //
    //---------------------------//
    //(    vyron::vyrio::IBase Class    )//
    //_ Constructor
    IBase::IBase (void)
        : _is       { *reinterpret_cast<std::istream*>(NULL) }
        , _type     { StreamType::UNDEFINED }
        , _status   { 0x11 }
        , _head_pos { -1 }
        , _prev_pos { -1 }
    {
        return;
    }


    IBase::IBase (std::istream& is_)
        : _is       { is_ }
        , _type     { StreamType::BASE }
        , _status   { static_cast<char>((!is_) ? 0x01 : 0x00) }
        , _head_pos { is_.tellg() }
        , _prev_pos { _head_pos }
    {
        return;
    }


    //_ Destructor
    IBase::~IBase (void)
    {
        return;
    }


    //_ Constant Accessor
    auto IBase::type (void) const noexcept
        -> StreamType
    {
        return _type;
    }


    auto IBase::fail (void) const noexcept
        -> bool
    {
        return (_status & 0x01) == 0x01;
    }

    
    auto IBase::good (void) const noexcept
        -> bool
    {
        return !_status;
    }


    auto IBase::eof (void) const noexcept
        -> bool
    {
        return (_status & 0x10) == 0x10;
    }


    auto IBase::headpos (void) const noexcept -> std::streampos
    {
        return _head_pos;
    }


    auto IBase::null (void) const
        -> bool
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::null" );
    }

    auto IBase::found (void) const
        -> bool
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::found" );
    }


    auto IBase::size (void) const
        -> size_t
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::size" );
    }


    auto IBase::sname (void) const
        -> const std::string&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::sname" );
    }


    auto IBase::vname (void) const
        -> const std::string&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    auto IBase::stream (void)
        -> std::istream&
    {
        return _is;
    }

    auto IBase::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void IBase::head (void) noexcept
    {
        _is.seekg(_head_pos);
        _prev_pos = _head_pos;
        if(!_is.good()) _status |= 0x01;
        return;
    }
    

    void IBase::tail (void) noexcept
    {
        while(this->skip(), !eof() and !fail());
        return;
    }


    auto IBase::tellg (void) noexcept -> std::streampos
    {
        return _is.tellg();
    }

    
   
    void IBase::seekg (std::streampos pos_) noexcept
    {
        _is.seekg(pos_);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IBase::seekg (void)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::seekg" );
    }


    void IBase::clear (void)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::clear" );
    }


    void IBase::skip (void)
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::skip" );
    }


    auto IBase::consume (void)
        -> std::string
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::consume" );
    }

    
    auto IBase::contains (const std::string&)
        -> bool
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::contains" );
    }


    //_ Unary Operator
    auto IBase::operator ! (void) const
        -> bool
    {
        return _status;
    }
    
    
    //_ Binary Operator
    auto IBase::operator [] (const std::string&)
        -> IValue&
    {
        throw exception::VYRCatchInvalidStream ( std::string{type_code} + "::operator []" );
    }




    //+    Static Function    +//
    auto IBase::rm_comma (const std::string& str_) -> std::string_view
    {
        return std::string_view{ str_.c_str(), str_.back()==',' ? str_.length()-1 : str_.length() };
    }


    bool IBase::read_string (const std::string& head_, std::istream& is_, std::string& out_)
    {
        out_ += head_;
        std::string_view str{ head_.c_str(), head_.back()==',' ? head_.length()-1 : head_.length() };
        if(str.back() != '"' or str.length() <= 1 or str[str.length()-2] == '\\'){
            is_ >> std::noskipws;
            char p1, p2 = 0;
            if(!(is_ >> p1)) return false;
            out_ += p1;
            while(is_ >> p2, p1 == '\\' or p2 != '"'){
                if(!is_) return false;
                out_ += p2;
                p1 = p2;
            }
            if(head_.front() == '"') out_ += p2;
            std::streampos pos = is_.tellg();
            is_ >> p1;
            if(p1 != ','){
                is_.seekg(pos);
                is_.clear();
            }
        }
        is_ >> std::skipws;

        return true;
    }
    

    bool IBase::skip_string (const std::string& head_, std::istream& is_)
    {
        std::string_view str{ head_.c_str(), head_.back()==',' ? head_.length()-1 : head_.length() };
        if(str.back() != '"' or str.length() <= 1 or str[str.length()-2] == '\\'){
            is_ >> std::noskipws;
            char p1, p2 = 0;
            if(!(is_ >> p1)) return false;
            while(is_ >> p2, p1 == '\\' or p2 != '"'){
                if(!is_) return false;
                p1 = p2;
            }
            std::streampos pos = is_.tellg();
            is_ >> p1;
            if(p1 != ','){
                is_.seekg(pos);
                is_.clear();
            }
        }
        is_ >> std::skipws;

        return true;
    }



    //(    vyron::vyrio::INumber Class    )//
    //_ Constructor
    INumber::INumber (std::istream& is_)
        : IBase     { is_ }
        , _next_eof { false }
    {
        _type = StreamType::NUMBER;
        
        std::string temp;
        if(!(is_ >> temp)){
            _status |= 0x01;
        }else{
            try{
                size_t pos;
                std::string str(rm_comma(temp));
                std::stod(str, &pos);
                if(pos != str.length()) _status |= 0x01;
            }catch(...){
                _status |= 0x01;
            }
        }
        head();
        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto INumber::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }


    [[noreturn]] auto INumber::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    [[noreturn]] auto INumber::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto INumber::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto INumber::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto INumber::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void INumber::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void INumber::clear (void) noexcept
    {
        _next_eof = false;
        _status = 0x00;
        _is.clear();
        return;
    }


    void INumber::skip (void) noexcept
    {
        std::string temp;
        (*this) >> temp;
        return;
    }


    auto INumber::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            if(_next_eof){
                _next_eof = false;
                _status |= 0x10;
                _prev_pos = tellg();
                return "";
            }
            if(!(_is >> out)){
                _status |= 0x01;
                return "";
            }else{
                _next_eof = true;
                _prev_pos = tellg();
                return std::string(rm_comma(out));
            }
        }
        
        return out;
    }

    
    [[noreturn]] auto INumber::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto INumber::operator [] (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::IString Class    )//
    //_ Constructor
    IString::IString (std::istream& is_)
        : IBase     { is_ }
        , _next_eof { false }
    {
        _type = StreamType::STRING;
        
        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto IString::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }


    [[noreturn]] auto IString::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    [[noreturn]] auto IString::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto IString::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto IString::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto IString::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void IString::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IString::clear (void) noexcept
    {
        _next_eof = false;
        _status = 0x00;
        _is.clear();
        return;
    }


    void IString::skip (void) noexcept
    {
        if(!_status){
            seekg();
            if(_next_eof){
                _status |= 0x10;
                _next_eof = false;
                _prev_pos = tellg();
                return;
            }
            std::string temp;
            if(!(_is >> temp)){
                _status |= 0x01;
                return;
            }else{
                if(rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(temp.front() != '"'){
                    _status |= 0x01;
                    return;
                }else{
                    if(!skip_string(temp.substr(1), _is)){
                       _status |= 0x01;
                        return;
                    }else{
                        _next_eof = true;
                        _prev_pos = tellg();
                        return;
                    }
                }
            }
        }
        return;
    }


    auto IString::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            if(_next_eof){
                _status |= 0x10;
                _next_eof = false;
                _prev_pos = tellg();
                return "";
            }
            std::string temp;
            if(!(_is >> temp)){
                _status |= 0x01;
                return "";
            }else{    
                if(rm_comma(temp).empty()){
                     _status |= 0x01;
                    return out;
                }else if(temp.front() != '"'){
                    _status |= 0x01;
                    return "";
                }else{
                    if(!read_string(temp.substr(1), _is, out)){
                        _status |= 0x01;
                        return "";
                    }else{
                        _next_eof = true;
                        _prev_pos = tellg();
                        return std::string(rm_comma(out));
                    }
                }
            }
        }
        return "";
    }

    
    [[noreturn]] auto IString::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto IString::operator [] (const std::string&)
       -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }


    
    //(    vyron::vyrio::IArray Class    )//
    //_ Constructor
    IArray::IArray (std::istream& is_)
        : IBase       { is_ }
        , _size       { 0 }
        , _const_size { false }
    {
        _type = StreamType::ARRAY;

        std::string temp;
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else{    
            if(rm_comma(temp).empty() or rm_comma(temp) != "["){
                _status |= 0x01;
                return;
            }
        }
        _head_pos = tellg();
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else if(rm_comma(temp).empty()){
            _status |= 0x01;
            return;
        }else if(temp.front() == '#'){
            std::istringstream iss(std::string(rm_comma(temp).substr(1, rm_comma(temp).length()-1)));
            INumber num(iss);
            num >> _size;
            _head_pos = tellg();
            _prev_pos = _head_pos;
            _const_size = true;
            return;
        }else{
            IBase::seekg(_head_pos);
            _prev_pos = _head_pos;
            return;
        }

        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto IArray::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }

    
    [[noreturn]] auto IArray::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    auto IArray::size (void) const noexcept
        -> size_t
    {
        return _size;
    }


    [[noreturn]] auto IArray::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto IArray::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }



    //_ Accessor
    [[noreturn]] auto IArray::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void IArray::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IArray::clear (void) noexcept
    {
        _size = 0;
        _const_size = false;
        _status = 0x00;
        _is.clear();
        return;
    }


    void IArray::skip (void) noexcept
    {
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(temp.front() == '"'){
                    skip_string(temp, _is);
                }else if(!depth and rm_comma(temp) == "]"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return;
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
            
            }while(depth);
            _prev_pos = tellg();
        }
        return;
    }


    auto IArray::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return "";
                }else if(temp.front() == '"'){
                    read_string(temp, _is, out);
                    if(depth) out += ' ';
                    continue;
                }else if(!depth and rm_comma(temp) == "]"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return out;
                }else{
                    out += rm_comma(temp);
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
                if(depth) out += ' ';
            }while(depth);
            if(!_const_size) ++_size;
            _prev_pos = tellg();
       }
        return out;
    }

    
    [[noreturn]] auto IArray::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto IArray::operator [] (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::IMap Class    )//
    //_ Constructor
    IMap::IMap (std::istream& is_)
        : IBase { is_ }
        , _size { 0 }
    {
        _type = StreamType::MAP;
        
        std::string temp;
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else{    
            if(rm_comma(temp).empty() or rm_comma(temp) != "<"){
                _status |= 0x01;
                return;
            }
        }
        _head_pos = tellg();
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else if(rm_comma(temp).empty()){
            _status |= 0x01;
            return;
        }else if(temp.front() == '#'){
            std::istringstream iss(std::string(rm_comma(temp).substr(1, rm_comma(temp).length()-1)));
            INumber num(iss);
            num >> _size;
            _head_pos = tellg();
            _prev_pos = _head_pos;
            return;
        }else{
            IBase::seekg(_head_pos);
            _prev_pos = _head_pos;
            return;
        }

        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto IMap::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }


    [[noreturn]] auto IMap::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    auto IMap::size (void) const noexcept
        -> size_t
    {
        return _size;
    }


    [[noreturn]] auto IMap::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto IMap::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto IMap::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void IMap::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IMap::clear (void) noexcept
    {
        _status = 0x00;
        _is.clear();
        return;
    }


   void IMap::skip (void) noexcept
    {
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(temp.front() == '"'){
                    skip_string(temp, _is);
                }else if(!depth and rm_comma(temp) == ">"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return;
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
            
            }while(depth);
            _prev_pos = tellg();
        }
        return;
    }


    auto IMap::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return "";
                }else if(temp.front() == '"'){
                    read_string(temp, _is, out);
                    if(depth) out += ' ';
                    continue;
                }else if(!depth and rm_comma(temp) == ">"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return out;
                }else{
                    out += rm_comma(temp);
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
                if(depth) out += ' ';

            }while(depth);
            _prev_pos = tellg();
        }
        return "";
    }

    
    [[noreturn]] auto IMap::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto IMap::operator [] (const std::string&) 
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }



    //(    vyron::vyrio::ITuple Class    )//
    //_ Constructor
    ITuple::ITuple (std::istream& is_)
        : IBase { is_ }
    {
        _type = StreamType::TUPLE;
        
        std::string temp;
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else{  
            if(rm_comma(temp).empty() or rm_comma(temp) != "("){
                _status |= 0x01;
                return;
            }
        }
        _head_pos = tellg();
        _prev_pos = _head_pos;        
        return;
    }


    //_ Constant Accessor
    [[noreturn]] auto ITuple::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }


    [[noreturn]] auto ITuple::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    auto ITuple::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto ITuple::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto ITuple::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto ITuple::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void ITuple::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void ITuple::clear (void) noexcept
    {
        _status = 0x00;
        _is.clear();
        return;
    }


   void ITuple::skip (void) noexcept
    {
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(temp.front() == '"'){
                    skip_string(temp, _is);
                }else if(!depth and rm_comma(temp) == ")"){
                    _status |= 0x10;
                    _prev_pos = tellg();;
                    return;
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
            
            }while(depth);
            _prev_pos = tellg();
        }
        return;
    }


    auto ITuple::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            std::string temp;
            int32_t depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return "";
                }else if(temp.front() == '"'){
                    read_string(temp, _is, out);
                    if(depth) out += ' ';
                    continue;
                }else if(!depth and rm_comma(temp) == ")"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return out;
                }else{
                    out += rm_comma(temp);
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
                if(depth) out += ' ';

            }while(depth);
            _prev_pos = tellg();
        }
        return out;
    }

    
    [[noreturn]] auto ITuple::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto ITuple::operator [] (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }


    //(    vyron::vyrio::IStruccture Class    )//
    //_ Constructor
    IStructure::IStructure (std::istream& is_)
        : IBase         { is_ }
        , _sname        { }
        , _vname        { }
        , _vname_enable { false }
        , _scanned_all  { false }
        , _found        { false }
        , _table        { }
    {
        _type = StreamType::STRUCTURE;
        
        std::string temp;
        if(!(_is >> temp)){
            _status |= 0x01;
            return;
        }else{    
            if(rm_comma(temp).empty() or rm_comma(temp).back() != '{'){
                _status |= 0x01;
                return;
            }
            _sname = rm_comma(temp).substr(0, rm_comma(temp).length()-1);
        }
        _head_pos = tellg();
        _prev_pos = _head_pos;

        return;
    }


    //_ Accessor
    [[noreturn]] auto IStructure::null (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::null" );
    }


    auto IStructure::found (void) const noexcept
        -> bool
    {
        return _found;
    }


    auto IStructure::size (void) const noexcept
        -> size_t
    {
        return _table.size();
    }


    auto IStructure::sname (void) const noexcept
        -> const std::string&
    {
        return _sname;
    }


    auto IStructure::vname (void) const noexcept
        -> const std::string&
    {
        return _vname;
    }


    //_ Accessor
    auto IStructure::at (const std::string& key_) noexcept
        -> IValue&
    {
        if(_table.count(key_)){
            _found = true;
            return _table[key_];
        }else if(_scanned_all){
            _found = false;
            return s_fail_chn;
        }else{
            if(!_status){
                if(!_vname_enable){
                    IBase::seekg(headpos());
                }else{
                    IBase::seekg(_table[_vname].headpos());
                    IValue(_is).tail();
                }
                while(!fail()){
                    std::string temp;
                    if(!(_is >> temp) or rm_comma(temp).empty()){
                        _status |= 0x01;
                        _found = false;
                        return s_fail_chn;
                    }else if(rm_comma(temp) == "}"){
                        _found = false;
                        _scanned_all = true;
                        return s_fail_chn;
                    }else if(rm_comma(temp).back() != ':'){
                        _status |= 0x01;
                        _found = false;
                        return s_fail_chn;
                    }else{
                        std::string vname;
                        vname = rm_comma(temp).substr(0, rm_comma(temp).length()-1);
                        if(!_table.count(vname)) _table.emplace(vname, _is);
                        _vname = vname;
                        _vname_enable = true;
                        if(vname == key_){
                            _found = true;
                            return _table[vname];
                        }else{
                            IBase::seekg(_table[vname].headpos());
                            IValue(_is).tail();
                        }
                    }
                }
            }
        }
        _found = false;
        return s_fail_chn;
    }


    //_ Destructive Function
    void IStructure::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        std::string str;
        this -> operator>(str);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IStructure::clear (void) noexcept
    {
        _vname_enable = false;
        _found = false;
        head();
        _status = 0x00;
        _is.clear();
        return;
    }


    void IStructure::skip (void) noexcept
    {
        if(!_status){
            IBase::seekg(_prev_pos);
            int depth = 0;
            int64_t loop_cnt = -1;
            do{
                std::string temp;
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(temp.front() == '"'){
                    skip_string(temp, _is);
                    continue;
                }else if(!depth and rm_comma(temp) == "}"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return;
                }else if(!depth and rm_comma(temp).back() == ':'){
                    std::string vname;
                    vname = rm_comma(temp).substr(0, rm_comma(temp).length()-1);
                    if(!_table.count(vname)) _table.emplace(vname, _is);
                    _vname = vname;
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
            
            } while(depth or loop_cnt++ < 0);
            _prev_pos = tellg();
        }
        return;
    }


    auto IStructure::consume (void) noexcept
        -> std::string
    {
        if(!_status){
            std::string out;
            IBase::seekg(_prev_pos);
            int depth = 0;
            int64_t loop_cnt = -1;
            do{
                std::string temp;
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return "";
                }else if(temp.front() == '"'){
                    read_string(temp, _is, out);
                    if(depth) out += ' ';
                    continue;
                }else if(!depth and rm_comma(temp) == "}"){
                    _status |= 0x10;
                    _prev_pos = tellg();
                    return "";
                }else if(!depth and rm_comma(temp).back() == ':'){
                    std::string vname;
                    vname = rm_comma(temp).substr(0, rm_comma(temp).length()-1);
                    if(!_table.count(vname)) _table.emplace(vname, _is);
                    _vname = vname;
                }else{
                    out += rm_comma(temp);
                }

                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
                if(depth) out += ' ';
            
            } while(depth or loop_cnt++ < 0);
            _prev_pos = tellg();
            return out;
        }
        return "";
    }

    
    auto IStructure::contains (const std::string& key_) noexcept
        -> bool
    {
        at(key_);
        return _found;
    }


    //_ Binary Operator
    auto IStructure::operator [] (const std::string& key_) noexcept
        -> IValue&
    {
        return at(key_);
    }



    //(    vyron::vyrio::IValue Class    )//
    //_ Constructor
    IValue::IValue (std::istream& is_)
        : IBase     { is_ }
        , _null     { false }
        , _next_eof { false }
    {
        _type = StreamType::VALUE;

        return;
    }


    //_ Constant Accessor
    auto IValue::null (void) const noexcept
        -> bool
    {
        return _null;
    }

    [[noreturn]] auto IValue::found (void) const
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::found" );
    }


    [[noreturn]] auto IValue::size (void) const
        -> size_t
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::size" );
    }


    [[noreturn]] auto IValue::sname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::sname" );
    }


    [[noreturn]] auto IValue::vname (void) const
        -> const std::string&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::vname" );
    }


    //_ Accessor
    [[noreturn]] auto IValue::at (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::at" );
    }


    //_ Destructive Function
    void IValue::seekg (void) noexcept
    {
        _is.seekg(_prev_pos);
        if(!_is.good()) _status |= 0x01;
        return;
    }


    void IValue::clear (void) noexcept
    {
        _null = false;
        _next_eof = false;
        _status = 0x00;
        _is.clear();
        return;
    }


    void IValue::skip (void) noexcept
    {
        if(!_status){
            seekg();
            if(_next_eof){
                _next_eof = false;
                _status |= 0x10;
                _prev_pos = tellg();
                return;
            }
            std::string temp;
            int depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return;
                }else if(rm_comma(temp) == "|"){
                    _status |= 0x01;
                    return;
                }else if(rm_comma(temp) == "_"){
                    _null = true;
                }else if(temp.front() == '"'){
                    skip_string(temp, _is);
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;

            }while(depth);
            _prev_pos = tellg();
            _next_eof = true;
            return;
        }
        return;
    }


    auto IValue::consume (void) noexcept
        -> std::string
    {
        std::string out;
        if(!_status){
            seekg();
            if(_next_eof){
                _next_eof = false;
                _status |= 0x10;
                _prev_pos = tellg();
                return "";
            }
            std::string temp;
            int depth = 0;
            do{
                _is >> temp;
                if(!_is or rm_comma(temp).empty()){
                    _status |= 0x01;
                    return "";
                }else if(rm_comma(temp) == "|"){
                    _status |= 0x01;
                    return "";
                }else if(rm_comma(temp) == "_"){
                    _null = true;
                    out += rm_comma(temp);
                }else if(temp.front() == '"'){
                    read_string(temp, _is, out);
                    if(depth) out += ' ';
                }else{
                    out += rm_comma(temp);
                }
                char b = rm_comma(temp).back();
                char f = temp.front();
                if(b == '{' or b == '[' or b == '<' or b == '(') depth +=1;
                if(f == '}' or f == ']' or f == '>' or f == ')') depth -=1;
                if(depth) out += ' ';

            }while(depth);
            if(out == "_") _null = true;
            _prev_pos = tellg();
            _next_eof = true;
            return out;
        }
        return "";
    }

    
    [[noreturn]] auto IValue::contains (const std::string&)
        -> bool
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::contains" );
    }


    //_ Binary Operator
    [[noreturn]] auto IValue::operator [] (const std::string&)
        -> IValue&
    {
        throw exception::VYRStreamMismatch ( std::string{type_code} + "::operator []" );
    }


    
    //(    vyron::vyrio::IChannel Class    )//
    //_ Constructor
    IChannel::IChannel (void)
        : _chn      { nullptr }
        , _ref_mode { false }
    {
        return;
    }
    

    IChannel::IChannel (std::istream& is_)
        : _chn      { nullptr }
        , _ref_mode { false }
    {
        std::streampos pos1 = is_.tellg();
        std::string str;
        if(!(is_ >> str) or str.empty()){
            _chn = nullptr;
            return;
        }
        is_.seekg(pos1);
        if      (str == "[")           _chn = new IArray(is_);
        else if (str == "(")           _chn = new ITuple(is_);
        else if (str == "<")           _chn = new IMap(is_);
        else if (str == "|")           _chn = new IBase(is_);
        else if (str.front() == '"')   _chn = new IString(is_);
        else if (str.back() == '{')    _chn = new IStructure(is_);
        else if (!INumber(is_).fail()) _chn = new INumber(is_);
        else                           _chn = new IValue(is_);

        return;
    }


    IChannel::IChannel (IBase& ib_)
        : _chn      { &ib_ }
        , _ref_mode { true }
    {
        return;
    }


    //_ Destructor
    IChannel::~IChannel (void)
    {
        if(!_ref_mode and _chn) delete _chn;
        return;
    }



    //_ Constant Accessor
    auto IChannel::type (void) const noexcept
        -> StreamType
    {
        return _chn -> type();
    }


    auto IChannel::fail (void) const noexcept
        -> bool
    {
        return _chn -> fail();
    }


    auto IChannel::good (void) const noexcept
        -> bool
    {
        return _chn -> good() and _chn;
    }


    auto IChannel::eof (void) const noexcept
        -> bool
    {
        return _chn -> eof();
    }


    auto IChannel::null (void) const
        -> bool
    {
        return _chn -> null();
    }


    auto IChannel::found (void) const
        -> bool
    {
        return _chn -> found();
    }


    auto IChannel::size (void) const
        -> size_t
    {
        return _chn -> size();
    }


    auto IChannel::sname (void) const
        -> const std::string&
    {
        return _chn -> sname();
    }


    auto IChannel::vname (void) const
        -> const std::string&
    {
        return _chn -> vname();
    }


    //_ Accessor
    auto IChannel::stream (void)
        -> std::istream&
    {
        return _chn -> stream();
    }

    auto IChannel::at (const std::string& key_)
        -> IValue&
    {
        return _chn -> at(key_);
    }


    //_ Destructive Function
    void IChannel::head (void) noexcept
    {
        return _chn -> head();
    }

    
    void IChannel::tail (void) noexcept
    {
        return _chn -> tail();
    }

    
    void IChannel::seekg (std::streampos pos_) noexcept
    {
        return _chn -> seekg(pos_);
    }


    void IChannel::seekg (void) noexcept
    {
        return _chn -> seekg();
    }


    auto IChannel::tellg (void) noexcept
        -> std::streampos
    {
        return _chn -> tellg();
    }


    void IChannel::clear (void) noexcept
    {
        return _chn -> clear();
    }


    void IChannel::skip (void)
    {
        return _chn -> skip();
    }


    auto IChannel::consume (void)
        -> std::string
    {
        return _chn -> consume();
    }
    

    auto IChannel::contains (const std::string& key_)
        -> bool
    {
        return _chn -> contains(key_);
    }
    

    //_ Unary Operator
    auto IChannel::operator ! (void) const
        -> bool
    {
        return _chn -> operator!();
    }


    //_ Binary Operator
    auto IChannel::operator [] (const std::string& key_)
        -> IValue&
    {
        return _chn -> operator[](key_);
    }
}
