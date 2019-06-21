/**
 * Created by veselink1.
 * Released under the MIT license.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <chrono>
#include <clang-c/Index.h>
#include "rht-engine.hpp"
#include "rht-fs.hpp"

std::unordered_map<std::string, std::string> parse_arguments(int cargs, const char* const* vargs);

int main(int cargs, const char *const vargs[])
{
    static const std::vector<std::string> supported_extensions{ ".cpp", ".h", ".hpp", ".h++" };

    auto process_start_time = std::chrono::high_resolution_clock::now();

    auto args = parse_arguments(cargs, vargs);

    long total_source_files_cnt = 0;
    long processed_source_files_cnt = 0;

    for (auto&& entry : fs::recursive_directory_iterator(args["dir"]))
    {
        auto filename = entry.path();
        if (filename.has_extension() && std::find(supported_extensions.begin(), supported_extensions.end(), filename.extension()) != supported_extensions.end())
        {
            total_source_files_cnt++;
            if (rht::engine::file_needs_processing(filename.c_str()))
            {
                processed_source_files_cnt++;
                rht::engine::process_file(filename.c_str());
            }
        }
    }

    auto process_total_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        std::chrono::high_resolution_clock::now() - process_start_time);

    // For time precision.
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Discovered " << total_source_files_cnt << " source file(s). "
              << processed_source_files_cnt << " metadata file(s) created.\n"
              << "Metadata processing took " << process_total_time.count() << "s and competed successfully.\n";
}

std::unordered_map<std::string, std::string> parse_arguments(int cargs, const char* const* vargs)
{
    static const std::vector<std::string> known_args{ "dir" };
    static const std::vector<std::string> required_args{ "dir" };
    std::unordered_map<std::string, std::string> args;

    for (int i = 1; i < cargs; i++)
    {
        std::string arg{vargs[i]};
        if (arg[0] != '-')
        {
            std::cerr << "Arguments must be passed in the format -arg=value!\n";
            std::exit(-1);
        }
        if (auto delim = arg.find('='); delim != std::string::npos)
        {
            std::string key{std::next(arg.begin()), std::next(arg.begin(), delim)};
            std::string value{std::next(arg.begin(), delim + 1), arg.end()};
            if (std::find(known_args.begin(), known_args.end(), key) == known_args.end()) 
            {
                std::cerr << "Unknown argument " << key << "!\n";
                std::cerr << "Known arguments: ";
                for (auto&& known_arg : known_args)
                {
                    std::cerr << known_arg << " ";
                }
                std::cerr << '\n';
                std::exit(-1);
            }

            args.insert({key, value});
        }
    }

    for (auto&& required_arg : required_args)
    {
        if (args[required_arg].size() == 0)
        {
            std::cerr << "Required argument -" << required_arg << "=<value> missing!\n";
            std::exit(-1);
        }
    }

    return args;
}