/*
 *
 * Authored by Liu Peng on: Mon Nov  3 14:29:55 CST 2014
 * command processing layer
 * @desc: The process(char **arglist)function is called by the main loop
 *	  It sits in front of the execute() function.This layer handles
 * 	  two main classes of processing:
 *	  a) built-in functions(e.g exit(), set, =, read, ...)
 *	  b) control structures(e.g if, while, for)
 *
 * @func:
**/

#include <stdio.h>
#include "smsh.h"

int	is_control_command(char *);
int	do_control_command(char **);
int	ok_to_execute();

/*
 * Funcion	: process user command
 *
 * details	: if a built-in the call appropriate function, if not
 *		  execute()
 * error	: arise from subroutines, handled there
 * return	: result of processing command
**/
int process(char **args)
{
	int	rv = 0;

	if (args[0] == NULL)
		rv	= 0;
	else if (is_control_command(args[0]))
		rv	= do_control_command(args);
	else if (ok_to_execute())
		rv	= execute(args);
	return rv;
}

