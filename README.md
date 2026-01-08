# Kütüphane Yönetim Sistemi

**GitHub Deposu:** [https://github.com/zeyneptugbakurt/Kutuphane](https://github.com/zeyneptugbakurt/Kutuphane)

Bu proje, **Veri Yapıları ve Algoritmalar** dersi kapsamında C programlama dili kullanılarak geliştirilmiştir. Projenin temel amacı, teorik olarak öğrenilen veri yapılarının (Trie, Stack, Queue, Linked List) ve sıralama algoritmalarının (Quick Sort, Heap Sort, Merge Sort) gerçek bir senaryo üzerinde, grafiksel bir arayüz (GUI) ile uygulanmasıdır.

Arayüz tasarımı ve kullanıcı etkileşimi için **Raylib** kütüphanesi kullanılmıştır.

---

##  Proje Hakkında

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

## Dosya ve Klasör Yapısı

### `src/` (Kaynak Kodlar)
* **`main.c`:** Programın giriş noktası. Verileri yükler, testleri çalıştırır ve GUI'yi başlatır.
* **`gui.c`:** Grafik arayüz, menüler, butonlar ve çizim işlemleri.
* **`graph.c`:** Graf yapısının oluşturulması ve BFS (En Kısa Yol) algoritması.
* **`avl.c`:** AVL Ağacı veri yapısı (ID tabanlı arama ve dengeleme işlemleri).
* **`trie.c`:** Trie ağacı yönetimi (Kelime bazlı ekleme ve gezme).
* **`search.c`:** GUI ile Trie/Linear arama arasındaki köprü fonksiyonlar.
* **`sort.c`:** Sıralama algoritmaları (Quick, Heap, Merge).
* **`queue.c`:** Ödünç alma kuyruğu ve aradan silme işlemleri.
* **`stack.c`:** Arama geçmişi yığını ve temizleme işlemleri.
* **`performance.c`:** Algoritmaların hızlarını (CPU süresi) ölçen performans test modülü.
* **`unit_tests.c`:** Veri yapılarının doğruluğunu kontrol eden birim testleri.
* **`io.c`:** JSON dosya okuma işlemleri.
* **`cJSON.c`:** JSON ayrıştırma (parsing) kütüphanesi.

### `include/` (Başlık Dosyaları)
* **`book.h`:** Temel `Book` veri yapısı ve sabitler.
* **`gui.h`:** GUI fonksiyon tanımları.
* **`graph.h`:** Graf ve BFS fonksiyon tanımları.
* **`avl.h`:** AVL ağacı yapıları ve fonksiyonları.
* **`trie.h`:** Trie düğüm yapısı ve fonksiyonları.
* **`stack.h`:** Stack yapısı ve fonksiyonları.
* **`queue.h`:** Queue yapısı ve fonksiyonları.
* **`sort.h`:** Sıralama fonksiyonları.
* **`search.h`:** Arama fonksiyonları.
* **`performance.h`:** Performans testi fonksiyon tanımları.
* **`io.h`:** Dosya okuma işlemleri tanımları.
* **`cJSON.h`:** JSON kütüphanesi başlık dosyası.

### `resources/` ve `data/`
* **`arial.ttf`:** Türkçe karakter destekli font dosyası.
* **`books.json`:** Kitap veri tabanı.

### Kök Dizin
* **`Makefile`:** Projenin derlenmesi için gerekli komut seti.
---

##  Zaman Karmaşıklığı Analizi

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

##  Kurulum ve Raylib Yükleme

Bu projeyi çalıştırmak için **Raylib** kütüphanesinin bilgisayarınızda yüklü olması gerekir.

### 1. macOS İçin Kurulum (Homebrew ile)
Eğer Homebrew yüklü değilse terminale şunu yazın:
```bash
/bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"
```
Ardından Raylib'i yükleyin:
```bash
brew install raylib
```
Projeyi Çalıştırma: Terminali proje klasöründe açın ve aşağıdaki komutu girin:
```bash
make run
```
## Windows İçin Kurulum (MinGW ile)

Windows’ta kurulum macOS’a göre biraz daha manueldir.

### Gerekli Araçlar

- **w64devkit** (GCC + Make içeren paket) indirin ve kurun.
- **Raylib GitHub Releases** sayfasından  
- `raylib-x.x.x_win64_mingw-w64.zip` dosyasını indirin.

### Dosyaların Yerleştirilmesi

İndirdiğiniz Raylib klasörü içerisindeki:

- `include/`
- `lib/`

klasörlerini, projenizin kök dizinine kopyalayın.

---

### Projeyi Çalıştırma (CMD)

Windows Komut Satırı’nı (**CMD**) proje klasöründe açın ve aşağıdaki komutu (tek satır halinde) çalıştırın:

```bash
gcc src/main.c src/gui.c src/graph.c src/avl.c src/trie.c src/sort.c src/queue.c src/stack.c src/search.c src/performance.c src/unit_tests.c src/io.c src/cJSON.c -o kutuphane.exe -O2 -Wall -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
```
Derleme tamamlandıktan sonra programı çalıştırmak için:
```bash
kutuphane.exe
```

## Birim Test Altyapisi (Unit Testing Framework)

Bu proje, geliştirilen algoritmalarin doğruluğunu ve veri bütünlüğünü garanti altina almak amaciyla kapsamli bir Birim Test protokolü içermektedir. Yazilimin temel bileşenleri olan veri yapilari, ana programdan bağimsiz olarak test edilmiştir.

### Test Edilen Modüller
* **AVL Agaci:** ID tabanli arama ve düğüm dengeleme (rebalancing) mekanizmalari doğrulanmiştir.
* **Trie Arama:** Kelime bazli hizli arama ve Türkçe karakter normalizasyonu (harf dönüşümleri) test edilmiştir.
* **QuickSort:** Kitap puanlarina göre siralamanin matematiksel tutarliliği kontrol edilmiştir.
* **Dijkstra (Graf):** Kütüphane haritasi üzerindeki bölümler arasi mesafe verileri doğrulanmiştir.

### Testleri Calistirma
Sistemin doğruluğunu teyit etmek için terminal üzerinden aşağıdaki komutu kullanabilirsiniz:

```bash
gcc -Iinclude src/unit_tests.c src/avl.c src/trie.c src/sort.c src/graph.c src/io.c src/cJSON.c -o TestSistemi && ./TestSistemi
```


### Performans Testleri Hakkında Not

Gerçekleştirilen performans testlerinde elde edilen süre değerleri **sabit ve mutlak sonuçlar değildir**.  
Ölçümler; çalıştırılan sistemin donanım özelliklerine, işlemci yüküne ve programın çalışma anındaki koşullara bağlı olarak **küçük farklılıklar gösterebilir**.

Bu nedenle raporlanan süreler, ilgili veri yapılarının ve algoritmaların **ortalama davranışını ve göreli performansını** göstermek amacıyla sunulmuştur.  
Amaç, teorik zaman karmaşıklıklarının (Trie için **O(L)**, AVL için **O(log n)**) pratikte de geçerli olduğunu gözlemlemektir; mutlak zaman değerleri üzerinden bir karşılaştırma yapmak değildir.


Bu çalışmada yapılan tekrarlar sonucunda elde edilen ölçümlere göre, testler birden fazla kez çalıştırıldığında **ortalama olarak** aşağıdaki süre değerleri gözlemlenmiştir:

- **Trie Search (1000 tekrar):** ≈ **0.000037 sn**
- **AVL Insert (1000 tekrar):** ≈ **0.000149 sn**

Bu değerler, ilgili veri yapılarının teorik zaman karmaşıklıklarıyla uyumlu olacak şekilde, sistemden bağımsız olarak **göreli performanslarını** temsil etmektedir.

## Geliştiriciler 
- **Rüya Taştan**
- **Zeynep Tuğba Kurt**