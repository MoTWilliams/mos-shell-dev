/* Operator parse tree node methods */

#include "ptNodeTypes.h"
#include "tokenList.h"

#include "mem.h"

Operator* op_create(struct Token* curTok) {
        Operator* op = safeMalloc(sizeof(*op), FATAL);

        op->opTok = curTok;

        return op;
}

void op_destroy(Operator* op) {
        op->opTok = NULL;

        safeFree(op);
}