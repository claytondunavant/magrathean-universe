#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>

#define RULE_SPACE_DELINEATED_LENGTH 3

#define SPHERE_TOKEN 'S'
#define SUB_SPACE_OPEN_TOKEN '('
#define SUB_SPACE_CLOSE_TOKEN ')'
#define SPHERE_RADIUS_INCREASE_TOKEN '+'
#define SPHERE_RADIUS_DECREASE_TOKEN '-'
#define ILLUMINATED_SPHERE_OPEN_TOKEN '['
#define ILLUMINATED_SPHERE_CLOSE_TOKEN ']'

static const std::set<char> terminals = {
    SPHERE_TOKEN,
    SUB_SPACE_OPEN_TOKEN,
    SUB_SPACE_CLOSE_TOKEN,
    SPHERE_RADIUS_INCREASE_TOKEN,
    SPHERE_RADIUS_DECREASE_TOKEN,
    ILLUMINATED_SPHERE_OPEN_TOKEN,
    ILLUMINATED_SPHERE_CLOSE_TOKEN,
};

std::string parse_file (std::string file_path);