#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h> // abs fonksiyonu için

#define KOSE_SOL_UST 201 
#define KOSE_SAG_UST 187 
#define KOSE_SOL_ALT 200 
#define KOSE_SAG_ALT 188 
#define DUZ_YATAY    205 
#define DUZ_DIKEY    186 
#define DUVAR_DOLGU  219  
#define YOL '*'
#define BOS ' '
#define IZ  '.'
#define COZUM '+' 

int genislik, yukseklik;
unsigned char harita[100][100]; 
int imX = 1, imY = 1;
int aktifMod = 1; // 0: Gezme, 1: Yol, 2: Duvar
int aramaModu = 0; // 0: BFS, 1: DFS

void gotoxy(int x, int y) {
    COORD coord = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void imleciKutuYap() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = TRUE; 
    cursorInfo.dwSize = 100;    
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void imleciNormalYap() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = TRUE; 
    cursorInfo.dwSize = 20;     
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void renk(int renkKodu) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), renkKodu);
}

void hucreyiGuncelle(int x, int y) {
    gotoxy(x, y);
    if (x == 1 && y == 1) { renk(10); printf("S"); return; } 
    if (x == genislik - 2 && y == yukseklik - 2) { renk(12); printf("E"); return; } 

    if (harita[y][x] == COZUM) { renk(10); printf("%c", COZUM); } 
    else if (harita[y][x] == YOL) { renk(14); printf("%c", YOL); } 
    else if (harita[y][x] == IZ) { renk(3); printf("%c", IZ); }    
    else if (harita[y][x] == BOS) { printf(" "); }
    else if (harita[y][x] == DUVAR_DOLGU) { renk(8); printf("%c", DUVAR_DOLGU); }
    else { renk(8); printf("%c", harita[y][x]); }
}

void tamEkranCiz() {
    int i, j;
    system("cls");
    for (i = 0; i < yukseklik; i++) {
        for (j = 0; j < genislik; j++) hucreyiGuncelle(j, i);
        printf("\n");
    }
    renk(15);
    gotoxy(0, yukseklik + 2);
    printf("F1: Yol | F2: Duvar | F3: Gez | F9: Rastgele Harita | F4: Kaydet | F5: Yukle\n");
    printf("ENTER: Yolu Bul | F7: Reset (Temizle) | F8: Algoritma | Esc: Cikis\n");
}

