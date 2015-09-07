/*
 *
 * Authored by Liu Peng on: Mon Nov  3 14:41:45 CST 2014
 *
 * @desc: "if" processing is done with two state variables
 *	  if_state and is_result
 * @func:
**/

#include <stdio.h>
#include "smsh.h"

enum states {
	NEUTRAL, 
	WANT_THEN, 
	THEN_BLOCK
};

enum results {

	SUCCESS, 
	FAIL
};

static int	if_state	= NEUTRAL;
static int	if_result	= SUCCESS;
static int	last_stat	= 0;

int syn_err(char *);

/*
 * Funcion	: determine the shell should execute a command
 *
 * details	: if in THEN_BLOCK and if_result was SUCCESS then yes
 *		  if in THEN_BLOCK and if_reault was FAIL then no
 *		  if in WANT_THEN the syntax error(sh is different)
 * arguments	:
 * return	: 1 for yes, 0 for no
**/
int ok_to_execute()
{
	int	rv	= 1;	/* defalut is positive */

	if (if_state == WANT_THEN) {
		syn_err("then expected");
		rv	= 0;
	} else if (if_state == THEN_BLOCK && if_result == SUCCESS) {
		rv	= 1;
	} else if (if_state == THEN_BLOCK && if_result == FAIL)
		rv	= 0;

	return rv;
}

/*
 * Funcion	: boolean to report if the command is a shell control command
 *
 * Author	:
 *
 * arguments	:
 * return	: 0 or 1
**/
int is_control_command(char *s)
{
	return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || 	\
			strcmp(s, "if") == 0);
}

/*
 * Funcion	: Process "if", "then", "fi" - change state or detect error
 *
 * Author	:
 *
 * arguments	:
 * return	: 0 if ok, -1 for syntax error
**/
int do_control_command(char **args)
{

	char	*cmd	= args[0];
	int	rv	= -1;

	if (strcmp(cmd, "if") == 0) {
		if (if_state != NEUTRAL)
			rv	= syn_err("if unexpected");
		else {
			last_stat	= process(args + 1);
			if_result	= (last_stat == 0 ? SUCCESS : FAIL);
			if_state	= WANT_THEN;
			rv		= 0;
		}
	} else if (strcmp(cmd, "then") == 0) {
		if ( if_state != WANT_THEN)
			rv	= syn_err("then unexpected");
		else {
			if_state	= THEN_BLOCK;
			rv		= 0;
		}
	} else if (strcmp(cmd, "fi") == 0) {
		if (if_state != THEN_BLOCK)
			rv	= syn_err("fi unexpected");
		else {
			if_state	= NEUTRAL;
			rv		= 0;
		}
	} else
		fatal("internal error processing:", cmd, 2);
	return rv;
}

/*
 * Funcion	: handles syntax errors in control structures
 *		  resets state to NEUTRAL
 * Author	:
 *
 * arguments	:
 * return	: -1 in interactive mode. Should call fatal in scripts
**/
int syn_err(char *msg)
{
	if_state	= NEUTRAL;
	fprintf(stderr, "syntax error:%s\n", msg);
	return -1;
}
