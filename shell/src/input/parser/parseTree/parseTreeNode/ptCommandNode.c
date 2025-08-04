/* Command parse tree node methods */

#include "ptNodeTypes.h"
#include "tokenList.h"

#include "mem.h"

Command* command_create(Token* curTok) {
        Command* cmd = safeMalloc(sizeof(*cmd), FATAL);

        cmd->first = curTok;
        cmd->last = curTok;

        switch (curTok->kType) {
                case KEY_IF:
                        cmd->type = CMD_IF;
                        break;
                case KEY_FOR: case KEY_WHILE: case KEY_UNTIL:
                        cmd->type = CMD_LOOP;
                        break;
                case KEY_CASE:
                        cmd->type = CMD_CASE;
                        break;
                case KEY_L_PAREN:
                        cmd->type = CMD_SUBSHELL;
                        break;
                case KEY_L_BRACE:
                        cmd->type = CMD_GROUP;
                        break;
                default:
                        cmd->type = CMD_SIMPLE;
                        break;
        }

        return cmd;
}

void command_destroy(Command* cmd) {
        cmd->first = NULL;
        cmd->last = NULL;
        cmd->type = CMD_SIMPLE;

        safeFree(cmd);
}

Token* command_append(Command* cmd, Token* curTok) {
        cmd->last = curTok;

        return cmd->last;
}