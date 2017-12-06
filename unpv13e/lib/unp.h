/* include unph */
/* Our own header.  Tabs are set for 4 spaces, not 8 */

#ifndef	__unp_h
#define	__unp_h

#include	"config.h"	/* configuration options for current OS */
							/* "../config.h" is generated by configure */

/* If anything changes in the following list of #includes, must change
   acsite.m4 also, for configure's tests. */

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#if TIME_WITH_SYS_TIME
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#else
#if HAVE_SYS_TIME_H
#include	<sys/time.h>	/* includes <time.h> unsafely */
#else
#include	<time.h>		/* old system? */
#endif
#endif
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */

#ifdef	HAVE_SYS_SELECT_H
# include	<sys/select.h>	/* for convenience */
#endif

#ifdef	HAVE_SYS_SYSCTL_H
#ifdef	HAVE_SYS_PARAM_H
# include	<sys/param.h>	/* OpenBSD prereq for sysctl.h */
#endif
# include	<sys/sysctl.h>
#endif

#ifdef	HAVE_POLL_H
# include	<poll.h>		/* for convenience */
#endif

#ifdef	HAVE_SYS_EVENT_H
# include	<sys/event.h>	/* for kqueue */
#endif

#ifdef	HAVE_STRINGS_H
# include	<strings.h>		/* for convenience */
#endif

/* Three headers are normally needed for socket/file ioctl's:
 * <sys/ioctl.h>, <sys/filio.h>, and <sys/sockio.h>.
 */
#ifdef	HAVE_SYS_IOCTL_H
# include	<sys/ioctl.h>
#endif
#ifdef	HAVE_SYS_FILIO_H
# include	<sys/filio.h>
#endif
#ifdef	HAVE_SYS_SOCKIO_H
# include	<sys/sockio.h>
#endif

#ifdef	HAVE_PTHREAD_H
# include	<pthread.h>
#endif

#ifdef HAVE_NET_IF_DL_H
# include	<net/if_dl.h>
#endif

#ifdef HAVE_NETINET_SCTP_H
#include	<netinet/sctp.h>
#endif

/* OSF/1 actually disables recv() and send() in <sys/socket.h> */
#ifdef	__osf__
#undef	recv
#undef	send
#define	recv(a,b,c,d)	recvfrom(a,b,c,d,0,0)
#define	send(a,b,c,d)	sendto(a,b,c,d,0,0)
#endif

#ifndef	INADDR_NONE
/* $$.Ic INADDR_NONE$$ */
#define	INADDR_NONE	0xffffffff	/* should have been in <netinet/in.h> */
#endif

#ifndef	SHUT_RD				/* these three POSIX names are new */
#define	SHUT_RD		0	/* shutdown for reading */
#define	SHUT_WR		1	/* shutdown for writing */
#define	SHUT_RDWR	2	/* shutdown for reading and writing */
/* $$.Ic SHUT_RD$$ */
/* $$.Ic SHUT_WR$$ */
/* $$.Ic SHUT_RDWR$$ */
#endif

/* *INDENT-OFF* */
#ifndef INET_ADDRSTRLEN
/* $$.Ic INET_ADDRSTRLEN$$ */
#define	INET_ADDRSTRLEN		16	/* "ddd.ddd.ddd.ddd\0"
								    1234567890123456 */
#endif

/* Define following even if IPv6 not supported, so we can always allocate
   an adequately sized buffer without #ifdefs in the code. */
#ifndef INET6_ADDRSTRLEN
/* $$.Ic INET6_ADDRSTRLEN$$ */
#define	INET6_ADDRSTRLEN	46	/* max size of IPv6 address string:
				   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx" or
				   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
				    1234567890123456789012345678901234567890123456 */
#endif
/* *INDENT-ON* */

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef	HAVE_BZERO
#define	bzero(ptr,n)		memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Older resolvers do not have gethostbyname2() */
#ifndef	HAVE_GETHOSTBYNAME2
#define	gethostbyname2(host,family)		gethostbyname((host))
#endif

