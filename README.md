# Sistem Rekap Kehadiran Pekerja

## Deskripsi Sistem

Sistem Rekap Kehadiran Pekerja adalah program berbasis C++ yang dirancang untuk membantu manajer atau departemen HRD dalam mengelola data kehadiran karyawan secara efisien. Program ini mengimplementasikan struktur data menggunakan `struct` dan `vector` untuk menyimpan informasi pekerja dan rekam kehadiran mereka, serta menerapkan algoritma sorting dan searching untuk analisis data kehadiran.

### Fitur Utama:
- **Manajemen Data Pekerja**: Menambahkan data pekerja baru dengan ID dan nama
- **Input Kehadiran Harian**: Mencatat jam masuk dan jam pulang karyawan setiap hari
- **Perhitungan Keterlambatan Otomatis**: Menghitung keterlambatan berdasarkan jam standar masuk (08:00)
- **Laporan Rekapitulasi**: Menampilkan total kehadiran dan total keterlambatan per pekerja
- **Pencarian Data**: Mencari pekerja berdasarkan nama dengan detail kehadiran lengkap
- **Pengurutan Data**: Mengurutkan pekerja berdasarkan total keterlambatan menggunakan algoritma Selection Sort
- **Export ke CSV**: Mengekspor laporan kehadiran ke file CSV untuk analisis lebih lanjut
- **Validasi Input Komprehensif**: Validasi format ID, tanggal, waktu, dan logika bisnis

### Algoritma yang Digunakan:
- **Selection Sort**: Untuk mengurutkan pekerja berdasarkan total keterlambatan (descending)
- **Linear Search**: Untuk mencari data pekerja berdasarkan nama

## Cara Menjalankan Program

### Prasyarat
- Compiler C++ (g++, MinGW, atau Visual Studio)
- C++ Standard 11 atau lebih tinggi

### Kompilasi dan Eksekusi

**Windows (PowerShell/CMD):**
```bash
g++ main.cpp -o main
.\main.exe
```

**Linux/Mac:**
```bash
g++ main.cpp -o main
./main
```

### Menu Program
Setelah program dijalankan, Anda akan melihat menu dengan 7 pilihan:
1. **Tambah Pekerja** - Menambahkan data pekerja baru ke sistem
2. **Input Kehadiran Harian** - Mencatat kehadiran untuk semua pekerja pada tanggal tertentu
3. **Tampilkan Laporan Rekapitulasi** - Menampilkan ringkasan kehadiran semua pekerja
4. **Tampilkan Urutan Pekerja Terlambat** - Menampilkan pekerja yang diurutkan berdasarkan keterlambatan
5. **Cari Pekerja** - Mencari data pekerja berdasarkan nama
6. **Export Laporan ke CSV** - Menyimpan laporan ke file CSV
7. **Keluar** - Keluar dari program

### Validasi Input
Program dilengkapi dengan validasi input yang ketat untuk memastikan data yang valid:

#### Validasi ID Pekerja
- Format wajib: `IDXXXX` (contoh: ID0001, ID0002, ID0123)
- Harus dimulai dengan "ID" diikuti 4 digit angka
- Tidak boleh ada duplikasi ID
- Contoh valid: `ID0001`, `ID0010`, `ID9999`
- Contoh invalid: `id0001`, `ID01`, `ID12345`, `0001`

#### Validasi Tanggal
- Format wajib: `DD-MM-YYYY` (contoh: 25-11-2025)
- Validasi jumlah hari per bulan (tidak bisa input tanggal 32, 33, dll)
- Mendukung perhitungan tahun kabisat untuk Februari
- Range tahun: 1900-2100
- Contoh valid: `25-11-2025`, `29-02-2024` (tahun kabisat)
- Contoh invalid: `32-01-2025`, `29-02-2025` (bukan tahun kabisat), `31-04-2025`

#### Validasi Waktu
- Format wajib: `HH:MM` (contoh: 08:30, 17:45)
- Jam: 00-23, Menit: 00-59
- Jam pulang harus lebih besar dari jam masuk
- Contoh valid: Masuk `08:30`, Pulang `17:00`
- Contoh invalid: Masuk `25:00`, Pulang `08:70`, atau Pulang lebih awal dari masuk

## Penjelasan Fungsi

### 1. `inputDataPekerja(std::vector<Pekerja>& daftarPekerja)`
Menambahkan data pekerja baru ke dalam sistem. Fungsi ini menerima parameter referensi ke vector pekerja, meminta input ID dan nama pekerja menggunakan `getline`, menginisialisasi total keterlambatan dengan nilai 0, kemudian menambahkan pekerja baru ke dalam vector menggunakan `push_back`. Fungsi dilengkapi dengan validasi format ID (IDXXXX) menggunakan helper function `isValidIDFormat()` dan pengecekan duplikasi ID menggunakan `isIDDuplicate()`. Loop validasi akan terus meminta input ulang hingga format ID benar dan tidak duplikat.

