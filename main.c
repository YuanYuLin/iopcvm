#include <lxc/lxccontainer.h>

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define MYNAME "vm001"

int main(int argc, char *argv[])
{
	struct lxc_container *c;
	int ret = 0;
	const char *s;
	bool b;
	char buf[201];
	int len;

	ret = 1;
	/* test a real container */
	c = lxc_container_new(MYNAME, NULL);
	if (!c) {
		fprintf(stderr, "%d: error creating lxc_container %s\n", __LINE__, MYNAME);
		ret = 1;
		goto out;
	}

	b = c->is_defined(c);
	if (!b) {
		fprintf(stderr, "%d: %s thought it was not defined\n", __LINE__, MYNAME);
		goto out;
	}

	len = c->get_cgroup_item(c, "cpuset.cpus", buf, 200);
	if (len >= 0) {
		fprintf(stderr, "%d: %s not running but had cgroup settings\n", __LINE__, MYNAME);
		goto out;
	}

	sprintf(buf, "0");
	b = c->set_cgroup_item(c, "cpuset.cpus", buf);
	if (b) {
		fprintf(stderr, "%d: %s not running but coudl set cgroup settings\n", __LINE__, MYNAME);
		goto out;
	}

	s = c->state(c);
	if (!s || strcmp(s, "STOPPED")) {
		fprintf(stderr, "%d: %s is in state %s, not in STOPPED.\n", __LINE__, c->name, s ? s : "undefined");
		goto out;
	}

	b = c->load_config(c, NULL);
	if (!b) {
		fprintf(stderr, "%d: %s failed to read its config\n", __LINE__, c->name);
		goto out;
	}

	if (!c->set_config_item(c, "lxc.utsname", MYNAME)) {
		fprintf(stderr, "%d: failed setting lxc.utsname\n", __LINE__);
		goto out;
	}

	if (!lxc_container_get(c)) {
		fprintf(stderr, "%d: failed to get extra ref to container\n", __LINE__);
		exit(1);
	}

	c->want_daemonize(c, false);
	if (!c->startl(c, 0, NULL)) {
		fprintf(stderr, "%d: %s failed to start\n", __LINE__, c->name);
		exit(1);
	}

	sleep(3);
	s = c->state(c);
	if (!s || strcmp(s, "RUNNING")) {
		fprintf(stderr, "%d: %s is in state %s, not in RUNNING.\n", __LINE__, c->name, s ? s : "undefined");
		goto out;
	}
#if 0
	len = c->get_cgroup_item(c, "cpuset.cpus", buf, 0);
	if (len <= 0) {
		fprintf(stderr, "%d: not able to get length of cpuset.cpus (ret %d)\n", __LINE__, len);
		goto out;
	}

	len = c->get_cgroup_item(c, "cpuset.cpus", buf, 200);
	if (len <= 0 || strncmp(buf, "0", 1)) {
		fprintf(stderr, "%d: not able to get cpuset.cpus (len %d buf %s)\n", __LINE__, len, buf);
		goto out;
	}

	sprintf(buf, "FROZEN");
	b = c->set_cgroup_item(c, "freezer.state", buf);
	if (!b) {
		fprintf(stderr, "%d: not able to set freezer.state.\n", __LINE__);
		goto out;
	}

    sprintf(buf, "XXX");
	len = c->get_cgroup_item(c, "freezer.state", buf, 200);
	if (len <= 0 || (strcmp(buf, "FREEZING\n") && strcmp(buf, "FROZEN\n"))) {
		fprintf(stderr, "%d: not able to get freezer.state (len %d buf %s)\n", __LINE__, len, buf);
		goto out;
	}

	c->set_cgroup_item(c, "freezer.state", "THAWED");

	c->stop(c);
#endif
	while(1) {
		sleep(10);
	}

    /* feh - multilib has moved the lxc-init crap */
	fprintf(stderr, "all lxc_container tests passed for %s\n", c->name);
	ret = 0;

out:
	if (c) {
		c->stop(c);
//		destroy_container();
	}
	lxc_container_put(c);
	exit(ret);
}