/* The structure returned by recvfrom_flags() */
struct unp_in_pktinfo {
  struct in_addr	ipi_addr;	/* dst IPv4 address */
  int				ipi_ifindex;/* received interface index */
};
/* $$.It unp_in_pktinfo$$ */
/* $$.Ib ipi_addr$$ */
/* $$.Ib ipi_ifindex$$ */

/* We need the newer CMSG_LEN() and CMSG_SPACE() macros, but few
   implementations support them today.  These two macros really need
    an ALIGN() macro, but each implementation does this differently. */
#ifndef	CMSG_LEN
/* $$.Im CMSG_LEN$$ */
#define	CMSG_LEN(size)		(sizeof(struct cmsghdr) + (size))
#endif
#ifndef	CMSG_SPACE
/* $$.Im CMSG_SPACE$$ */
#define	CMSG_SPACE(size)	(sizeof(struct cmsghdr) + (size))
#endif

/* POSIX requires the SUN_LEN() macro, but not all implementations DefinE
   it (yet).  Note that this 4.4BSD macro works regardless whether there is
   a length field or not. */
#ifndef	SUN_LEN
/* $$.Im SUN_LEN$$ */
# define	SUN_LEN(su) \
	(sizeof(*(su)) - sizeof((su)->sun_path) + strlen((su)->sun_path))
#endif

/* POSIX renames "Unix domain" as "local IPC."
   Not all systems DefinE AF_LOCAL and PF_LOCAL (yet). */
#ifndef	AF_LOCAL
#define AF_LOCAL	AF_UNIX
#endif
#ifndef	PF_LOCAL
#define PF_LOCAL	PF_UNIX
#endif

/* POSIX requires that an #include of <poll.h> DefinE INFTIM, but many
   systems still DefinE it in <sys/stropts.h>.  We don't want to include
   all the STREAMS stuff if it's not needed, so we just DefinE INFTIM here.
   This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
/* $$.Ic INFTIM$$ */
#ifdef	HAVE_POLL_H
#define	INFTIM_UNPH				/* tell unpxti.h we defined it */
#endif
#endif

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Define some port number that can be used for our examples */
#define	SERV_PORT		 9877			/* TCP and UDP */
#define	SERV_PORT_STR	"9877"			/* TCP and UDP */
#define	UNIXSTR_PATH	"/tmp/unix.str"	/* Unix domain stream */
#define	UNIXDG_PATH		"/tmp/unix.dg"	/* Unix domain datagram */
/* $$.ix [LISTENQ]~constant,~definition~of$$ */
/* $$.ix [MAXLINE]~constant,~definition~of$$ */
/* $$.ix [BUFFSIZE]~constant,~definition~of$$ */
/* $$.ix [SERV_PORT]~constant,~definition~of$$ */
/* $$.ix [UNIXSTR_PATH]~constant,~definition~of$$ */
/* $$.ix [UNIXDG_PATH]~constant,~definition~of$$ */

/* Following shortens all the typecasts of pointer arguments: */
#define	SA	struct sockaddr

#ifndef HAVE_STRUCT_SOCKADDR_STORAGE
/*
 * RFC 3493: protocol-independent placeholder for socket addresses
 */
#define	__SS_MAXSIZE	128
#define	__SS_ALIGNSIZE	(sizeof(int64_t))
#ifdef HAVE_SOCKADDR_SA_LEN
#define	__SS_PAD1SIZE	(__SS_ALIGNSIZE - sizeof(u_char) - sizeof(sa_family_t))
#else
#define	__SS_PAD1SIZE	(__SS_ALIGNSIZE - sizeof(sa_family_t))
#endif
#define	__SS_PAD2SIZE	(__SS_MAXSIZE - 2*__SS_ALIGNSIZE)

