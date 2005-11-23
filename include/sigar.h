#ifndef SIGAR_H
#define SIGAR_H

/* System Information Gatherer And Reporter */

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)

typedef unsigned __int64 sigar_uint64_t;

#elif ULONG_MAX > 4294967295UL

typedef unsigned long sigar_uint64_t;

#else

typedef unsigned long long sigar_uint64_t;

#endif

#define SIGAR_FIELD_NOTIMPL -1

#define SIGAR_OK 0
#define SIGAR_START_ERROR 20000
#define SIGAR_ENOTIMPL       (SIGAR_START_ERROR + 1)
#define SIGAR_OS_START_ERROR (SIGAR_START_ERROR*2)

#ifdef WIN32
#   define SIGAR_DECLARE(type) \
        __declspec(dllexport) type __stdcall
#else
#   define SIGAR_DECLARE(type) type
#endif

#if defined(PATH_MAX)
#   define SIGAR_PATH_MAX PATH_MAX
#elif defined(MAXPATHLEN)
#   define SIGAR_PATH_MAX MAXPATHLEN
#else
#   define SIGAR_PATH_MAX 8192
#endif

#ifdef WIN32
typedef sigar_uint64_t sigar_pid_t;
typedef unsigned long sigar_uid_t;
typedef unsigned long sigar_gid_t;
#else
#include <sys/types.h>
typedef pid_t sigar_pid_t;
typedef uid_t sigar_uid_t;
typedef gid_t sigar_gid_t;
#endif

typedef struct sigar_t sigar_t;

SIGAR_DECLARE(int) sigar_open(sigar_t **sigar);

SIGAR_DECLARE(int) sigar_close(sigar_t *sigar);

SIGAR_DECLARE(sigar_pid_t) sigar_pid_get(sigar_t *sigar);

SIGAR_DECLARE(int) sigar_proc_kill(sigar_pid_t pid, int signum);

SIGAR_DECLARE(char *) sigar_strerror(sigar_t *sigar, int err);

/* system memory info */

typedef struct {
    sigar_uint64_t
        ram,
        total,
        used, 
        free,
        shared,
        actual_used,
        actual_free;
} sigar_mem_t;

SIGAR_DECLARE(int) sigar_mem_get(sigar_t *sigar, sigar_mem_t *mem);

typedef struct {
    sigar_uint64_t
        total,
        used, 
        free;
} sigar_swap_t;

SIGAR_DECLARE(int) sigar_swap_get(sigar_t *sigar, sigar_swap_t *swap);

typedef struct {
    sigar_uint64_t
        user, 
        sys,
        nice,
        idle,
        wait,
        total;
} sigar_cpu_t;

SIGAR_DECLARE(int) sigar_cpu_get(sigar_t *sigar, sigar_cpu_t *cpu);

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_cpu_t *data;
} sigar_cpu_list_t;

SIGAR_DECLARE(int) sigar_cpu_list_get(sigar_t *sigar, sigar_cpu_list_t *cpulist);

SIGAR_DECLARE(int) sigar_cpu_list_destroy(sigar_t *sigar,
                                          sigar_cpu_list_t *cpulist);

typedef struct {
    char vendor[128];
    char model[128];
    int mhz;
    sigar_uint64_t cache_size;
} sigar_cpu_info_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_cpu_info_t *data;
} sigar_cpu_info_list_t;

SIGAR_DECLARE(int)
sigar_cpu_info_list_get(sigar_t *sigar,
                        sigar_cpu_info_list_t *cpu_infos);

SIGAR_DECLARE(int)
sigar_cpu_info_list_destroy(sigar_t *sigar,
                            sigar_cpu_info_list_t *cpu_infos);

typedef struct {
    double uptime;
} sigar_uptime_t;

SIGAR_DECLARE(int) sigar_uptime_get(sigar_t *sigar,
                                    sigar_uptime_t *uptime);

SIGAR_DECLARE(int) sigar_uptime_string(sigar_t *sigar, 
                                       sigar_uptime_t *uptime,
                                       char *buffer,
                                       int buflen);

SIGAR_DECLARE(char *) sigar_format_size(sigar_uint64_t size, char *buf);

typedef struct {
    double loadavg[3];
} sigar_loadavg_t;

SIGAR_DECLARE(int) sigar_loadavg_get(sigar_t *sigar,
                                     sigar_loadavg_t *loadavg);

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_pid_t *data;
} sigar_proc_list_t;

