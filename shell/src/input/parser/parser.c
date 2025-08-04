#include "input.h"
#include "parseTree.h"
#include "tokenList.h"

#include "mem.h"

PTree* input_parse(TokList* toks) {
        /* Store the parse tree */
        PTree* tree = NULL;

        /* Grab the current token in the list */
        DLNode* curTok = toks->tokList->head;

        /* If the first token is not a word token (can be open parentheses or
         * curly braces), the input is malformed, and parsing fails
         * immediately. */
        if (toks->tokList->head->data.token->tType != TOK_WORD) {
                REPORT_ERR(NONFATAL, ERR_INPUT, "Syntax error");
                return NULL;
        }

        /* Begin with the full token list in the root */
        tree = pTree_create(toks);

        /* Split at ;/& */

        /* Split at ||/&& */

        /* Split at | */

        return tree;
}