struct sockaddr_storage {
#ifdef HAVE_SOCKADDR_SA_LEN
	u_char		ss_len;
#endif
	sa_family_t	ss_family;
	char		__ss_pad1[__SS_PAD1SIZE];
	int64_t		__ss_align;
	char		__ss_pad2[__SS_PAD2SIZE];
};
#endif

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default file access permissions for new files */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
					/* default permissions for new directories */

typedef	void	Sigfunc(int);	/* for signal handlers */

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))

#ifndef	HAVE_ADDRINFO_STRUCT
# include	"../lib/addrinfo.h"
#endif

#ifndef	HAVE_IF_NAMEINDEX_STRUCT
struct if_nameindex {
  unsigned int   if_index;  /* 1, 2, ... */
  char          *if_name;   /* null-terminated name: "le0", ... */
};
/* $$.It if_nameindex$$ */
/* $$.Ib if_index$$ */
/* $$.Ib if_name$$ */
#endif

#ifndef	HAVE_TIMESPEC_STRUCT
struct timespec {
  time_t	tv_sec;		/* seconds */
  long		tv_nsec;	/* and nanoseconds */
};
/* $$.It timespec$$ */
/* $$.Ib tv_sec$$ */
/* $$.Ib tv_nsec$$ */
#endif
/* end unph */

			/* prototypes for our own library functions */
int		 connect_nonb(int, const SA *, socklen_t, int);
int		 connect_timeo(int, const SA *, socklen_t, int);
int	 daemon_init(const char *, int);
void	 daemon_inetd(const char *, int);
void	 dg_cli(FILE *, int, const SA *, socklen_t);
void	 dg_echo(int, SA *, socklen_t);
int		 family_to_level(int);
char	*gf_time(void);
void	 heartbeat_cli(int, int, int);
void	 heartbeat_serv(int, int, int);
struct addrinfo *host_serv(const char *, const char *, int, int);
int		 inet_srcrt_add(char *);
u_char  *inet_srcrt_init(int);
void	 inet_srcrt_print(u_char *, int);
void	 inet6_srcrt_print(void *);
char   **my_addrs(int *);
int		 readable_timeo(int, int);
ssize_t	 readline(int, void *, size_t);
ssize_t	 readn(int, void *, size_t);
ssize_t	 read_fd(int, void *, size_t, int *);
ssize_t	 recvfrom_flags(int, void *, size_t, int *, SA *, socklen_t *,
		 struct unp_in_pktinfo *);
Sigfunc *signal_intr(int, Sigfunc *);
int		 sock_bind_wild(int, int);
int		 sock_cmp_addr(const SA *, const SA *, socklen_t);
int		 sock_cmp_port(const SA *, const SA *, socklen_t);
int		 sock_get_port(const SA *, socklen_t);
void	 sock_set_addr(SA *, socklen_t, const void *);
void	 sock_set_port(SA *, socklen_t, int);
void	 sock_set_wild(SA *, socklen_t);
char	*sock_ntop(const SA *, socklen_t);
char	*sock_ntop_host(const SA *, socklen_t);
int		 sockfd_to_family(int);
void	 str_echo(int);
void	 str_cli(FILE *, int);
int		 tcp_connect(const char *, const char *);
int		 tcp_listen(const char *, const char *, socklen_t *);
void	 tv_sub(struct timeval *, struct timeval *);
int		 udp_client(const char *, const char *, SA **, socklen_t *);
int		 udp_connect(const char *, const char *);
int		 udp_server(const char *, const char *, socklen_t *);
int		 writable_timeo(int, int);
ssize_t	 writen(int, const void *, size_t);
ssize_t	 write_fd(int, void *, size_t, int);

#ifdef	MCAST
int		 mcast_leave(int, const SA *, socklen_t);
int		 mcast_join(int, const SA *, socklen_t, const char *, u_int);
int		 mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
								  const SA *grp, socklen_t grplen);
int		 mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
								 const SA *grp, socklen_t grplen,
								 const char *ifname, u_int ifindex);
int		 mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
							const SA *grp, socklen_t grplen);
