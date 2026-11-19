#pragma once
#include "main.hpp"

constexpr int MAX_REZERWACJI = MAX_POJEMNOSC * 2;

void handler_kasa();
void obsluz_rezerwacje(const KasaKomunikat &req, KasaKomunikat &resp);
void obsluz_zakup(const KasaKomunikat& req, KasaKomunikat& resp);
