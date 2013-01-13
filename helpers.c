#include "helpers.h"

/* convert a string to integer - return -1 if the string isn't a valid representation of
   an integer or if value is too big\small */
int strToInt(char *param)
{
	char *validate;
	int result;
	result = strtol(param, &validate, 10);

	if (result == LONG_MIN || result == LONG_MAX || *validate != '\0' || result < 0)
		return -1;

	return result;
}

/* check for an empty string (return 1 if true) */
int isEmpty(char *command)
{
	int i;

	if (command == '\0' || strcmp(command, "\n") == 0)
		return 1;

	for (i = 0; i < strlen(command) - 1; i++)
		if (!isspace(command[i]))
			return 0;

	return 1;
}

/* remove whitespace before string */
char *ltrim(char *str)
{
	if(str && *str) {
		char *cur = str;

		while(*cur && isspace(*cur))
			++cur;

		if(str != cur)
			memmove(str, cur, strlen(cur) + 1);
	}

	return str;
}

/* remove whitespace after string */
char *rtrim(char *str)
{
	if(str && *str) {
		char *cur = str + strlen(str) - 1;

		while(cur != str && isspace(*cur))
			--cur;

		cur[isspace(*cur) ? 0 : 1] = '\0';
	}

	return str;
}

/* remove whitespace before and after a string */
char *trim(char *str)
{
	rtrim(str);
    ltrim(str);

    return str;
}