typedef struct {
    /* RLIMIT_CPU */
    sigar_uint64_t cpu_cur, cpu_max;
    /* RLIMIT_FSIZE */
    sigar_uint64_t file_size_cur, file_size_max;
    /* PIPE_BUF */
    sigar_uint64_t pipe_size_cur, pipe_size_max;
    /* RLIMIT_DATA */
    sigar_uint64_t data_cur, data_max;
    /* RLIMIT_STACK */
    sigar_uint64_t stack_cur, stack_max;
    /* RLIMIT_CORE */
    sigar_uint64_t core_cur, core_max;
    /* RLIMIT_RSS */
    sigar_uint64_t memory_cur, memory_max;
    /* RLIMIT_NPROC */
    sigar_uint64_t processes_cur, processes_max;
    /* RLIMIT_NOFILE */
    sigar_uint64_t open_files_cur, open_files_max;
    /* RLIMIT_AS */
    sigar_uint64_t virtual_memory_cur, virtual_memory_max;
} sigar_resource_limit_t;

SIGAR_DECLARE(int) sigar_resource_limit_get(sigar_t *sigar,
                                            sigar_resource_limit_t *rlimit);

SIGAR_DECLARE(int) sigar_proc_list_get(sigar_t *sigar,
                                       sigar_proc_list_t *proclist);

SIGAR_DECLARE(int) sigar_proc_list_destroy(sigar_t *sigar,
                                           sigar_proc_list_t *proclist);

typedef struct {
    sigar_uint64_t total;
} sigar_proc_stat_t;

SIGAR_DECLARE(int) sigar_proc_stat_get(sigar_t *sigar,
                                       sigar_proc_stat_t *procstat);

typedef struct {
    sigar_uint64_t
        size,
        vsize,
        resident,
        share,
        rss;
} sigar_proc_mem_t;

SIGAR_DECLARE(int) sigar_proc_mem_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_mem_t *procmem);

typedef struct {
    sigar_uid_t uid;
    sigar_gid_t gid;
    sigar_uid_t euid;
    sigar_gid_t egid;
} sigar_proc_cred_t;

SIGAR_DECLARE(int) sigar_proc_cred_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_cred_t *proccred);

#define SIGAR_CRED_NAME_MAX 512

typedef struct {
    char user[SIGAR_CRED_NAME_MAX];
    char group[SIGAR_CRED_NAME_MAX];
} sigar_proc_cred_name_t;

SIGAR_DECLARE(int)
sigar_proc_cred_name_get(sigar_t *sigar, sigar_pid_t pid,
                         sigar_proc_cred_name_t *proccredname);

typedef struct {
    sigar_uint64_t
        start_time,
        user,
        sys,
        total;
} sigar_proc_time_t;

SIGAR_DECLARE(int) sigar_proc_time_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_time_t *proctime);

#define SIGAR_PROC_NAME_LEN 128

typedef struct {
    char name[SIGAR_PROC_NAME_LEN];
    char state;
    sigar_pid_t ppid;
    int tty;
    int priority;
    int nice;
    sigar_uint64_t threads;
} sigar_proc_state_t;

SIGAR_DECLARE(int) sigar_proc_state_get(sigar_t *sigar, sigar_pid_t pid,
                                        sigar_proc_state_t *procstate);

typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sigar_proc_args_t;

SIGAR_DECLARE(int) sigar_proc_args_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_args_t *procargs);

SIGAR_DECLARE(int) sigar_proc_args_destroy(sigar_t *sigar,
                                           sigar_proc_args_t *procargs);

typedef struct {
    void *data; /* user data */

    enum {
        SIGAR_PROC_ENV_ALL,
        SIGAR_PROC_ENV_KEY
    } type;

    /* used for SIGAR_PROC_ENV_KEY */
    const char *key;
    int klen;
    
    int (*env_getter)(void *, const char *, int, char *, int);
} sigar_proc_env_t;

SIGAR_DECLARE(int) sigar_proc_env_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_env_t *procenv);

typedef struct {
    sigar_uint64_t total;
    /* XXX - which are files, sockets, etc. */
} sigar_proc_fd_t;

SIGAR_DECLARE(int) sigar_proc_fd_get(sigar_t *sigar, sigar_pid_t pid,
                                     sigar_proc_fd_t *procfd);

typedef struct {
    char name[SIGAR_PATH_MAX+1];
    char cwd[SIGAR_PATH_MAX+1];
    char root[SIGAR_PATH_MAX+1];
} sigar_proc_exe_t;

SIGAR_DECLARE(int) sigar_proc_exe_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_exe_t *procexe);

typedef struct {
    void *data; /* user data */

    int (*module_getter)(void *, char *, int);
} sigar_proc_modules_t;

