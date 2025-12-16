#include "music.h"

// === GLOBAL ===
Node* head = NULL;
Node* tail = NULL;
Node* currentSong = NULL;
RiwayatNode* riwayatTop = NULL;
Playlist* playlistHead = NULL;
Playlist* activePlaylist = NULL;
PlaylistLaguNode* currentPlaylistSong = NULL;
FavoriteNode* favoriteHead = NULL;
FavoriteNode* currentFavorite = NULL;
bool sudahNext = false;
BSTNode* bstRoot = NULL;

//=== UTIL ===
Node* buatNode(Lagu L) {
    Node* n = new Node;
    n->data = L;
    n->next = n->prev = NULL;
    return n;
}
// === ID AI ===
int getNextID() {
    int maxID = 0;
    Node* curr = head;
    while (curr) {
        if (curr->data.id > maxID)
            maxID = curr->data.id;
        curr = curr->next;
    }
    return maxID + 1;
}

// === DATA LAGU ===
void initDefaultSongs() {
    Lagu data[3] = {
        {1, "Mata Turun Ke Hati", "Hivi", "Oh kasihku..."},
        {2, "XXL", "Lany", "I miss you XXL.."},
        {3, "everything u are", "Hindia", "cerita kita tak jauh berbeda.."}
    };

    for (int i = 0; i < 3; i++) {
        Node* baru = buatNode(data[i]);
        if (!head) head = tail = baru;
        else {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }
        bstRoot = insertBST(bstRoot, data[i]);
    }
}

//=== FITUR ADMIN ===
void tambahLagu() {
    Lagu L;
    L.id = getNextID();
    cout << "\nID Lagu: " << L.id << "\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Judul     : "; getline(cin, L.judul);
    cout << "Penyanyi  : "; getline(cin, L.penyanyi);
    cout << "Lirik     : "; getline(cin, L.lirik);

    Node* baru = buatNode(L);
    baru->next = head;
    if (head) head->prev = baru;
    head = baru;
    if (!tail) tail = baru;

    bstRoot = insertBST(bstRoot, L);
    cout << "Lagu ditambahkan!\n";
}

void tampilLagu() {
    Node* curr = head;
    while (curr) {
        cout << "\nID: " << curr->data.id
             << "\nJudul: " << curr->data.judul
             << "\nPenyanyi: " << curr->data.penyanyi
             << "\nLirik: " << curr->data.lirik
             << "\n------------------";
        curr = curr->next;
    }
}

void updateLagu() {
    int id;
    cout << "\n=== UPDATE LAGU ===\n";
    cout << "Masukkan ID lagu: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Node* curr = head;
    while (curr && curr->data.id != id)
        curr = curr->next;

    if (!curr) {
        cout << "Lagu tidak ditemukan.\n";
        return;
    }

    int pilih;
    cout << "\nPilih data yang ingin diupdate:\n";
    cout << "1. Judul Lagu\n";
    cout << "2. Penyanyi\n";
    cout << "3. Lirik\n";
    cout << "4. Update Semua\n";
    cout << "Pilih: ";
    cin >> pilih;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (pilih) {
        case 1:
            cout << "Judul Baru: ";
            getline(cin, curr->data.judul);
            break;

        case 2:
            cout << "Penyanyi Baru: ";
            getline(cin, curr->data.penyanyi);
            break;

        case 3:
            cout << "Lirik Baru: ";
            getline(cin, curr->data.lirik);
            break;

        case 4:
            cout << "Judul Baru    : ";
            getline(cin, curr->data.judul);
            cout << "Penyanyi Baru : ";
            getline(cin, curr->data.penyanyi);
            cout << "Lirik Baru    : ";
            getline(cin, curr->data.lirik);
            break;

        default:
            cout << "Pilihan tidak valid.\n";
            return;
    }
    syncUpdateLagu(curr->data);
    rebuildBST();
    cout << "Data lagu berhasil diupdate.\n";
}

