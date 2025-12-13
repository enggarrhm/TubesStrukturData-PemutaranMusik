#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <string>
#include <limits>
using namespace std;

// struct
struct Lagu {
    int id;
    string judul;
    string penyanyi;
    string lirik;
};

struct Node {
    Lagu data;
    Node* next;
    Node* prev;
};

// Playlist
struct PlaylistLaguNode {
    Lagu data;
    PlaylistLaguNode* next;
};

struct Playlist {
    string nama;
    PlaylistLaguNode* headLagu;
    Playlist* next;
};

// Riwayat
struct RiwayatNode {
    Lagu data;
    RiwayatNode* next;
};

// favorite
struct FavoriteNode {
    Lagu data;
    FavoriteNode* next;
};

// global
extern Node* head;
extern Node* tail;
extern Node* currentSong;
extern Playlist* playlistHead;
extern RiwayatNode* riwayatTop;
extern Playlist* activePlaylist;
extern PlaylistLaguNode* currentPlaylistSong;
extern FavoriteNode* favoriteHead;
extern FavoriteNode* currentFavorite;
extern bool modeFavorite;
extern bool sudahNext;

// util
Node* buatNode(Lagu lagu);
int getNextID();
void initDefaultSongs();

// admin
void tambahLagu();
void tampilLagu();
void updateLagu();
void hapusLagu();

// user
void userMenuLihatLagu();
Node* cariLaguDanPilih();
void playSong(Node* lagu);
void nextLagu();
void prevLagu();
void stopLagu();
void tambahRiwayat(Lagu lagu);
void tampilRiwayat();

// playlist
void menuPlaylist();
void buatPlaylist();
void editPlaylist();
void hapusPlaylist();
void tampilDaftarPlaylist();
void kelolaPlaylist(Playlist* p);
void nextPlaylistLagu();
void prevPlaylistLagu();
Playlist* pilihPlaylist();
Node* cariLaguMirip(Node* lagu);
void tambahFavorite(Node* lagu);
void tampilFavorite();
bool isFavorite(int id);
void hapusFavorite(int id);
void nextFavorite();
void prevFavorite();
#endif
