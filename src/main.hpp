#pragma once
#include "includes.hpp"

constexpr int PARTER_MIEJSCA = 120;
constexpr int BALKONY_MIEJSCA = 50;
constexpr int LOZE_MIEJSCA = 40;
constexpr int MAX_POJEMNOSC = (PARTER_MIEJSCA + BALKONY_MIEJSCA + LOZE_MIEJSCA);

#ifdef __APPLE__
    #define SIGUSR3 SIGURG
#elif defined(__linux__)
    #define SIGUSR3 SIGRTMIN
#else
    #error "Nieobslugiwany system operacyjny"
#endif

constexpr int SEM_SALA = 0;
constexpr int LICZBA_SEMAFOR = 1;
constexpr long TYP_ZAPYTANIE_DO_KASY = 1;


enum TypOperacji{OP_REZERWACJA, OP_OPLATA, OP_ZWROT};
enum TypMiejsca{Parter, Balkon, Loza, Foyer};

struct Miejsce {
    TypMiejsca miejsce;
    int numer;
    int nr_lozy;
    bool wolne, zarezerwowane, sprzedane;
    pid_t pid_klienta;
    time_t czas_rezerwacji;
};

struct Sala { // Struct ktory trzyma informacje o statusie sali i miejsc
	int wolne_miejsca;
	bool kasa_otwarta;
	bool szatnia_otwarta;
	bool ewakuacja;
	Miejsce miejsca[MAX_POJEMNOSC];
	bool dzien_koncertu;
};

struct KasaKomunikat {
    long mtype;
    pid_t pid_klienta;
    TypMiejsca preferencja;
    bool sukces,oplacone;
    int idx_miejsca;
    TypOperacji operacja;
};

inline int msg_kasa_id = -1;
inline int sem_id = -1;
inline int shm_id = -1;
inline Sala *sala = nullptr;

void inicjalizuj_ipc();
void definiuj_sale();
void handler_dyrektora (int sig);
void sprzataj_zasoby();

inline bool podlacz_zasoby(){
    key_t klucz_shm = ftok(".", 'K');
    key_t klucz_sem = ftok(".", 'S');
    key_t klucz_msg = ftok(".", 'M');

    if (klucz_shm == -1 || klucz_sem == -1 || klucz_msg == -1) {
        perror("Blad ftok w procesie potomnym");
        return false;
    }

    shm_id = shmget(klucz_shm, sizeof(Sala), 0600);
    sem_id = semget(klucz_sem, LICZBA_SEMAFOR, 0600);
    msg_kasa_id = msgget(klucz_msg, 0600);

    if (shm_id == -1 || sem_id == -1 || msg_kasa_id == -1) {
        perror("Blad podpiecia do zasobow IPC");
        return false;
    }

    sala = reinterpret_cast<Sala*>(shmat(shm_id, nullptr, 0));
    if (sala == reinterpret_cast<void*>(-1)) {
        perror("Blad shmat w procesie potomnym");
        sala = nullptr;
        return false;
    }
    return true;

}

inline void opusc_semafor(int nr_sem) {
    struct sembuf op{};
    op.sem_num = static_cast<unsigned short>(nr_sem);
    op.sem_op = -1;
    op.sem_flg = 0;
    while (semop(sem_id, &op, 1) == -1) {
        if (errno != EINTR) {
            perror("Blad semop (opusc)");
            break;
        }
    }
}

inline void podnies_semafor(int nr_sem) {
    struct sembuf op{};
    op.sem_num = static_cast<unsigned short>(nr_sem);
    op.sem_op = 1;
    op.sem_flg = 0;
    if (semop(sem_id, &op, 1) == -1) {
        perror("Blad semop (podnies)");
    }
}

extern "C"{
	void zrusta();
}
