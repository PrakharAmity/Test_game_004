#ifndef PIXELVERSE_JSONHELPER_HPP
#define PIXELVERSE_JSONHELPER_HPP

#include <string>
#include <map>
#include <vector>
#include <sstream>

namespace pixelverse {
namespace utils {

class JsonValue {
public:
    enum class Type { Null, Boolean, Number, String, Array, Object };

    Type type = Type::Null;
    bool boolValue = false;
    double numberValue = 0.0;
    std::string stringValue;
    std::vector<JsonValue> arrayValue;
    std::map<std::string, JsonValue> objectValue;

    JsonValue() : type(Type::Null) {}
    JsonValue(bool b) : type(Type::Boolean), boolValue(b) {}
    JsonValue(int n) : type(Type::Number), numberValue(static_cast<double>(n)) {}
    JsonValue(double n) : type(Type::Number), numberValue(n) {}
    JsonValue(const char* s) : type(Type::String), stringValue(s) {}
    JsonValue(const std::string& s) : type(Type::String), stringValue(s) {}

    bool isNull() const { return type == Type::Null; }
    bool isObject() const { return type == Type::Object; }
    bool isArray() const { return type == Type::Array; }
    bool isNumber() const { return type == Type::Number; }
    bool isString() const { return type == Type::String; }
    bool isBool() const { return type == Type::Boolean; }

    int asInt() const { return static_cast<int>(numberValue); }
    double asDouble() const { return numberValue; }
    const std::string& asString() const { return stringValue; }
    bool asBool() const { return boolValue; }

    bool contains(const std::string& key) const {
        if (type != Type::Object) return false;
        return objectValue.find(key) != objectValue.end();
    }

    const JsonValue& operator[](const std::string& key) const;
    JsonValue& operator[](const std::string& key);

    const JsonValue& operator[](size_t index) const;
    JsonValue& operator[](size_t index);

    std::string serialize(int indent = 0) const;
};

class JsonHelper {
public:
    static JsonValue parse(const std::string& jsonStr);
    static bool saveToFile(const std::string& filepath, const JsonValue& root);
    static JsonValue loadFromFile(const std::string& filepath);
};

} // namespace utils
} // namespace pixelverse

#endif // PIXELVERSE_JSONHELPER_HPP
