/*  $Id: funcplot.c,v 1.25 2016/09/11 20:23:13 sarrazip Exp $

    funcplot.c - A mathematical function plotter for the Tandy CoCo.
    Copyright (C) 2016 Pierre Sarrazin <http://sarrazip.com/>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define FUNCPLOT_VER_STR "0.1.0"

/*  Version history:
        0.1.0   2016-??-??      First public version.
*/

#pragma limit 0x7000  /* Ask compiler to warn if program end reaches this address. */

#include "hirestxt.h"  /* 51x24 graphical text screen. */
#include "fp.h"        /* Floating point library. */


#define PROMPT_TEXT "Function: "


enum
{
    TEXT_ROWS                   = 4,           // this many lines at top devoted to text
    PROMPT_ROW                  = 0,
    PROMPT_COL                  = 0,
    USER_INPUT_ROW              = PROMPT_ROW,  // user command uses this row AND next row
    USER_INPUT_COL              = 10,          // text column where user command starts
    PARAM_ROW                   = 2,           // text row for x interval, number of steps, etc.
    ERROR_ROW                   = 3,           // text row for error message about the expression.
    CLEAR_CHAR                  = 12,          // Clear key used as backspace key.
    SHIFT_CLEAR_CHAR            = 92,          // Shift-Clear used as delete key.
    LEFT_CHAR                   = 8,           // Left arrow
    RIGHT_CHAR                  = 9,           // Right arrow
    KILL_LINE_CHAR              = 21,          // Shift-Left deletes the whole line.
    BREAK_CHAR                  = 3,           // Code of the Break key.
    UNKNOWN_FUNCTION_INDEX      = 0xFF,        // The user-provided function name is not implemented.
};


void initPromptArea()
{
    moveCursor(0, USER_INPUT_ROW + 1);
    clrtoeol();
    moveCursor(USER_INPUT_COL, USER_INPUT_ROW);
    clrtoeol();
}


void moveBackwardOneByte(char *src, word numBytes)
{
    memcpy(src - 1, src, numBytes);
}


void moveForwardOneByte(char *src, word numBytes)
{
    for (word i = numBytes; i > 0; --i)
        src[i] = src[i - 1];
}


byte savedX, savedY;


void saveCursorPosition()
{
    savedX = textPosX;
    savedY = textPosY;
}


void restoreCursorPosition()
{
    moveCursor(savedX, savedY);
}


void advanceCursor(word numPositions)
{
    word newX = textPosX + numPositions;
    moveCursor((byte) (newX % HIRESWIDTH), (byte) (textPosY + newX / HIRESWIDTH));
}


// dest: MUST already contain a string, even if empty.
// maxLineLength: Must include room for terminating NUL.
// Returns TRUE if the user pressed Enter, FALSE if Break.
//
byte getLineFromUser(char *dest, word maxLineLength, word initIndex)
{
    saveCursorPosition();

    word index = initIndex;  // offset of cursor in 'dest'

    // Print the existing string, if any.
    //
    word curLen = strlen(dest);
    putstr(dest, curLen);

    // Put the cursor at 'initIndex'.
    //
    restoreCursorPosition();
    advanceCursor(initIndex);

    char key;

    for (;;)
    {
        animateCursor();

        key = (char) tolower(inkey());
        if (!key)
            continue;

        if (key == '\r' || key == '\n' || key == BREAK_CHAR)  // if Enter (CoCo or PC) or Break
        {
            dest[curLen] = 0;
            break;
        }

        removeCursor();

        if (key == CLEAR_CHAR)  // if backspace
        {
            if (index > 0)
            {
                word numCharsToMove = curLen - index;
                moveBackwardOneByte(dest + index, numCharsToMove);
                --index;
                --curLen;
                putchar('\b');
                byte finalX = textPosX, finalY = textPosY;
                putstr(dest + index, numCharsToMove);
                putchar(' ');  // erase last char from screen
                moveCursor(finalX, finalY);  // place cursor at erased char
            }
            continue;
        }

        if (key == SHIFT_CLEAR_CHAR)  // if delete
        {
            if (index < curLen)
            {
                word numCharsToMove = curLen - index - 1;
                moveBackwardOneByte(dest + index + 1, numCharsToMove);
                --curLen;
                byte finalX = textPosX, finalY = textPosY;
                putstr(dest + index, numCharsToMove);
                putchar(' ');  // erase last char from screen
                moveCursor(finalX, finalY);  // place cursor at erased char
            }
            continue;
        }

        if (key == LEFT_CHAR)  // if move cursor left
        {
            if (index > 0)
            {
                --index;
                putchar('\b');
            }
            continue;
        }

        if (key == RIGHT_CHAR)  // if move cursor right
        {
            if (index < curLen)
            {
                ++index;
                advanceCursor(1);
            }
            continue;
        }

        if (key == KILL_LINE_CHAR)  // if shift-backspace
        {
            for (word i = 0; i < index; ++i)  // back up cursor to beginning
                putchar('\b');
            byte finalX = textPosX, finalY = textPosY;
            for (word i = 0; i < curLen; ++i)  // erase entire line
                putchar(' ');
            moveCursor(finalX, finalY);  // put cursor at beginning
            index = 0;
            curLen = 0;
            continue;
        }

        if (key < ' ' || key >= 127)
            continue;  // ignore invalid characters

        if (curLen < maxLineLength - 1)  // if still room
        {
            moveForwardOneByte(dest + index, curLen + 1 - index);  // include terminating NUL
            dest[index] = key;
            ++curLen;
            saveCursorPosition();
            putstr(dest + index, curLen - index);
            restoreCursorPosition();
            advanceCursor(1);
            ++index;
        }
    }

    removeCursor();

    return key != BREAK_CHAR;
}