void hapusLagu() {
    int id;
    cout << "Masukkan ID: ";
    cin >> id;

    Node* curr = head;
    while (curr && curr->data.id != id)
        curr = curr->next;

    if (!curr) {
        cout << "Lagu tidak ditemukan.\n";
        return;
    }

    if (curr == head)
        head = curr->next;
    if (curr == tail)
        tail = curr->prev;
    if (curr->prev)
        curr->prev->next = curr->next;
    if (curr->next)
        curr->next->prev = curr->prev;

    delete curr;
    syncHapusLagu(id);
    rebuildBST();
    cout << "Lagu berhasil dihapus.\n";
}

//=== FITUR USER ===
Node* cariLaguMiripBerikutnya(Node* current) {
    if (!current) return NULL;

    string artist = toLower(current->data.penyanyi);
    string title  = toLower(current->data.judul);

    Node* curr = head;
    while (curr) {
        if (curr != current &&
            toLower(curr->data.penyanyi) == artist) {
            return curr;
        }
        curr = curr->next;
    }

    curr = head;
    while (curr) {
        if (curr != current &&
            toLower(curr->data.judul)
                .find(title.substr(0, 3)) != string::npos) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

Node* cariLaguMiripSebelumnya(Node* current) {
    if (!current) return NULL;

    string artist = toLower(current->data.penyanyi);
    string title  = toLower(current->data.judul);

    Node* kandidat = NULL;
    Node* curr = head;

    while (curr) {
        if (curr != current &&
            toLower(curr->data.penyanyi) == artist) {
            kandidat = curr;
        }
        curr = curr->next;
    }

    if (kandidat) return kandidat;

    curr = head;
    while (curr) {
        if (curr != current &&
            toLower(curr->data.judul)
                .find(title.substr(0, 3)) != string::npos) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

void cariLaguDanPilih() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string key;
    cout << "\nCari lagu: ";
    getline(cin, key);

    vector<Lagu> hasil;
    searchBSTAll(bstRoot, key, hasil);

    if (hasil.empty()) {
        cout << "Lagu tidak ditemukan.\n";
        return;
    }

    menuHasilSearch(hasil);
}

void playSong(Node* lagu) {
    if (!lagu) return;

    currentSong = lagu;
    tambahRiwayat(lagu->data);

    cout << "\n=== SEDANG DIPUTAR ===\n";
    cout << "Judul    : " << lagu->data.judul << endl;
    cout << "Penyanyi : " << lagu->data.penyanyi << endl;
    cout << "Lirik:\n" << lagu->data.lirik << endl;
}

void nextLagu() {
    if (activePlaylist) {
        nextPlaylistLagu();
        return;
    }
    if (!currentSong) {
        cout << "Belum ada lagu yang diputar.\n";
        return;
    }

    Node* mirip = cariLaguMiripBerikutnya(currentSong);

    if (mirip) {
        currentSong = mirip;
        sudahNext = true;
        playSong(currentSong);
    } else {
        cout << "Tidak ada lagu mirip berikutnya.\n";
    }
}


void prevLagu() {
    if (activePlaylist) {
        prevPlaylistLagu();
        return;
    }

    if (!currentSong) {
        cout << "Belum ada lagu yang diputar.\n";
        return;
    }

    Node* mirip = cariLaguMiripSebelumnya(currentSong);

    if (mirip) {
        currentSong = mirip;
        playSong(currentSong);
    } else {
        cout << "Tidak ada lagu mirip sebelumnya.\n";
    }
}


void stopLagu() {
    currentSong = NULL;
    cout << "Lagu dipause.\n";
}

void userMenuLihatLagu() {
    if (!head) {
        cout << "Belum ada lagu.\n";
        return;
    }

    Node* curr;
    int pilih;

    do {
        cout << "\n===== DAFTAR LAGU =====\n";
        curr = head;
        while (curr) {
            cout << curr->data.id << ". "
                 << curr->data.judul << " - "
                 << curr->data.penyanyi << endl;
            curr = curr->next;
        }

        cout << "\n===== KONTROL REKOMENDASI =====\n";
        cout << "1. Play Lagu\n";
        cout << "2. Next Lagu\n";
        cout << "3. Prev Lagu\n";
        cout << "4. Stop Lagu\n";
        cout << "5. Cari Lagu\n";
        cout << "6. Tambah Lagu ke Playlist\n";
        cout << "7. Tambah ke Favorite\n";
        cout << "8. Kembali ke Menu Utama\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {

            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;

                curr = head;
                while (curr && curr->data.id != id)
                    curr = curr->next;

                if (curr)
                    playSong(curr);
                else
                    cout << "Lagu tidak ditemukan.\n";
                break;
            }

            case 2:
                nextLagu();
                break;

            case 3:
                prevLagu();
                break;

            case 4:
                stopLagu();
                break;

            case 5: {
                cariLaguDanPilih();
                break;
            }

            case 6: {
                if (!currentSong) {
                    cout << "Putar lagu terlebih dahulu.\n";
                    break;
                }

                if (!playlistHead) {
                    cout << "Belum ada playlist. Buat playlist terlebih dahulu.\n";
                    break;
                }

                cout << "\n===== PILIH PLAYLIST =====\n";
                Playlist* p = playlistHead;
                int i = 1;

                while (p) {
                    cout << i++ << ". " << p->nama << endl;
                    p = p->next;
                }

                int pilihPl;
                cout << "Masukkan nomor playlist: ";
                cin >> pilihPl;

                p = playlistHead;
                for (int j = 1; p && j < pilihPl; j++)
                    p = p->next;

                if (!p) {
                    cout << "Playlist tidak valid.\n";
                    break;
                }

                PlaylistLaguNode* baru = new PlaylistLaguNode;
                baru->data = currentSong->data;
                baru->next = p->headLagu;
                p->headLagu = baru;

                cout << "Lagu berhasil ditambahkan ke playlist \""
                     << p->nama << "\".\n";
                break;
            }

            case 7:
                if (currentSong)
                    tambahFavorite(currentSong);
                else
                    cout << "Putar lagu terlebih dahulu.\n";
                break;

            case 8:
                return;

            default:
                cout << "Pilihan tidak valid.\n";
        }

    } while (true);
}

void tambahRiwayat(Lagu lagu) {
    RiwayatNode* baru = new RiwayatNode;
    baru->data = lagu;
    baru->next = riwayatTop;
    riwayatTop = baru;
}


void tampilRiwayat() {
    if (!riwayatTop) {
        cout << "Belum ada lagu yang diputar.\n";
        return;
    }

    RiwayatNode* daftar[100];
    RiwayatNode* curr = riwayatTop;
    int jumlah = 0;

    cout << "\n===== HISTORY LAGU =====\n";
    while (curr) {
        cout << jumlah + 1 << ". "
             << curr->data.judul << " - "
             << curr->data.penyanyi << endl;

        daftar[jumlah++] = curr;
        curr = curr->next;
    }

    int pilih;
    do {
        cout << "\n1. Play Lagu dari Riwayat\n";
        cout << "2. Kembali ke Menu Utama\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            int p;
            cout << "Pilih nomor lagu: ";
            cin >> p;

            if (p >= 1 && p <= jumlah) {
                Node* n = head;
                while (n && n->data.id != daftar[p - 1]->data.id)
                    n = n->next;

                if (n) playSong(n);
                else cout << "Lagu tidak ada di library.\n";
            } else {
                cout << "Pilihan tidak valid.\n";
            }
        }
        else if (pilih == 2) {
            return;
        }
        else {
            cout << "Pilihan salah!\n";
        }

    } while (true);
}

