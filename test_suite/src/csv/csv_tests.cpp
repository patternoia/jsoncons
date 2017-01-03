// Copyright 2013 Daniel Parker
// Distributed under Boost license

#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <jsoncons_ext/csv/csv_reader.hpp>
#include <jsoncons_ext/csv/csv_serializer.hpp>
#include <jsoncons/json_reader.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>

using namespace jsoncons;
using namespace jsoncons::csv;

BOOST_AUTO_TEST_SUITE(csv_tests)

BOOST_AUTO_TEST_CASE(csv_test_empty_values)
{
    std::string input = "bool-f,int-f,float-f,string-f"
"\n,,,,"
"\ntrue,12,24.7,\"test string\","
"\n,,,,";

    std::istringstream is(input);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true)
          .column_types({"boolean","integer","float","string"});

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val[0]["bool-f"].is_null());
    BOOST_CHECK(val[0]["bool-f"].is<null_type>());
    BOOST_CHECK(val[0]["int-f"].is_null());
    BOOST_CHECK(val[0]["int-f"].is<null_type>());
    BOOST_CHECK(val[0]["float-f"].is_null());
    BOOST_CHECK(val[0]["float-f"].is<null_type>());
    BOOST_CHECK(val[0]["string-f"].as<std::string>() == "");
    BOOST_CHECK(val[0]["string-f"].is<std::string>());

    BOOST_CHECK(val[1]["bool-f"] .as<bool>()== true);
    BOOST_CHECK(val[1]["bool-f"].is<bool>());
    BOOST_CHECK(val[1]["int-f"] .as<int>()== 12);
    BOOST_CHECK(val[1]["int-f"].is<int>());
    BOOST_CHECK(val[1]["float-f"] .as<double>()== 24.7);
    BOOST_CHECK(val[1]["float-f"].is<double>());
    BOOST_CHECK(val[1]["string-f"].as<std::string>() == "test string");
    BOOST_CHECK(val[1]["string-f"].is<std::string>());

    BOOST_CHECK(val[0]["bool-f"].is_null());
    BOOST_CHECK(val[0]["bool-f"].is<null_type>());
    BOOST_CHECK(val[0]["int-f"].is_null());
    BOOST_CHECK(val[0]["int-f"].is<null_type>());
    BOOST_CHECK(val[0]["float-f"].is_null());
    BOOST_CHECK(val[0]["float-f"].is<null_type>());
    BOOST_CHECK(val[0]["string-f"] .as<std::string>() == "");
    BOOST_CHECK(val[0]["string-f"].is<std::string>());
}

BOOST_AUTO_TEST_CASE(csv_test_empty_values_with_defaults)
{
    std::string input = "bool-f,int-f,float-f,string-f"
"\n,,,,"
"\ntrue,12,24.7,\"test string\","
"\n,,,,";

    std::istringstream is(input);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true) 
          .column_types({"boolean","integer","float","string"})
          .column_defaults({"false","0","0.0","\"\""});

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val[0]["bool-f"].as<bool>() == false);
    BOOST_CHECK(val[0]["bool-f"].is<bool>());
    BOOST_CHECK(val[0]["int-f"] .as<int>()== 0);
    BOOST_CHECK(val[0]["int-f"].is<int>());
    BOOST_CHECK(val[0]["float-f"].as<double>() == 0.0);
    BOOST_CHECK(val[0]["float-f"].is<double>());
    BOOST_CHECK(val[0]["string-f"] .as<std::string>() == "");
    BOOST_CHECK(val[0]["string-f"].is<std::string>());

    BOOST_CHECK(val[1]["bool-f"] .as<bool>()== true);
    BOOST_CHECK(val[1]["bool-f"].is<bool>());
    BOOST_CHECK(val[1]["int-f"] .as<int>()== 12);
    BOOST_CHECK(val[1]["int-f"].is<int>());
    BOOST_CHECK(val[1]["float-f"] .as<double>()== 24.7);
    BOOST_CHECK(val[1]["float-f"].is<double>());
    BOOST_CHECK(val[1]["string-f"].as<std::string>() == "test string");
    BOOST_CHECK(val[1]["string-f"].is<std::string>());

    BOOST_CHECK(val[2]["bool-f"].as<bool>() == false);
    BOOST_CHECK(val[2]["bool-f"].is<bool>());
    BOOST_CHECK(val[2]["int-f"] .as<int>()== 0);
    BOOST_CHECK(val[2]["int-f"].is<int>());
    BOOST_CHECK(val[2]["float-f"].as<double>() == 0.0);
    BOOST_CHECK(val[2]["float-f"].is<double>());
    BOOST_CHECK(val[2]["string-f"].as<std::string>() == "");
    BOOST_CHECK(val[2]["string-f"].is<std::string>());
}

