#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <unordered_set>


// NOLINTNEXTLINE(readability-function-cognitive-complexity) // I don't give a crap.
auto main(int argc, char** argv) -> int
{
    if (argc < 3) // at least program path, 2 files to merge and the output.
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic) // No other way to access argv
        std::print("Usage:\n\t{} <file1> [<file2]> ...] <output file>\n\tYou are expected to make sure not to provide the same file twice.\n", argv[0]);
        return 1;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic) // No other way to access argv
    std::string outputFilePath = argv[argc - 1];
    if (std::filesystem::exists(outputFilePath))
    {
        std::print("{} exists. Would overwrite. Delete manually or pick a different name.\n", outputFilePath);
        return 1;
    }

    std::vector<std::string> inputFilePaths;
    for (auto argIdx : std::ranges::views::iota(1) | std::ranges::views::take(argc - 2))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic) // No other way to access argv
        inputFilePaths.emplace_back(argv[argIdx]);
        std::print("Wordlist file {}: {}\n", argIdx, inputFilePaths.back());
    }

    auto outputFile = std::fstream(outputFilePath, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open())
    {
        std::print("Can not open {} for writing.", outputFilePath);
        return 1;
    }

    std::unordered_set<std::string> knownWordsCache{};

    for (const auto& inputFilePath : inputFilePaths)
    {
        std::print("{}", inputFilePath);
        std::cout << std::flush;

        auto inputFile = std::ifstream(inputFilePath, std::ios::in);
        if (!inputFile.is_open())
        {
            std::print("- Unable to open.\n");
            return 1;
        }
        // run over the input file, line by line
        std::string inputLine;
        std::uint64_t lineCount {0};
        std::uint64_t newLines {0};
        std::uint64_t duplicates {0};

        while (std::getline(inputFile, inputLine))
        {
            lineCount++;
            // run over the output file and find the line.
            bool lineFound = knownWordsCache.contains(inputLine);
            duplicates += static_cast<std::uint64_t>(lineFound);

            if (!lineFound)
            {
                newLines++;
                outputFile << inputLine << '\n';
                knownWordsCache.insert(inputLine);
                if (outputFile.fail())
                {
                    std::print(" - Write error in output file\n");
                    return 1;
                }
            }
        }

        if (inputFile.fail() && !inputFile.eof())
        {
            std::print(" - Read error\n");
            return 1;
        }
        inputFile.close();
        std::print(" - Done. {} Lines processed. {} new lines added. {} duplicates ignored.\n", lineCount, newLines, duplicates);
        std::cout << std::flush;
    }

    return 0;
}