//=== PLAYLIST ===
void tampilDaftarPlaylist() {
    if (!playlistHead) {
        cout << "Belum ada playlist.\n";
        return;
    }

    Playlist* p = playlistHead;
    int i = 1;
    cout << "\n===== DAFTAR PLAYLIST =====\n";
    while (p) {
        cout << i++ << ". " << p->nama << endl;
        p = p->next;
    }
}

void menuPlaylist() {
    int pilih;
    do {
        tampilDaftarPlaylist();

        cout << "\n===== MENU PLAYLIST =====\n";
        cout << "1. Pilih Playlist\n";
        cout << "2. Buat Playlist Baru\n";
        cout << "3. Edit Playlist\n";
        cout << "4. Hapus Playlist\n";
        cout << "5. Kembali ke Menu Utama\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1:
                kelolaPlaylist(pilihPlaylist());
                break;

            case 2:
                buatPlaylist();
                break;

            case 3:
                editPlaylist();
                break;

            case 4:
                hapusPlaylist();
                break;

            case 5:
                return;

            default:
                cout << "Pilihan salah!\n";
        }
    } while (true);
}

Playlist* pilihPlaylist() {
    if (!playlistHead) {
        cout << "Playlist kosong.\n";
        return NULL;
    }

    int pilih;
    cout << "Pilih playlist: ";
    cin >> pilih;

    Playlist* curr = playlistHead;
    for (int i = 1; curr && i < pilih; i++)
        curr = curr->next;

    return curr;
}

