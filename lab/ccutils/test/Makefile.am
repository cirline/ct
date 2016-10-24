AM_CPPFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = log_demo
log_demo_SOURCES = test_log.c
log_demo_LDADD = -lccutils -ldl -lpthread

check_PROGRAMS += dir_demo
dir_demo_SOURCES = test_dir.c
dir_demo_LDADD = -lccutils -ldl -lpthread

#check_PROGRAMS += db_demo
#db_demo_SOURCES = test_db.c
#db_demo_LDADD = -lccutils -ldl -lpthread

check_PROGRAMS += net_demo
net_demo_SOURCES = test_net.c
net_demo_LDADD = -lccutils -ldl -lpthread $(LIBCURL)
