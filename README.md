# Kutuphane
### Test Sistemi
Proje, tüm algoritmaların doğruluğunu onaylayan bağımsız bir **Birim Test (Unit Test)** altyapısına sahiptir.

**Testleri çalıştırmak için:**
`gcc -Iinclude src/unit_tests.c src/avl.c src/trie.c src/sort.c src/graph.c -o TestMerkezi && ./TestMerkezi`