enum RPNError validateSquareRootArgument()
{
    if (fpsgn() < 0)
        return RPN_ERR_SQRT_OF_ZERO;
    return RPN_ERR_NONE;
}


enum
{
    ERR_NONE,
    ERR_INVALID_CHAR,
    ERR_INVALID_NAME,
    ERR_INVALID_NUMBER,
    ERR_NUMBER_TOO_LONG,
    ERR_UNKNOWN_FUNCTION,
    ERR_MISS_CLOSING_PAREN,
    ERR_RPN_BUFFER_OVERFLOW,
};


typedef struct Parser
{
    char *input;
    byte error;  // ERR_* code
    byte rpnExpr[512];
    word rpnWriter;  // index into rpnExpr
} Parser;


void Parser_init(Parser *parser, char *input)
{
    parser->input = input;
    parser->error = ERR_NONE;
    parser->rpnWriter = 0;
}


// If parser->input starts with the given symbol,
// it gets advanced beyond that symbol and TRUE is returned.
// Otherwise, it does not get advanced and FALSE is returned.
//
byte Parser_passSymbol(Parser *parser, char *symbol)
{
    char *input = parser->input;

    for (;;)
    {
        if (!*symbol)
        {
            parser->input = input;
            return TRUE;
        }
        if (*input != *symbol)
            return FALSE;
        ++input;
        ++symbol;
    }
}


struct Function
{
    char *name;
    void *evalFuncPtr;  // void (*)(void): changes FP0, may trash FP1
    void *checkArgFuncPtr;  // optional function that checks if argument (FP0) is valid for function
};


// This array may have up to 254 entries.
//
struct Function knownFunctions[] =
{
    { "sin",  fpsin,  0 },
    { "cos",  fpcos,  0 },
    { "tan",  fptan,  0 },
    { "atn",  fpatn,  0 },
    { "log",  fplog,  0 },
    { "exp",  fpexp,  0 },
    { "sqr",  fpsqrt, validateSquareRootArgument },
    //{ "sqrt", fpsqrt, validateSquareRootArgument },
    { "int",  fpint,  0 },
    { "fix",  fpfix,  0 },
};


// Returns an index into knownFunctions[] if 'name' is a known
// functio name, or UNKNOWN_FUNCTION_INDEX otherwise.
//
byte getFunction(char *name)
{
    for (byte i = sizeof(knownFunctions) / sizeof(knownFunctions[0]); i--; )
        if (!strcmp(knownFunctions[i].name, name))
            return i;
    return UNKNOWN_FUNCTION_INDEX;
}


///////////////////////////////////////////////////////////////////////////////
// RPN language.
//

enum RPNCode
{
    RPN_VAR,            // Reference to the independent variable (x).
    RPN_FUNC,           // Function call (followed by 1-byte index
                        // into knownFunctions[]).
    RPN_NUM,            // Numerical constant (followed by 6-byte
                        // Color Basic floating point accumulator).
    // Operators:
    RPN_ADD,
    RPN_SUB,
    RPN_MUL,
    RPN_DIV,
    RPN_NEG,
    RPN_POW,
};


// Error codes returned by rpn_run().
enum RPNError
{
    RPN_ERR_NONE,               // Success.
    RPN_ERR_OVERFLOW,
    RPN_ERR_UNDERFLOW,
    RPN_ERR_INVALID_EXPR,
    RPN_ERR_INVALID_FUNC,
    RPN_ERR_DIV_BY_ZERO,
    RPN_ERR_POW_OF_NEG,         // taking non-integer power of a negative number (e.g., (-2)^3.1
    RPN_ERR_SQRT_OF_ZERO,
    RPN_ERR_UNEXPECTED,
};


Float verySmall;  // for division-by-zero checks

enum { RPN_STACK_SIZE = 64 };

Float rpnStack[RPN_STACK_SIZE];
byte rpnStackIndex = 0;
Float posInfinity  = { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F } };
Float negInfinity  = { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };



// Must be called before calling any other rpn_*() function.
//
void rpn_init()
{
    fpparse("1E-6");
    fp0save(&verySmall);
}


void rpn_write(Parser *parser, byte op)
{
    if (parser->rpnWriter >= sizeof(parser->rpnExpr))
    {
        parser->error = ERR_RPN_BUFFER_OVERFLOW;
        return;
    }
    //printf("[%02X]\n", op);
    parser->rpnExpr[parser->rpnWriter++] = op;
}