BOOST_AUTO_TEST_CASE(csv_test_empty_values_with_empty_defaults)
{
    std::string input = "bool-f,int-f,float-f,string-f"
"\n,,,,"
"\ntrue,12,24.7,\"test string\","
"\n,,,,";

    std::istringstream is(input);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true)
          .column_types({"boolean","integer","float","string"})
          .column_defaults({"","","",""});

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val[0]["bool-f"].is_null());
    BOOST_CHECK(val[0]["bool-f"].is<null_type>());
    BOOST_CHECK(val[0]["int-f"].is_null());
    BOOST_CHECK(val[0]["int-f"].is<null_type>());
    BOOST_CHECK(val[0]["float-f"].is_null());
    BOOST_CHECK(val[0]["float-f"].is<null_type>());
    BOOST_CHECK(val[0]["string-f"] .as<std::string>() == "");
    BOOST_CHECK(val[0]["string-f"].is<std::string>());

    BOOST_CHECK(val[1]["bool-f"] .as<bool>() == true);
    BOOST_CHECK(val[1]["bool-f"].is<bool>());
    BOOST_CHECK(val[1]["int-f"] .as<int>()== 12);
    BOOST_CHECK(val[1]["int-f"].is<int>());
    BOOST_CHECK(val[1]["float-f"] .as<double>()== 24.7);
    BOOST_CHECK(val[1]["float-f"].is<double>());
    BOOST_CHECK(val[1]["string-f"].as<std::string>() == "test string");
    BOOST_CHECK(val[1]["string-f"].is<std::string>());

    BOOST_CHECK(val[0]["bool-f"].is_null());
    BOOST_CHECK(val[0]["bool-f"].is<null_type>());
    BOOST_CHECK(val[0]["int-f"].is_null());
    BOOST_CHECK(val[0]["int-f"].is<null_type>());
    BOOST_CHECK(val[0]["float-f"].is_null());
    BOOST_CHECK(val[0]["float-f"].is<null_type>());
    BOOST_CHECK(val[0]["string-f"] .as<std::string>() == "");
    BOOST_CHECK(val[0]["string-f"].is<std::string>());
}

BOOST_AUTO_TEST_CASE(csv_test1_array_1col_skip1)
{
    std::string text = "a\n1\n4";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.header_lines(1);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0][0]==json("1"));
    BOOST_CHECK(val[1][0]==json("4"));
}


