# Mergedict

Merge multiple dictionaries/word lists into one while preserving the order
Unlike `cat file1 file2 file3 | sort -u > outputfile`, this program preserves
the order.
So if your word lists are ordered by frequency, then this is the tool to preserve
that order.

## Building

```bash
clang++ -O3 -std=c++23 -stdlib=libc++ main.cpp -o main
```

## Usage

```text
./main <file 1> [<file 2> ...] <outputfile>
```

Will create `outputfile` and add any words from the previously listed files,
while ignoring duplicate entries. The files listed earlier will get their
contents appended to the output file first,
so if you have a mix of word lists with both ordered by frequency and ordered alphabetical,
then place the files with the frequency ordering first.

You can use a single file as input to remove duplicates from that file only.
In that case the first occurrence gets written out first while any subsequent
ones are ignored.

If outputfile exists, then the program will not run and print an error.

## Limitations

This program uses an in memory `std::unordered_set<std::string>` for caching.
It should work fine with large word lists like rockyou.txt (140 MB)
as long as you have enough ram.

Extremely large lists, like crackstation-all (15 GB) may not work
unless you have a beefy system.
