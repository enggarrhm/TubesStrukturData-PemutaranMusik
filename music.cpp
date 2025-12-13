#include "music.h"

// global
Node* head = NULL;
Node* tail = NULL;
Node* currentSong = NULL;
RiwayatNode* riwayatTop = NULL;
Playlist* playlistHead = NULL;
Playlist* activePlaylist = NULL;
PlaylistLaguNode* currentPlaylistSong = NULL;
FavoriteNode* favoriteHead = NULL;
FavoriteNode* currentFavorite = NULL;
bool modeFavorite = false;
bool sudahNext = false;

//util
Node* buatNode(Lagu L) {
    Node* n = new Node;
    n->data = L;
    n->next = n->prev = NULL;
    return n;
}

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

void initDefaultSongs() {
    Lagu data[3] = {
        {1, "Mata Turun Ke Hati", "Hivi", "Oh kasihku..."},
        {2, "XXL", "Lany", "I miss you XXL"},
        {3, "123456", "Budi Doremi", "Satu kali ku bertemu"}
    };

    for (int i = 0; i < 3; i++) {
        Node* baru = buatNode(data[i]);
        if (!head) head = tail = baru;
        else {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }
    }
}

//admin
void tambahLagu() {
    Lagu L;
    L.id = getNextID();
    cout << "\nID Lagu: " << L.id << "\n";

    cin.ignore();
    cout << "Judul     : "; getline(cin, L.judul);
    cout << "Penyanyi  : "; getline(cin, L.penyanyi);
    cout << "Lirik     : "; getline(cin, L.lirik);

    Node* baru = buatNode(L);
    baru->next = head;
    if (head) head->prev = baru;
    head = baru;
    if (!tail) tail = baru;

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
    cout << "\n=== Update Lagu ===\n";
    cout << "Masukkan ID lagu: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Node *curr = head;
    while (curr != NULL && curr->data.id != id)
        curr = curr->next;

    if (curr == NULL) {
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
            cout << "Judul berhasil diupdate.\n";
            break;

        case 2:
            cout << "Penyanyi Baru: ";
            getline(cin, curr->data.penyanyi);
            cout << "Penyanyi berhasil diupdate.\n";
            break;

        case 3:
            cout << "Lirik Baru: ";
            getline(cin, curr->data.lirik);
            cout << "Lirik berhasil diupdate.\n";
            break;

        case 4:
            cout << "Judul Baru    : ";
            getline(cin, curr->data.judul);
            cout << "Penyanyi Baru : ";
            getline(cin, curr->data.penyanyi);
            cout << "Lirik Baru    : ";
            getline(cin, curr->data.lirik);
            cout << "Semua data berhasil diupdate.\n";
            break;

        default:
            cout << "Pilihan tidak valid. Update dibatalkan.\n";
            return;
    }
}


void hapusLagu() {
    int id;
    cout << "Masukkan ID: ";
    cin >> id;

    Node* curr = head;
    while (curr && curr->data.id != id) curr = curr->next;

    if (!curr) return;

    if (curr == head) head = curr->next;
    if (curr == tail) tail = curr->prev;
    if (curr->prev) curr->prev->next = curr->next;
    if (curr->next) curr->next->prev = curr->prev;

    delete curr;

    cout << "Lagu dihapus!\n";
}

//user
Node* cariLaguMiripBerikutnya(Node* current) {
    if (!current) return NULL;

    string artist = current->data.penyanyi;
    string title = current->data.judul;

    Node* curr = head;
    while (curr) {
        if (curr != current && curr->data.penyanyi == artist)
            return curr;
        curr = curr->next;
    }

    curr = head;
    while (curr) {
        if (curr != current && curr->data.judul.find(title.substr(0, 3)) != string::npos)
            return curr;
        curr = curr->next;
    }

    return NULL; 
}

Node* cariLaguMiripSebelumnya(Node* current) {
    if (!current) return NULL;

    string artist = current->data.penyanyi;
    string title = current->data.judul;

    Node* kandidat = NULL;

    Node* curr = head;
    while (curr) {
        if (curr != current) {
            if (curr->data.penyanyi == artist)
                kandidat = curr;
        }
        curr = curr->next;
    }

    if (kandidat) return kandidat;

    curr = head;
    while (curr) {
        if (curr != current &&
            curr->data.judul.find(title.substr(0, 3)) != string::npos)
            return curr;
        curr = curr->next;
    }

    return NULL;
}

Node* cariLaguDanPilih() {
    cin.ignore();
    string key;
    cout << "\nCari lagu (judul/penyanyi/lirik): ";
    getline(cin, key);

    Node* hasil[100];
    Node* curr = head;
    int n = 0;

    cout << "\n=== HASIL PENCARIAN ===\n";
    while (curr) {
        if (curr->data.judul.find(key) != string::npos ||
            curr->data.penyanyi.find(key) != string::npos ||
            curr->data.lirik.find(key) != string::npos) {

            cout << n + 1 << ". "
                 << curr->data.judul << " - "
                 << curr->data.penyanyi << endl;

            hasil[n++] = curr;
        }
        curr = curr->next;
    }

    if (n == 0) {
        cout << "Lagu tidak ditemukan.\n";
        return NULL;
    }

    int pilih;
    cout << "Pilih lagu (0 batal): ";
    cin >> pilih;

    if (pilih == 0 || pilih < 1 || pilih > n)
        return NULL;

    return hasil[pilih - 1];
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
    // Jika sedang di playlist → gunakan nextPlaylistLagu()
    if (activePlaylist) {
        nextPlaylistLagu();
        return;
    }

    // Tidak di playlist → cari lagu mirip
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

        cout << "\n===== MENU =====\n";
        cout << "1. Play Lagu\n";
        cout << "2. Next Lagu\n";
        cout << "3. Prev Lagu\n";
        cout << "4. Stop Lagu\n";
        cout << "5. Lihat Favorite\n";
        cout << "6. Tambah ke Favorite\n";
        cout << "7. Kembali\n";
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

            case 5:
                tampilFavorite();
                break;

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
        cout << "2. Kembali\n";
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


//playlist
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
        cout << "3. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: kelolaPlaylist(pilihPlaylist()); break;
            case 2: buatPlaylist(); break;
            case 3: return;
            default: cout << "Pilihan salah!\n";
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
    cin.ignore();
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

    cin.ignore();
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

    // hapus lagu dalam playlist
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
        cout << "5. Kembali\n";
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