BOOST_AUTO_TEST_CASE(csv_test1_array_1col)
{
    std::string text = "1\n4";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0][0]==json("1"));
    BOOST_CHECK(val[1][0]==json("4"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols)
{
    std::string text = "a,b,c\n1,2,3\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==3);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[2].size()==3);
    BOOST_CHECK(val[0][0]==json("a"));
    BOOST_CHECK(val[0][1]==json("b"));
    BOOST_CHECK(val[0][2]==json("c"));
    BOOST_CHECK(val[1][0]==json("1"));
    BOOST_CHECK(val[1][1]==json("2"));
    BOOST_CHECK(val[1][2]==json("3"));
    BOOST_CHECK(val[2][0]==json("4"));
    BOOST_CHECK(val[2][1]==json("5"));
    BOOST_CHECK(val[2][2]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols_trim_leading)
{
    std::string text = "a ,b ,c \n 1, 2, 3\n 4 , 5 , 6 ";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false)
          .trim_leading(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==3);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[2].size()==3);
    BOOST_CHECK(val[0][0]==json("a "));
    BOOST_CHECK(val[0][1]==json("b "));
    BOOST_CHECK(val[0][2]==json("c "));
    BOOST_CHECK(val[1][0]==json("1"));
    BOOST_CHECK(val[1][1]==json("2"));
    BOOST_CHECK(val[1][2]==json("3"));
    BOOST_CHECK(val[2][0]==json("4 "));
    BOOST_CHECK(val[2][1]==json("5 "));
    BOOST_CHECK(val[2][2]==json("6 "));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols_trim_trailing)
{
    std::string text = "a ,b ,c \n 1, 2, 3\n 4 , 5 , 6 ";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false)
          .trim_trailing(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==3);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[2].size()==3);
    BOOST_CHECK(val[0][0]==json("a"));
    BOOST_CHECK(val[0][1]==json("b"));
    BOOST_CHECK(val[0][2]==json("c"));
    BOOST_CHECK(val[1][0]==json(" 1"));
    BOOST_CHECK(val[1][1]==json(" 2"));
    BOOST_CHECK(val[1][2]==json(" 3"));
    BOOST_CHECK(val[2][0]==json(" 4"));
    BOOST_CHECK(val[2][1]==json(" 5"));
    BOOST_CHECK(val[2][2]==json(" 6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols_trim)
{
    std::string text = "a ,, \n 1, 2, 3\n 4 , 5 , 6 ";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false)
          .trim(true)
          .unquoted_empty_value_is_null(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==3);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[2].size()==3);
    BOOST_CHECK(val[0][0]==json("a"));
    BOOST_CHECK(val[0][1]==json::null());
    BOOST_CHECK(val[0][2]==json::null());
    BOOST_CHECK(val[1][0]==json("1"));
    BOOST_CHECK(val[1][1]==json("2"));
    BOOST_CHECK(val[1][2]==json("3"));
    BOOST_CHECK(val[2][0]==json("4"));
    BOOST_CHECK(val[2][1]==json("5"));
    BOOST_CHECK(val[2][2]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols_comment)
{
    std::string text = "a,b,c\n#1,2,3\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.comment_starter('#');

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0][0]==json("a"));
    BOOST_CHECK(val[0][1]==json("b"));
    BOOST_CHECK(val[0][2]==json("c"));
    BOOST_CHECK(val[1][0]==json("4"));
    BOOST_CHECK(val[1][1]==json("5"));
    BOOST_CHECK(val[1][2]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_1col)
{
    std::string text = "a\n1\n4";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[1]["a"]==json("4"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_3cols)
{
    std::string text = "a,b,c\n1,2,3\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[0]["b"]==json("2"));
    BOOST_CHECK(val[0]["c"]==json("3"));
    BOOST_CHECK(val[1]["a"]==json("4"));
    BOOST_CHECK(val[1]["b"]==json("5"));
    BOOST_CHECK(val[1]["c"]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_3cols_header)
{
    std::string text = "a,b,c\n1,2,3\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.column_names({"x","y","z"})
          .header_lines(1);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0]["x"]==json("1"));
    BOOST_CHECK(val[0]["y"]==json("2"));
    BOOST_CHECK(val[0]["z"]==json("3"));
    BOOST_CHECK(val[1]["x"]==json("4"));
    BOOST_CHECK(val[1]["y"]==json("5"));
    BOOST_CHECK(val[1]["z"]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_3cols_bool)
{
    std::string text = "a,b,c\n1,0,1\ntrue,FalSe,TrUe";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.column_names({"x","y","z"})
          .column_types({"boolean","boolean","boolean"})
          .header_lines(1);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0]["x"]==json(true));
    BOOST_CHECK(val[0]["y"]==json(false));
    BOOST_CHECK(val[0]["z"]==json(true));
    BOOST_CHECK(val[1]["x"]==json(true));
    BOOST_CHECK(val[1]["y"]==json(false));
    BOOST_CHECK(val[1]["z"]==json(true));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_1col_quoted)
{
    std::string text = "a\n\"1\"\n\"4\"";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[1]["a"]==json("4"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_3cols_quoted)
{
    std::string text = "a,b,c\n\"1\",\"2\",\"3\"\n4,5,\"6\"";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[0]["b"]==json("2"));
    BOOST_CHECK(val[0]["c"]==json("3"));
    BOOST_CHECK(val[1]["a"]==json("4"));
    BOOST_CHECK(val[1]["b"]==json("5"));
    BOOST_CHECK(val[1]["c"]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_1col_crlf)
{
    std::string text = "1\r\n4";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0][0]==json("1"));
    BOOST_CHECK(val[1][0]==json("4"));
}

