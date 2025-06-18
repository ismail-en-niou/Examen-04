#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>

// Your sandbox function here
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

// Test functions
void test1_good_function() {
    // Just return normally
}

void test2_exit_1() {
    exit(1);
}

void test3_exit_42() {
    exit(42);
}

void test4_infinite_loop() {
    while(1);
}

void test5_segfault() {
    int *p = NULL;
    *p = 42;
}

void test6_abort() {
    abort();
}

void test7_divide_by_zero() {
    int x = 1 / 0;
    (void)x;
}

void test8_stack_overflow() {
    test8_stack_overflow();
}

void test9_raise_sigterm() {
    raise(SIGTERM);
}

void test10_raise_sigkill() {
    raise(SIGKILL);
}

void test11_raise_sigusr1() {
    raise(SIGUSR1);
}

void test12_malloc_bomb() {
    while(1) {
        malloc(1024*1024);
    }
}

void test13_fork_bomb() {
    while(1) {
        if(fork() == 0) break;
    }
}

void test14_sleep_then_good() {
    sleep(1);
}

void test15_sleep_timeout() {
    sleep(10);
}

void test16_alarm_self() {
    alarm(1);
    sleep(5);
}

void test17_signal_handler() {
    signal(SIGALRM, SIG_IGN);
    sleep(10);
}

void test18_double_fork() {
    if(fork() == 0) {
        if(fork() == 0) {
            exit(0);
        }
        exit(0);
    }
}

void test19_exec_ls() {
    execl("/bin/ls", "ls", NULL);
}

void test20_write_file() {
    FILE *f = fopen("/tmp/test", "w");
    if(f) {
        fprintf(f, "test");
        fclose(f);
    }
}

void test21_infinite_malloc() {
    char *p;
    while((p = malloc(1))) {
        *p = 'A';
    }
}

void test22_negative_exit() {
    exit(-1);
}

void test23_exit_255() {
    exit(255);
}

void test24_sigsegv_handler() {
    signal(SIGSEGV, SIG_IGN);
    int *p = NULL;
    *p = 42;
}

void test25_alarm_ignore() {
    signal(SIGALRM, SIG_IGN);
    while(1)
    {
        printf("hake --- ");
    }
}

void test26_busy_cpu() {
    volatile int i = 0;
    while(i < 1000000000) i++;
}

void test27_raise_sigfpe() {
    raise(SIGFPE);
}

void test28_raise_sigbus() {
    raise(SIGBUS);
}

void test29_raise_sigpipe() {
    raise(SIGPIPE);
}

void test30_multiple_exits() {
    exit(1);
    exit(0);  // Never reached
}

void test31_longjmp_abuse() {
    // This would need setjmp/longjmp but let's simulate
    exit(3);
}

void test32_recursive_function() {
    static int depth = 0;
    if(++depth < 1000000) {
        test32_recursive_function();
    }
}

void test33_file_descriptor_leak() {
    for(int i = 0; i < 1000; i++) {
        fopen("/dev/null", "r");
    }
}

void test34_chdir_root() {
    chdir("/");
}

void test35_getpid_loop() {
    for(int i = 0; i < 1000000; i++) {
        getpid();
    }
}

void test36_signal_loop() {
    for(int i = 0; i < 100; i++) {
        signal(SIGUSR1, SIG_DFL);
        signal(SIGUSR1, SIG_IGN);
    }
}

void test37_alarm_reset() {
    alarm(100);
    alarm(0);
}

void test38_double_alarm() {
    alarm(1);
    alarm(2);
    sleep(5);
}

void test39_syscall_spam() {
    for(int i = 0; i < 1000000; i++) {
        getuid();
    }
}

void test40_zero_divide_signal() {
    signal(SIGFPE, SIG_IGN);
    int x = 1/0;
    (void)x;
}

void test41_memory_read_violation() {
    char *p = (char*)0x12345678;
    char c = *p;
    (void)c;
}

void test42_memory_write_violation() {
    char *p = (char*)0x12345678;
    *p = 'X';
}

void test43_raise_sigstop() {
    raise(SIGSTOP);
}

void test44_raise_sigcont() {
    raise(SIGCONT);
}

void test45_multiple_signals() {
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGTERM);
}

void test46_exit_in_signal_handler() {
    signal(SIGALRM, (void*)exit);
    raise(SIGALRM);
}

void test47_very_long_computation() {
    volatile long long sum = 0;
    for(long long i = 0; i < 10000000000LL; i++) {
        sum += i;
    }
}

void test48_resource_limit() {
    struct rlimit rl;
    rl.rlim_cur = 1;
    rl.rlim_max = 1;
    setrlimit(RLIMIT_NOFILE, &rl);
}

