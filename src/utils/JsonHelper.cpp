#include "JsonHelper.hpp"
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <iomanip>

namespace pixelverse {
namespace utils {

static JsonValue s_nullValue;

const JsonValue& JsonValue::operator[](const std::string& key) const {
    if (type != Type::Object) return s_nullValue;
    auto it = objectValue.find(key);
    if (it != objectValue.end()) {
        return it->second;
    }
    return s_nullValue;
}

JsonValue& JsonValue::operator[](const std::string& key) {
    if (type != Type::Object) {
        type = Type::Object;
        objectValue.clear();
    }
    return objectValue[key];
}

const JsonValue& JsonValue::operator[](size_t index) const {
    if (type != Type::Array || index >= arrayValue.size()) return s_nullValue;
    return arrayValue[index];
}

JsonValue& JsonValue::operator[](size_t index) {
    if (type != Type::Array) {
        type = Type::Array;
        arrayValue.clear();
    }
    if (index >= arrayValue.size()) {
        arrayValue.resize(index + 1);
    }
    return arrayValue[index];
}

std::string JsonValue::serialize(int indent) const {
    std::string ind(indent * 2, ' ');
    std::string indNext((indent + 1) * 2, ' ');

    switch (type) {
        case Type::Null:
            return "null";
        case Type::Boolean:
            return boolValue ? "true" : "false";
        case Type::Number: {
            std::ostringstream ss;
            if (numberValue == static_cast<long long>(numberValue)) {
                ss << static_cast<long long>(numberValue);
            } else {
                ss << numberValue;
            }
            return ss.str();
        }
        case Type::String: {
            std::string res = "\"";
            for (char c : stringValue) {
                if (c == '"') res += "\\\"";
                else if (c == '\\') res += "\\\\";
                else if (c == '\n') res += "\\n";
                else if (c == '\t') res += "\\t";
                else res += c;
            }
            res += "\"";
            return res;
        }
        case Type::Array: {
            if (arrayValue.empty()) return "[]";
            std::string res = "[\n";
            for (size_t i = 0; i < arrayValue.size(); ++i) {
                res += indNext + arrayValue[i].serialize(indent + 1);
                if (i + 1 < arrayValue.size()) res += ",";
                res += "\n";
            }
            res += ind + "]";
            return res;
        }
        case Type::Object: {
            if (objectValue.empty()) return "{}";
            std::string res = "{\n";
            size_t count = 0;
            for (const auto& pair : objectValue) {
                res += indNext + "\"" + pair.first + "\": " + pair.second.serialize(indent + 1);
                if (++count < objectValue.size()) res += ",";
                res += "\n";
            }
            res += ind + "}";
            return res;
        }
    }
    return "null";
}

class SimpleJsonParser {
public:
    explicit SimpleJsonParser(const std::string& src) : text(src), pos(0) {}

    JsonValue parse() {
        skipWhitespace();
        if (pos >= text.size()) return JsonValue();
        return parseValue();
    }

private:
    const std::string& text;
    size_t pos;

    void skipWhitespace() {
        while (pos < text.size() && (std::isspace(text[pos]) || text[pos] == '\r')) {
            pos++;
        }
    }

    char peek() {
        skipWhitespace();
        if (pos >= text.size()) return '\0';
        return text[pos];
    }

    char get() {
        skipWhitespace();
        if (pos >= text.size()) return '\0';
        return text[pos++];
    }

    JsonValue parseValue() {
        skipWhitespace();
        if (pos >= text.size()) return JsonValue();

        char c = text[pos];
        if (c == '{') return parseObject();
        if (c == '[') return parseArray();
        if (c == '"') return parseString();
        if (c == 't' || c == 'f') return parseBool();
        if (c == 'n') return parseNull();
        if (c == '-' || std::isdigit(c)) return parseNumber();

        return JsonValue();
    }

    JsonValue parseObject() {
        JsonValue val;
        val.type = JsonValue::Type::Object;
        get(); // consume '{'

        while (pos < text.size()) {
            skipWhitespace();
            if (peek() == '}') {
                get();
                break;
            }
            JsonValue keyVal = parseString();
            std::string key = keyVal.asString();

            skipWhitespace();
            if (get() != ':') break; // syntax error safeguard

            JsonValue memberVal = parseValue();
            val.objectValue[key] = memberVal;

            skipWhitespace();
            char nextChar = peek();
            if (nextChar == ',') {
                get();
            } else if (nextChar == '}') {
                get();
                break;
            } else {
                break;
            }
        }
        return val;
    }

    JsonValue parseArray() {
        JsonValue val;
        val.type = JsonValue::Type::Array;
        get(); // consume '['

        while (pos < text.size()) {
            skipWhitespace();
            if (peek() == ']') {
                get();
                break;
            }
            val.arrayValue.push_back(parseValue());
            skipWhitespace();
            char nextChar = peek();
            if (nextChar == ',') {
                get();
            } else if (nextChar == ']') {
                get();
                break;
            } else {
                break;
            }
        }
        return val;
    }

    JsonValue parseString() {
        get(); // consume opening quote
        std::string s;
        while (pos < text.size()) {
            char c = text[pos++];
            if (c == '"') {
                break;
            }
            if (c == '\\' && pos < text.size()) {
                char esc = text[pos++];
                if (esc == '"') s += '"';
                else if (esc == '\\') s += '\\';
                else if (esc == 'n') s += '\n';
                else if (esc == 't') s += '\t';
                else s += esc;
            } else {
                s += c;
            }
        }
        JsonValue val;
        val.type = JsonValue::Type::String;
        val.stringValue = s;
        return val;
    }

    JsonValue parseNumber() {
        size_t start = pos;
        if (pos < text.size() && text[pos] == '-') pos++;
        while (pos < text.size() && (std::isdigit(text[pos]) || text[pos] == '.' || text[pos] == 'e' || text[pos] == 'E' || text[pos] == '+')) {
            pos++;
        }
        std::string numStr = text.substr(start, pos - start);
        double num = 0.0;
        try {
            num = std::stod(numStr);
        } catch (...) {
            num = 0.0;
        }
        return JsonValue(num);
    }

    JsonValue parseBool() {
        if (text.substr(pos, 4) == "true") {
            pos += 4;
            return JsonValue(true);
        }
        if (text.substr(pos, 5) == "false") {
            pos += 5;
            return JsonValue(false);
        }
        return JsonValue(false);
    }

    JsonValue parseNull() {
        if (text.substr(pos, 4) == "null") {
            pos += 4;
        }
        return JsonValue();
    }
};

JsonValue JsonHelper::parse(const std::string& jsonStr) {
    SimpleJsonParser parser(jsonStr);
    return parser.parse();
}

bool JsonHelper::saveToFile(const std::string& filepath, const JsonValue& root) {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;
    out << root.serialize(0);
    return true;
}

JsonValue JsonHelper::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) return JsonValue();
    std::string content((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    return parse(content);
}

} // namespace utils
} // namespace pixelverse
