#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

#include <lxc/lxccontainer.h>

int main(int argc, char *argv[])
{
	int err = 1;
	struct lxc_container *c;

	const char *vm_lxcpath = "/var/lib/lxc";
	char *vm_name = argv[1];
	int vm_quiet = 0;
	int vm_daemonize = 1;

	printf("name:%s\n", vm_name);
	printf("quiet:%d\n", vm_quiet);
	printf("lxcpath:%s\n", vm_lxcpath);
	printf("daemonize:%d\n", vm_daemonize);

		c = lxc_container_new(vm_name, vm_lxcpath);
		if (!c) {
			printf("Failed to create lxc_container");
			return err;
		}

	if (c->is_running(c)) {
		printf("Container is already running.");
		err = 0;
		goto out;
	}

	printf("*******Default Args***\n");
	err = c->start(c, 0, NULL) ? 0 : 1;

	if (err) {
		printf("The container failed to start.");
		if (vm_daemonize)
			printf("To get more details, run the container in foreground mode.");
		printf("Additional information can be obtained by setting the "
		      "--logfile and --logpriority options.");
		err = c->error_num;
		lxc_container_put(c);
		return err;
	}

out:
//	lxc_container_put(c);
	return err;
}
