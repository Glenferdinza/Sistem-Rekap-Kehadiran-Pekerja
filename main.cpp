#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <limits>
#include <sstream>

// Struct to represent time
struct Waktu {
    int jam;
    int menit;

    // Helper to get total minutes from midnight for easy comparison
    int toMinutes() const {
        return jam * 60 + menit;
    }
};

// Struct for a single day's attendance record
struct Kehadiran {
    std::string tanggal;
    Waktu jamMasuk;
    Waktu jamPulang;
    int menitKeterlambatan;
};

// Struct for an employee
struct Pekerja {
    std::string id;
    std::string nama;
    std::vector<Kehadiran> rekamKehadiran;
    int totalMenitKeterlambatan;
};

// --- Function Prototypes based on "Rencana Modular" ---

// 1. inputDataPekerja (modified for modern C++)
void inputDataPekerja(std::vector<Pekerja>& daftarPekerja);

// 2. inputKehadiranHarian
void inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk);

// 3. hitungRekap (logic will be integrated into report generation) & 6. tampilkanLaporan
void tampilkanLaporan(const std::vector<Pekerja>& daftarPekerja);

// 4. cariDataPekerja
void cariDataPekerja(const std::vector<Pekerja>& daftarPekerja);

// 5. urutkanKeterlambatan
void tampilkanUrutanTerlambat(std::vector<Pekerja> daftarPekerja);

// Helper function to parse time from HH:MM string
Waktu parseTime(const std::string& timeStr) {
    Waktu t = {0, 0};
    if (timeStr.length() == 5 && timeStr[2] == ':') {
        t.jam = std::stoi(timeStr.substr(0, 2));
        t.menit = std::stoi(timeStr.substr(3, 2));
    }
    return t;
}

// Helper function to format Waktu struct to HH:MM string
std::string formatWaktu(const Waktu& t) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << t.jam << ":" << std::setw(2) << std::setfill('0') << t.menit;
    return ss.str();
}


// --- Main Program ---

int main() {
    std::vector<Pekerja> daftarPekerja;
    Waktu jamStandarMasuk = {8, 0}; // Standard entry time 08:00

    // Pre-populate with data from "Contoh Data" for demonstration
    daftarPekerja.push_back({"ID-001", "Budi", {}, 0});
    daftarPekerja.push_back({"ID-002", "Sinta", {}, 0});
    daftarPekerja.push_back({"ID-003", "Doni", {}, 0});
    
    // Simulate initial attendance records from PDF
    Kehadiran budi_k = {"22-08-2025", parseTime("08:05"), parseTime("17:00"), 0};
    budi_k.menitKeterlambatan = std::max(0, budi_k.jamMasuk.toMinutes() - jamStandarMasuk.toMinutes());
    daftarPekerja[0].rekamKehadiran.push_back(budi_k);
    daftarPekerja[0].totalMenitKeterlambatan += budi_k.menitKeterlambatan;

    Kehadiran sinta_k = {"22-08-2025", parseTime("07:58"), parseTime("17:02"), 0};
    sinta_k.menitKeterlambatan = std::max(0, sinta_k.jamMasuk.toMinutes() - jamStandarMasuk.toMinutes());
    daftarPekerja[1].rekamKehadiran.push_back(sinta_k);
    daftarPekerja[1].totalMenitKeterlambatan += sinta_k.menitKeterlambatan;

    Kehadiran doni_k = {"22-08-2025", parseTime("08:15"), parseTime("16:30"), 0};
    doni_k.menitKeterlambatan = std::max(0, doni_k.jamMasuk.toMinutes() - jamStandarMasuk.toMinutes());
    daftarPekerja[2].rekamKehadiran.push_back(doni_k);
    daftarPekerja[2].totalMenitKeterlambatan += doni_k.menitKeterlambatan;


    int pilihan = 0;
    while (pilihan != 6) {
        std::cout << "\n===== Sistem Rekap Kehadiran Pekerja =====\n";
        std::cout << "Jam Standar Masuk: " << formatWaktu(jamStandarMasuk) << "\n";
        std::cout << "==========================================\n";
        std::cout << "1. Tambah Pekerja\n";
        std::cout << "2. Input Kehadiran Harian\n";
        std::cout << "3. Tampilkan Laporan Rekapitulasi\n";
        std::cout << "4. Tampilkan Urutan Pekerja Terlambat\n";
        std::cout << "5. Cari Pekerja\n";
        std::cout << "6. Keluar\n";
        std::cout << "==========================================\n";
        std::cout << "Masukkan pilihan Anda: ";
        std::cin >> pilihan;

        // Clear input buffer
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pilihan = 0; // reset choice
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
                std::cout << "Terima kasih telah menggunakan sistem.\n";
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    }

    return 0;
}

