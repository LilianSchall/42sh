#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    WORD,
    SEMICOLON,
    NEWLINE,
    AND,
    OR,
    PIPE,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PARENTHESE,
    CLOSE_PARENTHESE,
    FOR,
    DO,
    DONE,
    WHILE,
    UNTIL,
    CASE,
    ESAC,
    IF,
    THEN,
    FI,
    DOUBLE_SEMICOLON,

    // TODO redirections token, ENV_VARIABLE ?, ...
};

struct token
{
    char *symbol;
    enum token_type type;
};

// create a token with the right attributes and returns it.
// returns null if symbol is NULL (wrong execution).
struct token *new_token(char *symbol, enum token_type type);

// free a token and its attributes.
// if the token is NULL, does nothing
void free_token(struct token *token);

#endif /* !TOKEN_H */
