#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t done = 0;

void handler(int sig) {
    const char* name = strsignal(sig);
    if (sig >= SIGRTMIN && sig <= SIGRTMAX) {
        printf("обработан %d SIGRTMIN+%d\n", sig, sig - SIGRTMIN);
    } else {
        printf("обработан %d %s\n", sig, name);
    }
    done = 1;
}

int main() {
    struct sigaction act;
    sigset_t mask, pending;

    // настройка обработчика
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // регистрируем обработчики для всех сигналов
    for (int i = 1; i < NSIG; i++) {
        if (i != SIGKILL && i != SIGSTOP) {
            sigaction(i, &act, NULL);
        }
    }

    // блокируем все сигналы
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    // отправляем сигналы в случайном порядке
    kill(getpid(), SIGUSR1);      // 10
    kill(getpid(), SIGRTMIN + 5); // 39
    kill(getpid(), SIGINT);       // 2
    kill(getpid(), SIGRTMIN);     // 34 
    kill(getpid(), SIGTERM);      // 15
    kill(getpid(), SIGUSR2);      // 12
    kill(getpid(), SIGRTMIN + 3); // 37

    // разблокируем все сигналы одновременно
    sigemptyset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    // ждем обработки хотя бы одного сигнала
    while (!done) pause();

    return 0;
}

