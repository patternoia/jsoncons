// Copyright 2017 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CBOR_CBOR_READER_HPP
#define JSONCONS_CBOR_CBOR_READER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <istream>
#include <cstdlib>
#include <memory>
#include <limits>
#include <cassert>
#include <iterator>
#include <jsoncons/json.hpp>
#include <jsoncons/detail/source.hpp>
#include <jsoncons/json_content_handler.hpp>
#include <jsoncons/config/binary_detail.hpp>
#include <jsoncons_ext/cbor/cbor_serializer.hpp>
#include <jsoncons_ext/cbor/cbor_error.hpp>
#include <jsoncons_ext/cbor/cbor_details.hpp>
#include <jsoncons_ext/cbor/cbor_view.hpp>

namespace jsoncons { namespace cbor {

template <class Source>
class basic_cbor_reader : public serializing_context
{
    Source source_;
    json_content_handler& handler_;
    size_t column_;
    size_t nesting_depth_;
    std::string buffer_;
public:
    basic_cbor_reader(Source&& source, json_content_handler& handler)
       : source_(std::move(source)),
         handler_(handler), 
         column_(1),
         nesting_depth_(0)
    {
    }

    void reset()
    {
        column_ = 1;
        nesting_depth_ = 0;
    }

    void read(std::error_code& ec)
    {
        bool has_cbor_tag = false;
        uint8_t cbor_tag = 0;

        if (get_major_type(source_.peek()) == cbor_major_type::semantic_tag)
        {
            has_cbor_tag = true;
            uint8_t c;
            if (source_.get(c) == 0)
            {
                ec = cbor_errc::unexpected_eof;
                return;
            }
            cbor_tag = get_additional_information_value(c);
        }

        switch (get_major_type(source_.peek()))
        {
            case cbor_major_type::unsigned_integer:
            {
                uint64_t val = jsoncons::cbor::detail::get_uint64_value(source_, ec);
                if (ec)
                {
                    return;
                }

                if (has_cbor_tag && cbor_tag == 1)
                {
                    handler_.uint64_value(val, semantic_tag_type::epoch_time, *this);
                }
                else
                {
                    handler_.uint64_value(val, semantic_tag_type::none, *this);
                }
                break;
            }
            case cbor_major_type::negative_integer:
            {
                int64_t val = jsoncons::cbor::detail::get_int64_value(source_, ec);
                if (ec)
                {
                    return;
                }
                if (has_cbor_tag && cbor_tag == 1)
                {
                    handler_.int64_value(val, semantic_tag_type::epoch_time, *this);
                }
                else 
                {
                    handler_.int64_value(val, semantic_tag_type::none, *this);
                }
                break;
            }
            case cbor_major_type::byte_string:
            {
                std::vector<uint8_t> v = jsoncons::cbor::detail::get_byte_string(source_, ec);
                if (ec)
                {
                    return;
                }

                if (has_cbor_tag)
                {
                    switch (cbor_tag)
                    {
                        case 0x2:
                            {
                                bignum n(1, v.data(), v.size());
                                buffer_.clear();
                                n.dump(buffer_);
                                handler_.bignum_value(buffer_, *this);
                                break;
                            }
                        case 0x3:
                            {
                                bignum n(-1, v.data(), v.size());
                                buffer_.clear();
                                n.dump(buffer_);
                                handler_.bignum_value(buffer_, *this);
                                break;
                            }
                        case 0x15:
                            {
                                handler_.byte_string_value(byte_string_view(v.data(), v.size()), byte_string_chars_format::base64url, semantic_tag_type::none, *this);
                                break;
                            }
                        case 0x16:
                            {
                                handler_.byte_string_value(byte_string_view(v.data(), v.size()), byte_string_chars_format::base64, semantic_tag_type::none, *this);
                                break;
                            }
                        case 0x17:
                            {
                                handler_.byte_string_value(byte_string_view(v.data(), v.size()), byte_string_chars_format::base16, semantic_tag_type::none, *this);
                                break;
                            }
                        default:
                            handler_.byte_string_value(byte_string_view(v.data(), v.size()), byte_string_chars_format::none, semantic_tag_type::none, *this);
                            break;
                    }
                }
                else
                {
                    handler_.byte_string_value(byte_string_view(v.data(), v.size()), byte_string_chars_format::none, semantic_tag_type::none, *this);
                }
                break;
            }
            case cbor_major_type::text_string:
            {
                std::string s = jsoncons::cbor::detail::get_text_string(source_, ec);
                if (ec)
                {
                    return;
                }
                if (has_cbor_tag && cbor_tag == 0)
                {
                    handler_.string_value(basic_string_view<char>(s.data(),s.length()), semantic_tag_type::date_time, *this);
                }
                else
                {
                    handler_.string_value(basic_string_view<char>(s.data(),s.length()), semantic_tag_type::none, *this);
                }
                break;
            }
            case cbor_major_type::array:
            {
                size_t info = get_additional_information_value(source_.peek());

                semantic_tag_type tag = semantic_tag_type::none;
                if (has_cbor_tag)
                {
                    switch (cbor_tag)
                    {
                        case 0x04:
                            tag = semantic_tag_type::decimal_fraction;
                            break;
                        case 0x05:
                            tag = semantic_tag_type::bigfloat;
                            break;
                        default:
                            break;
                    }
                }
                if (tag == semantic_tag_type::decimal_fraction)
                {
                    std::string s = jsoncons::cbor::detail::get_array_as_decimal_string(source_, ec);
                    if (ec)
                    {
                        return;
                    }
                    handler_.string_value(s, semantic_tag_type::decimal_fraction);
                }
                else
                {
                    switch (info)
                    {
                        case additional_info::indefinite_length:
                        {
                            ++nesting_depth_;
                            handler_.begin_array(tag, *this);
                            source_.increment();
                            while (source_.peek() != 0xff)
                            {
                                read(ec);
                                if (ec)
                                {
                                    return;
                                }
                                if (source_.eof())
                                {
                                    ec = cbor_errc::unexpected_eof;
                                    return;
                                }
                            }
                            source_.increment();
                            handler_.end_array(*this);
                            --nesting_depth_;
                            break;
                        }
                        default: // definite length
                        {
                            size_t len = jsoncons::cbor::detail::get_length(source_,ec);
                            if (ec)
                            {
                                return;
                            }
                            ++nesting_depth_;
                            handler_.begin_array(len, tag, *this);
                            for (size_t i = 0; i < len; ++i)
                            {
                                read(ec);
                                if (ec)
                                {
                                    return;
                                }
                            }
                            handler_.end_array(*this);
                            --nesting_depth_;
                            break;
                        }
                    }
                }
                break;
            }
            case cbor_major_type::map:
            {
                size_t info = get_additional_information_value(source_.peek());
                switch (info)
                {
                    case additional_info::indefinite_length: 
                    {
                        ++nesting_depth_;
                        handler_.begin_object(semantic_tag_type::none, *this);
                        source_.increment();
                        while (source_.peek() != 0xff)
                        {
                            parse_name(ec);
                            if (ec)
                            {
                                return;
                            }
                            read(ec);
                            if (ec)
                            {
                                return;
                            }
                        }
                        source_.increment();
                        handler_.end_object(*this);
                        --nesting_depth_;
                        break;
                    }
                    default: // definite_length
                    {
                        size_t len = jsoncons::cbor::detail::get_length(source_, ec);
                        if (ec)
                        {
                            return;
                        }
                        ++nesting_depth_;
                        handler_.begin_object(len, semantic_tag_type::none, *this);
                        for (size_t i = 0; i < len; ++i)
                        {
                            parse_name(ec);
                            if (ec)
                            {
                                return;
                            }
                            read(ec);
                            if (ec)
                            {
                                return;
                            }
                        }
                        handler_.end_object(*this);
                        --nesting_depth_;
                        break;
                    }
                }
                break;
            }
            case cbor_major_type::semantic_tag:
            {
                break;
            }
            case cbor_major_type::simple:
            {
                size_t info = get_additional_information_value(source_.peek());
                switch (info)
                {
                    case 0x14:
                        handler_.bool_value(false, semantic_tag_type::none, *this);
                        source_.increment();
                        break;
                    case 0x15:
                        handler_.bool_value(true, semantic_tag_type::none, *this);
                        source_.increment();
                        break;
                    case 0x16:
                        handler_.null_value(semantic_tag_type::none, *this);
                        source_.increment();
                        break;
                    case 0x17:
                        handler_.null_value(semantic_tag_type::undefined, *this);
                        source_.increment();
                        break;
                    case 0x19: // Half-Precision Float (two-byte IEEE 754)
                    case 0x1a: // Single-Precision Float (four-byte IEEE 754)
                    case 0x1b: // Double-Precision Float (eight-byte IEEE 754)
                        double val = jsoncons::cbor::detail::get_double(source_, ec);
                        if (ec)
                        {
                            return;
                        }
                        if (has_cbor_tag && cbor_tag == 1)
                        {
                            handler_.double_value(val, floating_point_options(), semantic_tag_type::epoch_time, *this);
                        }
                        else
                        {
                            handler_.double_value(val, floating_point_options(), semantic_tag_type::none, *this);
                        }
                        break;
                }
                break;
            }
        }
        if (nesting_depth_ == 0)
        {
            handler_.flush();
        }
    }

    size_t line_number() const override
    {
        return 1;
    }

    size_t column_number() const override
    {
        return column_;
    }
private:
    void parse_name(std::error_code& ec)
    {
        if (get_major_type(source_.peek()) == cbor_major_type::text_string)
        {
            std::string s = jsoncons::cbor::detail::get_text_string(source_,ec);
            if (ec)
            {
                return;
            }
            handler_.name(basic_string_view<char>(s.data(),s.length()), *this);
        }
        else
        {
            //cbor_view v(pos,end_input_ - pos);
            //std::string s = v.as_string();
            //handler_.name(basic_string_view<char>(s.data(),s.length()), *this);
        }
    }
};

typedef basic_cbor_reader<jsoncons::detail::buffer_source> cbor_reader;

}}

#endif