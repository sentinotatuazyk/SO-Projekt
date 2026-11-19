#include "kasa.hpp"
#include "main.hpp"

void handler_kasa(){}

void obsluz_rezerwacje(const KasaKomunikat &req, KasaKomunikat &resp) {
    resp.operacja = req.operacja;
    resp.pid_klienta = req.pid_klienta;
    resp.preferencja = req.preferencja;
    resp.idx_miejsca = (resp.operacja == OP_REZERWACJA) ? -1 : req.idx_miejsca;
    resp.oplacone = req.oplacone;
    resp.sukces = false;


   opusc_semafor(SEM_SALA);

   if (sala->kasa_otwarta &&
       !sala->ewakuacja) {
            switch(resp.operacja){
                case OP_REZERWACJA: {
                    for (int i = 0; i < MAX_POJEMNOSC; i++) {
                       Miejsce& m = sala->miejsca[i];
                       if (m.zarezerwowane || m.miejsce != resp.preferencja) continue;
                       m.pid_klienta = resp.pid_klienta;
                       m.zarezerwowane = true;
                       if (resp.oplacone) m.sprzedane = true;
                       resp.idx_miejsca = i;
                       //m.czas_rezerwacji DO DOKONCZENIA
                       resp.sukces = true;
                       break;
                   }
                   break;
                }
               case OP_OPLATA: {
                   if (resp.idx_miejsca < 0 || resp.idx_miejsca >= MAX_POJEMNOSC) break;
                   Miejsce& m = sala->miejsca[resp.idx_miejsca];
                   if (m.pid_klienta != resp.pid_klienta) break;
                   if (m.sprzedane) break;
                   m.sprzedane = true;
                   resp.sukces = true;
                   break;
               }
               case OP_ZWROT: {
                   if (resp.idx_miejsca < 0 || resp.idx_miejsca >= MAX_POJEMNOSC) break;
                   Miejsce& m = sala->miejsca[resp.idx_miejsca];
                   if (m.pid_klienta != resp.pid_klienta) break;
                   if (!m.sprzedane) break;
                   if (sala->dzien_koncertu) break;
                   m.zarezerwowane = false;
                   m.sprzedane = false;
                   resp.idx_miejsca = -1;
                   resp.sukces = true;
                   break;
                }
           }

       }
    podnies_semafor(SEM_SALA);
}
