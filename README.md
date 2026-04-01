# Labyrinth-Solver-Editor
TR: C dili ile geliştirilmiş, BFS ve DFS algoritmalarını görselleştiren interaktif labirent editörü ve çözücü. 
EN: Interactive maze editor and solver developed in C, visualizing BFS and DFS algorithms.
# 🧩 Pathfinder-C: Maze Editor & Solver

C dili ile geliştirilmiş, kullanıcı etkileşimli bir labirent tasarım ve çözümleme platformudur. Bu proje, temel veri yapılarını ve arama algoritmalarını görsel bir konsol arayüzünde birleştirir.

## 🚀 Öne Çıkan Özellikler
- **Dinamik Harita Editörü:** F1 (Yol), F2 (Duvar) ve F3 (Gezgin) tuşlarıyla gerçek zamanlı tasarım yapabilme.
- **Akıllı Arama Algoritmaları:** - **BFS (Breadth-First Search):** En kısa yolu bulmak için kuyruk (queue) yapısını kullanır.
  - **DFS (Depth-First Search):** Daha sezgisel bir arama için yığın (stack) mantığıyla çalışır.
- **Rastgele Harita Üretimi:** F9 ile "Recursive Backtracking" benzeri bir yöntemle karmaşık labirentler oluşturma.
- **Dosya Yönetimi:** Tasarlanan labirentleri `.txt` formatında kaydetme (F4) ve tekrar yükleme (F5).

## 🛠️ Teknik Detaylar
- **Geliştirme Dili:** C
- **Kütüphaneler:** `windows.h` (Konsol yönetimi), `conio.h` (Klavye etkileşimi), `time.h`
- **Algoritma Mantığı:** Izgara tabanlı (Grid-based) yol bulma ve matris manipülasyonu.

## 🎮 Kullanım Talimatı
Projeyi derleyip çalıştırdıktan sonra:
1. Harita boyutlarını girin (Örn: 50x15).
2. Yön tuşlarıyla hareket edin.
3. **ENTER** tuşuna basarak seçili algoritmanın labirenti çözmesini izleyin!
