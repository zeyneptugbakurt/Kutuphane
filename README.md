# Kutuphane Yonetim Sistemi

Bu proje, Veri Yapıları ve Algoritmalar dersi kapsamında C programlama dili kullanılarak geliştirilmiştir. Projenin temel amacı, teorik olarak öğrenilen veri yapılarının (Trie, Stack, Queue, Linked List) ve sıralama algoritmalarının (Quick Sort, Heap Sort, Merge Sort) gerçek bir senaryo üzerinde, grafiksel bir arayüz (GUI) ile uygulanmasıdır.

Arayüz tasarımı ve kullanıcı etkileşimi için Raylib kütüphanesi kullanılmıştır.

## Proje Hakkinda

Bu sistem, kullanıcıların geniş bir kitap veritabanı üzerinde hızlı (prefix) arama yapmasına, kitapları belirli kriterlere göre filtrelemesine, ödünç alma/iade etme işlemlerini yönetmesine ve arama geçmişlerini görüntülemesine olanak tanır.

### Temel Ozellikler ve Kullanilan Yontemler

1.  **Prefix Arama (On Ek Aramasi):**
    * Kullanıcıların kitap isimlerini ararken, her tuşlamada anlık sonuç alabilmesi için **Trie (Prefix Tree)** veri yapısı kullanılmıştır.
    * Bu yapı sayesinde arama işleminin zaman karmaşıklığı, veri setindeki toplam kitap sayısından bağımsız olarak, sadece aranan kelimenin uzunluğuna (m) bağlıdır: **O(m)**.
    * Türkçe karakterler (ç, ş, ğ, ü, ö, ı) ve rakamlar için özel normalizasyon algoritmaları geliştirilmiş, büyük/küçük harf duyarlılığı yönetilmiştir.

2.  **Siralama Filtreleri:**
    * Kitapları listelerken performans kıyaslaması yapabilmek adına üç farklı sıralama algoritması entegre edilmiştir:
        * **Artan Puan (0-5):** Quick Sort algoritması.
        * **Azalan Puan (5-0):** Heap Sort algoritması.
        * **Alfabetik (A-Z):** Merge Sort algoritması.
    * Tüm sıralama algoritmalarının ortalama zaman karmaşıklığı **O(n log n)** şeklindedir.

3.  **Odunc Alma ve Iade Sistemi:**
    * Ödünç alınan kitapların takibi için **Queue (Kuyruk)** veri yapısı (FIFO - İlk Giren İlk Çıkar mantığı) kullanılmıştır.
    * Bir kitap ödünç alındığında ana listeden çıkarılıp kuyruğa eklenir. İade işlemi yapıldığında ise kuyruğun ilgili konumundan çıkarılarak tekrar ana listeye dahil edilir.

4.  **Arama Gecmisi:**
    * Kullanıcının yaptığı son aramalar **Stack (Yigin)** veri yapısı (LIFO - Son Giren İlk Çıkar mantığı) ile tutulmaktadır.
    * "Geçmiş" ekranında kullanıcının son arattığı kelimeler listelenir.

---

## Dosya ve Klasor Yapisi

Proje modüler bir yapıda tasarlanmıştır. Kodun okunabilirliğini ve yönetilebilirliğini artırmak amacıyla kaynak kodlar, başlık dosyaları ve veri dosyaları ayrıştırılmıştır.

### 1. src/ (Kaynak Kodlar)
Bu klasör, projenin mantıksal işleyişini sağlayan `.c` uzantılı uygulama dosyalarını içerir.

* **main.c:** Programın başlangıç noktasıdır. Veri setini yükler ve GUI döngüsünü başlatır.
* **gui.c:** Kullanıcı arayüzünün çizilmesi, buton etkileşimleri, klavye girdilerinin okunması ve ekrana yazdırma işlemlerini yönetir.
* **trie.c:** Arama motorunun çekirdeğidir. Kelime ekleme, Türkçe karakter normalizasyonu ve prefix arama fonksiyonlarını barındırır.
* **search.c:** GUI ile Trie yapısı arasındaki köprüdür. Kullanıcı girdisini alıp Trie üzerinde arama yapar ve sonuçları döndürür.
* **sort.c:** Quick Sort, Heap Sort ve Merge Sort algoritmalarının implementasyonlarını içerir.
* **queue.c:** Ödünç alma ve iade işlemleri için kullanılan kuyruk yapısının (ekleme, çıkarma, listeleme) fonksiyonlarını içerir.
* **stack.c:** Arama geçmişini tutan yığın yapısının (push, pop, traverse) fonksiyonlarını içerir.
* **io.c:** `books.json` dosyasını okuyarak verileri belleğe yükleyen dosya giriş/çıkış işlemlerini yapar.
* **cJSON.c:** JSON formatındaki veri setini parçalamak (parse etmek) için kullanılan harici kütüphanedir.

### 2. include/ (Baslik Dosyalari)
Bu klasör, fonksiyon prototiplerini ve veri yapıları (struct) tanımlarını içeren `.h` uzantılı dosyaları barındırır.

* **book.h:** `Book` yapısının (struct) ve genel kütüphane tanımlarının bulunduğu temel dosyadır.
* **gui.h, trie.h, sort.h, queue.h, stack.h, search.h:** İlgili `.c` dosyalarındaki fonksiyonların dışarıya açılan prototiplerini içerir.

### 3. resources/ (Kaynaklar)
* **arial.ttf:** Arayüzde Türkçe karakterlerin (Ç, Ş, Ğ, İ, Ö, Ü) düzgün görüntülenebilmesi için gerekli olan font dosyasıdır.

### 4. Kok Dizin
* **Makefile:** Projenin derlenmesi için gerekli komut setlerini içeren konfigürasyon dosyasıdır.

---

## Kurulum ve Calistirma

Proje, kaynak kodlarından derlenerek çalıştırılmak üzere tasarlanmıştır.

### 1. Linux ve macOS (Make ile)

Terminali açın ve proje dizinine gidin. `Makefile` sayesinde aşağıdaki komutlarla kolayca işlem yapabilirsiniz:

* **Derle ve Calistir:**
    ```bash
    make run
    ```
* **Sadece Derle:**
    ```bash
    make
    ```
* **Temizlik:**
    ```bash
    make clean
    ```

### 2. Windows (MinGW ile)

Windows işletim sisteminde projeyi derlemek için **MinGW (GCC)** derleyicisine ve **Raylib** kütüphanesinin Windows sürümüne ihtiyacınız vardır.

**On Hazirlik:**
Raylib kütüphanesinin (MinGW uyumlu sürümü) `include` ve `lib` dosyalarının proje dizininde veya derleyicinin erişebileceği bir yolda olduğundan emin olun.

**Derleme Komutu:**
Windows komut satırını (CMD) proje klasöründe açın ve aşağıdaki komutu girin:

```cmd
gcc src/main.c src/gui.c src/trie.c src/sort.c src/queue.c src/stack.c src/search.c src/io.c src/cJSON.c -o kutuphane_gui.exe -O2 -Wall -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm