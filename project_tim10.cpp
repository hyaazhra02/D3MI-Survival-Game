#include <iostream>   
#include <string>     
#include <iomanip>    
#include <cstdlib>    
#include <ctime>      
#include <cmath>      
#include <cctype>     

#ifdef _WIN32
    #include <windows.h>  
#else
    #include <unistd.h>    
#endif
using namespace std;

bool warnaAktif      = true;   
bool animasiAktif    = true;   
int  kecepatanKetik  = 8;      

string RESET, MERAH, HIJAU, KUNING, BIRU, UNGU, CYAN, PUTIH, ABU;

void tampilkanLogo() {
    cout << CYAN << R"ASCII(
         ____   _____    __  __   ___
        |  _ \ |___ /   |  \/  | |_ _|
        | | | |  |_ \   | |\/| |  | |
        | |_| | ___) |  | |  | |  | |
        |____/ |____/   |_|  |_| |___|
)ASCII" << RESET;

    cout << KUNING << R"ASCII(
       .------------------------------.
       |  .------------------------.  |
       |  | > mulai_kuliah();      |  |
       |  | > ipk  = 4.00;         |  |
       |  | > misi = "Sidang TA";  |  |
       |  '------------------------'  |
       '------------------------------'
     /__________________________________\
    /____________________________________\
)ASCII" << RESET;

    cout << PUTIH << "\n         STARTUP & ACADEMIC TYCOON" << RESET << endl;
    cout << HIJAU << "           Mission to Sidang TA" << RESET << endl;
}

void aturWarna(bool aktif) {
    if (aktif) {
        RESET  = "\033[0m";     // kembali ke warna normal
        MERAH  = "\033[1;31m";  // bahaya / buruk
        HIJAU  = "\033[1;32m";  // aman / bagus
        KUNING = "\033[1;33m";  // waspada
        BIRU   = "\033[1;34m";  // informasi sistem
        UNGU   = "\033[1;35m";  // nama tokoh saat dialog
        CYAN   = "\033[1;36m";  // judul & input
        PUTIH  = "\033[1;37m";  // penekanan
        ABU    = "\033[0;90m";  // teks redup (LED mati, petunjuk)
    } else {
        RESET = MERAH = HIJAU = KUNING = BIRU = UNGU = CYAN = PUTIH = ABU = "";
    }
}

void aktifkanANSIWindows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | 0x0004);  
    }
#endif
}

