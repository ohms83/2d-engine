#pragma once

#include <utility>
#include <string>

/**
 * A simple and (not yet) portable class for dealing with file path.
 */
class Path
{
public:
    Path(Path&& rhs);
    Path(const std::string& path);
    Path(std::string&& path);
    
    Path& operator = (Path&& rhs);
    Path& operator = (const std::string& path);
    Path& operator = (std::string&& path);
    
    /// Return a normalized filepath of @c path where the extra path-separators are all removed.
    std::string normalize();
    /**
     * Return file name of the current filepath.
     * @param withExt If true, the returned file name will also contain file's extension (if exists).
     * @return If @c path is a directory (ending with the @c SEPARATOR ), root path (.) or top folder (..),
     *         an empty string will be returned; otherwise, returning file name with or without extension.
     */
    std::string fileName(bool withExt = true);
    std::string extension();
    /**
     * Split the pathname into a pair, (head, tail) where tail is the last pathname component
     * and head is everything leading up to that. The tail part will never contain a slash;
     * if path ends in a slash, tail will be empty. If there is no slash in path, head will be empty.
     * If path is empty, both head and tail are empty.
     * Trailing slashes are stripped from head unless it is the root (one or more slashes only)
     */
    std::pair<std::string, std::string> split();

    /**
     * A constant string represents directory separator.
     * On Windows, this is a backslash (\\); while on the Unix-like systems (Linux and MacOS), it's a forward slash (/).
     */
    static const std::string SEPARATOR;

    static const std::string ROOT;
    static const std::string TOP_FOLDER;

private:
    std::string _path;
};