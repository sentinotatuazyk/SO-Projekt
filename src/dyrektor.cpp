#include "main.hpp"
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#ifdef __linux__
    union semun {
        int val;
        struct semid_ds* buf;
        unsigned shor* array;
    }
#endif

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

void inicjalizuj_ipc() {
    key_t klucz_shm = ftok(".", 'K');
    key_t klucz_sem = ftok(".", 'S');
    key_t klucz_msg = ftok(".", 'M');

    if (klucz_shm == -1 || klucz_sem == -1 || klucz_msg == -1) {
        perror("Blad ftok\n");
        std::exit(EXIT_FAILURE);
    }
    shm_id = shmget(klucz_shm, sizeof(Sala), IPC_CREAT | 0600);
    if (shm_id == -1) {
        perror("Blad shmget");
        std::exit(EXIT_FAILURE);
    }

    sem_id = semget(klucz_sem, LICZBA_SEMAFOR, IPC_CREAT | 0600);
    if (sem_id == -1) {
        perror("Blad semget");
        std::exit(EXIT_FAILURE);
    }

    semun arg{};
    arg.val = 1;
    if (semctl(sem_id, SEM_SALA, SETVAL, arg) == -1) {
        perror("Blad semctl (SETVAL)\n");
        std::exit(EXIT_FAILURE);
    }

    msg_kasa_id = msgget(klucz_msg, IPC_CREAT | 0600);
    if (msg_kasa_id == -1) {
        perror("Blad msgget");
        std::exit(EXIT_FAILURE);
    }
    definiuj_sale();
}

void definiuj_sale() {
    if(shm_id == -1) {
        perror("Blad SHM_ID");
        std::exit(EXIT_FAILURE);
    }
    sala = reinterpret_cast<Sala*>(shmat(shm_id,nullptr,0));
    if (sala == reinterpret_cast<void*>(-1)) {
        perror("Blad shmat");
        sala = nullptr;
        sprzataj_zasoby();
        std::exit(EXIT_FAILURE);
    }
    sala->wolne_miejsca = MAX_POJEMNOSC;
    sala->kasa_otwarta = true;
    sala->szatnia_otwarta = true;
    sala->ewakuacja = false;
    for (int i = 0; i <MAX_POJEMNOSC; i++ ) {
        sala->miejsca[i].wolne = true;
        sala->miejsca[i].zarezerwowane = false;
        sala->miejsca[i].sprzedane = false;
        sala->miejsca[i].pid_klienta = 0;
        sala->miejsca[i].czas_rezerwacji = 0;
        if (i < PARTER_MIEJSCA) {
            sala->miejsca[i].numer = i + 1;
            sala->miejsca[i].nr_lozy = 0;
            sala->miejsca[i].miejsce = Parter;
        }
        else if (i < BALKONY_MIEJSCA + PARTER_MIEJSCA) {
            sala->miejsca[i].numer = i - PARTER_MIEJSCA + 1;
            sala->miejsca[i].nr_lozy = 0;
            sala->miejsca[i].miejsce = Balkon;
        }
        else {
            sala->miejsca[i].numer = i - PARTER_MIEJSCA - BALKONY_MIEJSCA + 1;
            sala->miejsca[i].nr_lozy = (i - PARTER_MIEJSCA - BALKONY_MIEJSCA)/4 + 1;
            sala->miejsca[i].miejsce = Loza;
        }
    }
    std::cout<< "[Dyrektor] Sala przygotowana! pid = " << getpid() << "\n";
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
			std::cout << "[SYSTEM] Pamiec dzielona usunieta.\n";
		}
	}

	if (sem_id != -1) {
	    if (semctl(sem_id, 0, IPC_RMID) == -1) {
			perror("Blad semctl(IPC_RMID)");
		} else {
			std::cout << "[SYSTEM] Semafory usuniete!\n";
		}
	}

	if(msg_kasa_id != -1) {
	    if (msgctl(msg_kasa_id, IPC_RMID, nullptr) == -1){
			perror("Blad msgctl(IPC_RMID)");
		} else {
		    std::cout << "[SYSTEM] Kolejka komunikatow usunieta!\n";
		}
	}
}