byte *rpn_advance(Parser *parser, byte numBytes)
{
    if (parser->rpnWriter + (word) numBytes > sizeof(parser->rpnExpr))
    {
        parser->error = ERR_RPN_BUFFER_OVERFLOW;
        return 0;
    }
    byte *ret = parser->rpnExpr + parser->rpnWriter;
    parser->rpnWriter += numBytes;
    return ret;
}


Float *rpn_get(byte index)
{
    return &rpnStack[index];
}


// rpnStackIndex must be lower than RPN_STACK_SIZE.
//
void rpn_push(Float *fp)
{
    fpcpy(rpn_get(rpnStackIndex), fp);
    ++rpnStackIndex;
}


enum RPNError rpn_run(byte *rpnExpr, word rpnLen, Float *yValue, Float *xValue)
{
    rpnStackIndex = 0;
    word reader = 0;
    while (reader < rpnLen)
    {
        byte op = rpnExpr[reader++];
        switch (op)
        {
        case RPN_VAR:  // put a copy of the independent variable's value on rpnStack
            if (rpnStackIndex == RPN_STACK_SIZE)
                return RPN_ERR_OVERFLOW;
            rpn_push(xValue);
            break;
        case RPN_FUNC:
        {
            if (rpnStackIndex < 1)
                return RPN_ERR_UNDERFLOW;
            fp0load(rpn_get(rpnStackIndex - 1));
            byte funcIndex = rpnExpr[reader++];
            if (funcIndex == UNKNOWN_FUNCTION_INDEX)
            {
                --reader;
                return RPN_ERR_INVALID_FUNC;
            }

            // Check the argument (FP0).
            void *check = knownFunctions[funcIndex].checkArgFuncPtr;
            if (check)
            {
                enum RPNError e = (*check)();  // check FP0
                if (e != RPN_ERR_NONE)
                    return e;
            }

            // Argument (FP0) is valid. Apply the function to it.
            (*knownFunctions[funcIndex].evalFuncPtr)();
            --rpnStackIndex;
            rpn_push(FP0ADDR);  // push the result
            break;
        }
        case RPN_NUM:  // put FP that follows on rpnStack
            if (rpnStackIndex == RPN_STACK_SIZE)
                return RPN_ERR_OVERFLOW;
            rpn_push((Float *) (rpnExpr + reader));  // push 6-byte accumulator
            reader += sizeof(Float);
            break;
        case RPN_ADD:
        case RPN_SUB:
        case RPN_MUL:
            if (rpnStackIndex < 2)
                return RPN_ERR_UNDERFLOW;
            fp0load(rpn_get(rpnStackIndex - 2));
            fp1load(rpn_get(rpnStackIndex - 1));
            switch (op)
            {
                case RPN_ADD: fpadd(); break;
                case RPN_SUB: fpsub(); break;
                case RPN_MUL: fpmul(); break;
            }
            rpnStackIndex -= 2;
            rpn_push(FP0ADDR);  // push the result
            break;
        case RPN_DIV:
        {
            if (rpnStackIndex < 2)
                return RPN_ERR_UNDERFLOW;

            // Check if the divisor is too close to zero
            Float *divisor = rpn_get(rpnStackIndex - 1);
            fp0load(divisor);
            fpabs();
            if (fpcmp(&verySmall) < 0)  // if abs(divisor) too small
                return RPN_ERR_DIV_BY_ZERO;  // avoid enormous (and unusable) quotient

            fp0load(divisor);
            fp1load(rpn_get(rpnStackIndex - 2));  // dividend
            fpdiv();
            rpnStackIndex -= 2;
            rpn_push(FP0ADDR);  // push the result
            break;
        }
        case RPN_NEG:
            if (rpnStackIndex < 1)
                return RPN_ERR_UNDERFLOW;
            fp0load(rpn_get(rpnStackIndex - 1));
            fpneg();
            --rpnStackIndex;
            rpn_push(FP0ADDR);  // push the result
            break;
        case RPN_POW:
            if (rpnStackIndex < 2)
                return RPN_ERR_UNDERFLOW;

            fp0load(rpn_get(rpnStackIndex - 1));  // power
            fp1load(rpn_get(rpnStackIndex - 2));  // base
            if (fp1sgn() < 0 && ! fpisint())  // if negative base and non-int power
                return RPN_ERR_POW_OF_NEG;

            fppow();
            rpnStackIndex -= 2;
            rpn_push(FP0ADDR);  // push the result
            break;
        default:
            --reader;
            return RPN_ERR_UNEXPECTED;
        }
    }

    if (rpnStackIndex != 1)
        return RPN_ERR_INVALID_EXPR;

    #if 0
    fp0load(rpn_get(0));
    char buf[FPSTRBUFSIZ];
    char *decimalResult = fpstr(buf);
    printf("Result: [%s]\n", decimalResult);
    #endif
    
    fpcpy(yValue, rpn_get(0));
    return RPN_ERR_NONE;
}


