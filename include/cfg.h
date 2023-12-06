#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>

#define RULE_SPACE_DELINEATED_LENGTH 3

#define SPHERE 'S'
#define SUB_SPACE_OPEN '('
#define SUB_SPACE_CLOSE ')'

static const std::set<char> terminals = {
    SPHERE,
    SUB_SPACE_OPEN,
    SUB_SPACE_CLOSE
};

std::string parse_file (std::string file_path);