SIGAR_DECLARE(int) sigar_proc_modules_get(sigar_t *sigar, sigar_pid_t pid,
                                          sigar_proc_modules_t *procmods);

typedef struct {
    sigar_uint64_t user;
    sigar_uint64_t sys;
    sigar_uint64_t total;
} sigar_thread_cpu_t;

SIGAR_DECLARE(int) sigar_thread_cpu_get(sigar_t *sigar,
                                        sigar_uint64_t id,
                                        sigar_thread_cpu_t *cpu);
                                            
typedef enum {
    SIGAR_FSTYPE_UNKNOWN,
    SIGAR_FSTYPE_NONE,
    SIGAR_FSTYPE_LOCAL_DISK,
    SIGAR_FSTYPE_NETWORK,
    SIGAR_FSTYPE_RAM_DISK,
    SIGAR_FSTYPE_CDROM,
    SIGAR_FSTYPE_SWAP,
    SIGAR_FSTYPE_MAX
} sigar_file_system_type_e;

#define SIGAR_FS_NAME_LEN 64

typedef struct {
    char dir_name[SIGAR_FS_NAME_LEN];
    char dev_name[SIGAR_FS_NAME_LEN];
    char type_name[SIGAR_FS_NAME_LEN];     /* e.g. "local" */
    char sys_type_name[SIGAR_FS_NAME_LEN]; /* e.g. "ext3" */
    sigar_file_system_type_e type;
    unsigned long flags;
} sigar_file_system_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_file_system_t *data;
} sigar_file_system_list_t;

SIGAR_DECLARE(int)
sigar_file_system_list_get(sigar_t *sigar,
                           sigar_file_system_list_t *fslist);

SIGAR_DECLARE(int)
sigar_file_system_list_destroy(sigar_t *sigar,
                               sigar_file_system_list_t *fslist);

typedef struct {
    sigar_uint64_t
        total,
        free,
        used,
        avail,
        files,
        free_files,
        disk_reads,
        disk_writes,
        disk_write_bytes,
        disk_read_bytes,
        disk_queue;
    double use_percent;
} sigar_file_system_usage_t;

SIGAR_DECLARE(int)
sigar_file_system_usage_get(sigar_t *sigar,
                            const char *dirname,
                            sigar_file_system_usage_t *fsusage);

#ifndef INET6_ADDRSTRLEN
#   define INET6_ADDRSTRLEN 46
#endif

#define SIGAR_MAXDOMAINNAMELEN 256
#define SIGAR_MAXHOSTNAMELEN 256

typedef struct {
    char default_gateway[INET6_ADDRSTRLEN];
    char host_name[SIGAR_MAXHOSTNAMELEN];
    char domain_name[SIGAR_MAXDOMAINNAMELEN];
    char primary_dns[INET6_ADDRSTRLEN];
    char secondary_dns[INET6_ADDRSTRLEN];
} sigar_net_info_t;

SIGAR_DECLARE(int)
sigar_net_info_get(sigar_t *sigar,
                   sigar_net_info_t *netinfo);

#define SIGAR_RTF_UP      0x1
#define SIGAR_RTF_GATEWAY 0x2
#define SIGAR_RTF_HOST    0x4

typedef struct {
    sigar_uint64_t
        destination,
        gateway,
        flags,
        refcnt,
        use,
        metric,
        mask,
        mtu,
        window,
        irtt;
    char ifname[16];
} sigar_net_route_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_net_route_t *data;
} sigar_net_route_list_t;

SIGAR_DECLARE(int) sigar_net_route_list_get(sigar_t *sigar,
                                            sigar_net_route_list_t *routelist);

SIGAR_DECLARE(int) sigar_net_route_list_destroy(sigar_t *sigar,
                                                sigar_net_route_list_t *routelist);

/*
 * platforms define most of these "standard" flags,
 * but of course, with different values in some cases.
 */
#define SIGAR_IFF_UP          0x1
#define SIGAR_IFF_BROADCAST   0x2
#define SIGAR_IFF_DEBUG       0x4
#define SIGAR_IFF_LOOPBACK    0x8
#define SIGAR_IFF_POINTOPOINT 0x10
#define SIGAR_IFF_NOTRAILERS  0x20
#define SIGAR_IFF_RUNNING     0x40
#define SIGAR_IFF_NOARP       0x80
#define SIGAR_IFF_PROMISC     0x100
#define SIGAR_IFF_ALLMULTI    0x200
#define SIGAR_IFF_MULTICAST   0x800
#define SIGAR_IFF_SLAVE       0x1000

#define SIGAR_NULL_HWADDR "00:00:00:00:00:00"

