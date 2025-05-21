#include <fstream>
#include <iostream>
#include <regex>

#include "../include/lexer.hpp"
#include "../include/lex_error.hpp"

namespace lexer {

    Lexer::Lexer(const std::string& file_name) {
        this->file_name = file_name;
        this->current_source = {};
        this->line = 0;
        this->index = 0;
    }

    char Lexer::peek() const {
        if (current_source.empty()) {
            return '\0';
        }

        return current_source[0];
    }

    char Lexer::consume() {
        if (current_source.empty()) {
            return '\0';
        }

        const char c = current_source[0];
        current_source = current_source.substr(1);
        index++;

        return c;
    }

    bool Lexer::match_symbol(std::vector<Token>& tokens) {
        const std::vector<std::pair<char, TokenType>> symbol_map = {
            { '+', ADD },
            { '-', SUBTRACT },
            { '*', MULTIPLY },
            { '/', DIVIDE },
            { '(', LEFT_PAREN },
            { ')', RIGHT_PAREN },
            { '=', EQUALS },
        };

        for (const auto& [key, value] : symbol_map) {
            if (current_source[0] == key) {
                tokens.emplace_back(value);
                current_source = current_source.substr(1);
                index++;

                return true;
            }
        }

        return false;
    }

    bool Lexer::match_builtin_func(std::vector<Token>& tokens) {
        std::smatch match{};
        const std::regex reg(R"([a-zA-Z_][a-zA-Z0-9_]*!)");

        if (
            std::regex_search(current_source, match, reg) &&
            match.position() == 0
        ) {
            const std::string m_str = match.str(0);

            tokens.emplace_back(
                BUILTIN_FUNC,
                m_str.substr(m_str.length())
            );

            index += static_cast<int>(m_str.length());
            current_source = current_source.substr(m_str.length());
            return true;
        }

        return false;
    }

     bool Lexer::match_keyword(std::vector<Token>& tokens) {
        std::smatch match;

        const std::vector<std::regex> keywords = {
            std::regex("decm"),
            std::regex("dec"),
        };

        for (const auto& keyword : keywords) {
            if (
                std::regex_search(current_source, match, keyword) &&
                match.position() == 0
            ) {
                const std::string& str = match.str(0);
                tokens.emplace_back(KEYWORD, str);
                current_source = current_source.substr(str.length());
                index += static_cast<int>(str.length());
                return true;
            }
        }

        return false;
     }

    bool Lexer::match_identifier(std::vector<Token>& tokens) {
        std::smatch match;
        const std::regex reg(R"([a-zA-Z_][a-zA-Z0-9_]*)");

        if (
            std::regex_search(current_source, match, reg) &&
            match.position() == 0
        ) {
            const std::string& name = match.str(0);
            tokens.emplace_back(IDENTIFIER, name);
            current_source = current_source.substr(name.length());
            index += static_cast<int>(name.length());
            return true;
        }

        return false;
    }

    bool Lexer::match_string_literal(std::vector<Token>& tokens) {
        if (peek() != '"') {
            return false;
        }

        // Consume opening quote
        consume();

        std::string str{};
        bool has_closing = false;

        while (peek() != '\0') {
            if (peek() == '"') {
                consume();
                has_closing = true;
                break;
            }

            str += consume();
        }

        if (!has_closing) {
            throw LexError("Missing closing quote for string literal.", line, index);
        }

        tokens.emplace_back(STRING_LITERAL, str);
        return true;
    }

    bool Lexer::match_number(std::vector<Token>& tokens) {
        std::smatch match;
        const std::regex reg(R"([0-9]+(\.[0-9]+)?)");

        if (
            std::regex_search(current_source, match, reg) &&
            match.position() == 0
        ) {
            const std::string m_str = match.str(0);

            if (m_str.find('.') != std::string::npos) {
                tokens.emplace_back(FLOAT, m_str);
            } else {
                tokens.emplace_back(INTEGER, m_str);
            }

            index += static_cast<int>(m_str.length());
            current_source = current_source.substr(m_str.length());
            return true;
        }

        return false;
    }

    void Lexer::lex_line(std::vector<Token>& tokens) {
        while (!current_source.empty()) {
            if (isspace(peek())) {
                consume();
                continue;
            }

            if (match_symbol(tokens)) continue;
            if (match_builtin_func(tokens)) continue;
            if (match_keyword(tokens)) continue;
            if (match_string_literal(tokens)) continue;
            if (match_identifier(tokens)) continue;
            if (match_number(tokens)) continue;

            throw LexError("Unable to identify token from source:\n" + current_source, line, index);
        }
    }

    std::vector<Token> Lexer::lex_file() {
        std::vector<Token> tokens{};
        std::ifstream file(file_name);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open source file.");
        }

        while (std::getline(file, current_source)) {
            lex_line(tokens);
            tokens.emplace_back(LINE_END);
            line++;
            index = 0;
        }

        file.close();
        return tokens;
    }

}