// --- Function Implementations ---

void inputDataPekerja(std::vector<Pekerja>& daftarPekerja) {
    Pekerja p;
    std::cout << "\n--- Tambah Pekerja Baru ---\n";
    std::cout << "Masukkan ID Pekerja: ";
    std::getline(std::cin, p.id);
    std::cout << "Masukkan Nama Pekerja: ";
    std::getline(std::cin, p.nama);
    p.totalMenitKeterlambatan = 0;
    daftarPekerja.push_back(p);
    std::cout << "Pekerja baru '" << p.nama << "' berhasil ditambahkan.\n";
}

void inputKehadiranHarian(std::vector<Pekerja>& daftarPekerja, const Waktu& jamStandarMasuk) {
    std::string tanggal, jamMasukStr, jamPulangStr;
    std::cout << "\n--- Input Kehadiran Harian ---\n";
    std::cout << "Masukkan Tanggal (DD-MM-YYYY): ";
    std::getline(std::cin, tanggal);

    for (auto& pekerja : daftarPekerja) {
        std::cout << "\nInput untuk " << pekerja.nama << " (" << pekerja.id << ")\n";
        std::cout << "Jam Masuk (HH:MM): ";
        std::getline(std::cin, jamMasukStr);
        std::cout << "Jam Pulang (HH:MM): ";
        std::getline(std::cin, jamPulangStr);

        Kehadiran k;
        k.tanggal = tanggal;
        k.jamMasuk = parseTime(jamMasukStr);
        k.jamPulang = parseTime(jamPulangStr);

        k.menitKeterlambatan = std::max(0, k.jamMasuk.toMinutes() - jamStandarMasuk.toMinutes());
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
    std::string targetNama;
    std::cout << "\n--- Cari Data Pekerja ---\n";
    std::cout << "Masukkan Nama Pekerja yang dicari: ";
    std::getline(std::cin, targetNama);

    bool ditemukan = false;
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
                 std::cout << std::left << std::setw(12) << "Tanggal" << std::setw(12) << "Jam Masuk" << std::setw(12) << "Jam Pulang" << "Keterlambatan\n";
                 std::cout << std::string(50, '-') << "\n";
                 for(const auto& k : pekerja.rekamKehadiran){
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
    // This function takes a copy of the vector to sort it without modifying the original order.
    
    // Selection Sort implementation as requested in the PDF
    for (size_t i = 0; i < daftarPekerja.size(); ++i) {
        size_t max_idx = i;
        for (size_t j = i + 1; j < daftarPekerja.size(); ++j) {
            if (daftarPekerja[j].totalMenitKeterlambatan > daftarPekerja[max_idx].totalMenitKeterlambatan) {
                max_idx = j;
            }
        }
        // Swap the found maximum element with the current element
        if (max_idx != i) {
            std::swap(daftarPekerja[i], daftarPekerja[max_idx]);
        }
    }

    std::cout << "\n--- Urutan Pekerja Berdasarkan Keterlambatan (Tertinggi ke Terendah) ---\n\n";
    std::cout << std::left << std::setw(15) << "Nama" << "Total Keterlambatan (menit)\n";
    std::cout << std::string(45, '-') << "\n";

    for (const auto& pekerja : daftarPekerja) {
        if(pekerja.totalMenitKeterlambatan > 0){
             std::cout << std::left << std::setw(15) << pekerja.nama << pekerja.totalMenitKeterlambatan << "\n";
        }
    }
}