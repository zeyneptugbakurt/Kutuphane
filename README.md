# Kütüphane Yönetim Sistemi

**GitHub Deposu:** [https://github.com/zeyneptugbakurt/Kutuphane](https://github.com/zeyneptugbakurt/Kutuphane)

Bu proje, **Veri Yapıları ve Algoritmalar** dersi kapsamında C programlama dili kullanılarak geliştirilmiştir. Projenin temel amacı, teorik olarak öğrenilen veri yapılarının (Trie, Stack, Queue, Linked List) ve sıralama algoritmalarının (Quick Sort, Heap Sort, Merge Sort) gerçek bir senaryo üzerinde, grafiksel bir arayüz (GUI) ile uygulanmasıdır.

Arayüz tasarımı ve kullanıcı etkileşimi için **Raylib** kütüphanesi kullanılmıştır.

---

## 📌 Proje Hakkında

Bu sistem, kullanıcıların JSON tabanlı geniş bir kitap veritabanı üzerinde anlık arama yapmasına, kitapları belirli kriterlere göre filtrelemesine, ödünç alma/iade etme işlemlerini yönetmesine ve arama geçmişlerini görüntülemesine olanak tanır.

### Temel Özellikler ve Kullanılan Yöntemler

1.  **Gelişmiş Arama (Trie + Linked List):**
    * Kitap adı, Yazar adı ve Tür bilgisine göre arama yapılabilir.
    * **Trie (Prefix Tree)** yapısı sayesinde arama hızı veri setinden bağımsızdır ($O(m)$).
    * Çakışmaları önlemek için Trie uçlarında **Linked List** kullanılmıştır.

2.  **Sıralama Filtreleri:**
    * **Artan Puan (0-5):** Quick Sort
    * **Azalan Puan (5-0):** Heap Sort
    * **Alfabetik (A-Z):** Merge Sort

3.  **Ödünç Alma ve İade Sistemi:**
    * Ödünç alınan kitaplar **Queue (Kuyruk)** veri yapısında (FIFO) tutulur.
    * İade işlemi sırasında kuyruk taranır ve ilgili kitap aradan çıkarılır.

4.  **Arama Geçmişi:**
    * Yapılan aramalar **Stack (Yığın)** veri yapısında (LIFO) saklanır.
    * Geçmişten tek tek silme veya toplu temizleme yapılabilir.

---

## 📂 Dosya ve Klasör Yapısı

Proje modüler bir yapıda tasarlanmıştır.

### `src/` (Kaynak Kodlar)
* **`main.c`:** Programın giriş noktası. Verileri yükler ve sistemi başlatır.
* **`gui.c`:** Grafik arayüz, butonlar, klavye işlemleri ve ekran çizimleri.
* **`trie.c`:** Trie ağacı yönetimi (Ekleme, Gezme).
* **`search.c`:** GUI ile Trie arasındaki arama köprüsü.
* **`sort.c`:** Sıralama algoritmaları (Quick, Heap, Merge).
* **`queue.c`:** Ödünç alma kuyruğu ve silme işlemleri.
* **`stack.c`:** Arama geçmişi yığını ve temizleme işlemleri.
* **`io.c`:** JSON dosya okuma işlemleri.
* **`cJSON.c`:** JSON ayrıştırma (parsing) kütüphanesi.

### `include/` (Başlık Dosyaları)
* **`book.h`:** Temel `Book` veri yapısı ve sabitler.
* **`gui.h`:** GUI fonksiyon tanımları.
* **`trie.h`:** Trie düğüm yapısı ve fonksiyonları.
* **`stack.h`:** Stack yapısı ve fonksiyonları.
* **`queue.h`:** Queue yapısı ve fonksiyonları.
* **`sort.h`:** Sıralama fonksiyonları.
* **`search.h`:** Arama fonksiyonları.

### `resources/` ve `data/`
* **`arial.ttf`:** Türkçe karakter destekli font dosyası.
* **`books.json`:** Kitap veri tabanı.

### Kök Dizin
* **`Makefile`:** Projenin derlenmesi için gerekli komut seti.

---

## ⏱️ Zaman Karmaşıklığı Analizi

| İşlem | Kullanılan Yapı | Zaman Karmaşıklığı |
| :--- | :--- | :--- |
| **Arama (Search)** | Trie + Linked List | **O(m)** (m: kelime uzunluğu) |
| **Sıralama (Artan)** | Quick Sort | **O(n log n)** |
| **Sıralama (Azalan)** | Heap Sort | **O(n log n)** |
| **Sıralama (Alfabetik)** | Merge Sort | **O(n log n)** |
| **Geçmişe Ekleme** | Stack (Push) | **O(1)** |
| **Ödünç Verme** | Queue (Enqueue) | **O(1)** |
| **İade Alma** | Queue Traversal | **O(k)** (k: kuyruk boyutu) |

---

## 🛠️ Kurulum ve Raylib Yükleme

Bu projeyi çalıştırmak için **Raylib** kütüphanesinin bilgisayarınızda yüklü olması gerekir.

### 1. macOS İçin Kurulum (Homebrew ile)
Eğer Homebrew yüklü değilse terminale şunu yazın:
```bash
/bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"
Tamamdır, "Sorunlar ve Çözümleri" kısmını çıkardım.

Aşağıda, GitHub linkinden Raylib kurulumuna, dosya açıklamalarından zaman karmaşıklığı tablosuna kadar her şeyi içeren, tek parça ve kopyalanabilir README.md dosyası bulunuyor.

Bunu tek seferde kopyalayıp projenin içine yapıştırabilirsin.

Markdown

# Kütüphane Yönetim Sistemi

**GitHub Deposu:** [https://github.com/zeyneptugbakurt/Kutuphane](https://github.com/zeyneptugbakurt/Kutuphane)

Bu proje, **Veri Yapıları ve Algoritmalar** dersi kapsamında C programlama dili kullanılarak geliştirilmiştir. Projenin temel amacı, teorik olarak öğrenilen veri yapılarının (Trie, Stack, Queue, Linked List) ve sıralama algoritmalarının (Quick Sort, Heap Sort, Merge Sort) gerçek bir senaryo üzerinde, grafiksel bir arayüz (GUI) ile uygulanmasıdır.

Arayüz tasarımı ve kullanıcı etkileşimi için **Raylib** kütüphanesi kullanılmıştır.

---

## 📌 Proje Hakkında

Bu sistem, kullanıcıların JSON tabanlı geniş bir kitap veritabanı üzerinde anlık arama yapmasına, kitapları belirli kriterlere göre filtrelemesine, ödünç alma/iade etme işlemlerini yönetmesine ve arama geçmişlerini görüntülemesine olanak tanır.

### Temel Özellikler ve Kullanılan Yöntemler

1.  **Gelişmiş Arama (Trie + Linked List):**
    * Kitap adı, Yazar adı ve Tür bilgisine göre arama yapılabilir.
    * **Trie (Prefix Tree)** yapısı sayesinde arama hızı veri setinden bağımsızdır ($O(m)$).
    * Çakışmaları önlemek için Trie uçlarında **Linked List** kullanılmıştır.

2.  **Sıralama Filtreleri:**
    * **Artan Puan (0-5):** Quick Sort
    * **Azalan Puan (5-0):** Heap Sort
    * **Alfabetik (A-Z):** Merge Sort

3.  **Ödünç Alma ve İade Sistemi:**
    * Ödünç alınan kitaplar **Queue (Kuyruk)** veri yapısında (FIFO) tutulur.
    * İade işlemi sırasında kuyruk taranır ve ilgili kitap aradan çıkarılır.

4.  **Arama Geçmişi:**
    * Yapılan aramalar **Stack (Yığın)** veri yapısında (LIFO) saklanır.
    * Geçmişten tek tek silme veya toplu temizleme yapılabilir.

---

## 📂 Dosya ve Klasör Yapısı

Proje modüler bir yapıda tasarlanmıştır.

### `src/` (Kaynak Kodlar)
* **`main.c`:** Programın giriş noktası. Verileri yükler ve sistemi başlatır.
* **`gui.c`:** Grafik arayüz, butonlar, klavye işlemleri ve ekran çizimleri.
* **`trie.c`:** Trie ağacı yönetimi (Ekleme, Gezme).
* **`search.c`:** GUI ile Trie arasındaki arama köprüsü.
* **`sort.c`:** Sıralama algoritmaları (Quick, Heap, Merge).
* **`queue.c`:** Ödünç alma kuyruğu ve silme işlemleri.
* **`stack.c`:** Arama geçmişi yığını ve temizleme işlemleri.
* **`io.c`:** JSON dosya okuma işlemleri.
* **`cJSON.c`:** JSON ayrıştırma (parsing) kütüphanesi.

### `include/` (Başlık Dosyaları)
* **`book.h`:** Temel `Book` veri yapısı ve sabitler.
* **`gui.h`:** GUI fonksiyon tanımları.
* **`trie.h`:** Trie düğüm yapısı ve fonksiyonları.
* **`stack.h`:** Stack yapısı ve fonksiyonları.
* **`queue.h`:** Queue yapısı ve fonksiyonları.
* **`sort.h`:** Sıralama fonksiyonları.
* **`search.h`:** Arama fonksiyonları.

### `resources/` ve `data/`
* **`arial.ttf`:** Türkçe karakter destekli font dosyası.
* **`books.json`:** Kitap veri tabanı.

### Kök Dizin
* **`Makefile`:** Projenin derlenmesi için gerekli komut seti.

---

## ⏱️ Zaman Karmaşıklığı Analizi

| İşlem | Kullanılan Yapı | Zaman Karmaşıklığı |
| :--- | :--- | :--- |
| **Arama (Search)** | Trie + Linked List | **O(m)** (m: kelime uzunluğu) |
| **Sıralama (Artan)** | Quick Sort | **O(n log n)** |
| **Sıralama (Azalan)** | Heap Sort | **O(n log n)** |
| **Sıralama (Alfabetik)** | Merge Sort | **O(n log n)** |
| **Geçmişe Ekleme** | Stack (Push) | **O(1)** |
| **Ödünç Verme** | Queue (Enqueue) | **O(1)** |
| **İade Alma** | Queue Traversal | **O(k)** (k: kuyruk boyutu) |

---

## 🛠️ Kurulum ve Raylib Yükleme

Bu projeyi çalıştırmak için **Raylib** kütüphanesinin bilgisayarınızda yüklü olması gerekir.

### 1. macOS İçin Kurulum (Homebrew ile)
Eğer Homebrew yüklü değilse terminale şunu yazın:
```bash
/bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"

Ardından Raylib'i yükleyin:
```bash
brew install raylib
Projeyi Çalıştırma: Terminali proje klasöründe açın ve aşağıdaki komutu girin:
```bash
make run
2. Windows İçin Kurulum (MinGW ile)
Windows'ta kurulum biraz daha manueldir.

w64devkit (GCC + Make içeren paket) indirin ve kurun.

Raylib GitHub Releases sayfasından raylib-x.x.x_win64_mingw-w64.zip dosyasını indirin.

İndirdiğiniz klasördeki include ve lib dosyalarını projenizin olduğu klasöre kopyalayın.

Projeyi Çalıştırma (CMD ile): Windows komut satırını (CMD) proje klasöründe açın ve:

DOS

gcc src/main.c src/gui.c src/trie.c src/sort.c src/queue.c src/stack.c src/search.c src/io.c src/cJSON.c -o kutuphane.exe -O2 -Wall -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
kutuphane.exe
👩‍💻 Geliştiriciler
Zeynep Tuğba Kurt

Rüya Taştan