///////////////////////////////////////////////////////////////////////////////
// Recursive descent parser.
// Calls rpn_write() to build the resulting RPN expression.
// rpn_init() must have been called first.
//

byte Parser_parseExpression(Parser *parser);


byte Parser_parsePrimaryExpr(Parser *parser)
{
    char buf[16];
    byte index = 0;

    // Check for a variable or function name.
    for (;;)
    {
        byte curChar = *parser->input;
        if (curChar < 'a' || curChar > 'z')
            break;
        if (index < sizeof(buf) - 1)
            buf[index++] = curChar;
        ++parser->input;
    }
    buf[index] = 0;
    if (!strcmp(buf, "x"))
    {
        //printf("VAR: '%s'\n", buf);
        rpn_write(parser, RPN_VAR);
        return TRUE;
    }
    if (*parser->input == '(')
    {
        byte funcIndex;
        if (index)  // if got name that could be function
        {
            funcIndex = getFunction(buf);
            if (funcIndex == UNKNOWN_FUNCTION_INDEX)
            {
                parser->error = ERR_UNKNOWN_FUNCTION;
                parser->input -= strlen(buf);
                return FALSE;
            }
        }
        ++parser->input;  // pass '('
        if (!Parser_parseExpression(parser))
            return FALSE;
        if (*parser->input != ')')
        {
            parser->error = ERR_MISS_CLOSING_PAREN;
            return FALSE;
        }
        ++parser->input;
        if (index == 0)
            return TRUE;  // parenthesis

        // Call to function whose name is in buf[].
        //printf("FUNC: '%s' #%u\n", buf, funcIndex);
        rpn_write(parser, RPN_FUNC);
        rpn_write(parser, funcIndex);
        return TRUE;
    }
    if (index > 0)
    {
        parser->error = ERR_INVALID_NAME;
        parser->input -= strlen(buf);
        return FALSE;  // invalid variable, function name without argument
    }

    // Check for numerical constant.
    index = 0;
    byte pointSeen = FALSE;  // remember when decimal point processed
    for (;;)
    {
        byte curChar = *parser->input;
        if (curChar >= '0' && curChar <= '9' || curChar == '.')
        {
            if (curChar == '.')
            {
                if (pointSeen)
                {
                    parser->error = ERR_INVALID_NUMBER;
                    return FALSE;  // 2nd point: bad
                }
                pointSeen = TRUE;
            }
            if (index >= sizeof(buf) - 1)
            {
                parser->error = ERR_NUMBER_TOO_LONG;
                return FALSE;  // constant too long
            }
            buf[index++] = curChar;
            ++parser->input;
        }
        else
            break;
    }
    buf[index] = 0;
    if (index > 0)
    {
        //printf("NUM: '%s'\n", buf);
        rpn_write(parser, RPN_NUM);
        fpparse(buf);  // parse ASCII decimal to Color Basic's FP0
        Float *addrForFP = (Float *) rpn_advance(parser, sizeof(Float));  // allocate room for number
        if (addrForFP)  // if allocation succeeded
            fp0save(addrForFP);  // store number after RPN_NUM
        return TRUE;
    }

    parser->error = ERR_INVALID_CHAR;
    return FALSE;
}


byte Parser_parseUnaryExpr(Parser *parser);


byte Parser_parsePostfixExpr(Parser *parser)
{
    if (!Parser_parsePrimaryExpr(parser))
        return FALSE;
    if (!Parser_passSymbol(parser, "^"))
        return TRUE;
    if (!Parser_parseUnaryExpr(parser))
        return FALSE;
    //printf("OP: '^'\n");
    rpn_write(parser, RPN_POW);
    return TRUE;
}


byte Parser_parseUnaryExpr(Parser *parser)
{
    byte positive = TRUE;
    for (;;)
    {
        if (Parser_passSymbol(parser, "-"))
            positive = !positive;
        else if (Parser_passSymbol(parser, "+"))
            ;
        else
            break;
    }

    if (!Parser_parsePostfixExpr(parser))
        return FALSE;

    if (!positive)
    {
        //printf("OP: NEG\n");
        rpn_write(parser, RPN_NEG);
    }
    return TRUE;
}


byte Parser_parseMulExpr(Parser *parser)
{
    if (!Parser_parseUnaryExpr(parser))
        return FALSE;

    for (;;)
    {
        byte op;
        if (Parser_passSymbol(parser, "*"))
            op = RPN_MUL;
        else if (Parser_passSymbol(parser, "/"))
            op = RPN_DIV;
        else
            return TRUE;

        if (!Parser_parseUnaryExpr(parser))
            return FALSE;

        //printf("OP: '%c'\n", op == RPN_MUL ? '*' : '/');
        rpn_write(parser, op);
    }

    return TRUE;
}