void kelolaPlaylist(Playlist* p) {
    if (!p) return;

    activePlaylist = p;
    currentPlaylistSong = NULL;

    int pilih;
    do {
        PlaylistLaguNode* daftar[100];
        PlaylistLaguNode* curr = p->headLagu;
        int jumlah = 0;

        cout << "\n=================================\n";
        cout << "PLAYLIST AKTIF : " << p->nama << endl;
        cout << "=================================\n";

        if (!curr) {
            cout << "(Playlist masih kosong)\n";
        }

        while (curr) {
            cout << jumlah + 1 << ". "
                 << curr->data.judul << " - "
                 << curr->data.penyanyi << endl;

            daftar[jumlah++] = curr;
            curr = curr->next;
        }

        cout << "\n===== KONTROL PLAYLIST =====\n";
        cout << "1. Play Lagu\n";
        cout << "2. Next Lagu\n";
        cout << "3. Prev Lagu\n";
        cout << "4. Stop Lagu\n";
        cout << "5. Tambah Lagu ke Playlist\n";
        cout << "6. Hapus Lagu dari Playlist\n";
        cout << "7. Kembali ke Menu Playlist\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {

            case 1: {
                if (jumlah == 0) {
                    cout << "Playlist kosong.\n";
                    break;
                }

                int pLagu;
                cout << "Pilih nomor lagu: ";
                cin >> pLagu;

                if (pLagu < 1 || pLagu > jumlah) {
                    cout << "Pilihan tidak valid.\n";
                    break;
                }

                currentPlaylistSong = daftar[pLagu - 1];

                Node* n = head;
                while (n && n->data.id != currentPlaylistSong->data.id)
                    n = n->next;

                if (n) playSong(n);
                else cout << "Lagu tidak ada di library.\n";

                break;
            }

            case 2:
                nextLagu();
                break;

            case 3:
                prevLagu();
                break;

            case 4:
                stopLagu();
                break;

            case 5: {
                cout << "Masukkan ID lagu yang ingin ditambahkan: ";
                int id;
                cin >> id;

                Node* n = head;
                while (n && n->data.id != id)
                    n = n->next;

                if (!n) {
                    cout << "Lagu tidak ditemukan.\n";
                    break;
                }

                PlaylistLaguNode* baru = new PlaylistLaguNode;
                baru->data = n->data;
                baru->next = p->headLagu;
                p->headLagu = baru;

                cout << "Lagu berhasil ditambahkan ke playlist.\n";
                break;
            }
            case 6: {
                if (!p->headLagu) {
                    cout << "Playlist kosong.\n";
                    break;
                }

                cout << "Masukkan ID lagu yang ingin dihapus: ";
                int id;
                cin >> id;

                PlaylistLaguNode* c = p->headLagu;
                PlaylistLaguNode* prev = NULL;

                while (c && c->data.id != id) {
                    prev = c;
                    c = c->next;
                }

                if (!c) {
                    cout << "Lagu tidak ditemukan di playlist.\n";
                    break;
                }

                if (!prev) p->headLagu = c->next;
                else prev->next = c->next;

                if (currentPlaylistSong == c)
                    currentPlaylistSong = NULL;

                delete c;
                cout << "Lagu berhasil dihapus.\n";
                break;
            }
            case 7:
                activePlaylist = NULL;
                currentPlaylistSong = NULL;
                return;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (true);
}

void buatPlaylist() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string nama;
    cout << "Nama playlist baru: ";
    getline(cin, nama);

    Playlist* baru = new Playlist;
    baru->nama = nama;
    baru->headLagu = NULL;
    baru->next = playlistHead;
    playlistHead = baru;

    cout << "Playlist berhasil dibuat.\n";
}

