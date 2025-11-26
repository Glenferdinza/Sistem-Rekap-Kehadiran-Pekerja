#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>
#include <fstream>
#include <cctype>

struct Waktu {
    int jam;
    int menit;

    int toMinutes() const {
        return jam * 60 + menit;
    }
};

struct Kehadiran {
    std::string tanggal;
    Waktu jamMasuk;
    Waktu jamPulang;
    int menitKeterlambatan;
};

struct Pekerja {
    std::string id;
    std::string nama;
    std::vector<Kehadiran> rekamKehadiran;
    int totalMenitKeterlambatan;
};

void inputDataPekerja(std::vector<Pekerja>& daftarPekerja);
void inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk);
void tampilkanLaporan(const std::vector<Pekerja>& daftarPekerja);
void cariDataPekerja(const std::vector<Pekerja>& daftarPekerja);
void tampilkanUrutanTerlambat(std::vector<Pekerja> daftarPekerja);
void exportLaporanCSV(const std::vector<Pekerja>& daftarPekerja);

bool isValidIDFormat(const std::string& id) {
    if (id.length() != 6) return false;
    if (id[0] != 'I' || id[1] != 'D') return false;
    for (int i = 2; i < 6; ++i) {
        if (!std::isdigit(id[i])) return false;
    }
    return true;
}

bool isIDDuplicate(const std::vector<Pekerja>& daftarPekerja, const std::string& id) {
    for (const auto& pekerja : daftarPekerja) {
        if (pekerja.id == id) return true;
    }
    return false;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return daysInMonth[month - 1];
}

bool isValidDate(const std::string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '-' || dateStr[5] != '-') return false;
    
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!std::isdigit(dateStr[i])) return false;
    }
    
    int day = std::stoi(dateStr.substr(0, 2));
    int month = std::stoi(dateStr.substr(3, 2));
    int year = std::stoi(dateStr.substr(6, 4));
    
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(month, year)) return false;
    if (year < 1900 || year > 2100) return false;
    
    return true;
}

bool isValidTime(const std::string& timeStr, Waktu& outTime) {
    if (timeStr.length() != 5) return false;
    if (timeStr[2] != ':') return false;
    
    for (int i = 0; i < 5; ++i) {
        if (i == 2) continue;
        if (!std::isdigit(timeStr[i])) return false;
    }
    
    int jam = std::stoi(timeStr.substr(0, 2));
    int menit = std::stoi(timeStr.substr(3, 2));
    
    if (jam < 0 || jam > 23) return false;
    if (menit < 0 || menit > 59) return false;
    
    outTime.jam = jam;
    outTime.menit = menit;
    return true;
}

std::string formatWaktu(const Waktu& t) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << t.jam << ":" 
       << std::setw(2) << std::setfill('0') << t.menit;
    return ss.str();
}

int main() {
    std::vector<Pekerja> daftarPekerja;
    Waktu jamStandarMasuk = {8, 0};
    int pilihan = 0;

    while (pilihan != 7) {
        std::cout << "\n===== Sistem Rekap Kehadiran Pekerja =====\n";
        std::cout << "Jam Standar Masuk: " << formatWaktu(jamStandarMasuk) << "\n";
        std::cout << "==========================================\n";
        std::cout << "1. Tambah Pekerja\n";
        std::cout << "2. Input Kehadiran Harian\n";
        std::cout << "3. Tampilkan Laporan Rekapitulasi\n";
        std::cout << "4. Tampilkan Urutan Pekerja Terlambat\n";
        std::cout << "5. Cari Pekerja\n";
        std::cout << "6. Export Laporan ke CSV\n";
        std::cout << "7. Keluar\n";
        std::cout << "==========================================\n";
        std::cout << "Masukkan pilihan Anda: ";
        std::cin >> pilihan;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pilihan = 0;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                inputDataPekerja(daftarPekerja);
                break;
            case 2:
                inputKehadiranHarian(daftarPekerja, jamStandarMasuk);
                break;
            case 3:
                tampilkanLaporan(daftarPekerja);
                break;
            case 4:
                tampilkanUrutanTerlambat(daftarPekerja);
                break;
            case 5:
                cariDataPekerja(daftarPekerja);
                break;
            case 6:
                exportLaporanCSV(daftarPekerja);
                break;
            case 7:
                std::cout << "Terima kasih telah menggunakan sistem.\n";
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    }

    return 0;
}