byte Parser_parseExpression(Parser *parser)
{
    if (!Parser_parseMulExpr(parser))
        return FALSE;

    for (;;)
    {
        byte op;
        if (Parser_passSymbol(parser, "+"))
            op = RPN_ADD;
        else if (Parser_passSymbol(parser, "-"))
            op = RPN_SUB;
        else
            return TRUE;

        if (!Parser_parseMulExpr(parser))
            return FALSE;

        //printf("OP: '%c'\n", op == RPN_ADD ? '+' : '-');
        rpn_write(parser, op);
    }

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////


// Reset the pixel at column x (0..255) and row y (0..191) in the PMODE 4
// screen used by the hi-res text screen.
//
void reset(byte x, byte y)
{
    byte *byteOfPixel = textScreenBuffer + (word) y * BYTES_PER_PIXEL_ROW + (x >> 3);
    byte mask = (byte) (0x80 >> (x & 7));
    *byteOfPixel = *byteOfPixel & ~mask;
}


///////////////////////////////////////////////////////////////////////////////


void enlargeIntervalIfTooSmall(Float *a, Float *b, unsigned minAdmissibleInterval)
{
    Float min, halfMin, middle;

    // Encode minAdmissibleInterval into 'min'.
    fpfromuint(minAdmissibleInterval);
    fp0save(&min);
    fp0save(&halfMin);  // will have halved later

    // Compute actual interval in FP0.
    fp0load(b);
    fp1load(a);
    fpsub();  // FP0 = b - a

    // Compare actual interval with minAdmissibleInterval.
    if (fpcmp(&min) >= 0)
        return;  // actual interval large enough: nothing to do


    // Compute the middle of the interval, then the new extremes.
    fp0load(b);
    fp1load(a);
    fpadd();
    fp0tofp1();  // a + b to FP1
    fpfromuint(2);  // FP0 = 2
    fpdiv();  // FP0 = FP1 / FP0 = (a + b) / 2
    fp0save(&middle);

    fp1load(&halfMin);  // actually 'min' for now
    fpfromuint(2);  // FP0 = 2
    fpdiv();  // FP0 = min / 2
    fp0save(&halfMin);

    // Compute middle - halfMin, which is the new value of a.
    fp0load(&middle);
    fp1load(&halfMin);
    fpsub();
    fp0save(a);

    // Compute middle - halfMin, which is the new value of a.
    fp0load(&middle);
    fp1load(&halfMin);
    fpadd();
    fp0save(b);
}


char getSign(Float *f)
{
    fp0load(f);
    return fpsgn();
}


// y: 0..191.
//
void drawHorizontalDottedLine(byte y)
{
    for (word x = 0; x < PIXEL_COLS_PER_SCREEN; x += 8)  // 'word' avoids overflow at 256
        reset((byte) x, y);
}


// x: 0..255.
//
void drawVerticalDottedLine(byte x, byte maxVertGraphOffset)
{
    byte yMin = PIXEL_ROWS_PER_SCREEN - 1 - maxVertGraphOffset;
    for (byte y = PIXEL_ROWS_PER_SCREEN - 1; y >= yMin; y -= 8)  // byte b/c no overflow possible
        reset(x, y);
}


void drawGraph(Float couples[], word numCouples,
               Float *startX,
               Float *minY,
               Float *xWorldToPixels,
               Float *yWorldToPixels)
{
    //printf("couples[]:\n");
    for (word i = 0; i < numCouples; ++i)
    {
        Float *couple = &couples[i * 2];
        // Bring the x value in 0..255 (pixels):
        //   (x - startX) * 255 / (adjustedEndX - startX).
        fp0load(couple);  // x
        fp1load(startX);
        fpsub();
        fp1load(xWorldToPixels);
        fpmul();

        unsigned screenX;
        if (!fptouint(&screenX))
            break;
        if (screenX > 255)
            screenX = 255;

        // Bring the y value in 191 - 0..159 (pixels):
        //   191 - (y - minY) * 159 / (maxY - minY).
        fp1load(minY);
        fp0load(couple + 1);
        fpsub();
        fp1load(yWorldToPixels);
        fpmul();
        fp0tofp1();
        fpfromuchar(PIXEL_ROWS_PER_SCREEN - 1);
        fpsub();
        unsigned screenY;
        if (!fptouint(&screenY))
            break;
        if (screenY >= PIXEL_ROWS_PER_SCREEN)
            screenY = PIXEL_ROWS_PER_SCREEN - 1;

        //printf("screen pixel: (%u, %u)\n", screenX, screenY);
        reset((byte) screenX, (byte) screenY);
    }
}


void clearGraphArea()
{
    moveCursor(0, TEXT_ROWS);
    clrtobot();
}


void printCentered(char *s)
{
    word len = strlen(s);
    byte x = (byte) (HIRESWIDTH - len) >> 1;
    moveCursor(textPosX + x, textPosY);
    putstr(s, len);
}


void down(byte numTextRows)
{
    moveCursor(textPosX, textPosY + numTextRows);
}


// Moves cursor down two rows, then prints the entry, centered
// horizontally as if it had 'maxEntryLen' characters.
//
void printMenuEntry(word maxEntryLen, char *entry)
{
    byte x = (byte) (HIRESWIDTH - maxEntryLen) >> 1;
    moveCursor(x, textPosY + 2);
    printf("%s", entry);
}


char waitKeyWithCursor()
{
    char key;
    for (;;)
    {
        animateCursor();
        key = (char) tolower(inkey());
        if (key)
            break;
    }
    removeCursor();
    return key;
}


void showHelp()
{
    char *lines[] =
    {
    "Edit a mathematical function of x at the",
    "Function prompt. For example: 2*x+x^3-sin(x)/4.",
    "",
    "Supported functions:",
    "sin, cos, tan, atn, log, exp, sqr, int, fix.",
    "",
    "Use the left and right arrow keys to move the",
    "cursor.  Note that the left arrow key does not",
    "erase the character that precedes the cursor.",
    "To do that, use the Clear key. To erase the",
    "character at the cursor, use Shift-Clear.",
    "",
    "Press the Break key to see the main menu, which",
    "allows you to quit this program.",
    "",
    };

    byte numLines = sizeof(lines) / sizeof(lines[0]);
    for (byte i = 0; i < numLines; ++i)
    {
        moveCursor(0, (TEXT_ROWS + 3) + i);
        printf("%s", lines[i]);
    }

    printf("\n""Press a key to continue: ");
    waitKeyWithCursor();
}


byte askForNewXInterval(Float *newStartX, Float *newEndX)
{
    moveCursor(0, TEXT_ROWS + 3);
    printf("New minimum X value: ");
    char line[FPSTRBUFSIZ];
    fp0load(newStartX);
    fpstr(line);
    char *start = (line[0] == ' ' ? line + 1 : line);
    if (! getLineFromUser(start, 30, strlen(start)))
        return FALSE;
    fpparse(line);
    fp0save(newStartX);

    printf("\n");
    printf("New maximum X value: ");
    fp0load(newEndX);
    fpstr(line);
    start = (line[0] == ' ' ? line + 1 : line);
    if (! getLineFromUser(start, 30, strlen(start)))
        return FALSE;
    fpparse(line);
    fp0save(newEndX);

    printf("\n\n""Are you sure you want to set the X interval to\n[");
        fpprint(newStartX);
        printf(", ");
        fpprint(newEndX);
        printf("]? (Y/N) ");

    for (;;)
    {
        char key = waitKeyWithCursor();
        if (key == 'y')
            return TRUE;
        if (key == 'n')
            return FALSE;
    }
}


enum { MAX_NUM_STEPS = 256 };  // Max. number of points along the x interval.


// *newNumSteps must contain original value.
//
byte askForNewNumSteps(word *newNumSteps)
{
    moveCursor(0, TEXT_ROWS + 3);
    printf("New number of steps (2..%u)? ", MAX_NUM_STEPS);
    char ascii[11], line[11];
    strcpy(line, dwtoa(ascii, 0, *newNumSteps));
    if (! getLineFromUser(line, 4, strlen(line)))  // max must be enough to type MAX_NUM_STEPS
        return FALSE;

    *newNumSteps = atoui(line);

    if (*newNumSteps < 2 || *newNumSteps > MAX_NUM_STEPS)
        return FALSE;  // invalid value

    return TRUE;
}


// Returns FALSE if the user wants to quit the program, TRUE otherwise.
//
byte processMenu(Float *startX, Float *endX, word *numSteps)
{
    clearGraphArea();
    moveCursor(0, TEXT_ROWS + 3);
    printCentered("MENU");
    down(2);
    byte maxEntryLen = 48;
    printMenuEntry(maxEntryLen, "[H] Help");
    printMenuEntry(maxEntryLen, "[I] Change the X interval");
    printMenuEntry(maxEntryLen, "[S] Change the number of steps in the X interval");
    printMenuEntry(maxEntryLen, "[R] Return to the Function prompt");
    printMenuEntry(maxEntryLen, "[Q] Quit this program");
    down(2);
    printMenuEntry(maxEntryLen, "Command: ");

    for (;;)
    {
        animateCursor();

        char key = (char) tolower(inkey());
        if (!key)
            continue;

        removeCursor();
        clearGraphArea();

        switch (key)
        {
        case 'h':
            showHelp();
            break;
        case 'i':
            {
                Float newStartX, newEndX;
                fpcpy(&newStartX, startX);
                fpcpy(&newEndX, endX);
                if (askForNewXInterval(&newStartX, &newEndX))
                {
                    fpcpy(startX, &newStartX);
                    fpcpy(endX, &newEndX);
                }
            }
            break;
        case 's':
            {
                word newNumSteps = *numSteps;
                if (askForNewNumSteps(&newNumSteps))
                    *numSteps = newNumSteps;
            }
            break;
        case 'q':
            return FALSE;
        default:
            ;
        }
        clearGraphArea();
        return TRUE;
    }
}


enum { RVEC17 = 0x0191 };
byte origErrorHandlerHook[3];

// Called by the Basic error driver $AC46.
//
void silentBasicErrorHandler()
{
    asm
    {
        leas    2,s             // do not return to error driver
    }
    // The RTS at the end of this function will return to the
    // Basic routine that called the error driver.
}


void divertBasicErrorHandler()
{
    byte *hook = RVEC17;
    memcpy(origErrorHandlerHook, hook, 3);
    hook[0] = 0x7E;  // JMP extended
    * (word *) (hook + 1) = (word) silentBasicErrorHandler;
}


void restoreBasicErrorHandler()
{
    memcpy(RVEC17, origErrorHandlerHook, 3);
}


int main()
{
    // Allocate space for a sequence of (x, f(x)) couples.
    //
    Float *couples = (Float *) sbrk(MAX_NUM_STEPS * sizeof(Float) * 2);
    if (couples == (Float *) -1)
    {
        printf("NOT ENOUGH MEMORY.\n");
        return 1;
    }

    // Initialize the Reverse Polish Notation interpreter.
    //
    rpn_init();

    // Divert Basic's error handler to catch OV ERRORs, etc.
    //
    divertBasicErrorHandler();

    // Go to the 51x24 graphical text screen.
    // Assumes 4 graphics pages reserved at $0E00.
    // TRUE requests that printf() be redirected to the VT-52 interpreter,
    // which writes to the 51x24 screen.
    //
    initHiResTextScreen(0x0E00 / 512, TRUE);

    moveCursor(0, HIRESHEIGHT >> 1);
    printCentered("Hit the BREAK key for a menu, or to quit.");

    moveCursor(0, HIRESHEIGHT - 2);
    printCentered("FuncPlot v" FUNCPLOT_VER_STR " - (C) 2016 - By Pierre Sarrazin ");
    moveCursor(0, HIRESHEIGHT - 1);
    printCentered("CMOC Home Page: http://sarrazip.com/dev/cmoc.html");

    if (isCoCo3)
        setHighSpeed(TRUE);

    // Buffer for 2-line command. Must be initialized to a valid string.
    //
    char command[HIRESWIDTH * 2 - USER_INPUT_COL];
    *command = 0;

    // Plotting parameters: interval of x values, how many steps in that interval.
    //
    Float startX, endX, step;
    fpfromchar(-3);  //TODO: make this interval configurable
    fp0save(&startX);
    fpfromchar(+3);
    fp0save(&endX);

    word numSteps = 64;  //TODO: make this configurable (between 2 and MAX_NUM_STEPS)

    Parser parser;
    byte parserUsed = FALSE;

    for (;;)
    {
        // Show the plotting parameters.
        //
	moveCursor(0, PARAM_ROW);
        printf("X interval: [");
            fpprint(&startX);
            printf(", ");
            fpprint(&endX);
            printf("]. Number of steps: %u.", numSteps);
            clrtoeol();

        // Ask the user for a function of x.
        // If this is not the first time the prompt is shown, 'command' may
        // already contain an expression and the cursor is placed where the
        // parser stopped last time. If there was a parsing error, the cursor
        // will be at the error.
        //
	moveCursor(PROMPT_COL, PROMPT_ROW);
        putstr(PROMPT_TEXT, sizeof(PROMPT_TEXT));
        initPromptArea();
	byte gotEnterKey = getLineFromUser(command, sizeof(command),
                                           parserUsed ? parser.input - command : strlen(command));
        moveCursor(0, ERROR_ROW);
        clrtoeol();
	if (! gotEnterKey)  // if Break key
        {
            if (!processMenu(&startX, &endX, &numSteps))
                break;  // user requested to quit
            continue;
        }

        // Compute a slightly adjusted end for the x interval,
        // to account for float imprecisions.
        //
        fp0load(&endX);
        fp1load(&verySmall);
        fpadd();  // account for float imprecision
        Float adjustedEndX;
        fp0save(&adjustedEndX);

        clearGraphArea();

        if (!*command)
            continue;

        // Parse the expression and convert it to an RPN expression.
        //
	//printf("Command: %u chars\n", strlen(command));
        Parser_init(&parser, command);
        parserUsed = TRUE;
	if (!Parser_parseExpression(&parser))
        {
            moveCursor(0, ERROR_ROW);
            printf("Error #%u at offset %u.\n", parser.error, parser.input - command);
	    continue;
        }
        if (*parser.input)
        {
            moveCursor(0, ERROR_ROW);
            printf("Extra input at offset %u after expression.\n",
                    parser.input - command);
            continue;
        }

        #if 0  // Dump the RPN in hex.
        printf("Function parsed successfully: RPN has %u bytes\n", parser.rpnWriter);
        for (word i = 0; i < parser.rpnWriter; ++i)
            printf("%02X ", parser.rpnExpr[i]);
        printf(".\n");
        waitkey(FALSE);
        #endif

        Float xInterval;

        // Set the interval of x values on which the function will be evaluated.
        //

        // The step along that x interval is (adjustedEndX - startX) / (numSteps - 1).
        //
        fp0load(&adjustedEndX);
        fp1load(&startX);
        fpsub();
        fp0save(&xInterval);  // save adjustedEndX - startX
        fp0tofp1();  // dividend in FP1
        fpfromuint(numSteps - 1);  // divisor in FP0
        fpdiv();
        fp0save(&step);
        //printf("Step: "); fpprint(&step); printf("\n");


        word numCouples = 0;  // number of elements accumulated in couples[]


        // Initialize the independent variable (x) to the first x value.
        Float xValue;
        fpcpy(&xValue, &startX);

        // Initialize the variables that will indicate the interval of y values
        // seen across all function evaluations.
        //
        Float minY, maxY;
        fpparse("1E37");
        fp0save(&minY);
        fpneg();
        fp0save(&maxY);

        // Loop while xValue <= adjustedEndX
        //
        for (;;)
        {
            //printf("xValue: "); fpprint(&xValue); printf("; ");
            //printf("adjustedEndX: "); fpprint(&adjustedEndX); printf("\n");
            fp0load(&xValue);
            if (fpcmp(&adjustedEndX) > 0)
            {
                //printf("break at %d\n", __LINE__);
                break;
            }

            Float yValue;
            enum RPNError error = rpn_run(parser.rpnExpr, parser.rpnWriter, &yValue, &xValue);

            if (error == RPN_ERR_NONE)
            {
                // Print result.
                //fpprint(&xValue); printf(" -> "); fpprint(&yValue); printf("\n");

                // Store result in couples[].
                Float *couple = &couples[numCouples * 2];
                fpcpy(couple, &xValue);
                fpcpy(couple + 1, &yValue);
                ++numCouples;

                // Keep min and max y value.
                fp0load(&yValue);
                if (fpcmp(&minY) < 0)
                    fpcpy(&minY, &yValue);
                if (fpcmp(&maxY) > 0)
                    fpcpy(&maxY, &yValue);
            }
            else if (   error != RPN_ERR_DIV_BY_ZERO
                     && error != RPN_ERR_POW_OF_NEG
                     && error != RPN_ERR_SQRT_OF_ZERO)
            {
                // An error other than a mathematically impossible evaluation has occurred.
                printf("RPN error #%u\n", error);
                break;
            }

            // Advance xValue by step.
            fp0load(&xValue);
            fp1load(&step);
            fpadd();
            fp0save(&xValue);
        }

        enlargeIntervalIfTooSmall(&minY, &maxY, 2);

        #if 0
        printf("Y interval: [");
        fpprint(&minY);
        printf(", ");
        fpprint(&maxY);
        printf("]\n");
        waitkey(FALSE);
        #endif

        // Prepare intermediate values.

        byte maxVertGraphOffset = PIXEL_ROWS_PER_SCREEN - 1 - TEXT_ROWS * PIXEL_ROWS_PER_TEXT_ROW;

        Float yInterval;  // maxY - minY

        // World-to-vertical-pixels conversion factor.
        Float yWorldToPixels;  // 159 / (maxY - minY)
        fp0load(&maxY);
        fp1load(&minY);
        fpsub();
        fp0save(&yInterval);
        fp0tofp1();
        fpfromuchar(maxVertGraphOffset);  // dividend in FP0
        fpswap();  // dividend now in FP1 for fpdiv()
        fpdiv();  // FP0 = FP1 / FP0
        fp0save(&yWorldToPixels);

        // World-to-horizontal-pixels conversion factor.
        Float xWorldToPixels;  // 255 / (adjustedEndX - startX).
        fpfromuchar(PIXEL_COLS_PER_SCREEN - 1);
        fp0tofp1();  // dividend in FP1
        fp0load(&xInterval);  // divisor in FP0
        fpdiv();
        fp0save(&xWorldToPixels);

        // Pixel position where to draw the x axis.
        if (getSign(&minY) <= 0 && getSign(&maxY) >= 0)  // if x axis visible
        {
            // Draw x axis at 191 - |minY| / (maxY - minY) * maxVertGraphOffset.
            fp1load(&minY);
            fp1abs();
            fp0load(&yInterval);
            fpdiv();
            fp0tofp1();  // quotient to FP1
            fpfromuchar(maxVertGraphOffset);
            fpmul();
            unsigned yPos;
            fptouint(&yPos);
            drawHorizontalDottedLine(191 - (byte) yPos);
        }

        // Pixel position where to draw the y axis.
        if (getSign(&startX) <= 0 && getSign(&adjustedEndX) >= 0)  // if y axis visible
        {
            // Draw y axis at |minX| / xInterval * 255.
            fp1load(&startX);
            fp1abs();
            fp0load(&xInterval);
            fpdiv();
            fp0tofp1();  // quotient to FP1
            fpfromuchar(PIXEL_COLS_PER_SCREEN - 1);
            fpmul();
            unsigned xPos;
            fptouint(&xPos);
            drawVerticalDottedLine((byte) xPos, maxVertGraphOffset);
        }


        // Draw a graph from couples[].
        //
        drawGraph(couples, numCouples, &startX, &minY, &xWorldToPixels, &yWorldToPixels);
    }

    restoreBasicErrorHandler();

    closeHiResTextScreen();  // this unhooks printf() from VT-52

    cls(255);  // go back to hardware text mode

    if (isCoCo3)
        setHighSpeed(FALSE);
    return 0;
}
