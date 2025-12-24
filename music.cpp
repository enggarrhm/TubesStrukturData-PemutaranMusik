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

Node* cariLaguByID(int id) {
    Node* curr = head;
    while (curr) {
        if (curr->data.id == id)
            return curr;
        curr = curr->next;
    }
    return NULL;
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

// === TABEL LAGU ===
void tampilTabelHeader() {
    cout << "\n========================================================================================\n";
    cout << left
         << setw(5)  << "ID"
         << setw(25) << "Judul"
         << setw(20) << "Penyanyi"
         << setw(40) << "Lirik"
         << endl;
    cout << "========================================================================================\n";
}

void tampilTabelRow(const Lagu& L) {
    cout << left
         << setw(5)  << L.id
         << setw(25) << L.judul.substr(0, 24)
         << setw(20) << L.penyanyi.substr(0, 19)
         << setw(40) << L.lirik.substr(0, 39)
         << endl;
}

void tampilTabelFooter() {
    cout << "========================================================================================\n";
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
    if (!head) {
        cout << "Belum ada lagu.\n";
        return;
    }

    cout << "\n========================================================================================\n";
    cout << left
         << setw(5)  << "ID"
         << setw(25) << "Judul"
         << setw(20) << "Penyanyi"
         << setw(40) << "Lirik"
         << endl;
    cout << "========================================================================================\n";

    Node* curr = head;
    while (curr) {
        cout << left
             << setw(5)  << curr->data.id
             << setw(25) << curr->data.judul.substr(0, 24)
             << setw(20) << curr->data.penyanyi.substr(0, 19)
             << setw(40) << curr->data.lirik.substr(0, 39)
             << endl;
        curr = curr->next;
    }

    cout << "========================================================================================\n";
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

    cout << "\n+====================================================================+\n";
    cout << "|                         SEDANG DIPUTAR                              |\n";
    cout << "+====================================================================+\n";
    cout << "| Judul    : " << left << setw(55) << lagu->data.judul << "|\n";
    cout << "| Penyanyi : " << left << setw(55) << lagu->data.penyanyi << "|\n";
    cout << "+--------------------------------------------------------------------+\n";
    cout << "| Lirik                                                            |\n";
    cout << "+--------------------------------------------------------------------+\n";

    string lirik = lagu->data.lirik;
    int lebar = 68;

    for (int i = 0; i < lirik.length(); i += lebar) {
        cout << "| " << left << setw(lebar) << lirik.substr(i, lebar) << " |\n";
    }

    cout << "+====================================================================+\n";
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

    int pilih;
    do {
        cout << "\n===== DAFTAR LAGU =====\n";
        tampilTabelHeader();

        Node* curr = head;
        while (curr) {
            tampilTabelRow(curr->data);
            curr = curr->next;
        }
        tampilTabelFooter();

        cout << "\n===== KONTROL =====\n";
        cout << "1. Play Lagu (ID)\n";
        cout << "2. Next Lagu\n";
        cout << "3. Prev Lagu\n";
        cout << "4. Stop Lagu\n";
        cout << "5. Cari Lagu\n";
        cout << "6. Tambah ke Playlist\n";
        cout << "7. Tambah ke Favorite\n";
        cout << "8. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;

                Node* n = cariLaguByID(id);
                if (n) playSong(n);
                else cout << "Lagu tidak ditemukan.\n";
                break;
            }
            case 2: nextLagu(); break;
            case 3: prevLagu(); break;
            case 4: stopLagu(); break;
            case 5: cariLaguDanPilih(); break;
            case 6:
                if (currentSong) tambahPlaylist(currentSong);
                else cout << "Putar lagu terlebih dahulu.\n";
                break;
            case 7:
                if (currentSong) tambahFavorite(currentSong);
                else cout << "Putar lagu terlebih dahulu.\n";
                break;
            case 8: return;
            default: cout << "Pilihan tidak valid.\n";
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
        cout << "Belum ada riwayat.\n";
        return;
    }

    int pilih;
    do {
        cout << "\n===== RIWAYAT LAGU =====\n";
        tampilTabelHeader();

        RiwayatNode* curr = riwayatTop;
        while (curr) {
            tampilTabelRow(curr->data);
            curr = curr->next;
        }
        tampilTabelFooter();

        cout << "\n=== KONTROL RIWAYAT ===\n";
        cout << "1. Play Lagu (ID)\n";
        cout << "2. Next\n";
        cout << "3. Prev\n";
        cout << "4. Stop\n";
        cout << "5. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;
                Node* n = cariLaguByID(id);
                if (n) playSong(n);
                else cout << "Lagu tidak ditemukan.\n";
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

            case 5:
                return;

            default:
                cout << "Pilihan tidak valid.\n";
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

    int pilih;
    do {
        cout << "\n===== PLAYLIST : " << p->nama << " =====\n";
        tampilTabelHeader();

        PlaylistLaguNode* curr = p->headLagu;
        while (curr) {
            tampilTabelRow(curr->data);
            curr = curr->next;
        }
        tampilTabelFooter();
        cout << "\n===== KONTROL PLAYLIST =====\n";
        cout << "1. Play Lagu (ID)\n";
        cout << "2. Next\n";
        cout << "3. Prev\n";
        cout << "4. Stop\n";
        cout << "5. Hapus Lagu (ID)\n";
        cout << "6. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;

                PlaylistLaguNode* pl = p->headLagu;
                while (pl && pl->data.id != id)
                    pl = pl->next;

                if (!pl) {
                    cout << "Lagu tidak ada di playlist.\n";
                    break;
                }

                currentPlaylistSong = pl;
                Node* n = cariLaguByID(id);
                if (n) playSong(n);
                break;
            }
            case 2: nextLagu(); break;
            case 3: prevLagu(); break;
            case 4: stopLagu(); break;
            case 5: {
                int id;
                cout << "ID lagu yang dihapus: ";
                cin >> id;
                hapusLaguDariPlaylist(p, id);
                break;
            }
            case 6:
                activePlaylist = NULL;
                return;
        }
    } while (true);
}

