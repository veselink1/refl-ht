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

std::unordered_map<std::string, std::string> parse_arguments(int cargs, const char *const *vargs);

int main(int cargs, const char *const vargs[]) try
{
    auto process_start_time = std::chrono::high_resolution_clock::now();

    auto args = parse_arguments(cargs, vargs);

    if (!args["verbose"].empty())
    {
        if (args["verbose"] == "y" || args["verbose"] == "yes")
        {
            rht::util::enable_log_info();
        }
        else if (args["verbose"] != "n" && args["verbose"] != "no")
        {
            std::cerr << "Argument '-verbose' must be one of y|yes|n|no!\n";
            std::exit(-1);
        }
    }

    rht::engine::process_file(rht::fs::canonical(args["input"]).c_str());

    auto process_total_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        std::chrono::high_resolution_clock::now() - process_start_time);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << rht::fs::filename(vargs[0]) << "]: Metadata processing took " << process_total_time.count() << "s. [exited]\n";
}
catch (const std::exception& e)
{
    rht::util::log_error() << rht::util::to_string(e) << "\n";
}

std::unordered_map<std::string, std::string> parse_arguments(int cargs, const char *const *vargs)
{
    static const std::vector<std::string> known_args{"input", "verbose"};
    static const std::vector<std::string> required_args{"input"};
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
                std::cerr << "Unknown argument '-" << key << "=<value>'!\n";
                std::cerr << "Known arguments: ";
                for (auto&& known_arg : known_args)
                {
                    std::cerr << '-' << known_arg << "=<value> ";
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