#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <string>
#include <limits>
#include <vector>
using namespace std;

// === STRUCT ===
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

// === BST ===
struct BSTNode {
    Lagu data;
    BSTNode* left;
    BSTNode* right;
};

// === PLAYLIST ===
struct PlaylistLaguNode {
    Lagu data;
    PlaylistLaguNode* next;
};

struct Playlist {
    string nama;
    PlaylistLaguNode* headLagu;
    Playlist* next;
};

// === RIWAYAT ===
struct RiwayatNode {
    Lagu data;
    RiwayatNode* next;
};

// === FAVORITE ===
struct FavoriteNode {
    Lagu data;
    FavoriteNode* next;
};

// === GLOBAL ===
extern Node* head;
extern Node* tail;
extern Node* currentSong;
extern Playlist* playlistHead;
extern RiwayatNode* riwayatTop;
extern Playlist* activePlaylist;
extern PlaylistLaguNode* currentPlaylistSong;
extern FavoriteNode* favoriteHead;
extern FavoriteNode* currentFavorite;
extern bool sudahNext;

// === BST ===
BSTNode* insertBST(BSTNode* root, Lagu lagu);
void searchBSTMulti(BSTNode* root, string key, BSTNode*& hasil);
void rebuildBST();

// ==== UTIL ===
Node* buatNode(Lagu lagu);
int getNextID();
void initDefaultSongs();
string toLower(string s);

// === BST === 
BSTNode* insertBST(BSTNode* root, Lagu lagu);

// === ADMIN ===
void tambahLagu();
void tampilLagu();
void updateLagu();
void hapusLagu();

// === USER ===
void userMenuLihatLagu();
void cariLaguDanPilih();
void playSong(Node* lagu);
void nextLagu();
void prevLagu();
void stopLagu();
void tambahRiwayat(Lagu lagu);
void tampilRiwayat();

// === PLAYLIST ===
void menuPlaylist();
void buatPlaylist();
void editPlaylist();
void hapusPlaylist();
void tampilDaftarPlaylist();
void kelolaPlaylist(Playlist* p);
void nextPlaylistLagu();
void prevPlaylistLagu();
Playlist* pilihPlaylist();

// === FAVORITE ===
void tambahFavorite(Node* lagu);
void tampilFavorite();
bool isFavorite(int id);
void hapusFavorite(int id);
void nextFavorite();
void prevFavorite();

// === BST === 
BSTNode* insertBST(BSTNode* root, Lagu lagu);
BSTNode* searchBST(BSTNode* root, string key);
void searchBSTAll(BSTNode* root, string key, vector<Lagu>& hasil);
void menuHasilSearch(const vector<Lagu>& hasil);

// === SINKRON DATA ===
void syncUpdateLagu(const Lagu& laguBaru);
void syncHapusLagu(int id);


#endif