void test49_nested_forks() {
    for(int i = 0; i < 10; i++) {
        if(fork() == 0) {
            exit(i);
        }
    }
}

void test50_good_with_output() {
    printf("Hello from test function!\n");
}

// Test runner
typedef struct {
    void (*func)(void);
    const char *name;
    int expected_result;
    unsigned int timeout;
} test_case_t;

int main(void) {
    test_case_t tests[] = {
        {test1_good_function, "Good function", 1, 2},
        {test2_exit_1, "Exit with code 1", 0, 2},
        {test3_exit_42, "Exit with code 42", 0, 2},
        {test4_infinite_loop, "Infinite loop (timeout)", 0, 2},
        {test5_segfault, "Segmentation fault", 0, 2},
        {test6_abort, "Abort signal", 0, 2},
        {test7_divide_by_zero, "Divide by zero", 0, 2},
        {test8_stack_overflow, "Stack overflow", 0, 2},
        {test9_raise_sigterm, "Raise SIGTERM", 0, 2},
        {test10_raise_sigkill, "Raise SIGKILL", 0, 2},
        {test11_raise_sigusr1, "Raise SIGUSR1", 0, 2},
        {test12_malloc_bomb, "Malloc bomb", 0, 1},
        {test13_fork_bomb, "Fork bomb", 0, 1},
        {test14_sleep_then_good, "Sleep 1s then good", 1, 3},
        {test15_sleep_timeout, "Sleep timeout", 0, 2},
        {test16_alarm_self, "Self alarm", 0, 3},
        {test17_signal_handler, "Ignore SIGALRM", 0, 2},
        {test18_double_fork, "Double fork", 1, 2},
        {test19_exec_ls, "Exec /bin/ls", 1, 2},
        {test20_write_file, "Write to file", 1, 2},
        {test21_infinite_malloc, "Infinite malloc", 0, 2},
        {test22_negative_exit, "Exit with -1", 0, 2},
        {test23_exit_255, "Exit with 255", 0, 2},
        // {test24_sigsegv_handler, "Ignore SIGSEGV", 0, 2},
        // {test25_alarm_ignore, "Ignore alarm + loop", 0, 2},
        {test26_busy_cpu, "CPU intensive", 1, 5},
        {test27_raise_sigfpe, "Raise SIGFPE", 0, 2},
        {test28_raise_sigbus, "Raise SIGBUS", 0, 2},
        {test29_raise_sigpipe, "Raise SIGPIPE", 0, 2},
        {test30_multiple_exits, "Multiple exits", 0, 2},
        {test31_longjmp_abuse, "Longjmp abuse", 0, 2},
        {test32_recursive_function, "Deep recursion", 0, 2},
        {test33_file_descriptor_leak, "FD leak", 1, 2},
        {test34_chdir_root, "Chdir to root", 1, 2},
        {test35_getpid_loop, "Getpid loop", 1, 3},
        {test36_signal_loop, "Signal setup loop", 1, 2},
        {test37_alarm_reset, "Alarm reset", 1, 2},
        {test38_double_alarm, "Double alarm", 0, 5},
        {test39_syscall_spam, "Syscall spam", 1, 3},
        {test40_zero_divide_signal, "Zero divide ignored", 1, 2},
        {test41_memory_read_violation, "Memory read violation", 0, 2},
        {test42_memory_write_violation, "Memory write violation", 0, 2},
        {test43_raise_sigstop, "Raise SIGSTOP", 0, 2},
        {test44_raise_sigcont, "Raise SIGCONT", 1, 2},
        {test45_multiple_signals, "Multiple signals", 0, 2},
        {test46_exit_in_signal_handler, "Exit in signal handler", 0, 2},
        {test47_very_long_computation, "Very long computation", 0, 3},
        {test48_resource_limit, "Resource limit", 1, 2},
        {test49_nested_forks, "Nested forks", 1, 2},
        {test50_good_with_output, "Good with output", 1, 2},
    };
    
    int total_tests = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    int failed = 0;
    
    printf("Running %d hard tests for sandbox function...\n\n", total_tests);
    
    for(int i = 0; i < total_tests; i++) {
        printf("Test %2d: %-30s ", i+1, tests[i].name);
        
        int result = sandbox(tests[i].func, tests[i].timeout, false);
        
        if(result == tests[i].expected_result) {
            printf("✓ PASS (got %d)\n", result);
            passed++;
        } else {
            printf("✗ FAIL (expected %d, got %d)\n", tests[i].expected_result, result);
            failed++;
        }
    }
    
    printf("\n=== RESULTS ===\n");
    printf("Passed: %d/%d\n", passed, total_tests);
    printf("Failed: %d/%d\n", failed, total_tests);
    printf("Success rate: %.1f%%\n", (float)passed/total_tests*100);
    
    return (failed == 0) ? 0 : 1;
}