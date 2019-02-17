/*
 * Copyright 2019 Diomidis Spinellis
 *
 * Drop-in replacement for a GIT_EXTERNAL_DIFF program
 * By default it outputs values similar to git-diff --numstat
 * When the environment variable HECKEL_DIFF_LINES is set, it
 * behaves similarly to the default git-diff output, with the
 * exception that moved lines are prefixed with '|'.
 */

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "heckel_diff.hpp"
#include "helpers.hpp"

static char *program_name;

/*
 * Return the specified file's contents as a vector of strings
 */
static std::vector<std::string>
read_file(std::string const& path)
{
	std::ifstream ifs(path);

	if (!ifs) {
		std::cerr << program_name << ": " << path <<
			": " << std::strerror(errno) << '\n';
		exit(1);
	}

	std::string line;
	std::vector<std::string> result;

	while (std::getline(ifs, line))
		result.push_back(line);

	return result;
}

static void
output_prefixed_lines(char prefix, const std::vector<std::string> &lines)
{
	for (const auto &i : lines)
		std::cout << prefix << i << '\n';
}

int
main(int argc, char *argv[])
{
	program_name = argv[0];
	bool output_lines = (getenv("HECKEL_DIFF_LINES") != NULL);

	if (argc < 7) {
		std::cerr << "Usage " << program_name <<
			" name-a path-a hash-a mode-a path-b hash-b mode-b "
			"[string ...]\n";
		std::cerr << argc << " arguments given:\n";
		for (int i = 0; i < argc; i++)
			std::cerr << argv[i] << "\n";
		return 1;
	}
	const char *name_a = argv[1];
	const char *path_a = argv[2];
	const char *hash_a = argv[3];
	const char *mode_a = argv[4];
	const char *path_b = argv[5];
	const char *hash_b = argv[6];
	const char *mode_b = argv[7];
	const char *name_b = argv[8];

	std::vector<std::string> o(read_file(path_a));
	std::vector<std::string> n(read_file(path_b));

	HeckelDiff::Algorithm<std::string> heckel_diff;

	auto diff = heckel_diff.diff(o, n);

	auto inserted = diff[HeckelDiff::INSERTED];
	auto deleted = diff[HeckelDiff::DELETED];
	if (output_lines) {
		if (argc == 8)
			std::cout << program_name << " a/" << name_a <<
				" b/" << name_a << '\n';
		else
			// Rename
			std::cout << program_name << " a/" << name_a <<
				" b/" << name_b << '\n';
		std::cout << "index " << hash_a << ".." << hash_b << ' ' <<
			mode_a << ' ' << mode_b << '\n';
		for (int i = 9; i < argc; i++)
			std::cout << argv[i] << '\n';
		auto moved = diff[HeckelDiff::MOVED];
		auto unchanged = diff[HeckelDiff::UNCHANGED];
		output_prefixed_lines('+', inserted);
		output_prefixed_lines('-', deleted);
		output_prefixed_lines('|', moved);
		output_prefixed_lines(' ', unchanged);
	} else {
		std::cout << inserted.size() << '\t' <<
			deleted.size() << '\t' <<
			name_a;
		if (name_b)
			std::cout << " => " << name_b;
		std::cout << '\n';
	}

	return 0;
}