void haritaKaydet() {
    char dosyaAdi[50];
    FILE *fp;
    int i, j; 
    
    gotoxy(0, yukseklik + 4); 
    renk(11);
    printf("                                                                  "); 
    gotoxy(0, yukseklik + 4);
    printf("Kaydedilecek dosya adi (ornek: harita.txt): ");
    
    imleciNormalYap(); 
    scanf("%s", dosyaAdi);
    imleciKutuYap();   

    fp = fopen(dosyaAdi, "w");
    if (fp == NULL) {
        gotoxy(0, yukseklik + 5); renk(12); printf("HATA: Dosya olusturulamadi!");
        return;
    }

    fprintf(fp, "%d %d\n", genislik, yukseklik);

    for (i = 0; i < yukseklik; i++) {
        for (j = 0; j < genislik; j++) {
            fprintf(fp, "%d ", harita[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    gotoxy(0, yukseklik + 5); renk(10); printf("Basariyla kaydedildi: %s      ", dosyaAdi);
    Sleep(1000);
    
    gotoxy(0, yukseklik + 4); printf("                                                 ");
    gotoxy(0, yukseklik + 5); printf("                                                 ");
}

// --- GUNCELLENEN YUKLEME FONKSIYONU (Temizleyerek Yukleme) ---
void haritaYukle() {
    char dosyaAdi[50];
    FILE *fp;
    int okunanDeger;
    unsigned char okunanKarakter; // Karakter kontrolu icin
    int i, j;

    gotoxy(0, yukseklik + 4); 
    renk(11);
    printf("                                                                  ");
    gotoxy(0, yukseklik + 4);
    printf("Yuklenecek dosya adi (ornek: harita.txt): ");
    
    imleciNormalYap();
    scanf("%s", dosyaAdi);
    imleciKutuYap();

    fp = fopen(dosyaAdi, "r");
    if (fp == NULL) {
        gotoxy(0, yukseklik + 5); renk(12); printf("HATA: Dosya bulunamadi!        ");
        Sleep(1000);
        return;
    }

    fscanf(fp, "%d %d", &genislik, &yukseklik);

    for (i = 0; i < yukseklik; i++) {
        for (j = 0; j < genislik; j++) {
            fscanf(fp, "%d", &okunanDeger);
            
            // Okunan degeri karaktere cevirip kontrol ediyoruz
            okunanKarakter = (unsigned char)okunanDeger;
            
            // Eger okunan sey bir COZUM (+) veya IZ (.) ise onu YOL (*) yap
            if (okunanKarakter == COZUM || okunanKarakter == IZ) {
                harita[i][j] = YOL;
            } else {
                harita[i][j] = okunanKarakter;
            }
        }
    }
    fclose(fp);

    imX = 1; imY = 1;
    tamEkranCiz();
    gotoxy(0, yukseklik + 5); renk(10); printf("Harita yuklendi: %s       ", dosyaAdi);
    Sleep(1000);
    gotoxy(0, yukseklik + 5); printf("                                                 ");
}

void labirentOlustur(int x, int y) {
    int i, r, temp, nx, ny;
    int dx = 0, dy = 0;
    int yonler[] = {0, 1, 2, 3}; 
    
    harita[y][x] = YOL; 

    for (i = 0; i < 4; i++) {
        r = rand() % 4;
        temp = yonler[i];
        yonler[i] = yonler[r];
        yonler[r] = temp;
    }

    for (i = 0; i < 4; i++) {
        dx = 0; dy = 0; 
        switch(yonler[i]) {
            case 0: dy = -2; break; 
            case 1: dy = 2; break;  
            case 2: dx = -2; break; 
            case 3: dx = 2; break;  
        }
        nx = x + dx; ny = y + dy;

        if (nx > 0 && nx < genislik - 1 && ny > 0 && ny < yukseklik - 1 && harita[ny][nx] == DUVAR_DOLGU) {
            harita[y + dy/2][x + dx/2] = YOL;
            labirentOlustur(nx, ny);
        }
    }
}

void rastgeleHaritaBaslat() {
    int i, j;
    for (i = 0; i < yukseklik; i++) {
        for (j = 0; j < genislik; j++) {
            if (i == 0 || i == yukseklik - 1 || j == 0 || j == genislik - 1) continue; 
            harita[i][j] = DUVAR_DOLGU;
        }
    }
    labirentOlustur(1, 1);
    harita[yukseklik-2][genislik-2] = YOL;
    if(harita[yukseklik-2][genislik-3] == DUVAR_DOLGU && harita[yukseklik-3][genislik-2] == DUVAR_DOLGU) {
        harita[yukseklik-2][genislik-3] = YOL;
    }
    tamEkranCiz();
}

void yoluBul() {
    int startX = 1, startY = 1;
    int hedefX = genislik - 2, hedefY = yukseklik - 2;
    int dist[100][100], pX[100][100], pY[100][100];
    int i, j, bulundu = 0;
    int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1}; 

    // Temizlik
    for(i=0; i<yukseklik; i++) {
        for(j=0; j<genislik; j++) {
            if(harita[i][j] == IZ || harita[i][j] == COZUM) {
                harita[i][j] = YOL; 
                hucreyiGuncelle(j, i);
            }
            dist[i][j] = -1; pX[i][j] = -1; 
        }
    }

    // ================= BFS ALGORITMASI =================
    if (aramaModu == 0) {
        int kX[10000], kY[10000];
        int bas = 0, son = 0;

        kX[son] = startX; kY[son] = startY; dist[startY][startX] = 0; son++;

        while (bas < son) {
            int x = kX[bas]; int y = kY[bas]; bas++; 
            
            if (x == hedefX && y == hedefY) { bulundu = 1; break; }

            for (i = 0; i < 4; i++) {
                int nx = x + dx[i]; int ny = y + dy[i];

                if (nx >= 0 && nx < genislik && ny >= 0 && ny < yukseklik && 
                    harita[ny][nx] == YOL && dist[ny][nx] == -1) {
                    
                    dist[ny][nx] = dist[y][x] + 1; 
                    pX[ny][nx] = x; pY[ny][nx] = y;

                    if (!(nx == startX && ny == startY) && !(nx == hedefX && ny == hedefY)) {
                        harita[ny][nx] = IZ; hucreyiGuncelle(nx, ny); Sleep(5); 
                    }
                    kX[son] = nx; kY[son] = ny; son++;
                }
            }
        }
    }
    // ================= DENGELİ DFS ALGORITMASI =================
    else {
        int stackX[10000], stackY[10000], top = 0;
        int dirs[4] = {0, 1, 2, 3}; 
        int skorlar[4]; // Her yön için puanlama
        int k, m, temp, tempScore;
        
        stackX[top] = startX; stackY[top] = startY; 
        dist[startY][startX] = 0; 

        while (top >= 0) {
            int x = stackX[top]; 
            int y = stackY[top];

            if (x == hedefX && y == hedefY) { bulundu = 1; break; }

            int yolBulundu = 0;
            
            for(k=0; k<4; k++) {
                dirs[k] = k;
                int nx = x + dx[k];
                int ny = y + dy[k];
                int uzaklik = abs(hedefX - nx) + abs(hedefY - ny);
                
                skorlar[k] = uzaklik + (rand() % 15); 
            }

            for(k=0; k<3; k++) {
                for(m=0; m<3-k; m++) {
                    if(skorlar[m] > skorlar[m+1]) {
                        tempScore = skorlar[m];
                        skorlar[m] = skorlar[m+1];
                        skorlar[m+1] = tempScore;
                        
                        temp = dirs[m];
                        dirs[m] = dirs[m+1];
                        dirs[m+1] = temp;
                    }
                }
            }

            for (k = 0; k < 4; k++) {
                int dirIndex = dirs[k];
                int nx = x + dx[dirIndex]; 
                int ny = y + dy[dirIndex];

                if (nx >= 0 && nx < genislik && ny >= 0 && ny < yukseklik && 
                    harita[ny][nx] == YOL && dist[ny][nx] == -1) {
                    
                    dist[ny][nx] = dist[y][x] + 1;
                    pX[ny][nx] = x; pY[ny][nx] = y;
                    
                    top++; 
                    stackX[top] = nx; stackY[top] = ny;

                    if (!(nx == startX && ny == startY) && !(nx == hedefX && ny == hedefY)) {
                        harita[ny][nx] = IZ; hucreyiGuncelle(nx, ny); Sleep(25); 
                    }
                    
                    yolBulundu = 1;
                    break; 
                }
            }

            if (yolBulundu == 0) {
                top--; 
            }
        }
    }

    if (bulundu) {
        int cX = hedefX, cY = hedefY, adim = 0;
        int nX, nY;
        while(cX != -1) {
            if (!(cX == startX && cY == startY) && !(cX == hedefX && cY == hedefY)) {
                harita[cY][cX] = COZUM; hucreyiGuncelle(cX, cY);
            }
            nX = pX[cY][cX]; nY = pY[cY][cX];
            cX = nX; cY = nY; 
            adim++; Sleep(30); 
        }
        gotoxy(0, yukseklik + 1); renk(10); 
        printf("%s: Yol bulundu! Uzunluk: %d adim         ", (aramaModu == 0 ? "BFS" : "DFS"), adim);
    } else {
        gotoxy(0, yukseklik + 1); renk(12); printf("HATA: Cizdiginiz yollar bitise ulasmiyor!         ");
    }
}

void haritaSifirla() {
    int i, j;
    for (i = 0; i < yukseklik; i++) {
        for (j = 0; j < genislik; j++) {
            if (i == 0 && j == 0) harita[i][j] = KOSE_SOL_UST;
            else if (i == 0 && j == genislik - 1) harita[i][j] = KOSE_SAG_UST;
            else if (i == yukseklik - 1 && j == 0) harita[i][j] = KOSE_SOL_ALT;
            else if (i == yukseklik - 1 && j == genislik - 1) harita[i][j] = KOSE_SAG_ALT;
            else if (i == 0 || i == yukseklik - 1) harita[i][j] = DUZ_YATAY;
            else if (j == 0 || j == genislik - 1) harita[i][j] = DUZ_DIKEY;
            else harita[i][j] = DUVAR_DOLGU; 
        }
    }
    harita[1][1] = YOL;
    harita[yukseklik-2][genislik-2] = YOL;
}

int main() {
    int tus, ek, eX, eY;
    
    srand(time(NULL));
    imleciKutuYap();

    system("cls"); renk(11);

    while(1) {
        printf("Genislik (30-70): "); 
        scanf("%d", &genislik);
        if (genislik >= 30 && genislik <= 70) break; 
        renk(12); 
        printf("HATA: Lutfen 30 ile 70 arasinda bir deger girin!\n");
        renk(11); 
    }

    while(1) {
        printf("Yukseklik (5-15): "); 
        scanf("%d", &yukseklik);
        if (yukseklik >= 5 && yukseklik <= 15) break; 
        renk(12); 
        printf("HATA: Lutfen 5 ile 15 arasinda bir deger girin!\n");
        renk(11); 
    }

    haritaSifirla(); tamEkranCiz();

    while (1) {
        gotoxy(0, yukseklik); 
        
        if (aktifMod == 1) { renk(12); printf(">> MOD: YOL ACIYORSUN (*)       "); }
        else if (aktifMod == 2) { renk(12); printf(">> MOD: DUVAR ORIYORSUN ( )      "); }
        else if (aktifMod == 0) { renk(15); printf(">> MOD: SERBEST GEZINIYORSUN     "); }
        
        renk(13);
        printf(" | ALGORITMA: %s    ", (aramaModu == 0 ? "BFS (En Kisa)" : "DFS (Dengeli)"));

        gotoxy(imX, imY); 
        tus = getch();
        eX = imX; eY = imY;

        if (tus == 13) { yoluBul(); continue; }

        if (tus == 224 || tus == 0) {
            ek = getch();
            if (ek == 72 && imY > 1) imY--;                
            else if (ek == 80 && imY < yukseklik - 2) imY++; 
            else if (ek == 75 && imX > 1) imX--;       
            else if (ek == 77 && imX < genislik - 2) imX++; 
            else if (ek == 59) aktifMod = 1; // F1
            else if (ek == 60) aktifMod = 2; // F2
            else if (ek == 61) aktifMod = 0; // F3
            else if (ek == 62) { haritaKaydet(); } // F4: Kaydet
            else if (ek == 63) { haritaYukle(); }  // F5: Yukle
            else if (ek == 65) { 
                imX = 1; imY = 1; 
                haritaSifirla(); 
                tamEkranCiz(); 
            } 
            else if (ek == 66) { aramaModu = (aramaModu == 0) ? 1 : 0; tamEkranCiz(); } // F8
            else if (ek == 67) { // F9
                imX = 1; imY = 1;
                rastgeleHaritaBaslat();
            }
            
            if (ek != 62 && ek != 63 && ek != 67 && ek != 65) {
                hucreyiGuncelle(eX, eY);
                if (aktifMod == 1) harita[imY][imX] = YOL;
                else if (aktifMod == 2) harita[imY][imX] = DUVAR_DOLGU;
                hucreyiGuncelle(imX, imY);
            }
        } else if (tus == 27) break;
    }
    return 0;
}