### 2. `inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk)`
Mencatat kehadiran harian untuk semua pekerja pada tanggal tertentu. Fungsi ini meminta input tanggal sekali dengan validasi menggunakan `isValidDate()` yang memeriksa format DD-MM-YYYY, jumlah hari per bulan, dan tahun kabisat. Kemudian melakukan iterasi untuk setiap pekerja dan meminta input jam masuk dan jam pulang dengan validasi menggunakan `isValidTime()` yang memeriksa format HH:MM dan range jam (0-23) serta menit (0-59). Fungsi juga memvalidasi bahwa jam pulang harus lebih besar dari jam masuk. Keterlambatan dihitung dengan rumus `max(0, jamMasuk - jamStandar)` menggunakan method `toMinutes()` untuk konversi ke total menit sejak tengah malam. Semua variabel diinisialisasi di luar loop untuk performa optimal.

### 3. `tampilkanLaporan(const std::vector<Pekerja>& daftarPekerja)`
Menampilkan laporan rekapitulasi kehadiran dalam bentuk tabel. Fungsi ini menerima parameter const reference untuk mencegah modifikasi data. Menggunakan manipulator `iomanip` seperti `setw` dan `left` untuk formatting tabel dengan kolom ID, Nama, Total Hadir, dan Total Keterlambatan. Total hari hadir dihitung dari ukuran vector `rekamKehadiran`.

### 4. `cariDataPekerja(const std::vector<Pekerja>& daftarPekerja)`
Mencari pekerja berdasarkan nama menggunakan algoritma Linear Search. Fungsi ini melakukan iterasi dengan loop `for` dari indeks 0 hingga ukuran vector, membandingkan nama input dengan nama setiap pekerja. Jika ditemukan, menampilkan informasi lengkap termasuk index posisi dalam array, ID, nama, total hari hadir, total keterlambatan, dan rincian kehadiran harian dalam bentuk tabel. Kompleksitas waktu: O(n).

### 5. `tampilkanUrutanTerlambat(std::vector<Pekerja> daftarPekerja)`
Menampilkan urutan pekerja berdasarkan total keterlambatan dari tertinggi ke terendah. Fungsi ini mengimplementasikan algoritma Selection Sort dengan nested loop. Loop luar mengiterasi dari indeks 0 hingga n-1, mencari elemen dengan keterlambatan maksimum dari sisa elemen yang belum terurut, kemudian melakukan swap. Fungsi menerima copy dari vector (bukan reference) agar tidak mengubah urutan data asli. Hanya menampilkan pekerja dengan keterlambatan > 0 menit. Kompleksitas waktu: O(n²).

### 6. `exportLaporanCSV(const std::vector<Pekerja>& daftarPekerja)`
Mengekspor laporan rekapitulasi kehadiran ke file CSV (Comma-Separated Values). Fungsi ini menerima const reference vector pekerja, meminta input nama file dari user, kemudian menggunakan `ofstream` untuk membuat dan menulis data ke file. File CSV berisi header (ID, Nama, Total Hadir, Total Keterlambatan) dan data setiap pekerja dalam format yang dapat dibuka dengan Excel atau aplikasi spreadsheet lainnya. File disimpan di direktori yang sama dengan program executable.

### Fungsi Helper:
- **`isValidIDFormat(const std::string& id)`**: Validasi format ID harus IDXXXX dengan 2 huruf "ID" dan 4 digit angka
- **`isIDDuplicate(const std::vector<Pekerja>& daftarPekerja, const std::string& id)`**: Cek apakah ID sudah terdaftar
- **`isLeapYear(int year)`**: Menentukan apakah tahun adalah tahun kabisat
- **`getDaysInMonth(int month, int year)`**: Mendapatkan jumlah hari dalam bulan tertentu dengan mempertimbangkan tahun kabisat
- **`isValidDate(const std::string& dateStr)`**: Validasi format tanggal DD-MM-YYYY dan memastikan tanggal valid
- **`isValidTime(const std::string& timeStr, Waktu& outTime)`**: Validasi format waktu HH:MM dan range nilai yang valid
- **`formatWaktu(const Waktu& t)`**: Mengkonversi struct Waktu menjadi string format HH:MM dengan padding zero
- **`toMinutes()`**: Method dalam struct Waktu yang mengkonversi jam dan menit menjadi total menit sejak tengah malam

## Struktur Data

### Struct Waktu
```cpp
struct Waktu {
    int jam;
    int menit;
    int toMinutes() const;  // Konversi ke total menit
};
```

### Struct Kehadiran
```cpp
struct Kehadiran {
    std::string tanggal;
    Waktu jamMasuk;
    Waktu jamPulang;
    int menitKeterlambatan;
};
```

### Struct Pekerja
```cpp
struct Pekerja {
    std::string id;
    std::string nama;
    std::vector<Kehadiran> rekamKehadiran;
    int totalMenitKeterlambatan;
};
```

## Dibuat Oleh

- **Haidar Tafazul Fikri** (24051130004)
- **Glenferdinza Aghis Asyadda Rayndika Efian** (24051130005)

## Lisensi

Proyek ini dilisensikan di bawah [MIT License](LICENSE).