void buatPlaylist() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string nama;
    cout << "\nNama playlist baru: ";
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

void hapusLaguDariPlaylist(Playlist* p, int id) {
    if (!p || !p->headLagu) {
        cout << "Playlist kosong.\n";
        return;
    }

    PlaylistLaguNode* curr = p->headLagu;
    PlaylistLaguNode* prev = NULL;

    while (curr && curr->data.id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        cout << "Lagu tidak ditemukan di playlist.\n";
        return;
    }

    if (!prev)
        p->headLagu = curr->next;
    else
        prev->next = curr->next;

    delete curr;
    cout << "Lagu berhasil dihapus dari playlist.\n";
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

void tambahPlaylist(Node* lagu) {
    if (!lagu) return;
    
    if (!playlistHead) {
        cout << "Belum ada playlist.\n";
        cout << "Silakan buat playlist terlebih dahulu.\n";
        buatPlaylist();  
        if (!playlistHead) {
            cout << "Playlist belum tersedia.\n";
            return;
        }
    }

    tampilDaftarPlaylist();

    int pilih;
    cout << "Pilih playlist: ";
    cin >> pilih;

    Playlist* p = playlistHead;
    for (int i = 1; p && i < pilih; i++)
        p = p->next;

    if (!p) {
        cout << "Playlist tidak ditemukan.\n";
        return;
    }

    PlaylistLaguNode* curr = p->headLagu;
    while (curr) {
        if (curr->data.id == lagu->data.id) {
            cout << "Lagu sudah ada di playlist.\n";
            return;
        }
        curr = curr->next;
    }

    PlaylistLaguNode* baru = new PlaylistLaguNode;
    baru->data = lagu->data;
    baru->next = p->headLagu;
    p->headLagu = baru;

    cout << "Lagu berhasil ditambahkan ke playlist \"" 
         << p->nama << "\".\n";
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
        cout << "Favorite kosong.\n";
        return;
    }

    int pilih;
    do {
        cout << "\n===== FAVORITE =====\n";
        tampilTabelHeader();

        FavoriteNode* curr = favoriteHead;
        while (curr) {
            tampilTabelRow(curr->data);
            curr = curr->next;
        }
        tampilTabelFooter();

        cout << "\n=== KONTROL FAVORITE ===\n";
        cout << "1. Play Lagu (ID)\n";
        cout << "2. Next\n";
        cout << "3. Prev\n";
        cout << "4. Stop\n";
        cout << "5. Hapus Lagu (ID)\n";
        cout << "6. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;
                Node* n = cariLaguByID(id);
                if (n) playSong(n);
                else cout << "Lagu tidak ditemukan.\n";
                break;
            }
            case 2: nextFavorite(); break;
            case 3: prevFavorite(); break;
            case 4: stopLagu(); break;
            case 5: {
                int id;
                cout << "ID lagu yang dihapus: ";
                cin >> id;
                hapusFavorite(id);
                break;
            }
            case 6: return;
            default: cout << "Pilihan salah.\n";
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
    do {
        cout << "\n===== HASIL PENCARIAN =====\n";
        tampilTabelHeader();
        for (auto& L : hasil)
            tampilTabelRow(L);
        tampilTabelFooter();
        cout << "\n=== KONTROL PENCARIAN ===\n";
        cout << "1. Play Lagu (ID)\n";
        cout << "2. Next\n";
        cout << "3. Prev\n";
        cout << "4. Stop\n";
        cout << "5. Tambah ke Playlist\n";
        cout << "6. Tambah ke Favorite\n";
        cout << "7. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int id;
                cout << "Masukkan ID lagu: ";
                cin >> id;
                Node* n = cariLaguByID(id);
                if (n) playSong(n);
                else cout << "Lagu tidak ditemukan.\n";
                break;
            }
            case 2: nextLagu(); break;
            case 3: prevLagu(); break;
            case 4: stopLagu(); break;
            case 5:
                if (currentSong) tambahPlaylist(currentSong);
                else cout << "Putar lagu terlebih dahulu.\n";
                break;
            case 6:
                if (currentSong) tambahFavorite(currentSong);
                else cout << "Putar lagu terlebih dahulu.\n";
                break;
            case 7: return;
            default: cout << "Pilihan salah.\n";
        }
    } while (true);
}