void inputDataPekerja(std::vector<Pekerja>& daftarPekerja) {
    Pekerja p;
    std::string inputID;
    bool validInput = false;
    
    std::cout << "\n--- Tambah Pekerja Baru ---\n";
    
    while (!validInput) {
        std::cout << "Masukkan ID Pekerja (Format: IDXXXX, contoh: ID0001): ";
        std::getline(std::cin, inputID);
        
        if (!isValidIDFormat(inputID)) {
            std::cout << "Error: Format ID tidak valid! Harus IDXXXX (contoh: ID0001, ID0002)\n";
            continue;
        }
        
        if (isIDDuplicate(daftarPekerja, inputID)) {
            std::cout << "Error: ID '" << inputID << "' sudah terdaftar! Gunakan ID lain.\n";
            continue;
        }
        
        validInput = true;
    }
    
    p.id = inputID;
    std::cout << "Masukkan Nama Pekerja: ";
    std::getline(std::cin, p.nama);
    p.totalMenitKeterlambatan = 0;
    daftarPekerja.push_back(p);
    std::cout << "Pekerja baru '" << p.nama << "' berhasil ditambahkan.\n";
}

void inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk) {
    if (daftarPekerja.empty()) {
        std::cout << "\nTidak ada pekerja terdaftar. Tambahkan pekerja terlebih dahulu.\n";
        return;
    }
    
    std::string tanggal, jamMasukStr, jamPulangStr;
    Waktu jamMasuk, jamPulang;
    bool validDate = false;
    
    std::cout << "\n--- Input Kehadiran Harian ---\n";
    
    while (!validDate) {
        std::cout << "Masukkan Tanggal (DD-MM-YYYY): ";
        std::getline(std::cin, tanggal);
        
        if (isValidDate(tanggal)) {
            validDate = true;
        } else {
            std::cout << "Error: Format tanggal tidak valid atau tanggal tidak ada!\n";
            std::cout << "Contoh format yang benar: 25-11-2025\n";
        }
    }

    for (auto& pekerja : daftarPekerja) {
        bool validJamMasuk = false;
        bool validJamPulang = false;
        
        std::cout << "\nInput untuk " << pekerja.nama << " (" << pekerja.id << ")\n";
        
        while (!validJamMasuk) {
            std::cout << "Jam Masuk (HH:MM): ";
            std::getline(std::cin, jamMasukStr);
            
            if (isValidTime(jamMasukStr, jamMasuk)) {
                validJamMasuk = true;
            } else {
                std::cout << "Error: Format waktu tidak valid! (Jam: 00-23, Menit: 00-59)\n";
            }
        }
        
        while (!validJamPulang) {
            std::cout << "Jam Pulang (HH:MM): ";
            std::getline(std::cin, jamPulangStr);
            
            if (!isValidTime(jamPulangStr, jamPulang)) {
                std::cout << "Error: Format waktu tidak valid! (Jam: 00-23, Menit: 00-59)\n";
                continue;
            }
            
            if (jamPulang.toMinutes() <= jamMasuk.toMinutes()) {
                std::cout << "Error: Jam pulang harus lebih besar dari jam masuk!\n";
                continue;
            }
            
            validJamPulang = true;
        }

        Kehadiran k;
        k.tanggal = tanggal;
        k.jamMasuk = jamMasuk;
        k.jamPulang = jamPulang;
        k.menitKeterlambatan = std::max(0, jamMasuk.toMinutes() - jamStandarMasuk.toMinutes());
        
        pekerja.totalMenitKeterlambatan += k.menitKeterlambatan;
        pekerja.rekamKehadiran.push_back(k);
        
        std::cout << "Kehadiran untuk '" << pekerja.nama << "' berhasil dicatat.\n";
        if (k.menitKeterlambatan > 0) {
            std::cout << "Catatan: Terlambat " << k.menitKeterlambatan << " menit.\n";
        }
    }
}

void tampilkanLaporan(const std::vector<Pekerja>& daftarPekerja) {
    std::cout << "\n--- Laporan Rekapitulasi Kehadiran ---\n\n";
    if (daftarPekerja.empty()) {
        std::cout << "Tidak ada data pekerja.\n";
        return;
    }

    std::cout << std::left << std::setw(8) << "ID" << std::setw(15) << "Nama"
              << std::setw(12) << "Total Hadir" << std::setw(20) << "Total Keterlambatan" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    for (const auto& pekerja : daftarPekerja) {
        int totalHadir = pekerja.rekamKehadiran.size();
        std::string keterlambatanStr = std::to_string(pekerja.totalMenitKeterlambatan) + " menit";
        
        std::cout << std::left << std::setw(8) << pekerja.id << std::setw(15) << pekerja.nama
                  << std::setw(12) << totalHadir << std::setw(20) << keterlambatanStr << std::endl;
    }
}

