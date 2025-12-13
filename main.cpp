#include "music.h"
#include "music.cpp"

int main() {
    initDefaultSongs();

    string username, password;

login:
    while (true) {
        cout << "\n===== LOGIN =====\n";
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        cin >> password;

        // ADMIN
        if (username == "admin" && password == "123") {
            int pilih;
            do {
                cout << "\n===== MENU ADMIN =====\n";
                cout << "1. Tambah Lagu\n";
                cout << "2. Lihat Lagu\n";
                cout << "3. Update Lagu\n";
                cout << "4. Hapus Lagu\n";
                cout << "5. Logout\n";
                cout << "6. Keluar\nPilih: ";
                cin >> pilih;

                switch (pilih) {
                    case 1: tambahLagu(); break;
                    case 2: tampilLagu(); break;
                    case 3: updateLagu(); break;
                    case 4: hapusLagu(); break;
                    case 5: goto login;
                    case 6: return 0;
                    default: cout << "Pilihan salah!\n";
                }
            } while (true);
        }

       //user
else if (username == "user" && password == "123") {
    int pilih;
    do {
       cout << "\n===== MENU USER =====\n";
       cout << "1. Rekomendasi Lagu\n";
       cout << "2. Playlist\n";
       cout << "3. History Lagu\n";
       cout << "4. Favorite Lagu\n";
       cout << "5. Logout\n";
       cout << "6. Keluar\n";
       cout << "Pilih: ";
       cin >> pilih;

        switch (pilih) {
            case 1: userMenuLihatLagu(); break;
            case 2: menuPlaylist(); break;
            case 3: tampilRiwayat(); break;
            case 4: tampilFavorite(); break;
            case 5: goto login;
            case 6: return 0;
            default: cout << "Pilihan tidak valid!\n";
}

    } while (true);
}
    }
}