void editPlaylist() {
    tampilDaftarPlaylist();
    if (!playlistHead) return;

    int pilih;
    cout << "Pilih playlist yang ingin diedit: ";
    cin >> pilih;

    Playlist* curr = playlistHead;
    for (int i = 1; curr && i < pilih; i++)
        curr = curr->next;

    if (!curr) {
        cout << "Playlist tidak ditemukan.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nama playlist baru: ";
    getline(cin, curr->nama);

    cout << "Playlist berhasil diperbarui.\n";
}

void hapusPlaylist() {
    tampilDaftarPlaylist();
    if (!playlistHead) return;

    int pilih;
    cout << "Pilih playlist yang ingin dihapus: ";
    cin >> pilih;

    Playlist* curr = playlistHead;
    Playlist* prev = NULL;

    for (int i = 1; curr && i < pilih; i++) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        cout << "Playlist tidak ditemukan.\n";
        return;
    }

    if (!prev) playlistHead = curr->next;
    else prev->next = curr->next;
    PlaylistLaguNode* l = curr->headLagu;
    while (l) {
        PlaylistLaguNode* tmp = l;
        l = l->next;
        delete tmp;
    }

    delete curr;
    cout << "Playlist berhasil dihapus.\n";
}
void nextPlaylistLagu() {
    if (!currentPlaylistSong || !currentPlaylistSong->next) {
        cout << "Tidak ada lagu berikutnya di playlist.\n";
        return;
    }

    currentPlaylistSong = currentPlaylistSong->next;
    sudahNext = true;

    Node* n = head;
    while (n && n->data.id != currentPlaylistSong->data.id)
        n = n->next;

    if (n) playSong(n);
}

void prevPlaylistLagu() {
    if (!sudahNext) {
        cout << "Prev belum tersedia. Gunakan Next terlebih dahulu.\n";
        return;
    }

    PlaylistLaguNode* curr = activePlaylist->headLagu;
    PlaylistLaguNode* prev = NULL;

    while (curr && curr != currentPlaylistSong) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        cout << "Tidak ada lagu sebelumnya.\n";
        return;
    }

    currentPlaylistSong = prev;

    Node* n = head;
    while (n && n->data.id != currentPlaylistSong->data.id)
        n = n->next;

    if (n) playSong(n);
}

// === FAVORITE ===
bool isFavorite(int id) {
    FavoriteNode* curr = favoriteHead;
    while (curr) {
        if (curr->data.id == id)
            return true;
        curr = curr->next;
    }
    return false;
}

void tambahFavorite(Node* lagu) {
    if (!lagu) return;

    if (isFavorite(lagu->data.id)) {
        cout << "Lagu sudah ada di favorite.\n";
        return;
    }

    FavoriteNode* baru = new FavoriteNode;
    baru->data = lagu->data;
    baru->next = favoriteHead;
    favoriteHead = baru;

    cout << "Lagu ditambahkan ke favorite. \n";
}

void tampilFavorite() {
    if (!favoriteHead) {
        cout << "Belum ada lagu favorite.\n";
        return;
    }

    FavoriteNode* daftar[100];
    FavoriteNode* curr = favoriteHead;
    int jumlah = 0;

    cout << "\n===== LAGU FAVORITE =====\n";
    while (curr) {
        cout << jumlah + 1 << ". "
             << curr->data.judul << " - "
             << curr->data.penyanyi << endl;

        daftar[jumlah++] = curr;
        curr = curr->next;
    }

    int pilih;
    do {
        cout << "\n===== KONTROL FAVORITE =====\n";
        cout << "1. Play Lagu\n";
        cout << "2. Next Lagu\n";
        cout << "3. Prev Lagu\n";
        cout << "4. Stop Lagu\n";
        cout << "5. Kembali ke Menu Utama\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {

            case 1: {
                int p;
                cout << "Pilih nomor lagu: ";
                cin >> p;

                if (p >= 1 && p <= jumlah) {
                    currentFavorite = daftar[p - 1];

                    Node* n = head;
                    while (n && n->data.id != currentFavorite->data.id)
                        n = n->next;

                    if (n)
                        playSong(n);   
                    else
                        cout << "Lagu tidak ada di library.\n";
                } else {
                    cout << "Pilihan tidak valid.\n";
                }
                break;
            }

            case 2:
                nextFavorite();
                break;

            case 3:
                prevFavorite();
                break;

            case 4:
                stopLagu();
                break;

            case 5:
                currentFavorite = NULL;
                return;

            default:
                cout << "Pilihan salah!\n";
        }

    } while (true);
}


