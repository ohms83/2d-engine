#include "Path.hpp"

#include <regex>

using namespace std;

const string Path::SEPARATOR = "/";
const string Path::ROOT = ".";
const string Path::TOP_FOLDER = "..";


Path::Path(Path&& rhs)
{
    _path = std::move(rhs._path);
}

Path::Path(const std::string& path)
{
    _path = path;
}

Path::Path(std::string&& path)
{
    _path = std::move(path);
}

    
Path& Path::operator = (Path&& rhs)
{
    _path = std::move(rhs._path);
}
Path& Path::operator = (const std::string& path)
{
    _path = path;
}

Path& Path::operator = (std::string&& path)
{
    _path = std::move(path);
}

string Path::normalize()
{
    // TODO: Implementation.
    throw new runtime_error("Unimplemented function!");
}

string Path::fileName(bool withExt)
{
    const auto sepIndex = _path.find_last_of(SEPARATOR); 
    const auto extIndex = _path.find_last_of(".");
    const auto pathLen  = _path.size();

    // If path ends with . or SEPARATOR, returns an empty string.
    if (extIndex == pathLen - 1 || sepIndex == pathLen - 1) {
        return string();
    }

    if (sepIndex == string::npos) {
        return _path;
    }

    const auto startIndex = sepIndex + 1;
    const auto endIndex = (withExt ? string::npos : extIndex - 1);
    const auto length = (endIndex == string::npos || endIndex < startIndex ? string::npos : endIndex - startIndex);
    return _path.substr(startIndex + 1, length);
}

string Path::extension()
{
    const auto extIndex = _path.find_last_of(".");
    const auto pathLen  = _path.size();

    // If path ends with '.' or SEPARATOR, or the extension cannot be found, returns an empty string.
    if (extIndex == pathLen - 1 || extIndex == string::npos) {
        return string();
    }

    return _path.substr(extIndex + 1);
}

pair<string, string> Path::split()
{
    const auto sepIndex = _path.find_last_of(SEPARATOR); 
    const auto pathLen  = _path.size();

    // If path ends with SEPARATOR, tail will be empty.
    if (sepIndex == pathLen - 1) {
        return pair<string, string>(_path, string());
    }

    // If there's no SEPARATOR, head will be empty
    if (sepIndex == string::npos) {
        return pair<string, string>(string(), _path);
    }

    return pair<string, string>(_path.substr(0, sepIndex), _path.substr(sepIndex + 1));
}