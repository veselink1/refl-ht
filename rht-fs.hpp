/**
 * Created by veselink1.
 * Released under the MIT license.
 */

#ifndef RHT_FS_HPP
#define RHT_FS_HPP

#if __has_include(<filesystem>)

#include <filesystem>
namespace fs = std::filesystem;

#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#else
#error Missing support for std::filesystem
#endif

#endif // RHT_FS_HPP