void hapusFavorite(int id) {
    if (!favoriteHead) {
        cout << "Favorite masih kosong.\n";
        return;
    }

    FavoriteNode* curr = favoriteHead;
    FavoriteNode* prev = NULL;

    while (curr && curr->data.id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        cout << "Lagu tidak ditemukan di favorite.\n";
        return;
    }

    if (!prev)
        favoriteHead = curr->next;
    else
        prev->next = curr->next;

    delete curr;
    cout << "Lagu berhasil dihapus dari favorite.\n";
}

void nextFavorite() {
    if (!currentFavorite || !currentFavorite->next) {
        cout << "Tidak ada lagu berikutnya di favorite.\n";
        return;
    }

    currentFavorite = currentFavorite->next;

    Node* n = head;
    while (n && n->data.id != currentFavorite->data.id)
        n = n->next;

    if (n) playSong(n); 
}

void prevFavorite() {
    if (!currentFavorite) {
        cout << "Belum ada lagu yang diputar.\n";
        return;
    }

    FavoriteNode* curr = favoriteHead;
    FavoriteNode* prev = NULL;

    while (curr && curr != currentFavorite) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        cout << "Tidak ada lagu sebelumnya di favorite.\n";
        return;
    }

    currentFavorite = prev;

    Node* n = head;
    while (n && n->data.id != currentFavorite->data.id)
        n = n->next;

    if (n) playSong(n); 
}
// === CASE SESITIVE ===
string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

void syncUpdateLagu(const Lagu& laguBaru) {

    // === PLAYLIST ===
    Playlist* p = playlistHead;
    while (p) {
        PlaylistLaguNode* l = p->headLagu;
        while (l) {
            if (l->data.id == laguBaru.id)
                l->data = laguBaru;
            l = l->next;
        }
        p = p->next;
    }

    // === FAVORITE ===
    FavoriteNode* f = favoriteHead;
    while (f) {
        if (f->data.id == laguBaru.id)
            f->data = laguBaru;
        f = f->next;
    }

    // === RIWAYAT ===
    RiwayatNode* r = riwayatTop;
    while (r) {
        if (r->data.id == laguBaru.id)
            r->data = laguBaru;
        r = r->next;
    }
}

