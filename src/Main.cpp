#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded_value(const std::string& encoded_value, size_t& index);

json decode_bencoded_string(const std::string& encoded_value, size_t& index) {

    std::string result = "";
    while (std::isdigit(encoded_value[index]))
    {
        result += encoded_value[index];
        index++;
    }
    int length = std::atoll(result.c_str());
    result = "";
    index++;
    while (length--)
    {
        result += encoded_value[index];
        index++;
    }
    return result;
}

json decode_bencoded_integer(const std::string& encoded_value, size_t& index) {
    index++;
    std::string result = "";
    while(encoded_value[index] != 'e')
    {
        result += encoded_value[index];
        index++;
    }
    index++;
    return json(std::atoll(result.c_str()));
}

json decode_bencoded_value(const std::string &encoded_value) {

  size_t index = 0;

  json res = decode_bencoded_value(encoded_value, index);

  if (index != encoded_value.size()) {

    throw std::runtime_error("String not fully consumed.");

  }

  return res;

}

json decode_bencoded_value(const std::string& encoded_value, size_t& index)
{
    if (std::isdigit(encoded_value[index]))
    {
        // Example: "5:hello" -> "hello"
        return decode_bencoded_string(encoded_value, index);
    }
    else if (encoded_value[index] == 'i')
    {
        // Example: "i45e" - > "45"
        return decode_bencoded_integer(encoded_value, index);
    }
    else if (encoded_value[index] == 'l')
    {
        // Example: "l10:strawberryi559ee" -> "[strawberry, 559]"
        // return decode_bencoded_list(encoded_value, index);
    }
    else if (encoded_value[index] == 'd')
    {
        // Example: "d3:foo3:bar5:helloi52ee" -> {"foo":"bar", "hello":"52"}
        // return decode_bencoded_dict(encoded_value, index);
    }
    else
    {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        // You can use print statements as follows for debugging, they'll be visible when running tests.
        std::cerr << "Logs from your program will appear here!" << std::endl;

        // Uncomment this block to pass the first stage
        std::string encoded_value = argv[2];
        json decoded_value = decode_bencoded_value(encoded_value);
        std::cout << decoded_value.dump() << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