typedef struct {
    char name[16];
    char hwaddr[64];
    char type[64];
    sigar_uint64_t
        address,
        destination,
        broadcast,
        netmask,
        flags,
        mtu,
        metric;
} sigar_net_interface_config_t;

SIGAR_DECLARE(int)
sigar_net_interface_config_get(sigar_t *sigar,
                               const char *name,
                               sigar_net_interface_config_t *ifconfig);

typedef struct {
    sigar_uint64_t
        /* received */
        rx_packets,
        rx_bytes,
        rx_errors,
        rx_dropped,
        rx_overruns,
        rx_frame,
        /* transmitted */
        tx_packets,
        tx_bytes,
        tx_errors,
        tx_dropped,
        tx_overruns,
        tx_collisions,
        tx_carrier;
} sigar_net_interface_stat_t;

SIGAR_DECLARE(int)
sigar_net_interface_stat_get(sigar_t *sigar,
                             const char *name,
                             sigar_net_interface_stat_t *ifstat);

typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sigar_net_interface_list_t;

SIGAR_DECLARE(int)
sigar_net_interface_list_get(sigar_t *sigar,
                             sigar_net_interface_list_t *iflist);

SIGAR_DECLARE(int)
sigar_net_interface_list_destroy(sigar_t *sigar,
                                 sigar_net_interface_list_t *iflist);

#define SIGAR_NETCONN_CLIENT 0x01
#define SIGAR_NETCONN_SERVER 0x02

#define SIGAR_NETCONN_TCP  0x10
#define SIGAR_NETCONN_UDP  0x20
#define SIGAR_NETCONN_RAW  0x40
#define SIGAR_NETCONN_UNIX 0x80

enum {
    SIGAR_TCP_ESTABLISHED = 1,
    SIGAR_TCP_SYN_SENT,
    SIGAR_TCP_SYN_RECV,
    SIGAR_TCP_FIN_WAIT1,
    SIGAR_TCP_FIN_WAIT2,
    SIGAR_TCP_TIME_WAIT,
    SIGAR_TCP_CLOSE,
    SIGAR_TCP_CLOSE_WAIT,
    SIGAR_TCP_LAST_ACK,
    SIGAR_TCP_LISTEN,
    SIGAR_TCP_CLOSING,
    SIGAR_TCP_IDLE,
    SIGAR_TCP_BOUND,
    SIGAR_TCP_UNKNOWN
};

typedef struct {
    unsigned long local_port;
    char local_address[INET6_ADDRSTRLEN];
    unsigned long remote_port;
    char remote_address[INET6_ADDRSTRLEN];
    sigar_uid_t uid;
    unsigned long inode;
    int type;
    int state;
    unsigned long send_queue;
    unsigned long receive_queue;
} sigar_net_connection_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_net_connection_t *data;
} sigar_net_connection_list_t;

SIGAR_DECLARE(int)
sigar_net_connection_list_get(sigar_t *sigar,
                              sigar_net_connection_list_t *connlist,
                              int flags);

SIGAR_DECLARE(int)
sigar_net_connection_list_destroy(sigar_t *sigar,
                                  sigar_net_connection_list_t *connlist);

SIGAR_DECLARE(const char *)sigar_net_connection_type_get(int type);

SIGAR_DECLARE(const char *)sigar_net_connection_state_get(int state);

typedef struct {
    char user[32];
    char device[32];
    char host[256];
    sigar_uint64_t time;
} sigar_who_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_who_t *data;
} sigar_who_list_t;

SIGAR_DECLARE(int) sigar_who_list_get(sigar_t *sigar,
                                      sigar_who_list_t *wholist);

SIGAR_DECLARE(int) sigar_who_list_destroy(sigar_t *sigar,
                                          sigar_who_list_t *wholist);

SIGAR_DECLARE(int) sigar_proc_port_get(sigar_t *sigar, 
                                       int protocol, unsigned long port,
                                       sigar_pid_t *pid);

#define SIGAR_INET_ADDR_LEN (3 * 4 + 3 + 1)

SIGAR_DECLARE(int) sigar_inet_ntoa(sigar_t *sigar,
                                   sigar_uint64_t address,
                                   char *addr_str);

#define SIGAR_FQDN_LEN 512

SIGAR_DECLARE(int) sigar_fqdn_get(sigar_t *sigar, char *name, int namelen);

SIGAR_DECLARE(int) sigar_nfs_ping(char *hostname);

SIGAR_DECLARE(char *) sigar_password_get(const char *prompt);

#ifdef __cplusplus
}
#endif

#endif
