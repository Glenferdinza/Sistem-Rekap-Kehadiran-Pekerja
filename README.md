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
g++ main.cpp -o attendance
.\attendance.exe
```

**Linux/Mac:**
```bash
g++ main.cpp -o attendance
./attendance
```

## Penjelasan Fungsi

### 1. `inputDataPekerja(std::vector<Pekerja>& daftarPekerja)`
Menambahkan data pekerja baru ke dalam sistem. Fungsi ini menerima parameter referensi ke vector pekerja, meminta input ID dan nama pekerja menggunakan `getline`, menginisialisasi total keterlambatan dengan nilai 0, kemudian menambahkan pekerja baru ke dalam vector menggunakan `push_back`.

### 2. `inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk)`
Mencatat kehadiran harian untuk semua pekerja pada tanggal tertentu. Fungsi ini meminta input tanggal sekali, kemudian melakukan iterasi untuk setiap pekerja dan meminta input jam masuk dan jam pulang. Fungsi helper `parseTime` digunakan untuk mengkonversi string format HH:MM menjadi struct Waktu. Keterlambatan dihitung dengan rumus `max(0, jamMasuk - jamStandar)` menggunakan method `toMinutes()` untuk konversi ke total menit sejak tengah malam.

### 3. `tampilkanLaporan(const std::vector<Pekerja>& daftarPekerja)`
Menampilkan laporan rekapitulasi kehadiran dalam bentuk tabel. Fungsi ini menerima parameter const reference untuk mencegah modifikasi data. Menggunakan manipulator `iomanip` seperti `setw` dan `left` untuk formatting tabel dengan kolom ID, Nama, Total Hadir, dan Total Keterlambatan. Total hari hadir dihitung dari ukuran vector `rekamKehadiran`.

### 4. `cariDataPekerja(const std::vector<Pekerja>& daftarPekerja)`
Mencari pekerja berdasarkan nama menggunakan algoritma Linear Search. Fungsi ini melakukan iterasi dengan loop `for` dari indeks 0 hingga ukuran vector, membandingkan nama input dengan nama setiap pekerja. Jika ditemukan, menampilkan informasi lengkap termasuk index posisi dalam array, ID, nama, total hari hadir, total keterlambatan, dan rincian kehadiran harian dalam bentuk tabel. Kompleksitas waktu: O(n).

### 5. `tampilkanUrutanTerlambat(std::vector<Pekerja> daftarPekerja)`
Menampilkan urutan pekerja berdasarkan total keterlambatan dari tertinggi ke terendah. Fungsi ini mengimplementasikan algoritma Selection Sort dengan nested loop. Loop luar mengiterasi dari indeks 0 hingga n-1, mencari elemen dengan keterlambatan maksimum dari sisa elemen yang belum terurut, kemudian melakukan swap. Fungsi menerima copy dari vector (bukan reference) agar tidak mengubah urutan data asli. Hanya menampilkan pekerja dengan keterlambatan > 0 menit. Kompleksitas waktu: O(n²).

### Fungsi Helper:
- **`parseTime(const std::string& timeStr)`**: Mengkonversi string format HH:MM menjadi struct Waktu
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
