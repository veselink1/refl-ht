/**
 * Created by veselink1.
 * Released under the MIT license.
 */

#ifndef RHT_FS_HPP
#define RHT_FS_HPP

#include <string>
#include <string_view>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#define PATH_MAX 4096

namespace rht::fs
{
    constexpr char PATH_SEPARATOR =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

    std::string canonical(const std::string& path)
    {
        char result[PATH_MAX + 1];
        bool success = false;

        #ifdef _WIN32
            char** lppPart = { nullptr };
            HRESULT hr = GetFullPathNameA(path.c_str(), PATH_MAX, result, lppPart);
            success = hr == 0;
        #else
            char* ptr = realpath(path.c_str(), result);
            success = ptr != nullptr;
        #endif

        if (!success) {
            throw std::runtime_error("Failed to resolve path " + path);
        }

        return result;
    }

    void create_directory(const std::string& path)
    {
        #ifdef _WIN32
            CreateDirectoryA(path.c_str(), NULL);
        #else
            mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #endif
    }

    std::string parent_path(std::string_view path)
    {
        size_t sep = path.rfind(PATH_SEPARATOR);
        if (sep == path.npos) {
            throw std::runtime_error("Incorrectly formatted path: " + std::string(path));
        }
        if (sep == path.size() - 1) {
            return parent_path(path.substr(0, path.size() - 1));
        }
        return std::string(path.substr(0, sep));
    }

    std::string filename(std::string_view path)
    {
        size_t sep = path.rfind(PATH_SEPARATOR);
        if (sep == path.npos) {
            throw std::runtime_error("Incorrectly formatted path: " + std::string(path));
        }
        return std::string(path.substr(sep + 1));
    }

    std::string concat(std::string_view left, std::string_view right)
    {
        int left_end = left.size();
        if (left.at(left_end - 1) == PATH_SEPARATOR) {
            left_end--;
        }
        
        int right_start = 0;
        if (right.at(0) == PATH_SEPARATOR) {
            right_start++;
        }

        std::string result(left.substr(0, left_end));
        result.push_back(PATH_SEPARATOR);
        result.append(right.substr(right_start));

        return result;
    }

    std::string current_directory()
    {
        char result[PATH_MAX + 1];
        bool success = false;

        #ifdef _WIN32
            success = _getcwd(result, PATH_MAX) != nullptr;
        #else
            success = getcwd(result, PATH_MAX) != nullptr;
        #endif

        if (!success) {
            throw std::runtime_error("Failed to get current working directory!");
        }

        return result;
    }

    std::string relative(const std::string& path)
    {
        std::string cwd = current_directory();
        size_t substr = path.find(cwd);
        if (substr != 0) { 
            return path;
        }

        return std::string(path).replace(0, cwd.size() , ".");     
    }
}

#endif // RHT_FS_HPP