BOOST_AUTO_TEST_CASE(csv_test1_array_3cols_crlf)
{
    std::string text = "a,b,c\r\n1,2,3\r\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(false);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==3);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[2].size()==3);
    BOOST_CHECK(val[0][0]==json("a"));
    BOOST_CHECK(val[0][1]==json("b"));
    BOOST_CHECK(val[0][2]==json("c"));
    BOOST_CHECK(val[1][0]==json("1"));
    BOOST_CHECK(val[1][1]==json("2"));
    BOOST_CHECK(val[1][2]==json("3"));
    BOOST_CHECK(val[2][0]==json("4"));
    BOOST_CHECK(val[2][1]==json("5"));
    BOOST_CHECK(val[2][2]==json("6"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_1col_crlf)
{
    std::string text = "a\r\n1\r\n4";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==1);
    BOOST_CHECK(val[1].size()==1);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[1]["a"]==json("4"));
}

BOOST_AUTO_TEST_CASE(csv_test1_object_3cols_crlf)
{
    std::string text = "a,b,c\r\n1,2,3\r\n4,5,6";
    std::istringstream is(text);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json val = decoder.get_result();

    BOOST_CHECK(val.size()==2);
    BOOST_CHECK(val[0].size()==3);
    BOOST_CHECK(val[1].size()==3);
    BOOST_CHECK(val[0]["a"]==json("1"));
    BOOST_CHECK(val[0]["b"]==json("2"));
    BOOST_CHECK(val[0]["c"]==json("3"));
    BOOST_CHECK(val[1]["a"]==json("4"));
    BOOST_CHECK(val[1]["b"]==json("5"));
    BOOST_CHECK(val[1]["c"]==json("6"));
}

BOOST_AUTO_TEST_CASE(read_comma_delimited_file)
{
    std::string in_file = "input/countries.csv";
    std::ifstream is(in_file);

    json_decoder<json> decoder;

    csv_parameters params;
    params.assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json countries = decoder.get_result();

    BOOST_CHECK_EQUAL(4,countries.size());
    BOOST_CHECK_EQUAL("ABW",countries[0]["country_code"].as<const char *>());
    BOOST_CHECK_EQUAL("ARUBA",countries[0]["name"].as<const char *>());
    BOOST_CHECK_EQUAL("ATF",countries[1]["country_code"].as<const char *>());
    BOOST_CHECK_EQUAL("FRENCH SOUTHERN TERRITORIES, D.R. OF",countries[1]["name"].as<const char *>());
    BOOST_CHECK_EQUAL("VUT",countries[2]["country_code"].as<const char *>());
    BOOST_CHECK_EQUAL("VANUATU",countries[2]["name"].as<const char *>());
    BOOST_CHECK_EQUAL("WLF",countries[3]["country_code"].as<const char *>());
    BOOST_CHECK_EQUAL("WALLIS & FUTUNA ISLANDS",countries[3]["name"].as<const char *>());
}