void syncHapusLagu(int id) {

    // === PLAYLIST ===
    Playlist* p = playlistHead;
    while (p) {
        PlaylistLaguNode* curr = p->headLagu;
        PlaylistLaguNode* prev = NULL;

        while (curr) {
            if (curr->data.id == id) {
                if (!prev) p->headLagu = curr->next;
                else prev->next = curr->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        p = p->next;
    }

    // === FAVORITE ===
    FavoriteNode* f = favoriteHead;
    FavoriteNode* prevF = NULL;
    while (f) {
        if (f->data.id == id) {
            if (!prevF) favoriteHead = f->next;
            else prevF->next = f->next;
            delete f;
            break;
        }
        prevF = f;
        f = f->next;
    }

    // === RIWAYAT ===
    RiwayatNode* r = riwayatTop;
    RiwayatNode* prevR = NULL;
    while (r) {
        if (r->data.id == id) {
            if (!prevR) riwayatTop = r->next;
            else prevR->next = r->next;
            delete r;
            break;
        }
        prevR = r;
        r = r->next;
    }
}

// === BST ===
BSTNode* insertBST(BSTNode* root, Lagu lagu) {
    if (!root) {
        BSTNode* node = new BSTNode;
        node->data = lagu;
        node->left = node->right = NULL;
        return node;
    }

    if (toLower(lagu.judul) < toLower(root->data.judul))
        root->left = insertBST(root->left, lagu);
    else
        root->right = insertBST(root->right, lagu);

    return root;
}

void searchBSTAll(BSTNode* root, string key, vector<Lagu>& hasil) {
    if (!root) return;

    key = toLower(key);

    string judul    = toLower(root->data.judul);
    string penyanyi = toLower(root->data.penyanyi);
    string lirik    = toLower(root->data.lirik);

    if (judul.find(key) != string::npos ||
        penyanyi.find(key) != string::npos ||
        lirik.find(key) != string::npos) {
        hasil.push_back(root->data);
    }

    searchBSTAll(root->left, key, hasil);
    searchBSTAll(root->right, key, hasil);
}

void rebuildBST() {
    bstRoot = NULL;
    Node* curr = head;
    while (curr) {
        bstRoot = insertBST(bstRoot, curr->data);
        curr = curr->next;
    }
}

void menuHasilSearch(const vector<Lagu>& hasil) {
    if (hasil.empty()) {
        cout << "Tidak ada hasil pencarian.\n";
        return;
    }

    int pilih;
    int index = 0;

    do {
        cout << "\n===== HASIL PENCARIAN =====\n";
        for (int i = 0; i < hasil.size(); i++) {
            cout << i + 1 << ". "
                 << hasil[i].judul << " - "
                 << hasil[i].penyanyi << endl;
        }

        cout << "\n===== KONTROL PENCARIAN =====\n";
        cout << "1. Play Lagu\n";
        cout << "2. Next\n";
        cout << "3. Prev\n";
        cout << "4. Stop\n";
        cout << "5. Tambah ke Playlist\n";
        cout << "6. Tambah ke Favorite\n";
        cout << "7. Kembali ke Menu Utama\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {

            case 1: {
                cout << "Pilih nomor lagu: ";
                cin >> index;
                index--;

                if (index < 0 || index >= hasil.size()) {
                    cout << "Pilihan tidak valid.\n";
                    break;
                }

                Node* n = head;
                while (n && n->data.id != hasil[index].id)
                    n = n->next;

                if (n) playSong(n);
                break;
            }

            case 2: {
                index = (index + 1) % hasil.size();

                Node* n = head;
                while (n && n->data.id != hasil[index].id)
                    n = n->next;

                if (n) playSong(n);
                break;
            }

            case 3: {
                index = (index - 1 + hasil.size()) % hasil.size();

                Node* n = head;
                while (n && n->data.id != hasil[index].id)
                    n = n->next;

                if (n) playSong(n);
                break;
            }

            case 4:
                stopLagu();
                break;

            case 5: {
                if (!currentSong) {
                    cout << "Putar lagu terlebih dahulu.\n";
                    break;
                }

                if (!playlistHead) {
                    cout << "Belum ada playlist. Buat playlist terlebih dahulu.\n";
                    break;
                }

                cout << "\n===== PILIH PLAYLIST =====\n";
                Playlist* p = playlistHead;
                int i = 1;

                while (p) {
                    cout << i++ << ". " << p->nama << endl;
                    p = p->next;
                }

                int pilihPl;
                cout << "Masukkan nomor playlist: ";
                cin >> pilihPl;

                p = playlistHead;
                for (int j = 1; p && j < pilihPl; j++)
                    p = p->next;

                if (!p) {
                    cout << "Playlist tidak valid.\n";
                    break;
                }

                PlaylistLaguNode* baru = new PlaylistLaguNode;
                baru->data = currentSong->data;
                baru->next = p->headLagu;
                p->headLagu = baru;

                cout << "Lagu berhasil ditambahkan ke playlist \""
                     << p->nama << "\".\n";
                break;
            }

      
            case 6:
                if (currentSong)
                    tambahFavorite(currentSong);
                else
                    cout << "Putar lagu terlebih dahulu.\n";
                break;


            case 7:
                return;

            default:
                cout << "Pilihan tidak valid.\n";
        }

    } while (true);
}


