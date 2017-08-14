#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#pragma clang diagnostic push /* push state */
#pragma clang diagnostic ignored "-W#warnings" /* just to ignore the system header information */
#include <sys/syslimits.h> /* PATH_MAX */
#pragma clang diagnostic pop /* pop state */
#include "kqclient.h"

int
main(int argc, char **argv)
{
	struct kevent tevent; /* triggered event */
	struct kevent mevent; /* monitored event */
	int kq, fd, ret;
	char *event_name;

	if (argc != 2)
		err(EXIT_FAILURE, "Usage: %s path\n", argv[0]);

	char *file_name = argv[1];
	char actualpath [PATH_MAX+1]; /* check dirname(), who needs it */

	char *ptr;
	ptr = realpath(file_name, actualpath);

	fd = open(argv[1], O_RDONLY);
	if (fd	== -1)
		err(EXIT_FAILURE, "Failed to open '%s'", argv[1]);

	/* Create kqueue. */
	kq = kqueue();
	if (kq	== -1)
		err(EXIT_FAILURE, "kqueue() failed");

	/* Initialize kevent structure. */
	EV_SET(&mevent,	fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, VNODE_NOTES, 0, NULL);

	/* Attach mevent to the	kqueue.	*/
	ret = kevent(kq, &mevent, 1, NULL, 0, NULL);
	if (ret == -1)
		err(EXIT_FAILURE, "kevent register");
	if (mevent.flags & EV_ERROR)
		errx(EXIT_FAILURE,	"Event error: %s", strerror( (int) mevent.data ));

	for (;;) {
		/*	Sleep until something happens. */
		ret = kevent(kq, NULL, 0, &tevent,	1, NULL);
		if	(ret ==	-1) {
			err(EXIT_FAILURE, "kevent (cannot) wait");
		} else {
			if (ret > 0) {
				
				if (NOTE_DELETE & tevent.fflags)
					event_name="DELETE";
				else if (NOTE_WRITE & tevent.fflags)
					event_name="WRITE";
				else if (NOTE_RENAME & tevent.fflags)
					event_name="RENAME";
				else
					event_name="UNKNOWN_EVENT";
					printf("%s %s %s (%i)\n", actualpath, event_name, file_name, tevent.fflags);
			}
		}
	}
}
