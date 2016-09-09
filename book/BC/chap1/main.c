/*
 *
 * Authored by Liu Peng on: 2016年 07月 20日 星期三 15:44:52 CST
 *
 * @desc: a demo of regular expression.
 *
 * @func:
 * 	Character Meaning
 * 	c Matches any literal character c.
 * 	. (period) Matches any single character.
 * 	^ Matches the beginning of the input string.
 * 	$ Matches the end of the input string.
 * 	* Matches zero or more occurrences of the previous character
**/

#include <stdlib.h>
#include "debug.h"
#include "mem.h"

static int matchstar(int c, char *regexp, char *text);
static int matchhere(char *regexp, char *text);
static int match(char *regexp, char *text);

/*
 * matchstar: search for c*regexp at beginning of text.
 */
static int matchstar(int c, char *regexp, char *text)
{
	do {	// a * matches zero or more instances.
		if (matchhere(regexp, text))
			return 1;
	} while(*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}

/*
 * matchhere: search for regexp at beginning of text
 */
static int matchhere(char *regexp, char *text)
{
	if (regexp[0] == '\0')
		return 1;
	if (regexp[1] == '*')
		return matchstar(regexp[0], regexp+2, text);

	if (regexp[0] == '$' && regexp[1] == '\0')
		return *text == '\0';
	if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
		return matchhere(regexp+1, text+1);
	return 0;
}

/*
 * match: search for regexp anywhere in text.
 */
static int match(char *regexp, char *text)
{
	if (regexp[0] == '^')
		return matchhere(regexp+1, text);
	do {	// must look even if string is empty.
		if (matchhere(regexp, text))
			return 1;
	} while (*text++ != '\0');
	return 0;
}

int main(int argc, char *argv[])
{
	int i = 0;
	char *param[] = {
		"Hello, world",
		"Who are you",
		"I love you",
	};
	char *regexp = NULL;

	if (argc < 2) {
		DEBUG("usage %s regexp.\n", argv[0]);
		return -1;
	}

	regexp	= argv[1];
	DEBUG("Got regexp: \"%s\"\n", regexp);
	for (i = 0; i < ARRAY_SIZE_GET(param); i++) {
		DEBUG("try match regexp: \"%s\" with test: \"%s\" result: %s!\n", regexp, param[i], 
				match(regexp, param[i]) ? "Matched" : "Not match");
	}

	return 0;
}