int		 mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
							  const SA *grp, socklen_t grplen);
int		 mcast_get_if(int);
int		 mcast_get_loop(int);
int		 mcast_get_ttl(int);
int		 mcast_set_if(int, const char *, u_int);
int		 mcast_set_loop(int, int);
int		 mcast_set_ttl(int, int);

void	 Mcast_leave(int, const SA *, socklen_t);
void	 Mcast_join(int, const SA *, socklen_t, const char *, u_int);
void	 Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen,
								  const SA *grp, socklen_t grplen);
void	 Mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen,
								 const SA *grp, socklen_t grplen,
								 const char *ifname, u_int ifindex);
void	 Mcast_block_source(int sockfd, const SA *src, socklen_t srclen,
							const SA *grp, socklen_t grplen);
void	 Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen,
							  const SA *grp, socklen_t grplen);
int		 Mcast_get_if(int);
int		 Mcast_get_loop(int);
int		 Mcast_get_ttl(int);
void	 Mcast_set_if(int, const char *, u_int);
void	 Mcast_set_loop(int, int);
void	 Mcast_set_ttl(int, int);
#endif

uint16_t	in_cksum(uint16_t *, int);

#ifndef	HAVE_GETADDRINFO_PROTO
int		 getaddrinfo(const char *, const char *, const struct addrinfo *,
					 struct addrinfo **);
void	 freeaddrinfo(struct addrinfo *);
char	*gai_strerror(int);
#endif

#ifndef	HAVE_GETNAMEINFO_PROTO
int		 getnameinfo(const SA *, socklen_t, char *, size_t, char *, size_t, int);
#endif

#ifndef	HAVE_GETHOSTNAME_PROTO
int		 gethostname(char *, int);
#endif

#ifndef	HAVE_HSTRERROR_PROTO
const char	*hstrerror(int);
#endif

#ifndef	HAVE_IF_NAMETOINDEX_PROTO
unsigned int	 if_nametoindex(const char *);
char			*if_indextoname(unsigned int, char *);
void			 if_freenameindex(struct if_nameindex *);
struct if_nameindex *if_nameindex(void);
#endif

#ifndef	HAVE_INET_PTON_PROTO
int			 inet_pton(int, const char *, void *);
const char	*inet_ntop(int, const void *, char *, size_t);
#endif

#ifndef	HAVE_INET_ATON_PROTO
int		 inet_aton(const char *, struct in_addr *);
#endif

#ifndef	HAVE_PSELECT_PROTO
int		 pselect(int, fd_set *, fd_set *, fd_set *,
				 const struct timespec *, const sigset_t *);
#endif

#ifndef	HAVE_SOCKATMARK_PROTO
int		 sockatmark(int);
#endif

#ifndef	HAVE_SNPRINTF_PROTO
int		 snprintf(char *, size_t, const char *, ...);
#endif

			/* prototypes for our own library wrapper functions */
void	 Connect_timeo(int, const SA *, socklen_t, int);
int		 Family_to_level(int);
struct addrinfo *Host_serv(const char *, const char *, int, int);
const char		*Inet_ntop(int, const void *, char *, size_t);
void			 Inet_pton(int, const char *, void *);
char			*If_indextoname(unsigned int, char *);
unsigned int		 If_nametoindex(const char *);
struct if_nameindex	*If_nameindex(void);
char   **My_addrs(int *);
ssize_t	 Read_fd(int, void *, size_t, int *);
int		 Readable_timeo(int, int);
ssize_t	 Recvfrom_flags(int, void *, size_t, int *, SA *, socklen_t *,
		 struct unp_in_pktinfo *);