BOOST_AUTO_TEST_CASE(read_comma_delimited_file_header)
{
    std::string in_file = "input/countries.csv";
    std::ifstream is(in_file);

    json_decoder<json> decoder;

    csv_parameters params;
    params.column_names({"Country Code","Name"})
          .header_lines(1);

    csv_reader reader(is,decoder,params);
    reader.read();
    json countries = decoder.get_result();
    BOOST_CHECK_EQUAL(4,countries.size());
    BOOST_CHECK_EQUAL("ABW",countries[0]["Country Code"].as<const char *>());
    BOOST_CHECK_EQUAL("ARUBA",countries[0]["Name"].as<const char *>());
    BOOST_CHECK_EQUAL("ATF",countries[1]["Country Code"].as<const char *>());
    BOOST_CHECK_EQUAL("FRENCH SOUTHERN TERRITORIES, D.R. OF",countries[1]["Name"].as<const char *>());
    BOOST_CHECK_EQUAL("VUT",countries[2]["Country Code"].as<const char *>());
    BOOST_CHECK_EQUAL("VANUATU",countries[2]["Name"].as<const char *>());
    BOOST_CHECK_EQUAL("WLF",countries[3]["Country Code"].as<const char *>());
    BOOST_CHECK_EQUAL("WALLIS & FUTUNA ISLANDS",countries[3]["Name"].as<const char *>());
}
 
BOOST_AUTO_TEST_CASE(serialize_comma_delimited_file)
{
    std::string in_file = "input/countries.json";
    std::ifstream is(in_file);

    csv_parameters params;
    params.assume_header(false);

    json_decoder<ojson> encoder1;
    json_reader reader1(is,encoder1);
    reader1.read();
    ojson countries1 = encoder1.get_result();

    std::stringstream ss;
    csv_serializer serializer(ss,params);
    countries1.dump(serializer);

    json_decoder<ojson> encoder2;
    csv_reader reader2(ss,encoder2,params);
    reader2.read();
    ojson countries2 = encoder2.get_result();

    BOOST_CHECK_EQUAL(countries1,countries2);
}

BOOST_AUTO_TEST_CASE(test_tab_delimited_file)
{
    std::string in_file = "input/employees.txt";
    std::ifstream is(in_file);

    json_decoder<json> decoder;
    csv_parameters params;
    params.field_delimiter('\t')
          .assume_header(true);

    csv_reader reader(is,decoder,params);
    reader.read();
    json employees = decoder.get_result();
    BOOST_CHECK_EQUAL(4,employees.size());
    BOOST_CHECK_EQUAL("00000001",employees[0]["employee-no"].as<const char*>());
    BOOST_CHECK_EQUAL("00000002",employees[1]["employee-no"].as<const char*>());
    BOOST_CHECK_EQUAL("00000003",employees[2]["employee-no"].as<const char*>());
    BOOST_CHECK_EQUAL("00000004",employees[3]["employee-no"].as<const char*>());
}

BOOST_AUTO_TEST_CASE(serialize_tab_delimited_file)
{
    std::string in_file = "input/employees.json";
    std::ifstream is(in_file);

    json_decoder<ojson> decoder;
    csv_parameters params;
    params.assume_header(false)
          .header_lines(1)
          .column_names({"dept","employee-name","employee-no","note","comment","salary"})
          .field_delimiter('\t');

    json_reader reader(is,decoder);
    reader.read_next();
    ojson employees1 = decoder.get_result();

    std::stringstream ss;
    csv_serializer serializer(ss,params);
    employees1.dump(serializer);

    json_decoder<ojson> encoder2;
    csv_reader reader2(ss,encoder2,params);
    reader2.read();
    ojson employees2 = encoder2.get_result();

    BOOST_CHECK_EQUAL(employees1.size(), employees2.size());

    for (size_t i = 0; i < employees1.size(); ++i)
    {
        BOOST_CHECK_EQUAL(employees1[i]["dept"], employees2[i]["dept"]);
        BOOST_CHECK_EQUAL(employees1[i]["employee-name"], employees2[i]["employee-name"]);
        BOOST_CHECK_EQUAL(employees1[i]["employee-no"], employees2[i]["employee-no"]);
        BOOST_CHECK_EQUAL(employees1[i]["salary"], employees2[i]["salary"]);
        BOOST_CHECK_EQUAL(employees1[i].get_with_default("note",""), employees2[i].get_with_default("note",""));
    }
}

BOOST_AUTO_TEST_SUITE_END()