void cariDataPekerja(const std::vector<Pekerja>& daftarPekerja) {
    if (daftarPekerja.empty()) {
        std::cout << "\nTidak ada data pekerja.\n";
        return;
    }
    
    std::string targetNama;
    bool ditemukan = false;
    
    std::cout << "\n--- Cari Data Pekerja ---\n";
    std::cout << "Masukkan Nama Pekerja yang dicari: ";
    std::getline(std::cin, targetNama);

    for (size_t i = 0; i < daftarPekerja.size(); ++i) {
        if (daftarPekerja[i].nama == targetNama) {
            ditemukan = true;
            const auto& pekerja = daftarPekerja[i];
            std::cout << "\n--- Data Ditemukan ---\n";
            std::cout << "Index: " << i << "\n";
            std::cout << "ID: " << pekerja.id << "\n";
            std::cout << "Nama: " << pekerja.nama << "\n";
            std::cout << "Total Hari Hadir: " << pekerja.rekamKehadiran.size() << "\n";
            std::cout << "Total Keterlambatan: " << pekerja.totalMenitKeterlambatan << " menit\n";
            
            if (!pekerja.rekamKehadiran.empty()) {
                std::cout << "\n--- Rincian Kehadiran ---\n";
                std::cout << std::left << std::setw(12) << "Tanggal" << std::setw(12) << "Jam Masuk" 
                          << std::setw(12) << "Jam Pulang" << "Keterlambatan\n";
                std::cout << std::string(50, '-') << "\n";
                for (const auto& k : pekerja.rekamKehadiran) {
                    std::cout << std::left << std::setw(12) << k.tanggal
                              << std::setw(12) << formatWaktu(k.jamMasuk)
                              << std::setw(12) << formatWaktu(k.jamPulang)
                              << k.menitKeterlambatan << " menit\n";
                }
            }
            break; 
        }
    }

    if (!ditemukan) {
        std::cout << "Pekerja dengan nama '" << targetNama << "' tidak ditemukan.\n";
    }
}

void tampilkanUrutanTerlambat(std::vector<Pekerja> daftarPekerja) {
    if (daftarPekerja.empty()) {
        std::cout << "\nTidak ada data pekerja.\n";
        return;
    }
    
    size_t n = daftarPekerja.size();
    
    for (size_t i = 0; i < n; ++i) {
        size_t max_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (daftarPekerja[j].totalMenitKeterlambatan > daftarPekerja[max_idx].totalMenitKeterlambatan) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            std::swap(daftarPekerja[i], daftarPekerja[max_idx]);
        }
    }

    std::cout << "\n--- Urutan Pekerja Berdasarkan Keterlambatan (Tertinggi ke Terendah) ---\n\n";
    std::cout << std::left << std::setw(15) << "Nama" << "Total Keterlambatan (menit)\n";
    std::cout << std::string(45, '-') << "\n";

    bool adaKeterlambatan = false;
    for (const auto& pekerja : daftarPekerja) {
        if (pekerja.totalMenitKeterlambatan > 0) {
            std::cout << std::left << std::setw(15) << pekerja.nama << pekerja.totalMenitKeterlambatan << "\n";
            adaKeterlambatan = true;
        }
    }
    
    if (!adaKeterlambatan) {
        std::cout << "Tidak ada pekerja yang terlambat.\n";
    }
}

void exportLaporanCSV(const std::vector<Pekerja>& daftarPekerja) {
    std::cout << "\n--- Export Laporan ke CSV ---\n";
    
    if (daftarPekerja.empty()) {
        std::cout << "Tidak ada data pekerja untuk di-export.\n";
        return;
    }
    
    std::string namaFile;
    std::cout << "Masukkan nama file (tanpa ekstensi .csv): ";
    std::getline(std::cin, namaFile);
    namaFile += ".csv";
    
    std::ofstream file(namaFile);
    if (!file.is_open()) {
        std::cout << "Gagal membuat file " << namaFile << "\n";
        return;
    }
    
    file << "ID,Nama,Total Hadir,Total Keterlambatan (menit)\n";
    
    for (const auto& pekerja : daftarPekerja) {
        int totalHadir = pekerja.rekamKehadiran.size();
        file << pekerja.id << ","
             << pekerja.nama << ","
             << totalHadir << ","
             << pekerja.totalMenitKeterlambatan << "\n";
    }
    
    file.close();
    std::cout << "Laporan berhasil di-export ke file '" << namaFile << "'\n";
    std::cout << "File tersimpan di direktori yang sama dengan program.\n";
}