Sigfunc *Signal(int, Sigfunc *);
Sigfunc *Signal_intr(int, Sigfunc *);
int		 Sock_bind_wild(int, int);
char	*Sock_ntop(const SA *, socklen_t);
char	*Sock_ntop_host(const SA *, socklen_t);
int		 Sockfd_to_family(int);
int		 Tcp_connect(const char *, const char *);
int		 Tcp_listen(const char *, const char *, socklen_t *);
int		 Udp_client(const char *, const char *, SA **, socklen_t *);
int		 Udp_connect(const char *, const char *);
int		 Udp_server(const char *, const char *, socklen_t *);
ssize_t	 Write_fd(int, void *, size_t, int);
int		 Writable_timeo(int, int);

			/* prototypes for our Unix wrapper functions: see {Sec errors} */
void	*Calloc(size_t, size_t);
void	 Close(int);
void	 Dup2(int, int);
int		 Fcntl(int, int, int);
void	 Gettimeofday(struct timeval *, void *);
int		 Ioctl(int, int, void *);
pid_t	 Fork(void);
void	*Malloc(size_t);
int	 Mkstemp(char *);
void	*Mmap(void *, size_t, int, int, int, off_t);
int		 Open(const char *, int, mode_t);
void	 Pipe(int *fds);
ssize_t	 Read(int, void *, size_t);
void	 Sigaddset(sigset_t *, int);
void	 Sigdelset(sigset_t *, int);
void	 Sigemptyset(sigset_t *);
void	 Sigfillset(sigset_t *);
int		 Sigismember(const sigset_t *, int);
void	 Sigpending(sigset_t *);
void	 Sigprocmask(int, const sigset_t *, sigset_t *);
char	*Strdup(const char *);
long	 Sysconf(int);
void	 Sysctl(int *, u_int, void *, size_t *, void *, size_t);
void	 Unlink(const char *);
pid_t	 Wait(int *);
pid_t	 Waitpid(pid_t, int *, int);
void	 Write(int, void *, size_t);

			/* prototypes for our stdio wrapper functions: see {Sec errors} */
void	 Fclose(FILE *);
FILE	*Fdopen(int, const char *);
char	*Fgets(char *, int, FILE *);
FILE	*Fopen(const char *, const char *);
void	 Fputs(const char *, FILE *);

			/* prototypes for our socket wrapper functions: see {Sec errors} */
int		 Accept(int, SA *, socklen_t *);
void	 Bind(int, const SA *, socklen_t);
void	 Connect(int, const SA *, socklen_t);
void	 Getpeername(int, SA *, socklen_t *);
void	 Getsockname(int, SA *, socklen_t *);
void	 Getsockopt(int, int, int, void *, socklen_t *);
#ifdef	HAVE_INET6_RTH_INIT
int		 Inet6_rth_space(int, int);
void	*Inet6_rth_init(void *, socklen_t, int, int);
void	 Inet6_rth_add(void *, const struct in6_addr *);
void	 Inet6_rth_reverse(const void *, void *);
int		 Inet6_rth_segments(const void *);
struct in6_addr *Inet6_rth_getaddr(const void *, int);
#endif
#ifdef	HAVE_KQUEUE
int		 Kqueue(void);
int		 Kevent(int, const struct kevent *, int,
				struct kevent *, int, const struct timespec *);
#endif
void	 Listen(int, int);
#ifdef	HAVE_POLL
int		 Poll(struct pollfd *, unsigned long, int);
#endif
ssize_t	 Readline(int, void *, size_t);
ssize_t	 Readn(int, void *, size_t);
ssize_t	 Recv(int, void *, size_t, int);
ssize_t	 Recvfrom(int, void *, size_t, int, SA *, socklen_t *);
ssize_t	 Recvmsg(int, struct msghdr *, int);
int		 Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void	 Send(int, const void *, size_t, int);
void	 Sendto(int, const void *, size_t, int, const SA *, socklen_t);
void	 Sendmsg(int, const struct msghdr *, int);
void	 Setsockopt(int, int, int, const void *, socklen_t);
void	 Shutdown(int, int);
int		 Sockatmark(int);
int		 Socket(int, int, int);
void	 Socketpair(int, int, int, int *);
void	 Writen(int, void *, size_t);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);

//#include "error_unp.c"

#endif	/* __unp_h */