void jeda(int ms) {
    if (!animasiAktif) return;
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void bersihLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

const string KAMPUS  = "Universitas Lampung";
const string DOSEN   = "Pak Dwi";    
const string ASISTEN = "Kak Arafi & Kak Dimas";   
const string TEMAN_A = "Haya gemec";
const string TEMAN_B = "Renanda imup";
const string TEMAN_C = "Daffa gantenk";

string namaPemain = "Mahasiswa";
double ipk        = 4.00;   // awal 4.00, minimal kelulusan 2.00
int    energi     = 100;    // 0 - 100 (%)
int    stres      = 0;      // 0 - 100 (%), 100 = Game Over (burnout)
int    koin       = 0;      // didapat dari mini-game (Bagian 2)

// --- Progres game ---
int    semesterSekarang = 1;
bool   gameOver         = false;
string alasanGameOver   = "";

// --- Variabel tambahan untuk strategi ---
int kesiapanUjian = 0;   // "skor tersembunyi": makin tinggi, makin aman saat ujian
int sisaKopi      = 3;   // jatah Tombol Kopi per semester
int sisaIstirahat = 2;   // jatah tidur siang per semester

string namaProyek = "-"; // ide startup/proyek yang dipilih tim
int bebanProyek   = 1;   // 1 = mudah, 2 = sedang, 3 = sulit

bool punyaRelasi      = false; 
bool sudahBacaModul   = false;
bool ketinggalanInfo  = false; 
bool timSolid         = false; 
bool timPasif         = false;  
bool kerjaSolo        = false;  
bool skillDebug       = false;  
bool pernahContek     = false;  
bool sudahBackup      = false;  

// --- Inventaris Kantin (ARRAY). Dipakai penuh di Bagian 2. ---
const int JUMLAH_ITEM = 3;
string namaItem[JUMLAH_ITEM]  = {"Kopi Ekstra", "Modul Praktikum", "Nasi Kucing Kantin"};
int    hargaItem[JUMLAH_ITEM] = {15, 30, 10};   // harga dalam koin
int    stokItem[JUMLAH_ITEM]  = {0, 0, 0};      // jumlah yang dimiliki

// --- Tips yang muncul acak di layar loading (ARRAY) ---
string tipsLoading[4] = {
    "Tips: Tombol Kopi menambah energi, tapi jangan berlebihan!",
    "Tips: Keputusan di Semester 1 memengaruhi Semester 2 dan 3.",
    "Tips: Selalu backup proyekmu. Laptop bisa rusak kapan saja.",
    "Tips: LED merah berkedip = kondisi bahaya. Segera ambil tindakan!"
};


// Mencetak garis pemisah, contoh: garis('=', 50)
void garis(char simbol = '=', int panjang = 50) {
    cout << string(panjang, simbol) << endl;   // string(n, c) = c diulang n kali
}


void ketik(const string& teks, int ms) {
    for (size_t i = 0; i < teks.length(); i++) {
        cout << teks[i] << flush;
        jeda(ms);
    }
}

// Satu baris narasi cerita (dengan efek ketik).
void cerita(const string& teks) {
    cout << "  ";
    ketik(teks, kecepatanKetik);
    cout << endl;
}

// Satu baris dialog tokoh: nama berwarna ungu, lalu ucapannya.
void dialog(const string& pembicara, const string& teks) {
    cout << "  " << UNGU << pembicara << RESET << ": ";
    ketik(teks, kecepatanKetik);
    cout << endl;
}


void tekanEnter() {
    cout << ABU << "\n  [Tekan ENTER untuk lanjut]" << RESET;
    string sisa;
    if (!getline(cin, sisa)) exit(0);
}

int bacaPilihan(int minimal, int maksimal) {
    string masukan;
    while (true) {
        cout << CYAN << "\n  Pilihanmu [" << minimal << "-" << maksimal << "]: " << RESET;
        if (!getline(cin, masukan)) exit(0);
        if (masukan.length() == 1 && isdigit((unsigned char)masukan[0])) {
            int angka = masukan[0] - '0';   // ubah karakter '3' menjadi angka 3
            if (angka >= minimal && angka <= maksimal) return angka;
        }
        cout << MERAH << "  Input tidak valid, coba lagi." << RESET << endl;
    }
}


void loadingBar(const string& label, int langkah, int msPerLangkah) {
    for (int i = 0; i <= langkah; i++) {
        int persen = i * 100 / langkah;
        cout << "\r  " << label << " [" << HIJAU << string(i, '#') << RESET
             << string(langkah - i, '-') << "] " << setw(3) << persen << "%" << flush;
        jeda(msPerLangkah);
    }
    cout << endl;
}


string barTeks(int nilai, int panjang) {
    int isi = nilai * panjang / 100;
    return "[" + string(isi, '#') + string(panjang - isi, '-') + "]";
}

// Judul adegan: membersihkan layar lalu menampilkan kotak judul.
void judulAdegan(const string& kode, const string& judul) {
    bersihLayar();
    cout << CYAN;
    garis('=', 50);
    cout << "  " << kode << endl;
    cout << "  " << judul << endl;
    garis('=', 50);
    cout << RESET;
}

// Predikat berdasarkan IPK (juga akan dipakai lagi di Bagian 3).
string predikatIPK(double nilai) {
    if (nilai >= 3.50)      return "Sangat Memuaskan";
    else if (nilai >= 3.00) return "Memuaskan";
    else if (nilai >= 2.50) return "Cukup";
    else                    return "Perlu Perbaikan";
}

void buzzer(int kali) {
    cout << MERAH << "  [BUZZER] ";
    for (int i = 0; i < kali; i++) {
        cout << "BEEP! \a" << flush;
        jeda(250);
    }
    cout << RESET << endl;
}

int levelBahaya() {
    if (ipk < 2.50 || stres >= 80) return 2;                    // merah
    if (ipk < 3.00 || stres >= 50 || energi < 25) return 1;     // kuning
    return 0;                                                   // hijau
}

// Mencetak 3 lampu LED. Lampu yang menyala = lampu pada 'level'.
void cetakLED(int level, bool nyala) {
    string warna[3] = {HIJAU, KUNING, MERAH};
    for (int i = 0; i < 3; i++) {
        if (i == level && nyala) cout << warna[i] << "(O)" << RESET << " ";
        else                     cout << ABU << "( )" << RESET << " ";
    }
}


void tampilLED(bool kedip) {
    int level = levelBahaya();
    string label[3] = {"AMAN - IPK terkendali", "WASPADA", "BAHAYA!"};
    string warna[3] = {HIJAU, KUNING, MERAH};

    if (kedip && level == 2) {
        for (int i = 0; i < 6; i++) {
            cout << "\r  LED   : ";
            cetakLED(level, i % 2 == 0);          // genap = nyala, ganjil = mati
            cout << warna[level] << "BAHAYA!            " << RESET << flush;
            jeda(200);
        }
    }
    cout << "\r  LED   : ";
    cetakLED(level, true);
    cout << warna[level] << label[level] << RESET << "        " << endl;
}

// Panel status karakter (ditampilkan di awal setiap adegan).
void tampilStatus() {
    // Warna ditentukan oleh kondisi masing-masing statistik.
    string wIpk    = (ipk >= 3.00)  ? HIJAU : ((ipk >= 2.50)  ? KUNING : MERAH);
    string wEnergi = (energi >= 50) ? HIJAU : ((energi >= 25) ? KUNING : MERAH);
    string wStres  = (stres < 40)   ? HIJAU : ((stres < 70)   ? KUNING : MERAH);

    garis('=', 50);
    cout << "  " << PUTIH << namaPemain << RESET
         << "   |   Semester " << semesterSekarang << " dari 3" << endl;
    garis('-', 50);
    cout << "  IPK    : " << wIpk << ipk << RESET << "   (minimal lulus 2.00)" << endl;
    cout << "  Energi : " << wEnergi << barTeks(energi, 20) << " " << energi << "%" << RESET << endl;
    cout << "  Stres  : " << wStres  << barTeks(stres, 20)  << " " << stres  << "%" << RESET << endl;
    cout << "  Koin   : " << KUNING << koin << RESET << " koin" << endl;
    tampilLED(false);
    garis('=', 50);
}

void batasiNilai() {
    if (energi > 100) energi = 100;
    if (energi < 0)   energi = 0;
    if (stres > 100)  stres = 100;
    if (stres < 0)    stres = 0;
    if (ipk > 4.00)   ipk = 4.00;
    if (ipk < 0.00)   ipk = 0.00;
    ipk = floor(ipk * 100 + 0.5) / 100;   // bulatkan 2 desimal (hindari 1.9999999)
    if (koin < 0)     koin = 0;
}

// Memeriksa kejadian khusus setelah statistik berubah.
void periksaStatus() {
    // Energi habis -> tumbang
    if (energi <= 0 && !gameOver) {
        cout << MERAH << "\n  !! Energi habis! Kamu tumbang dan dibawa ke klinik kampus." << RESET << endl;
        buzzer(1);
        energi = 30;
        stres += 15;
        ipk -= 0.10;
        batasiNilai();
        cout << "  Dampak tumbang: Energi pulih ke 30%, Stres +15, IPK -0.10" << endl;
    }
    // Kondisi Game Over
    if (ipk < 2.00) {
        gameOver = true;
        alasanGameOver = "IPK-mu jatuh di bawah 2.00 (batas minimal kelulusan).";
    } else if (stres >= 100) {
        gameOver = true;
        alasanGameOver = "Stres mencapai 100%. Kamu mengalami BURNOUT.";
    }
}

bool cetakDampak(const string& nama, double nilai, bool naikItuBaik, bool pecahan) {
    if (nilai > -0.0001 && nilai < 0.0001) return false;   // tidak berubah
    bool baik = (nilai > 0) == naikItuBaik;
    cout << (baik ? HIJAU : MERAH) << nama << " ";
    if (nilai > 0) cout << "+";
    if (pecahan) cout << nilai;
    else         cout << (int)nilai;
    cout << RESET << "   ";
    return true;
}

void efek(int dEnergi, int dStres, double dIpk, int dKoin = 0, int dPersiapan = 0) {
    int    ledLama    = levelBahaya();
    int    energiLama = energi;
    int    stresLama  = stres;
    int    koinLama   = koin;
    double ipkLama    = ipk;

    energi        += dEnergi;
    stres         += dStres;
    ipk           += dIpk;
    koin          += dKoin;
    kesiapanUjian += dPersiapan;
    batasiNilai();

    // Tampilkan ringkasan dampak (nilai sebenarnya setelah dibatasi)
    cout << "\n  " << ABU << "Dampak:" << RESET << " ";
    bool adaDampak = false;
    adaDampak = cetakDampak("Energi",    energi - energiLama, true,  false) || adaDampak;
    adaDampak = cetakDampak("Stres",     stres - stresLama,   false, false) || adaDampak;
    adaDampak = cetakDampak("IPK",       ipk - ipkLama,       true,  true)  || adaDampak;
    adaDampak = cetakDampak("Koin",      koin - koinLama,     true,  false) || adaDampak;
    adaDampak = cetakDampak("Persiapan", dPersiapan,          true,  false) || adaDampak;
    if (!adaDampak) {
        cout << ABU << "tidak ada perubahan langsung (tapi efeknya bisa muncul nanti!)" << RESET;
    }
    cout << endl;

    periksaStatus();   // tumbang? game over?

    // Jika warna LED berubah, tampilkan animasi (+ buzzer jika jadi merah)
    int ledBaru = levelBahaya();
    if (ledBaru != ledLama) {
        cout << "\n  " << PUTIH << ">> Indikator LED berubah!" << RESET << endl;
        tampilLED(true);
        if (ledBaru == 2) buzzer(2);
    }
}

void tombolKopi() {
    cout << "\n  " << CYAN << "[IoT] Tombol Kopi (simulasi tombol fisik)" << RESET << endl;
    if (sisaKopi <= 0) {
        cout << MERAH << "  Mesin kopi kosong! Jatah kopi semester ini sudah habis." << RESET << endl;
        return;
    }
    cout << "  Jatah kopi tersisa : " << sisaKopi << endl;
    cout << "  Ketik K lalu ENTER untuk MENEKAN tombol (ENTER saja = batal): ";
    string masukan;
    if (!getline(cin, masukan)) exit(0);
    if (masukan != "K" && masukan != "k") {
        cout << "  Kamu mengurungkan niat minum kopi." << endl;
        return;
    }
    cout << KUNING << "  *KLIK!*" << RESET << endl;
    loadingBar("Menyeduh kopi", 15, 80);

    bool cangkirBiasa = (sisaKopi > 1);   // cangkir ke-3 = kafein berlebihan
    sisaKopi--;
    if (cangkirBiasa) {
        cout << "  Slurp... kamu merasa segar kembali!" << endl;
        efek(25, 2, 0.0);
    } else {
        cout << MERAH << "  Kafein berlebihan! Jantungmu berdebar kencang..." << RESET << endl;
        efek(10, 10, 0.0);
    }
}

// Tidur siang: memulihkan tenaga, tetapi mengurangi waktu belajar.
void istirahatSebentar() {
    if (sisaIstirahat <= 0) {
        cout << MERAH << "\n  Tidak ada waktu luang lagi semester ini!" << RESET << endl;
        return;
    }
    sisaIstirahat--;
    cout << "\n  Kamu merebahkan diri sebentar... ZzZzZ" << endl;
    loadingBar("Tidur siang", 15, 80);
    cout << "  Segar! Tapi waktu belajarmu berkurang." << endl;
    efek(20, -10, 0.0, 0, -1);
}

void kantinShop() {
    while (true) {
        judulAdegan("KANTIN KAMPUS", "Tukar koin dengan item penambah stamina & belajar");
        cout << "  Koin tersedia: " << KUNING << koin << " koin" << RESET << "\n\n";

        // Menampilkan daftar menu item dari array
        for (int i = 0; i < JUMLAH_ITEM; i++) {
            cout << "  [" << i + 1 << "] " << left << setw(20) << namaItem[i]
                 << " | Harga: " << KUNING << setw(2) << hargaItem[i] << " koin" << RESET
                 << " | Stok milikmu: " << HIJAU << stokItem[i] << RESET << endl;
        }
        cout << "  [4] Kembali ke Waktu Luang" << endl;

        int pilihan = bacaPilihan(1, 4);
        if (pilihan == 4) return;

        int indeks = pilihan - 1; // ubah pilihan 1-3 menjadi indeks array 0-2

        // Cek apakah koin cukup untuk membeli item
        if (koin >= hargaItem[indeks]) {
            koin -= hargaItem[indeks];
            stokItem[indeks]++;
            cout << HIJAU << "\n  Berhasil membeli 1 " << namaItem[indeks] << "!" << RESET << endl;
            cout << "  Sisa koin: " << KUNING << koin << RESET
                 << " | Total stok " << namaItem[indeks] << ": " << stokItem[indeks] << endl;
        } else {
            cout << MERAH << "\n  Koin tidak cukup! Butuh " << hargaItem[indeks]
                 << " koin (koinmu: " << koin << ")." << RESET << endl;
        }
        tekanEnter();
    }
}

void menuWaktuLuang(const string& keterangan) {
    while (true) {
        judulAdegan("WAKTU LUANG", keterangan);
        tampilStatus();
        cout << "\n  Apa yang ingin kamu lakukan?" << endl;
        cout << "  [1] Lanjut ke kegiatan berikutnya" << endl;
        cout << "  [2] Tekan Tombol Kopi        (jatah: " << sisaKopi << ")" << endl;
        cout << "  [3] Tidur siang              (jatah: " << sisaIstirahat << ")" << endl;
        cout << "  [4] Kantin Kampus (Shop)" << endl;

        int pilihan = bacaPilihan(1, 4);
        if (pilihan == 1) return;

        switch (pilihan) {
            case 2: tombolKopi();        break;
            case 3: istirahatSebentar(); break;
            case 4: kantinShop();        break;
        }
        if (gameOver) return;
        tekanEnter();
    }
}

void layarPembuka() {
    bersihLayar();
    tampilkanLogo();

    cout << "\n  " << BIRU << "[IoT] Menguji perangkat..." << RESET << endl;
    for (int i = 0; i < 3; i++) {          
        cout << "\r  LED   : ";
        cetakLED(i, true);
        cout << flush;
        jeda(350);
    }
    cout << HIJAU << " OK" << RESET << endl;
    buzzer(1);
    loadingBar("Memuat game", 25, 50);

    cout << "\n  " << ABU << tipsLoading[rand() % 4] << RESET << endl;
    tekanEnter();
}

void caraBermain() {
    judulAdegan("CARA BERMAIN", "Baca dulu sebelum mulai!");
    cout << "\n  " << PUTIH << "TUJUAN" << RESET << endl;
    cout << "  Bertahan 3 semester dengan IPK >= 2.00 dan Stres < 100%." << endl;
    cout << "\n  " << PUTIH << "STATISTIK" << RESET << endl;
    cout << "  IPK    : awal 4.00. Turun jika nilai buruk. < 2.00 = GAME OVER" << endl;
    cout << "  Energi : awal 100%. Habis dipakai tugas & begadang." << endl;
    cout << "           Jika 0% kamu tumbang (stres naik, IPK turun)." << endl;
    cout << "  Stres  : awal 0%. Jika mencapai 100% = BURNOUT (GAME OVER)" << endl;
    cout << "  Koin   : didapat dari mini-game, ditukar di Kantin (Bagian 2)" << endl;
    cout << "\n  " << PUTIH << "PERANGKAT IoT (SIMULASI)" << RESET << endl;
    cout << "  LED    : " << HIJAU << "Hijau" << RESET << " = aman | " << KUNING << "Kuning" << RESET
         << " = waspada | " << MERAH << "Merah berkedip" << RESET << " = bahaya" << endl;
    cout << "  Tombol Kopi : ketik K lalu ENTER untuk menambah energi" << endl;
    cout << "  Buzzer      : berbunyi saat bahaya, gagal, atau Game Over" << endl;
    cout << "\n  " << PUTIH << "TIPS" << RESET << endl;
    cout << "  Setiap keputusan punya konsekuensi di tahap berikutnya!" << endl;
    cout << "  Pilih dengan mengetik angka lalu tekan ENTER." << endl;
    tekanEnter();
}

void pengaturan() {
    while (true) {
        judulAdegan("PENGATURAN", "Sesuaikan tampilan terminalmu");
        cout << "\n  Contoh warna: " << MERAH << "MERAH " << HIJAU << "HIJAU "
             << KUNING << "KUNING" << RESET << endl;
        cout << ABU << "  (Jika muncul simbol aneh seperti [1;31m, matikan warna)" << RESET << endl;
        cout << "\n  [1] Warna ANSI : " << (warnaAktif ? "ON" : "OFF") << endl;
        cout << "  [2] Animasi    : " << (animasiAktif ? "ON" : "OFF") << endl;
        cout << "  [3] Kembali" << endl;

        int pilihan = bacaPilihan(1, 3);
        if (pilihan == 1) {
            warnaAktif = !warnaAktif;
            aturWarna(warnaAktif);
        } else if (pilihan == 2) {
            animasiAktif = !animasiAktif;
        } else {
            return;
        }
    }
}

void menuUtama() {
    while (true) {
        bersihLayar();
        cout << CYAN;
        garis('=', 50);
        cout << "   D3 MI STARTUP & ACADEMIC TYCOON" << endl;
        cout << "   Mission to Sidang TA" << endl;
        garis('=', 50);
        cout << RESET;
        cout << "\n  [1] Mulai Game" << endl;
        cout << "  [2] Cara Bermain" << endl;
        cout << "  [3] Pengaturan (warna & animasi)" << endl;
        cout << "  [4] Keluar" << endl;

        int pilihan = bacaPilihan(1, 4);
        switch (pilihan) {
            case 1: return;
            case 2: caraBermain(); break;
            case 3: pengaturan();  break;
            case 4:
                cout << "\n  Sampai jumpa, semoga sukses kuliahnya!" << endl;
                exit(0);
        }
    }
}

void inputNama() {
    bersihLayar();
    cout << CYAN;
    garis('=', 50);
    cout << "   PENDAFTARAN MAHASISWA BARU" << endl;
    garis('=', 50);
    cout << RESET;
    cout << "\n  Siapa namamu, calon Ahli Madya Komputer? " << endl;
    cout << "  Nama (maks. 15 huruf): ";
    string masukan;
    if (!getline(cin, masukan)) exit(0);
    if (masukan.length() > 15) masukan = masukan.substr(0, 15);
    if (!masukan.empty()) namaPemain = masukan;
}

void prolog() {
    judulAdegan("PROLOG", "Selamat Datang, Mahasiswa Baru!");
    cout << endl;
    cerita("Halo, " + namaPemain + "! Kamu baru diterima di D3 Manajemen");
    cerita("Informatika, " + KAMPUS + ".");
    cerita("Impianmu: lulus tepat waktu dan menjalani Sidang TA.");
    cerita("Jalannya panjang: tugas, praktikum, deadline, dan kopi.");
    cout << "\n  " << PUTIH << "MISI KAMU:" << RESET << endl;
    cout << "   - Jaga IPK minimal 2.00 sampai akhir Semester 3" << endl;
    cout << "   - Jaga Stres di bawah 100% (100% = Burnout)" << endl;
    cout << "   - Kelola Energi dan koin dengan cerdas" << endl;
    tekanEnter();
}


void catatKeputusan() {
    cout << "\n  Jejak keputusanmu (terbawa ke semester berikutnya):" << endl;
    if (punyaRelasi)      cout << HIJAU  << "   [+] Punya relasi pertemanan di kampus" << RESET << endl;
    if (sudahBacaModul)   cout << HIJAU  << "   [+] Disiplin membaca modul" << RESET << endl;
    if (timSolid)         cout << HIJAU  << "   [+] Tim yang kompak dan rajin" << RESET << endl;
    if (sudahBackup)      cout << HIJAU  << "   [+] Kebiasaan backup proyek" << RESET << endl;
    if (skillDebug)       cout << HIJAU  << "   [+] Skill debugging meningkat" << RESET << endl;
    if (kerjaSolo)        cout << KUNING << "   [~] Terbiasa bekerja sendirian" << RESET << endl;
    if (ketinggalanInfo)  cout << MERAH  << "   [-] Sering ketinggalan info kampus" << RESET << endl;
    if (timPasif)         cout << MERAH  << "   [-] Tim yang pasif" << RESET << endl;
    if (pernahContek)     cout << MERAH  << "   [-] Pernah menyalin kode (risiko integritas)" << RESET << endl;
}

// ---------------------------------------------------------------- FASE 1 ---
void faseOrientasi() {
    judulAdegan("SEMESTER 1 | FASE 1 dari 6", "Hari Pertama: Orientasi Mahasiswa Baru");
    tampilStatus();
    cout << endl;
    cerita("Pagi pertama di " + KAMPUS + ".");
    cerita("Gerbang kampus ramai oleh ratusan mahasiswa baru.");
    cerita("Panitia mengumumkan: sesi inti, sesi kenalan, dan sesi sore.");
    dialog(DOSEN, "Ciee masuk UNILA. Selamat bersenang-senang nyak semester 1. PRENGG ");
    dialog(DOSEN, "Semester akhir nanti kita buat proyek C++ ya. Tim nya 4 orang. jangan ganjil, nanti yang genapin setan");

    cout << "\n  Bagaimana caramu menjalani orientasi?" << endl;
    cout << "  [1] Ikut SEMUA sesi & aktif berkenalan   (seru, tapi capek)" << endl;
    cout << "  [2] Ikut sesi inti, lalu baca modul DDP (hemat tenaga)" << endl;
    cout << "  [3] Izin pulang cepat & istirahat penuh   (aman...?)" << endl;

    int pilihan = bacaPilihan(1, 3);
    cout << endl;
    switch (pilihan) {
        case 1:
            cerita("Kamu berkenalan dengan " + TEMAN_A + ", " + TEMAN_B + ", dan " + TEMAN_C +" (sekelas denganmu).");
            cerita("Kontak WhatsApp bertambah, tapi kakimu pegal luar biasa.");
            punyaRelasi = true;
            efek(-15, 5, 0.0);

            break;
        case 2:
            cerita("Setelah sesi inti, kamu pulang dan membaca modul DDP.");
            cerita("Kamu jadi paham alur materi: variabel, percabangan, perulangan.");
            sudahBacaModul = true;
            efek(-5, 0, 0.0, 0, 1);
            break;
        case 3:
            cerita("Kamu pulang cepat dan tidur nyenyak. Energimu utuh!");
            cerita("Sayang, jadwal praktikum diumumkan di sesi sore tadi...");
            ketinggalanInfo = true;
            efek(0, 0, 0.0);
            break;
    }
    if (gameOver) return;
    tekanEnter();
    menuWaktuLuang("Orientasi hari pertama selesai");
}

// ---------------------------------------------------------------- FASE 2 ---
void faseTim() {
    judulAdegan("SEMESTER 1 | FASE 2 dari 6", "Membentuk Tim Proyek DDP");
    tampilStatus();
    cout << endl;
    cerita("Hari ke-5. " + DOSEN + " membagikan tugas besar semester:");
    cerita("bentuk TIM 4 ORANG dan bangun 'startup mini' berupa program C++.");
    dialog(DOSEN, "Pilih temen yg bisa diajak kerja sama, Jangan yang numpang nama ajaa.");

    cout << "\n  Bagaimana kamu membentuk tim?" << endl;
    cout << "  [1] Ajak " << TEMAN_A << " , " << TEMAN_B << ", dan " << TEMAN_C << " (kenalan dari orientasi)";
    if (!punyaRelasi) cout << MERAH << " [TERKUNCI]" << RESET;

    cout << endl;
    cout << "  [2] Terima tim acak dari dosen         (untung-untungan)" << endl;
    cout << "  [3] Kerjakan sendiri, tak mau bergantung" << endl;

    int pilihan;
    while (true) {
        pilihan = bacaPilihan(1, 3);
        if (pilihan == 1 && !punyaRelasi) {
            cout << MERAH << "  Kamu belum kenal siapa pun! Pilih opsi lain." << RESET << endl;
            continue;
        }
        break;
    }

    cout << endl;
    switch (pilihan) {
        case 1:
            cerita(TEMAN_A + " jago koding, " + TEMAN_C + " jago MTK, " + TEMAN_B + " emm y bgs . Hebatt!");
            timSolid = true;
            efek(-5, -5, 0.0, 0, 1);
            break;
        case 2:
            if (rand() % 2 == 0) {      // 50% beruntung, 50% apes
                cerita("Timmu ternyata rajin dan komunikatif. Beruntung!");
                timSolid = true;
                efek(-5, 2, 0.0);
            } else {
                cerita("Tiga anggotamu sulit dihubungi dan sering menghilang...");
                timPasif = true;
                efek(-10, 12, 0.0);
            }
            break;
        case 3:
            dialog(DOSEN, "serius sendiri aja? bukan kelompok dong.");
            cerita("Kamu menerima tantangan itu dengan percaya diri.");
            kerjaSolo = true;
            efek(-20, 10, 0.0, 0, 2);
            break;
    }
    if (gameOver) return;
    tekanEnter();

    // ---- Keputusan lanjutan: memilih ide proyek / startup ----
    judulAdegan("SEMESTER 1 | FASE 2 (lanjutan)", "Memilih Ide Startup");
    tampilStatus();
    cout << endl;
    cerita("Sekarang pilih IDE STARTUP proyekmu. Makin sulit, makin banyak");
    cerita("ilmu yang didapat, tapi makin besar tenaga dan stres terkuras.");
    cout << "\n  [1] Kalkulator IPK Mini      (mudah, ilmu secukupnya)" << endl;
    cout << "  [2] Kasir Kantin Digital     (sedang, seimbang)" << endl;
    cout << "  [3] Smart Kos IoT Simulator  (sulit, ilmu maksimal)" << endl;

    pilihan = bacaPilihan(1, 3);
    cout << endl;
    switch (pilihan) {
        case 1:
            namaProyek = "Kalkulator IPK Mini";
            bebanProyek = 1;
            cerita("Aman dan sederhana. Tidak ada yang terlalu dipusingkan.");
            efek(0, -2, 0.0);
            break;
        case 2:
            namaProyek = "Kasir Kantin Digital";
            bebanProyek = 2;
            cerita("Ide yang realistis dan bisa dipakai kantin kampus!");
            efek(-5, 3, 0.0, 0, 1);
            break;
        case 3:
            namaProyek = "Smart Kos IoT Simulator";
            bebanProyek = 3;
            cerita("Ide ambisius! Lampu, sensor, dan buzzer virtual.");
            efek(-10, 8, 0.0, 0, 2);
            break;
    }
    if (gameOver) return;
    tekanEnter();
    menuWaktuLuang("Tim & ide proyek sudah ditentukan");
}

void tampilKodeError() {
    cout << ABU;
    cout << "   1| #include <iostream>" << endl;
    cout << "   2| using namespace std;" << endl;
    cout << "   3| int main() {" << endl;
    cout << "   4|     cout << \"Halo Dunia\"" << endl;
    cout << "   5|     return 0;" << endl;
    cout << "   6| }" << endl;
    cout << RESET;
    cout << MERAH << "   error: expected ';' before 'return'" << RESET << endl;
}

void fasePraktikum() {
    judulAdegan("SEMESTER 1 | FASE 3 dari 6", "Praktikum Pertama: Error Misterius");
    tampilStatus();
    cout << endl;

    // Konsekuensi berantai dari keputusan di Fase 1
    if (ketinggalanInfo) {
        cerita("Kamu salah ruangan dan telat 20 menit karena jadwal terlewat!");
        dialog(ASISTEN, "Kok telat? nilai nya dikurangin ya. saya lagi betmut.");
        efek(-5, 8, -0.10);
        if (gameOver) return;
        tekanEnter();
        judulAdegan("SEMESTER 1 | FASE 3 dari 6", "Praktikum Pertama: Error Misterius");
        tampilStatus();
        cout << endl;
    }

    cerita("Soal praktikum: tampilkan 'Halo Dunia' ke layar.");
    cerita("Mudah, kan? Tapi kodemu menolak berjalan...");
    cout << endl;
    tampilKodeError();

    cout << "\n  Apa yang kamu lakukan?" << endl;
    cout << "  [1] Debug sendiri, baca pesan error pelan-pelan" << endl;
    cout << "  [2] Bertanya kepada " << ASISTEN << " (asisten lab)" << endl;
    cout << "  [3] Salin kode teman di sebelah (cepat!)" << endl;

    int pilihan = bacaPilihan(1, 3);
    cout << endl;
    switch (pilihan) {
        case 1:
            cerita("Kamu membaca error baris demi baris... Ketemu! Titik koma!");
            cerita("Rasanya puas sekali menemukan bug-nya sendiri.");
            skillDebug = true;
            efek(-10, 6, 0.0, 0, 2);
            break;
        case 2:
            if (ketinggalanInfo) {
                dialog(ASISTEN, "Hmm... makanya jangan telat. Lihat baris 4 tuh.");
                efek(-3, 2, 0.0, 0, 1);
            } else {
                dialog(ASISTEN, "Coba lihat baris 4. Apa coba yang salah?.");
                efek(-3, -3, 0.0, 0, 1);
            }
            break;
        case 3:
            cerita("Selesai dalam semenit! Tapi hatimu tidak tenang...");
            cerita("Kamu tidak benar-benar paham apa yang salah.");
            pernahContek = true;
            efek(0, 3, 0.0, 0, -1);
            break;
    }
    if (gameOver) return;
    tekanEnter();
    menuWaktuLuang("Praktikum pertama selesai");
}

// ---------------------------------------------------------------- FASE 4 ---
void faseProyek() {
    judulAdegan("SEMESTER 1 | FASE 4 dari 6", "Tugas Besar: Perang Melawan Deadline");
    tampilStatus();
    cout << endl;
    cerita("Deadline proyek '" + namaProyek + "' tinggal 5 hari lagi.");
    if (timSolid)       cerita("Timmu solid: tugas terbagi rata dan grup chat ramai.");
    else if (timPasif)  cerita("Timmu pasif: grup chat sepi, tugas menumpuk di kamu.");
    else if (kerjaSolo) cerita("Kamu sendirian: semua modul harus kamu tulis sendiri.");

    cout << "\n  Bagaimana strategimu mengerjakan proyek?" << endl;
    cout << "  [1] Begadang kebut sampai pagi    (boros energi)" << endl;
    cout << "  [2] Cicil rapi + Git/Drive        (rapi, ada versioning)" << endl;
    cout << "  [3] Tunda dulu, kebut H-1         (santai... untuk sekarang)" << endl;

    int pilihan = bacaPilihan(1, 3);
    cout << endl;

    // Proyek yang lebih sulit membutuhkan tenaga tambahan
    int bebanTambahan = (bebanProyek - 1) * 5;

    switch (pilihan) {
        case 1: {
            int dE = -35, dS = 12;
            if (timSolid)  { dE += 15; dS -= 4; }   // kerja dibagi
            if (kerjaSolo) { dE -= 5;  dS += 3; }   // semua sendirian
            if (timPasif)  { dS += 3; }
            dE -= bebanTambahan;
            cerita("Kamu begadang ditemani lagu dan mie instan sampai subuh.");
            cerita("Kode jadi... tapi filenya hanya ada di satu laptop.");
            efek(dE, dS, 0.0, 0, 2);
            break;
        }
        case 2: {
            int dE = -12 - (bebanTambahan * 3 / 5), dS = 4;
            double dI = 0.0;
            if (kerjaSolo) dS += 2;
            if (timPasif) {
                dS += 6;
                dI = -0.05;
                cerita("Anggota tim pasif, jadi ada fitur yang kurang lengkap.");
            }
            cerita("Kamu menulis kode sedikit demi sedikit dan rutin commit ke Git.");
            cerita("Semua file juga tersimpan aman di cloud.");
            sudahBackup = true;
            efek(dE, dS, dI, 0, 2);
            break;
        }
        case 3:
            cerita("Kamu menunda... lalu panik di H-1 dan menulis kode terburu-buru.");
            cerita("Hasilnya jalan, tapi banyak bagian yang asal jadi.");
            efek(-25 - bebanTambahan, 14, -0.10);
            break;
    }
    if (gameOver) return;
    tekanEnter();
    menuWaktuLuang("Proyek hampir rampung. Dua hari menuju pengumpulan");
}

// ---------------------------------------------------------------- FASE 5 ---
void faseInsidenLaptop() {
    judulAdegan("SEMESTER 1 | FASE 5 dari 6", "Insiden: Layar Biru Kematian!");
    tampilStatus();
    cout << endl;
    cerita("H-2 pengumpulan. Kamu menyalakan laptop... dan muncul layar biru!");
    cerita("BLUE SCREEN OF DEATH. File proyek tidak bisa dibuka.");
    cerita("Inilah pentingnya manajemen risiko dalam dunia IT.");

    bool adaTeman = (punyaRelasi || timSolid);

    cout << "\n  Apa yang kamu lakukan?" << endl;
    cout << "  [1] Restore dari backup Git/Drive";
    if (!sudahBackup) cout << MERAH << " [TERKUNCI: tidak punya backup]" << RESET;
    cout << endl;
    cout << "  [2] Tulis ulang dari nol semalaman" << endl;
    cout << "  [3] Pinjam laptop teman";
    if (!adaTeman) cout << MERAH << " [TERKUNCI: tidak ada teman dekat]" << RESET;
    cout << endl;

    int pilihan;
    while (true) {
        pilihan = bacaPilihan(1, 3);
        if (pilihan == 1 && !sudahBackup) {
            cout << MERAH << "  Kamu tidak punya backup! Pilih opsi lain." << RESET << endl;
            continue;
        }
        if (pilihan == 3 && !adaTeman) {
            cout << MERAH << "  Kamu tidak punya teman yang bisa dipinjami laptop." << RESET << endl;
            continue;
        }
        break;
    }

    cout << endl;
    switch (pilihan) {
        case 1:
            cerita("Kamu clone ulang repositori. Semua file kembali dalam 5 menit!");
            cerita("Kebiasaan backup-mu menyelamatkan semester ini.");
            efek(-3, -5, 0.0, 0, 1);
            break;
        case 2:
            cerita("Kamu mengetik ulang semuanya sampai pagi. Mata panda parah.");
            if (sudahBackup) cerita("(Padahal kamu punya backup... ya sudahlah.)");
            efek(-30, 15, -0.05, 0, 1);
            break;
        case 3:
            if (timSolid) {
                cerita("Rekan timmu meminjamkan laptop dan ikut membantu mengetik.");
                efek(-8, 2, 0.0);
            } else {
                cerita("Temanmu meminjamkan laptopnya, tapi kamu harus kerja ekstra.");
                efek(-12, 6, 0.0);
            }
            break;
    }
    if (gameOver) return;
    tekanEnter();
    menuWaktuLuang("Malam sebelum UAS. Siapkan dirimu!");
}

// ---------------------------------------------------------------- FASE 6 ---
void faseUAS() {
    judulAdegan("SEMESTER 1 | FASE 6 dari 6", "Ujian Akhir Semester (UAS)");
    tampilStatus();
    cout << endl;
    cerita("Hari yang ditakuti tiba: UAS Dasar-Dasar Pemrograman.");
    cerita("Soalnya: menelusuri kode, mencari bug, dan membuat program kecil.");
    cout << endl;
    loadingBar("Mengerjakan soal ujian", 25, 90);

    // Penalti IPK ditentukan oleh skor persiapan (hasil keputusan sebelumnya)
    double penalti;
    if (kesiapanUjian >= 6)      penalti = 0.05;
    else if (kesiapanUjian >= 4) penalti = 0.20;
    else if (kesiapanUjian >= 2) penalti = 0.40;
    else                         penalti = 0.70;

    int tambahanStres = 0;
    cout << "\n  Skor persiapan ujianmu: " << CYAN << kesiapanUjian << RESET
         << ABU << "  (6 ke atas = sangat siap)" << RESET << endl;

    // Kondisi fisik & mental saat ujian
    if (energi < 30) {
        cerita("Matamu berat sekali... kamu sempat blank di soal nomor 3.");
        penalti += 0.10;
    }
    if (stres >= 70) {
        cerita("Tanganmu gemetar karena stres. Banyak salah ketik!");
        penalti += 0.10;
        tambahanStres += 5;
    }
    // Konsekuensi dari menyalin kode di Fase 3 (peluang 40% ketahuan)
    if (pernahContek) {
        if (rand() % 100 < 40) {
            dialog(DOSEN, "Kok sama persis sama sebelahnya? AI ya? hayoo. atau nyontek temen?.");
            dialog(DOSEN, "Karena nyontek nilai nya saya kurangi!.");
            penalti += 0.30;
            tambahanStres += 15;
            buzzer(2);
        } else {
            cerita("Kamu lolos dari kecurigaan, tapi soal logika terasa asing...");
        }
    }

    efek(-10, tambahanStres, -penalti);
    if (gameOver) return;

    cout << "\n" << CYAN << "  ";
    garis('=', 44);
    cout << "   HASIL SEMESTER 1" << endl;
    cout << "  ";
    garis('=', 44);
    cout << RESET;
    cout << "  IPK sementara : " << ipk << "  (" << predikatIPK(ipk) << ")" << endl;
    catatKeputusan();

    if (penalti >= 0.40) {
        buzzer(1);
        cerita("Nilai semester ini kurang memuaskan. Perbaiki di semester depan!");
    } else {
        cerita("Semester pertama berhasil dilalui. Kerja bagus!");
    }
    tekanEnter();
}

// Rangkaian lengkap Semester 1. Setelah tiap fase, cek apakah sudah Game Over.
void semester1() {
    faseOrientasi();       if (gameOver) return;
    faseTim();             if (gameOver) return;
    fasePraktikum();       if (gameOver) return;
    faseProyek();          if (gameOver) return;
    faseInsidenLaptop();   if (gameOver) return;
    faseUAS();
}

void resetJatahSemester() {
    sisaKopi      = 3;
    sisaIstirahat = 2;
    kesiapanUjian = 0;
}

// Libur antar-semester: energi dan stres pulih sebagian.
void liburSemester() {
    judulAdegan("LIBUR SEMESTER", "Waktunya mengisi ulang tenaga!");
    cout << endl;
    loadingBar("Recharge energi", 20, 60);
    efek(50, -20, 0.0);
    tekanEnter();
}

// Layar Game Over versi sederhana (ASCII Art lengkap menyusul di Bagian 3).
void layarGameOver() {
    bersihLayar();
    cout << MERAH;
    garis('=', 50);
    cout << "                 G A M E   O V E R" << endl;
    garis('=', 50);
    cout << RESET;
    cout << "\n  " << alasanGameOver << endl;
    cout << endl;
    buzzer(4);
    tampilLED(true);   // LED merah berkedip
    cout << "\n  Perjalananmu berhenti di Semester " << semesterSekarang << "." << endl;
    cout << "  IPK akhir: " << ipk << "   Stres akhir: " << stres << "%" << endl;
    cout << "\n  Terima kasih sudah bermain, " << namaPemain << "!" << endl;
    cout << "  Jalankan ulang program untuk mencoba strategi lain." << endl;
}

void semester2() {                       // diganti di BAGIAN 2
    cout << "\n  " << KUNING << "[Semester 2 akan hadir di Bagian 2]" << RESET << endl;
    jeda(900);
}
void semester3() {                       // diganti di BAGIAN 2
    cout << "\n  " << KUNING << "[Semester 3 akan hadir di Bagian 2]" << RESET << endl;
    jeda(900);
}
void layarMenang() {                     // diganti di BAGIAN 3
    bersihLayar();
    cout << HIJAU;
    garis('=', 50);
    cout << "   DEMO BAGIAN 1 SELESAI" << endl;
    garis('=', 50);
    cout << RESET;
    cout << "\n  Kamu berhasil melewati Semester 1, " << namaPemain << "!" << endl;
    cout << "  Layar kelulusan & ASCII Art ending ada di Bagian 3." << endl;
    cout << "\n  IPK saat ini: " << ipk << "  (" << predikatIPK(ipk) << ")" << endl;
}
// --------------------------------------------------------------------------

int main() {
    srand((unsigned)time(0));          // bibit angka acak (supaya tiap main beda)
    aktifkanANSIWindows();
    aturWarna(true);
    cout << fixed << setprecision(2);  // angka desimal selalu 2 digit (IPK 3.50)

    layarPembuka();
    menuUtama();
    inputNama();
    prolog();

    // Siklus semester: berulang selama belum tamat dan belum Game Over.
    while (semesterSekarang <= 3 && !gameOver) {
        resetJatahSemester();

        switch (semesterSekarang) {
            case 1: semester1(); break;
            case 2: semester2(); break;
            case 3: semester3(); break;
        }

        if (!gameOver) {
            if (semesterSekarang < 3) liburSemester();
            semesterSekarang++;
        }
    }

    if (gameOver) layarGameOver();
    else          layarMenang();

    return 0;
}
