#include "main.hpp"
#include <csignal>
#include <cstdlib>
#include <sys/ipc.h>

int main(){
	zrusta();

	struct sigaction sa;
	sa.sa_handler = handler_dyrektora;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGUSR1, &sa, nullptr) == -1 ||
	    sigaction(SIGUSR2, &sa, nullptr) == -1 ||
		sigaction(SIGUSR3, &sa, nullptr) == -1) {
		    perror("Blad sigaction");
			std::exit(EXIT_FAILURE);
	}

	inicjalizuj_ipc();

	pid_t pid_kasa = fork();
	if (pid_kasa == -1) {
	    perror("Blad fork(pid_kasa)");
		sprzataj_zasoby();
		std::exit(EXIT_FAILURE);
	} else if (pid_kasa == 0) {
	    execl("./kasa", "kasa", nullptr);
		perror("Blad execl (kasa.cpp)");
		std::exit(EXIT_FAILURE);
	}

	while(!sala->ewakuacja) {
	    pause();
	}

	kill(pid_kasa, SIGTERM);

	while(wait(nullptr) > 0) {}

	sprzataj_zasoby();
	return 0;
}
