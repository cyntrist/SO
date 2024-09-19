#include <stdio.h>
#include <string.h>

void main(void) {
	char msg[10];		/* array of 10 chars */
	char *p;		/* pointer to a char */
	char msg2[]="Hello";	/* msg2 = ’H’’e’’l’’l’’o’’\0’ */

	//msg = "Bonjour";/* ERROR. msg has a const address.*/
	p = "Bonjour";	/* address of "Bonjour" goes into p */

	//strcpy(msg, p);
	//strcpy(p, msg2);

	//p = msg;	/* OK */
	//p[0]='H', p[1]='i',p[2]='\0'; /* msg and *p are now "Hi" */

	printf("MSG:  %s\nMSG2: %s\np:    %s\n", msg, msg2, p);
}
