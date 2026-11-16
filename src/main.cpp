#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <csignal>
#include <cerrno>

constexpr int PARTER_MIEJSCA = 120;
constexpr int BALKONY_MIEJSCA = 50;
constexpr int LOZE_MIEJSCA = 40;
constexpr int MAX_POJEMNOSC = (PARTER_MIEJSCA + BALKONY_MIEJSCA + LOZE_MIEJSCA);

struct Sala {
	int wolne_miejsca;
	int kasa_otwarta;
	int szatnia_otwarta;
	int ewakulacja;
	int miejsca[MAX_POJEMNOSC];
};

int shm_id = -1;
Sala *sala = nullptr;

void handler_dyrektora (int sig){
	if (sala == nullptr) return;
	if (sig == SIGUSR1) {
		std::cout << "[DYREKTOR] Sygnal 1: Kasa przestaje sprzedawac bilety.\n";
		sala->kasa_otwarta = 0;
	}else if (sig == SIGUSR2) {
		std::cout << "[DYREKTOR] Sygnal 2: Szatnia przestaje przyjmowac odziez.\n";
		sala->szatnia_otwarta = 0;
	}else if (sig == SIGUSR3) {
		std::cout << "[DYREKTOR] Sygnal 3: EWAKUACJA! Wszyscy opuszczaja budynek.\n";
		sala->ewakuacja = 1;
	}
}

void sprzataj_zasoby() {
	if (sala != nullptr) {
		if (shmdt(sala) == -1) {
			perror("Blad Shmdt");
		}
	}
	
	if (shm_id != -1){
		if (shmctl(shm_id, IPC_RMID, nullptr) == -1) {
			perror("Blad shmctl (IPC_RMID)");
		} else {
			std::cout << "[SYSTEM] Pamiec dzielona usunieta.\n"
		}
	}
}

extern "C"{
	void zrusta();
}

void test(){
	std::cout << "Test\n";
}

int main(){
	test();
	zrusta();
	return 0;
}
