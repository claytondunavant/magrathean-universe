#include "cfg.h"

std::unordered_map<char, std::string> rules;

// tokens are single chars
bool token_is_terminal(char token) {
    return terminals.find(token) != terminals.end();
}

// if token is used to define rule
bool token_is_rule(char token) {
    return rules.find(token) != rules.end();
}

void print_terminals() {
    std::cout << "Current Terminals are:" << std::endl;
    for (char terminal : terminals) {
        std::cout << terminal << std::endl;
    }
}

bool string_is_char(std::string string) {
    return string.size() == 1;
}

/*
Tokens must be 1 character that is not a terminal
 
File Format:
<num_iterations>
<starting token>
<token> : <rule>
<token> : <rule>
*/
std::string parse_file (std::string file_path) {
    
    std::ifstream config_file(file_path);
    int num_iterations;
    int line_number = 0;
    char first_token;

    if (!config_file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
    }
    
    // read line by line
    std::string line_buf;
    while ( std::getline(config_file, line_buf) ) {
        
        switch (line_number) {
            case 0: // number of iterations
                num_iterations = std::stoi(line_buf);
                break;
            case 1: // first token
                
                if(!string_is_char(line_buf)) {
                    std::cerr << "CONFIG_ERROR: First token line is not a single character" << std::endl;
                    return "";
                }
                first_token = line_buf.at(0);
                
                if (token_is_terminal(first_token)) {
                    std::cerr << "CONFIG_ERROR: First token cannot be a terminal" << std::endl;
                    print_terminals();
                }
                break;
            default: // rule

                // https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/
				// split by spaces
				std::string s;
				std::vector<std::string> words;
				std::stringstream ss(line_buf);
				while ( std::getline(ss, s, ' ') ) {
					words.push_back(s);
				}
                
                // check space delineated length of rule
                if ( words.size() != RULE_SPACE_DELINEATED_LENGTH ) {
                    std::cerr << "CONFIG_ERROR: Rule not of space-delineated length " << RULE_SPACE_DELINEATED_LENGTH << std::endl;
                    return "";
                }
                
                // check rule token
                if (!string_is_char(words[0])) {
                    std::cerr << "CONFIG_ERROR: Rule token is not a char" << std::endl;
                    return "";
                }
                char rule_token = words[0][0];
                
                // make sure rule token is not terminal
                if (token_is_terminal(rule_token)) {
                    std::cerr << "CONFIG_ERROR: Rule token cannot be a terminal" << std::endl;
                    print_terminals();
                    return "";
                }
                
                auto new_pair = std::make_pair(rule_token, words[2]);
                rules.insert(new_pair);

                break;            
        }
        
        line_number++;
    }
    
    // apply rules and iterations
    std::string universe_string;
    universe_string.push_back(first_token); // fill it with the first token
    
    std::cout << "Universe String Iteration 0: " << universe_string << std::endl;
                                            
    for ( int i = 0; i < num_iterations; i++ ) {
        
        std::string buf;

        // apply rules to tokens
        for (char c : universe_string) {

            if (token_is_terminal(c)) {
                buf.push_back(c);
            } else if (token_is_rule(c)) {
                buf += rules[c];
            } else {
                std::cerr << "Token " << c << " is neither a token nor terminal" << std::endl;
                return "";
            }
        }

        // set the universe string to the buf
        universe_string = buf;

        std::cout << "Universe String Iteration " << i + 1 << ": " << universe_string << std::endl;
    }
    
    // remove non-terminals from universe string
    for ( int i = 0; i < universe_string.size(); i++) {
        char c = universe_string.at(i);        
        if ( !token_is_terminal(c) ) {
            universe_string.erase(i, 1);
        }
    }
    
    // TODO: remove empty spaces
    
    std::cout << "Processed Universe String: " << universe_string << std::endl;
    
    return universe_string;
}